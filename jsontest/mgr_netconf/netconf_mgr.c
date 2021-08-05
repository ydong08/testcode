#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include "openssl/md5.h"
#include "openssl/pem.h"
#include "openssl/bio.h"
#include "openssl/evp.h"
#include "netconf_mgr.h"

/*************************************************
 * 1. 解析开局邮件 
 * 2. 根据邮件内容准备启动netopeer2-server 
 *************************************************/
 
 
 
int md5calc(char *src, int inlen, char *out, int olen)
{
	int i = 0;
	MD5_CTX x;
	unsigned char d[16];

	MD5_Init(&x);       
	MD5_Update (&x, src, inlen);
	MD5_Final(d, &x);
	memset(out, 0x00, olen);
	for (i = 0; i < 16; i++) 
	{
		sprintf (out + (i*2), "%02X", d[i]);
	}
	out[32] = 0;
	return 0;
}

int base64_encode(char *in_str, int in_len, char *out_str)
{
	BIO *b64, *bio;
	BUF_MEM *bptr = NULL;
	size_t size = 0;

	if (in_str == NULL || out_str == NULL)
		return -1;

	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new(BIO_s_mem());
	bio = BIO_push(b64, bio);

	BIO_write(bio, in_str, in_len);
	BIO_flush(bio);

	BIO_get_mem_ptr(bio, &bptr);
	memcpy(out_str, bptr->data, bptr->length);
	out_str[bptr->length] = '\0';
	size = bptr->length;

	BIO_free_all(bio);
	return size;
}

int base64_decode(char *in_str, int in_len, char *out_str)
{
	BIO *b64, *bio;
	int size = 0;

	if (in_str == NULL || out_str == NULL)
		return -1;

	b64 = BIO_new(BIO_f_base64());
	//BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

	bio = BIO_new_mem_buf(in_str, in_len);
	bio = BIO_push(b64, bio);

	size = BIO_read(bio, out_str, in_len);
	out_str[size] = '\0';

	BIO_free_all(bio);
	return size;
}

/*
Netconf:
Common:
enable=
user=
passwd=
authen=
wanname=
wantype=
wanip=
lans=
EntryX:
rtIp=
nexthop=
ipver=
*/
int parse_cfg_json(const char *jsonstr)
{
	int  ipversion = -1; //1:ipv4,2:ipv6
	int  wanipver = -1;
	int  wantype = 0x01;
	enum json_error rc = 0;
	unsigned int i = 0;
	json_t *root = NULL;
	json_t *item = NULL;
	json_t *temp = NULL;
	char *node[5] = {"uuid", "dev", "authen", "user", "pwd"};
	char cmd[64] = {0};
	char rtIp[48] = {0};
	char nexthop[48] = {0};
	char buf[256] = {0};

	if (!jsonstr)
	{
		ERR("invalid parameter.");
		return -1;	
	}

	rc = json_parse_document(&root, jsonstr);
	if (JSON_OK != rc || !root)
	{
		ERR("parse json string to tree fail.");
		return -1;	
	}

	for (i = 0; i < sizeof(node)/sizeof(node[0]); ++i)
	{
		item = json_find_first_label(root, node[i]);
		NP2_CHECK_JSON_NODE(item, JSON_STRING, node[i]);
		snprintf(cmd, sizeof(cmd), "Netconf_Entry %s", node[i]);
		set_cfg_node_value(cmd, (char*)item->child->text);
		DBG("%s:%s.", node[i], item->child->text);
	}

	/* lans:["ge-2","ge-3"] */
	item = json_find_first_label(root, "lans");
	NP2_CHECK_JSON_NODE(item, JSON_ARRAY, "lans");
	temp = item->child->child;
	memset(buf, 0, sizeof(buf));
	while (temp)
	{
		if (sizeof(buf) < strlen(buf) + strlen(temp->text) + 1)
			break;
		if (buf[0])
			strcat(buf, ",");
		strcat(buf, temp->text);
		temp = temp->next;
	} 
	snprintf(cmd, sizeof(cmd), "Netconf_Common lans");
	set_cfg_node_value(cmd, buf);

	/* wans: */
	item = json_find_first_label(root, "wans");
	NP2_CHECK_JSON_NODE(item, JSON_ARRAY, "wans");
	item = item->child->child;
	temp = json_find_first_label(item, "name");
	NP2_CHECK_JSON_NODE(temp, JSON_STRING, "name");
	snprintf(cmd, sizeof(cmd), "Netconf_Common wanname");
	set_cfg_node_value(cmd, temp->child->text);

	temp = json_find_first_label(item, "type");
	NP2_CHECK_JSON_NODE(temp, JSON_STRING, "type");
	snprintf(cmd, sizeof(cmd), "Netconf_Common wantype");
	set_cfg_node_value(cmd, temp->child->text);
	if (!strcasecmp("dhcp", temp->child->text))
		wantype = 0x03;

	if (0x01 == wantype)  //static ip
	{
		temp = json_find_first_label(item, "ipv4");
		NP2_CHECK_JSON_NODE(temp, JSON_STRING, "ipv4");
		snprintf(cmd, sizeof(cmd), "Netconf_Common wanip");
		set_cfg_node_value(cmd, temp->child->text);
		if (0 < inet_pton(AF_INET, temp->child->text, rtIp))
			wanipver = 1;
		if (0 < inet_pton(AF_INET6, temp->child->text, rtIp))
			wanipver = 2;
			
		temp = json_find_first_label(item, "dnsSvr");
		NP2_CHECK_JSON_NODE(temp, JSON_STRING, "dnsSvr");
		snprintf(cmd, sizeof(cmd), "Netconf_Common dnsSvr");
		set_cfg_node_value(cmd, temp->child->text);
	}
	
	/* statRtV4 */
	i = 0;
	item = json_find_first_label(root, "statRtV4");
	NP2_CHECK_JSON_NODE(item, JSON_ARRAY, "statRtV4");
	item = item->child->child;
	while (item)
	{
		/* rtIp */
		temp = json_find_first_label(item, "rtIp");
		NP2_CHECK_JSON_NODE(temp, JSON_STRING, "rtIp");
		snprintf(rtIp, sizeof(rtIp), "%s", temp->child->text);
		
		/* nexthop */
		temp = json_find_first_label(item, "nexthop");
		NP2_CHECK_JSON_NODE(temp, JSON_STRING, "nexthop");
		snprintf(nexthop, sizeof(nexthop), "%s", temp->child->text);
		
		ipversion = -1;
		if (0 < inet_pton(AF_INET, rtIp, cmd) 
			&&  (wantype & 0x03 ? 1 : 0x01 & wanipver))
			ipversion = 1;
		if (0 < inet_pton(AF_INET6, rtIp, cmd) 
			&&  (wantype & 0x03 ? 1 : 0x01 & wanipver))
			ipversion = 2;
		
		if (0 < inet_pton(AF_INET, nexthop, cmd) 
			&& 1 == ipversion)
			ipversion = 1;
		if (0 < inet_pton(AF_INET6, nexthop, cmd) 
			&& 2 == ipversion)
			ipversion = 2;
			
		if (-1 < ipversion)
		{
			snprintf(cmd, sizeof(cmd), "Netconf_Entry%d rtIp", i);
			set_cfg_node_value(cmd, rtIp);

			snprintf(cmd, sizeof(cmd), "Netconf_Entry%d nexthop", i);
			set_cfg_node_value(cmd, nexthop);
			
			snprintf(cmd, sizeof(cmd), "Netconf_Entry%d ipver", i);
			set_cfg_node_value(cmd, 1 == ipversion?"1":"2");
		}
		++i;
		item = item->next;
	}

	// clean up
	json_free_value(&root);
	return 0;
}

/* cfg is web encode string */
int parse_cfg_info(const char *cfgstr)
{
	char *token = NULL;
	char *config = NULL;
	int  rc = -1;
	int  len = 0;
	char md5str[36] = {0};
	char buf[512] = {0};

	if (!cfgstr)
	{
		ERR("invalid parameter cfgstr.");
		return -1;	
	}

	/* config string */
	token = strrchr(cfgstr, '/');
	len = strlen(token + 1);
	config = (char*)malloc(len + 1 + 37);
	if (!config)
	{
		ERR("invalid config memory.n");
		return -1;	
	}
	config[len] = 0;
	strncpy(config, token + 1, len);

	/* md5 */
	token = strstr(cfgstr, "md5=");
	strncpy(md5str, token + 4, 32);

	md5calc(config, strlen(config), buf, sizeof(buf));
	if (strcasecmp(buf, md5str))
	{
		ERR("config md5 is different.");
		goto errorcfg;	
	}

	/* decode config */
	base64_decode(config, strlen(config), buf);

	/* parse config(json) */
	rc = parse_cfg_json(buf);
	if (rc < 0)
		goto errorcfg;

	rc = 0;

errorcfg:
	if (config) free(config);
	config = NULL;
	return rc;
} 

int set_cfg_node_value_vas(const char *node, char *value)
{
	FILE *fp = NULL;
	long vlen = 0;
	char *cmd = NULL;

	if (!node || !value)
	{
		ERR("invalid parameter.");
		return -1;	
	}
	
	vlen = strlen(value) + 23;
	cmd = (char*)malloc(vlen + 1);
	if (!cmd)
	{
		ERR("invalid memory for value.");
		return -1;
	}
	
	snprintf(cmd, sizeof(cmd), "/userfs/bin/tcapi set %s %s", node, vlen ? value : "");
	fp = popen(cmd, "r");
	free(cmd);
	cmd = NULL;
	if (!fp)
	{
		ERR("set %s %s fail.", node, value);
		return -1;
	}
	
	return 0;
}

int get_cfg_node_value_vas(const char *node, char **value)
{
	FILE *fp = NULL;
	char *tbuf = NULL;
	long vlen = 0;
	char cmd[64] = {0};
	
	if (!node || !value)
	{
		ERR("invalid parameter.");
		return -1;	
	}
	
	*value = NULL;
	snprintf(cmd, sizeof(cmd), "/userfs/bin/tcapi get %s", node);
	fp = popen(cmd, "r");
	if (!fp)
	{
		ERR("get netconf entry fail.");
		return -1;
	}
	
	fseek(fp, 0L, SEEK_END);
	vlen = ftell(fp);
	if (!vlen)
	{
		fclose(fp);
		ERR("get invalid authen url.");
		return -1;		
	}
	
	tbuf = (char*)malloc(vlen+1);
	if (!tbuf)
	{
		fclose(fp);
		ERR("create new url memory fail.");
		return -1;
	}

	tbuf[vlen] = 0;
	rewind(fp);
	fread(tbuf, 1, vlen, fp);
	fclose(fp);
	*value = tbuf;
	
	return 0;
}

int set_cfg_node_value(const char *node, char *value)
{
	FILE *fp = NULL;
	long vlen = 0;
	char cmd[512] = {0};

	if (!node || !value)
	{
		ERR("invalid parameter.");
		return -1;	
	}
	
	vlen = strlen(value);
	snprintf(cmd, sizeof(cmd), "/userfs/bin/tcapi set %s %s", node, vlen ? value : "");
	fp = popen(cmd, "r");
	if (!fp)
	{
		ERR("set %s %s fail.", node, value);
		return -1;
	}
	
	return 0;
}

int get_cfg_node_value(const char *node, char *value, int len)
{
	FILE *fp = NULL;
	char *tbuf = value;
	long vlen = 0;
	char cmd[64] = {0};
	
	if (!node || !value)
	{
		ERR("invalid parameter.");
		return -1;	
	}
	
	
	snprintf(cmd, sizeof(cmd), "/userfs/bin/tcapi get %s", node);
	fp = popen(cmd, "r");
	if (!fp)
	{
		ERR("get netconf entry fail.");
		return -1;
	}
	
	fseek(fp, 0L, SEEK_END);
	vlen = ftell(fp);
	rewind(fp);
	memset(tbuf, 0, len);
	fread(tbuf, 1, vlen < len ? vlen : len, fp);
	fclose(fp);
	
	return 0;

}
/* get a available wan that could be connect authen server
 * REQUIRED STEPS:
 *   1. wire interfaces ares prior to wireless interface
 *   2. retrive per 15 seconds if no avaiable wan by ping probing
 *   3. chose another one if cannot connect per 180 seconds server 
 */
int get_avl_wan(char *wanname)
{
	char *token        = NULL;
  	char *saveptr      = NULL;
	int  rc            = 0;
    int  i             = 0;
    int  pvc_index     = 0;
    int  entry_index   = 0;
    char nodeName[64]  = {0};
    char valid_if[128] = {0};
    char value[128]    = {0};
    
  	if (!wanname)
  	{
  		ERR("invalid parameter.");
  		return -1;
  	}
  	
	rc = get_cfg_node_value("WanInfo_Common ValidIFIndex", value, sizeof(value));
	if (rc < 0)
	{
		ERR("get WanInfo ValidIFIndex fail.");
		return -1;		
	}

	strncpy(valid_if, value, sizeof(valid_if) - 1);
    token = strtok_r(valid_if, ",", &saveptr);
    while(token)
    {
        i = atoi(token);
        token = strtok_r(NULL, ",", &saveptr);
        pvc_index = i / 8;
        entry_index = i - pvc_index * 8;

        snprintf(nodeName, sizeof(nodeName), 
        		"Wan_PVC%d_Entry%d Active", pvc_index, entry_index);
        rc = get_cfg_node_value(nodeName, value, sizeof(value));
        if (rc < 0 || !value[0])
        {
        	continue;	
        }
		
		 if(strcasecmp("Yes", value))
        {
            continue;
        }

        memset(value, 0, sizeof(value));
        snprintf(nodeName, sizeof(nodeName), 
        		"Wan_PVC%d_Entry%d ServiceList", pvc_index, entry_index);
        rc = get_cfg_node_value(nodeName, value, sizeof(value));
        if (rc < 0 || !value[0])
        {
        	continue;	
        }
  
        if (strstr(value,"INTERNET"))
        {
            break;
        }
    }

    return 0;
}

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t          realsize = size * nmemb;
    memorystruct_t  *mem = (memorystruct_t*)userp;
    char            *ptr = NULL;

    if(!mem || !ptr)
    {
        ERR("ptr or data is NULL.");
        return 0;
    }

    ptr = (char *)realloc(mem->data, mem->size + realsize + 1);
    if (!ptr)
   	{
   		ERR("not enough memory to recalloc"); 	
   		return 0;
    }
    
	mem->data = ptr;
    memcpy(&(mem->data[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->data[mem->size] = 0;

    return realsize;
}

//初始化CURL Handle
static char g_curl_err_buf[CURL_ERROR_SIZE]={0};
int init_curl(CURL **pcurl)
{
    CURLcode code;
    CURL     *curl = NULL;
    int      rc = -1;
    int      i_ssl_cert_enable = 0;
    char     *usr = NULL;
    char     *passwd = NULL;
    char     authsrv[128] = {0};
    
    curl = curl_easy_init();
    if (!curl)
    {
        ERR("curl_easy_init fail");
        return -1;
    }

    code = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, g_curl_err_buf);
    NP2_CHECK_CURL_RC(curl, code, "Failed to set error buffer");

    //设置取得返回信息的callback函数
    code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    NP2_CHECK_CURL_RC(curl, code, "Failed to set error buffer");

    code = curl_easy_setopt(curl, CURLOPT_COOKIEFILE,  "");
    NP2_CHECK_CURL_RC(curl, code, "Failed to set cookie file");

    //设置登录用户名和密码
    code = curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
    NP2_CHECK_CURL_RC(curl, code, "Failed to set http auth");
    if (usr && passwd)
    {
        code = curl_easy_setopt(curl, CURLOPT_USERNAME, usr);
        code += curl_easy_setopt(curl, CURLOPT_PASSWORD, passwd);
        NP2_CHECK_CURL_RC(curl, code, "auth server usrname and passwd");
 	}

    code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    NP2_CHECK_CURL_RC(curl, code, "Failed to set follow location");

    code = curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5);
    NP2_CHECK_CURL_RC(curl, code, "Failed to set max redirs");

    code = curl_easy_setopt(curl, CURLOPT_POSTREDIR, CURL_REDIR_POST_ALL);
    NP2_CHECK_CURL_RC(curl, code, "Failed to set postredir");

    code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
    NP2_CHECK_CURL_RC(curl, code, "Failed to set timeout");
    
    rc = get_cfg_node_value("Netconf_Entry authen", authsrv, sizeof(authsrv));
    if (rc < 0 || !authsrv[0])
    {
    	ERR("Failed to get authsrv");
        NP2_CLEAN_CURL(curl);	
    }
    code = curl_easy_setopt(curl, CURLOPT_URL, authsrv);
    NP2_CHECK_CURL_RC(curl, code, "Failed to set url");
	
    if (i_ssl_cert_enable)
    {
		code = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
		code = code | curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
		code = code | curl_easy_setopt(curl, CURLOPT_CAINFO , "/etc/ssl/certs/ca.crt");
		code = code | curl_easy_setopt(curl,CURLOPT_SSLCERT,"/etc/ssl/certs/client.pem");
		code = code | curl_easy_setopt(curl,CURLOPT_SSLCERTTYPE,"PEM");
		NP2_CHECK_CURL_RC(curl, code, "Failed to set SSL option");
    }
    
    if (!i_ssl_cert_enable)
    {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    }

    *pcurl = curl;
    
    return 0;
}

int get_info_from_httpbody(const char *body, int len)
{
	char *token = NULL;
	char *saveptr = NULL;
	char *ptr = NULL;
	char key[32] = {0};
	char value[132] = {0};
	
	if (len <= 0 || !body)
	{
		WRN("get zero length response body");
		return -1;
	}
	
	token = strtok_r((char*)body, ",", &saveptr);
	while (token)
	{
		ptr = strstr(token, ":");
		if (ptr && 2 < ptr - token) //": 2 characters
		{
			strncpy(key, token + 1, ptr - token - 2);
			strncpy(value, token + 2, sizeof(value) - 1);
			value[strlen(value) - 1] = 0;
			DBG("response body [%s:%s]", key, value);
		}
		token = strtok_r(NULL, ",", &saveptr);
	}
	
	return 0;
}

int http_access_request(CURL **curlhandle)
{
	CURL *curl = *curlhandle;
	struct curl_slist  *headers = NULL;
	char *url = NULL;
	long response = 0;
	CURLcode code;
	memorystruct_t memory;
	char snid[32] = {0};
	char uuid[132] = {0};
	char authenurl[128] = {0};
	char sndbuf[32] = {0};
	char nodename[32] = {0};
	
	if (!curl)
	{
		ERR("invalid curl handler.");
		return -1;		
	}
	
	memset(&memory, 0, sizeof(memory));
	snprintf(nodename, sizeof(nodename), "Netconf_Common uuid");
	get_cfg_node_value(nodename, uuid, sizeof(uuid));
	
	snprintf(nodename, sizeof(nodename), "Netconf_Common authen");
	get_cfg_node_value(nodename, authenurl, sizeof(authenurl));
	
	snprintf(nodename, sizeof(nodename), "DeviceInfo_devParaDynamic SerialNum");
	get_cfg_node_value(nodename, snid, sizeof(snid));
	snprintf(sndbuf, sizeof(sndbuf), "\"uuid\":\"%s\",\"sn\":\"%s\"", uuid, snid);
	
	code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&memory);
	NP2_CHECK_CURL_RC(curl, code, "Failed to set write data");
    
	code = curl_easy_setopt(curl, CURLOPT_URL, authenurl);
	NP2_CHECK_CURL_RC(curl, code, "Failed to set url");
	
	code = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sndbuf);
	code = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(sndbuf));
	NP2_CHECK_CURL_RC(curl, code, "Failed to set postfields");
	
	code = curl_easy_setopt(curl, CURLOPT_INTERFACE, sndbuf);
	NP2_CHECK_CURL_RC(curl, code, "Failed to set interface");
	
	headers = curl_slist_append(headers, "Connection: Keep-Alive");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "SOAPAction: \"\"");  
	code = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	NP2_CHECK_CURL_RC(curl, code, "Failed to set httpheader");
	
	code = curl_easy_perform(curl);
	NP2_CHECK_CURL_RC(curl, code, "Failed to perform curl");
	
	curl_slist_free_all(headers);
	code = curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);
	code = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response);
	switch (response)
	{
		case 200:
			get_info_from_httpbody(memory.data, memory.size);
			break;
		case 500: /* Internal Server Error */
		case 502: /* Bad Gateway */
		case 503: /* Service Unavailable */
		case 504: /* Gateway Timeout */
		case 401:	
		case 400:
		default:
			ERR("get invalid report response");
			break;	
	}
	
	if (memory.data)
	{
		free(memory.data);
		memory.data = NULL;
	}
		
	NP2_CLEAN_CURL(curl);
	return 0;
}


int main()
{
	FILE* fp = NULL;
	const char *cfgstr = NULL;
	CURL *pcurl = NULL;
	int  rc = -1;
	
	/* 1. recv setup email */
	rc = parse_cfg_info(cfgstr);
	if (rc < 0)
	{
		ERR("parse config info fail.");
		return -1;		
	}
	
	/* 2. report SN/UUID to authen server */
	rc = init_curl(&pcurl);
	if (rc < 0)
	{
		ERR("init curl fail.");
		return -1;		
	}
	
	rc = http_access_request(&pcurl);
	if (rc < 0)
	{
		ERR("report sn/uuid to authen server fail.");
		return -1;		
	}
	
	/* 3. start netopeer2 */
	fp = popen(NP2_SETUP_SH, "r");
	if (!fp)
	{
		ERR("run %s fail.", NP2_SETUP_SH);
		return -1;
	}
	pclose(fp);
	sleep(10);
	
	fp = popen(NP2_MERGE_HOSTKEY_SH, "r");
	if (!fp)
	{
		ERR("run %s fail.", NP2_MERGE_HOSTKEY_SH);
		return -1;
	}
	pclose(fp);
	
	fp = popen(NP2_MERGE_CONFIG_SH, "r");
	if (!fp)
	{
		ERR("run %s fail.", NP2_MERGE_CONFIG_SH);
		return -1;
	}
	pclose(fp);
	
	fp = popen(NP2_NETOPEER2_SERVER, "r");
	if (!fp)
	{
		ERR("run %s fail.", NP2_NETOPEER2_SERVER);
		return -1;
	}
	pclose(fp);

	return 0;	
}


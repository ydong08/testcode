
#include "curl/curl.h"
#include "mjson-1.5/json.h"

typedef struct  memorystruct {
      char *data;
      size_t size;
} memorystruct_t;

#define ERR(format, args ...) printf(format,##args)  
#define WRN(format, args ...) printf(format,##args) 
#define DBG(format, args ...) printf(format,##args) 

#define NP2_CLEAN_CURL(x)        {if (x) curl_easy_cleanup(x); return -1;}

#define NP2_CHECK_CURL_RC(x, rc, str)    \
			if (NULL != x && CURLE_OK != rc) { \
				ERR(str ":%s\n", curl_easy_strerror(rc)); \
				curl_easy_cleanup(x); \
				return -1; \
			}

#define NP2_CHECK_JSON_NODE(r, t, x)    \
			{ if (NULL == (r)) { \
				ERR("parse json-format config child[%s] fail.", (x)); \
				return -1; } \
			  else if (t != (r)->child->type) { \
				ERR("parse json-format config type invalid."); \
				return -1;} \
			}

#define NP2_CHECK_JSON_NODE_VALUE(r, x)    \
			{ if (NULL == (r)) { \
				ERR("parse json-format config child[%s] fail.", (x)); \
				return -1; } \
			}


#define NP2_SETUP_SH              "/etc/yang/modules/netopeer2/setup.sh"
#define NP2_MERGE_HOSTKEY_SH      "/etc/yang/modules/netopeer2/merge_hostkey.sh"
#define NP2_MERGE_CONFIG_SH       "/etc/yang/modules/netopeer2/merge_config.sh"
#define NP2_NETOPEER2_SERVER      "netopeer2-server -d -v2"



int md5calc(char *src, int inlen, char *out, int olen);
int base64_encode(char *in_str, int in_len, char *out_str);
int base64_decode(char *in_str, int in_len, char *out_str);

int set_cfg_node_value_vas(const char *node, char *value);
int get_cfg_node_value_vas(const char *node, char **value);
int set_cfg_node_value(const char *node, char *value);
int get_cfg_node_value(const char *node, char *value, int len);

int get_avl_wan(char *wanname);
int parse_cfg_info(const char *cfgstr);
int parse_cfg_json(const char *jsonstr);


size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp);
int init_curl(CURL **pcurl);
int http_access_request(CURL **curlhandle);
int get_info_from_httpbody(const char *body, int len);

 

  
  
  
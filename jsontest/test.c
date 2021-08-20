#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include <sys/types.h>
#include <openssl/md5.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

#include "json.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

#define NP2_CHECK_JSON_NODE(r, t, x)    \
					{ if (NULL == (r)) { \
						printf("[%s:%d] parse json-format config child[%s] fail.\n", __FUNCTION__, __LINE__, (x)); \
						return -1; } \
					  else if (t != (r)->child->type) { \
						printf("[%s:%d] parse json-format config type invalid[%d->%d].\n", __FUNCTION__, __LINE__,t,(r)->child->type); \
						return -1;} \
					}

#define NP2_CHECK_JSON_NODE_VALUE(r, x)    \
					{ if (NULL == (r)) { \
						printf("[%s:%d] parse json-format config child[%s] fail.\n", __FUNCTION__, __LINE__, (x)); \
						return -1; } \
					}

static void
test_json_unescape (void)
{
  char* strings[][2] =
    {
      { /* make sure the test works */
        "simple ascii-conformant string",
        "simple ascii-conformant string"
      },
      { /* make sure we decode »\"« properly */
        "\\\"quoted string\\\"",
        "\"quoted string\""
      },
      { /* »\\« */
        "C:\\\\Windows\\\\win.com",
        "C:\\Windows\\win.com"
      },
      { /* »\/« */
        "\\/bin\\/true",
        "/bin/true"
      },
      { /* »\b« - backspace U+0008 */
        "\\b",
        "\x8"
      },
      {/* »\f« - form feed U+000C */
        "\\f",
        "\xC"
      },
      { /* »\n« - newline U+0010 */
        "\\n",
        "\n"
      },
      { /* »\r« - carriage return U+000D */
        "\\r",
        "\r"
      },
      { /* »\t« - tab U+0009 */
        "\\t",
        "\t"
      },
      { /* »\uXXXX« - UTF-8 BMP character */
        "\\u0041pfelb\\u00E4ume\\u2026",
        "Apfelb\303\244ume\342\200\246"
        /* Apfelbäume… */
      },
      { /* »\uXXXX\uXXXX« - UTF-16 surrogates */
        "\\uD835\\uDE79\\uD835\\uDE82\\uD835\\uDE7E\\uD835\\uDE7D",
        "\360\235\231\271\360\235\232\202\360\235\231\276\360\235\231\275"
        /* 𝙹𝚂𝙾𝙽 (From Unicode's "Mathematical Monospace Capitals") */ }
    };
  size_t i;

  printf ("test_json_unescape(): ");

  for (i = 0; i < (sizeof(strings)/sizeof(strings[0])); i++)
    {
      char* test = json_unescape (strings[i][0]);

      assert (test);
      if (strcmp (test, strings[i][1]))
        {
          printf ("\nerror unescaping: got »%s« (expected »%s«)\n",
                  test, strings[i][1]);
          assert (!strcmp (test, strings[i][1]));
        }

      printf (".");
      free (test);
    }

  printf ("\n");
}

static void
test_remove_items (void)
{
  char* json = "[{\"first\" : 1, \"second\" : 2}]";
  json_t* root = NULL;
  json_t* item;

  assert (JSON_OK == json_parse_document (&root, json));
  json_free_value (&root);

  assert (JSON_OK == json_parse_document (&root, json));
  assert (root->child);
  item = json_find_first_label (root->child, "first");
  json_free_value (&item);
  json_free_value (&root);

  assert (JSON_OK == json_parse_document (&root, json));
  assert (root->child);
  item = json_find_first_label (root->child, "second");
  json_free_value (&item);
  json_free_value (&root);
}

int parse_stream_json(const char *jsonfile)
{
	json_t *document = NULL;
	FILE *fp = NULL;
	char *ptr = NULL;
	char *token = NULL;
	char *saveptr = NULL;

	printf("[%s:%d] jsonfile:%s.\n", __FUNCTION__, __LINE__, jsonfile);
	ptr = (char*)jsonfile;
	token = strtok_r(ptr, " ", &saveptr);
	while (token)
	{
		printf("processing file %s...\n", token);
		fp = fopen(token,"r");
		if(fp == NULL)
		{
			printf("file \"%s\" couldn't be open\n", token);
			continue;
		}
		
		token = strtok_r(NULL, " ", &saveptr);
		switch(json_stream_parse(fp, &document))
		{
			case JSON_OK:
				json_stream_output(stdout,document);
				json_free_value(&document);
				break;

			default:
				printf("some problem occurred\n");
				break;
		}
		fclose(fp);
	}

	return 0;
}

int parse_cfg_json(const char *jsonstr)
{
	int  i = 0;
	enum json_error rc = 0;
	char   *value = NULL;
	json_t *root = NULL;
	json_t *item = NULL;
	json_t *temp = NULL;
	const char *pnode[5] = {"uuid", "dev", "authen", "user", "pwd"};
	char   buf[512] = {0};

	if (!jsonstr)
	{
		printf("[%s:%d] invalid parameter.\n", __FUNCTION__, __LINE__);
		return -1;	
	}
	printf("[%s:%d] decode config : %s.\n", __FUNCTION__, __LINE__, jsonstr);

	rc = json_parse_document(&root, jsonstr);
	if (JSON_OK != rc)
	{
		printf("[%s:%d] parse json string to tree fail.\n", __FUNCTION__, __LINE__);
		return -1;	
	}
	assert (root->child);

	for (i = 0; i < sizeof(pnode)/sizeof(pnode[0]); ++i)
	{
		printf("[%s:%d] parse json node %s.\n", __FUNCTION__,__LINE__, pnode[i]);
		item = json_find_first_label(root, pnode[i]);
		NP2_CHECK_JSON_NODE(item, JSON_STRING, pnode[i]);
		printf("[%s:%d] %s:%s.\n", __FUNCTION__,__LINE__, pnode[i], item->child->text);
	}

	/* lans:["ge-2","ge-3"] */
	item = json_find_first_label(root, "lans");
	NP2_CHECK_JSON_NODE(item, JSON_ARRAY, "lans");
	temp = item->child->child;
	printf("[%s:%d] lans: ", __FUNCTION__, __LINE__);
	while (temp)
	{
		printf("%s,", temp->text);
		temp = temp->next;
	} 
	printf("\n");

	/* wans: */
	item = json_find_first_label(root, "wans");
	NP2_CHECK_JSON_NODE(item, JSON_ARRAY, "wans");
	item = item->child->child;
	temp = json_find_first_label(item, "name");
	NP2_CHECK_JSON_NODE(temp, JSON_STRING, "name");
	printf("[%s:%d] %s:%s.\n", __FUNCTION__,__LINE__, "name", temp->child->text);

	temp = json_find_first_label(item, "type");
	NP2_CHECK_JSON_NODE(temp, JSON_STRING, "type");
	printf("[%s:%d] %s:%s.\n", __FUNCTION__,__LINE__, "type", temp->child->text);

	temp = json_find_first_label(item, "ipv4");
	NP2_CHECK_JSON_NODE(temp, JSON_STRING, "ipv4");
	printf("[%s:%d] %s:%s.\n", __FUNCTION__,__LINE__, "ipv4", temp->child->text);


	/* statRtV4 */
	item = json_find_first_label(root, "statRtV4");
	NP2_CHECK_JSON_NODE(item, JSON_ARRAY, "statRtV4");
	item = item->child->child;
	while (item)
	{
		/* rtIp */
		temp = json_find_first_label(item, "rtIp");
		NP2_CHECK_JSON_NODE(temp, JSON_STRING, "rtIp");
		printf("[%s:%d] %s:%s.\n", __FUNCTION__,__LINE__, "rtIp", temp->child->text);

		/* nexthop */
		temp = json_find_first_label(item, "nexthop");
		NP2_CHECK_JSON_NODE(temp, JSON_STRING, "nexthop");
		printf("[%s:%d] %s:%s.\n", __FUNCTION__,__LINE__, "nexthop", temp->child->text);

		item = item->next;
	}


	// clean up
	json_free_value(&root);
	return 0;
}

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
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
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
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

	bio = BIO_new_mem_buf(in_str, in_len);
	bio = BIO_push(b64, bio);

	size = BIO_read(bio, out_str, in_len);
	out_str[size] = '\0';

	BIO_free_all(bio);
	return size;
}

int parse_cfg_info(const char *cfgstr)
{
	char *token = NULL;
	char *config = NULL;
	int  rc = -1;
	int  len = 0;
	char md5str[48] = {0};
	char buf[512] = {0};

	if (!cfgstr)
	{
		printf("[%s:%d] invalid parameter cfgstr.\n", __FUNCTION__, __LINE__);
		return -1;	
	}

	printf("[%s:%d] cfgstr: %s.\n", __FUNCTION__, __LINE__, cfgstr);

	/* config string */
	token = strrchr(cfgstr, '/');
	len = strlen(token + 1);
	config = (char*)malloc(len + 1 + 37);
	if (!config)
	{
		printf("[%s:%d] invalid config memory.\n", __FUNCTION__, __LINE__);
		return -1;	
	}
	config[len] = 0;
	strncpy(config, token + 1, len);

	/* md5 */
	token = strstr(cfgstr, "md5=");
	strncpy(md5str, token + 4, 32);
	printf("[%s:%d] md5 : %s.\n", __FUNCTION__, __LINE__, md5str);

	md5calc(config, strlen(config), buf, sizeof(buf));
	printf("[%s:%d] calc md5 : %s.\n", __FUNCTION__, __LINE__, buf);
	if (strcasecmp(buf, md5str))
	{
		printf("[%s:%d] config md5 is different.\n", __FUNCTION__, __LINE__);
		goto errorcfg;	
	}

	/* decode config */
	printf("[%s:%d] config entire: %s.\n", __FUNCTION__, __LINE__, config);
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


int
main (int argc, char **argv)
{
	FILE *fp = NULL;
	int len = 0;
	char md5str[64] = {0};
	char ptr[512] = { 0 };
	char content[512] = {0};
	if(argc < 2)
	{
		printf("[%s:%d] usage: test document1.json\n", __FUNCTION__, __LINE__);
		return EXIT_SUCCESS;
	}

	if (access(argv[1], F_OK))
	{
		printf("[%s:%d] json file %s is missing.\n", __FUNCTION__, __LINE__, argv[1]);
		return EXIT_SUCCESS;
	}

	parse_stream_json((const char*)argv[1]);	
	printf("***************************SEP**************************\n");

	fp = fopen(argv[1], "r");
	if (!fp)
	{
		printf("[%s:%d] json file %s open failed.\n", __FUNCTION__, __LINE__, argv[1]);
		return EXIT_SUCCESS;
	}

	len = fread(ptr, 1, sizeof(ptr), fp);
	if (len <= 0)
	{
		printf("[%s:%d] json file %s is null.\n", __FUNCTION__, __LINE__, argv[1]);
		return EXIT_SUCCESS;
	}
	printf("[%s:%d] json file %s content: %s.\n", __FUNCTION__, __LINE__, argv[1], ptr);
	base64_encode(ptr, len, content);
	//content[strlen(content) - 1] = 0;
	printf("[%s:%d] json file %s encode content[%d]:%s.\n", __FUNCTION__, __LINE__, argv[1], strlen(content), content);

	md5calc(content, strlen(content), md5str, sizeof(md5str));
	printf("[%s:%d] json encode content md5 string: %s.\n", __FUNCTION__, __LINE__, md5str);
	memset(ptr, 0, sizeof(ptr));
	strcpy(ptr, "https://192.168.0.1/emailsetup.cgi?md5=");
	strcat(ptr, md5str);
	strcat(ptr, "/");
	strcat(ptr, content);

	parse_cfg_info(ptr);

#if 0
	test_json_unescape ();
	test_remove_items ();
#endif
	return EXIT_SUCCESS;
}

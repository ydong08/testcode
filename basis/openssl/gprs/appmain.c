

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <ctosapi.h>

#define DISABLE_AUTOCONNAP                  (BYTE *)"0"
#define ENABLE_AUTOCONNAP                   (BYTE *)"1"
#define DISABLE_DHCP                        (BYTE *)"0"
#define ENABLE_DHCP                         (BYTE *)"1"

#define SSL_CA_PATH_LEN                     256
#define SSL_CLIENT_CA_PATH                  "client.pem"
#define SSL_ERR_STRINGS()                   ERR_error_string(ERR_get_error(),NULL)


int closewifi(void) 
{
    CTOS_WifiDisconnectAP();
    usleep(2000000);
    CTOS_WifiClose();
    usleep(2000000);
    
    return 0;
    
}

int openwifi(void)
{
    WORD wError;
    BOOL bPortable, bPCI;
    WORD wHwSettings;
    DWORD dwStatus = 0;

    wError = CTOS_HWSettingGet(&bPortable, &bPCI, &wHwSettings);
    if (d_OK != wError)
        return 1;
    
    printf("[log] CTOS_HWSettingGet done\n");
    if (!(d_MK_HW_WIFI & wHwSettings))
        return 2;
    
    wError = CTOS_WifiConfigSet(d_WIFI_CONFIG_AUTOCON_AP, ENABLE_AUTOCONNAP, 1);
    if (d_OK != wError) 
        return 3;
    printf("[log] CTOS_WifiConfigSet (auto) done\n");
    
    wError = CTOS_WifiConfigSet(d_WIFI_CONFIG_DHCP, ENABLE_DHCP, 1);
    if (d_OK != wError)
        return 4;
    printf("[log] CTOS_WifiConfigSet (dhcp) done\n");
    
    wError = CTOS_WifiOpen();
    time_t start = time(0);
    while (d_WIFI_IO_PROCESSING == wError) {
        wError = CTOS_WifiStatus(&dwStatus);
        if (5000 < time(0) - start) {
            printf("[log] get open status timeout\n");
            return 5;
        }
        if (dwStatus & d_WIFI_STATE_AP_CONNECTED) {
            printf("[log] wifi open ok\n");
            break;       
        }
        usleep(100 * 1000);
    }
    
    printf("[log] CTOS_WifiOpen done\n");
    wError = CTOS_WifiScan();
    start = time(0);
    while (d_WIFI_IO_PROCESSING == wError) {
        wError = CTOS_WifiStatus(&dwStatus);
        if (50 < time(0) - start)
        {
            return 6;
        }
        usleep(20 * 1000);
    }
    printf("[log] CTOS_WifiScan done\n");
    
    wError = CTOS_WifiStatus(&dwStatus);
    if (d_OK != wError)
    {
        if (wError & d_WIFI_IO_APCONNECTFAILED)
        {
            CTOS_WifiDisconnectAP();
            printf("[log] wifi disconnect\n");
            return 7;
        }
    }
    
    BYTE nAPCount = 0;
    CTOS_stWifiInfo *stWifiInfo;
    wError = CTOS_WifiInfoGet(&nAPCount, &stWifiInfo);
    if (0 == wError) {
        BYTE byId = 0;
        for (; byId < nAPCount; ++byId) {
            if (0 == strcmp((char *) (stWifiInfo + byId)->ESSID, "Castles-2.4G")){
                printf("[log] wifi essid: %s\n", (char *) (stWifiInfo + byId)->ESSID);
                nAPCount = byId;
                break;
            }
        }
    }
    printf("[log] CTOS_WifiInfoGet done\n");
    
    wError = CTOS_WifiConnectAP((stWifiInfo + nAPCount), "CastSz602", strlen("CastSz602"));
    start = time(0);
    while (d_WIFI_IO_PROCESSING == wError) {
        wError = CTOS_WifiStatus(&dwStatus);
        if (120 < time(0) - start)
        {
            return 8;
        }
        usleep(20 * 1000);
    }
    
    wError = CTOS_WifiStatus(&dwStatus);
    if (d_OK != wError)
    {
        if (wError & d_WIFI_IO_APCONNECTFAILED)
        {
            CTOS_WifiDisconnectAP();
            printf("[log] wifi disconnect\n");
            return 9;
        }
    }
    printf("[log] CTOS_WifiConnectAP done\n");
    usleep(2000 * 1000);
    
    char ip[128] = {0};
    nAPCount = sizeof(ip);
    start = time(0);
    while (1) {
        wError = CTOS_WifiConfigGet(d_WIFI_CONFIG_IP, ip, &nAPCount);
        if (d_OK != wError)
            printf("[log] get ip error: %#X\n", wError);
        
        if (0 < nAPCount) {
            printf("[log] connect wifi ok\n");
            printf("[log] wifi ip: %s\n", ip);
            break;
        }
        if (30 < time(0) - start)
        {
            printf("[log] get wifi ip timeout\n");
            return 11;
        }
        usleep(200 * 1000);
    }  
    
    usleep(2000 * 1000);
    
    return 0;
}


int OpenConnection(const char *hostname, int port)
{   
  int sd;
  struct hostent *host;
  struct sockaddr_in addr;
  if ((host = gethostbyname(hostname)) == NULL )
  {
    printf("[log] Eroor: %s\n", hostname);
    perror("gethostbyname");
    abort();
  }
  sd = socket(PF_INET, SOCK_STREAM|SOCK_NONBLOCK, 0);
  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = *(long*)(host->h_addr);
  if (connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
  if (EINPROGRESS != errno) {
      printf("[log] connect error: %d\n", errno);
      return -1;
  }
  
  struct timeval tmv = {30, 0};
  fd_set rset;
  fd_set wset;
  
  FD_ZERO(&rset);
  FD_ZERO(&wset);
  
  FD_SET(sd, &rset);
  FD_SET(sd, &wset);
  int ret = select(sd+1, &rset, &wset, NULL, &tmv);
  if (ret < 0) {
      printf("[log] select error: %d\n", errno);
      return -1;
  }
  
  int error = 0;
  int len = sizeof(error);
  ret = getsockopt(sd, SOL_SOCKET, SO_ERROR, &error, (socklen_t*)&len);
  if (ret < 0) {
      printf("[log] getsockopt error: %d\n", errno);
      return -1;
  }
  
  if (FD_ISSET(sd, &rset) || FD_ISSET(sd, &wset))
  {
      printf("[log] connect ok\n");
    return sd;
  }
  
  return -1;
}


SSL_CTX* InitCTX(void)
{   
  const SSL_METHOD *method = NULL;
  SSL_CTX *ctx;
  OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
  SSL_load_error_strings();   /* Bring in and register error messages */
  method = TLSv1_client_method();  /* Create new client-method instance */
  ctx = SSL_CTX_new(method);   /* Create new context */
  if ( ctx == NULL )
  {
    ERR_print_errors_fp(stderr);
    abort();
  }
  
  return ctx;
}


int LoadCerts(SSL_CTX* stCTX) 
{
    CHAR szPath[SSL_CA_PATH_LEN] = {0};
    int nLen = readlink("/proc/self/exe", szPath, sizeof(szPath));
    if (nLen < 0)
    {
        printf("[log] get ca path errno: %d\n", errno);
        return -1;
    }

    memset(szPath+nLen, 0, sizeof(szPath) - nLen);
    char* pSep = strrchr(szPath, '/');
    if (NULL == pSep)
    {
        printf("[log] invalid ca path\n");
        return -1;
    }

    memcpy(pSep+1, SSL_CLIENT_CA_PATH, strlen(SSL_CLIENT_CA_PATH)+1);
    printf("[log] ca path: %s\n", szPath);
    if (access(szPath, F_OK) < 0)
    {
        printf("[log] ca not exist\n");
        return -1;
    }

    /* 加载CA证书 和 private key */
    if (0 == SSL_CTX_load_verify_locations(stCTX, szPath, szPath))
    {
        printf("[log] locate ca: %s\n", SSL_ERR_STRINGS());
        return -1; 
    }

    if (0 == SSL_CTX_use_certificate_file(stCTX, szPath, SSL_FILETYPE_PEM))
    {
        printf("[log] load client ca: %s\n", SSL_ERR_STRINGS());
        return -1; 
    }

    if (0 == SSL_CTX_use_PrivateKey_file(stCTX, szPath, SSL_FILETYPE_PEM))
    {
        printf("[log] load client private key: %s\n", SSL_ERR_STRINGS());
        return -1; 
    }

    /* 验证private key 和证书 一致性 */
    if (0 == SSL_CTX_check_private_key(stCTX))
    {
        printf("[log] check ca and private key: %s\n", SSL_ERR_STRINGS());
        return -1; 
    }
    printf("[log] load client ca ok\n");
    return 0;
}


void ShowCerts(SSL* ssl)
{   
  X509 *cert;
  char *line;
  cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
  if ( cert != NULL )
  {
    printf("[log] Server certificates\n");
    line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
    printf("[log] Subject: %s\n", line);
    free(line);       /* free the malloc'ed string */
    line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
    printf("[log] Issuer: %s\n", line);
    free(line);       /* free the malloc'ed string */
    X509_free(cert);     /* free the malloc'ed certificate copy */
  }
  else
   printf("[log] No certificates\n");
}


int main(int count, char *strings[])
{   
  SSL_CTX *ctx;
  int server;
  SSL *ssl;
  char buf[1024];
  int bytes;
  char *hostname = NULL, *portnum = NULL;
  
  bytes = openwifi();
  if (0 < bytes) {
      printf("[log] open wifi error\n");
      if (4 < bytes)
        closewifi();
      return -1;
  }
  
  SSL_library_init();
  hostname="192.168.31.113";
  portnum="60011";
  ctx = InitCTX();
  if (LoadCerts(ctx) == -1)
  {
      printf("[log] load certs error\n");
      ERR_print_errors_fp(stderr);
      return 0;
  }
  
  server = OpenConnection(hostname, atoi(portnum));
  if (server < 0) {
      goto Close;
  }
  
  ssl = SSL_new(ctx);      /* create new SSL connection state */
  SSL_set_connect_state(ssl);
  SSL_set_fd(ssl, server);    /* attach the socket descriptor */
  SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
  int error = 0;
  int conn_error = 0;
  
  do
  {
    error = SSL_connect(ssl); /* perform the connection */
    if (error  == 1) {
        printf("[log] ssl connect ok\n");
        break;
    } else if (error  == -1) {
        conn_error = SSL_get_error(ssl, error);
        if (SSL_ERROR_WANT_READ == conn_error || SSL_ERROR_WANT_WRITE == conn_error) {
            printf("[log] ssl connect error\n");
            ERR_print_errors_fp(stderr);
            usleep(2000000);
            continue;
        }
    } else {
        if (0 != error)
            SSL_shutdown(ssl);
        SSL_free(ssl);
        printf("[log] ssl connect fatal\n");
        break;
    }
  } while(1);
  
    char *msg = "HelloWorld";
    printf("[log] Connected with %s encryption\n", SSL_get_cipher(ssl));
    ShowCerts(ssl);        /* get any certs */
    SSL_write(ssl, msg, strlen(msg));   /* encrypt & send message */
    bytes = SSL_read(ssl, buf, sizeof(buf)); /* get reply & decrypt */
    buf[bytes] = 0;
    printf("[log] Received: %s\n", buf);
    SSL_free(ssl);        /* release connection state */
    close(server);         /* close socket */

Close:  
  SSL_CTX_free(ctx);        /* release context */
  closewifi();
  return 0;
}
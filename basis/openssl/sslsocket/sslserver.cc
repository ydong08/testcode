
/* test ssl scoekt */
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include <sys/syscall.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <pthread.h>

#define SSL_CERT_PATH     "server.pem"

#define KERNEL_TID         syscall(SYS_gettid)

char resbuf[8] = {0x73, 0x65, 0x6E, 0x64, 0x21, 0x00, 0x00, 0x00};
void* DealData(void* p)
{
  pthread_detach(pthread_self());
  if (!p)
    pthread_exit(NULL);

  SSL* ssl = (SSL*)p;
  char databuf[1024] = {0};
  int ret = -1;
  int readerr = -1;
  do
  {
    memset(databuf, 0, sizeof(databuf));
	printf("ssl read in \n");
    ret = SSL_read(ssl, databuf, sizeof(databuf));
	printf("ssl read out \n");
    if (0 < ret) {
      printf("[%ld] recv: %s\n", KERNEL_TID, databuf);
      ret = SSL_write(ssl, resbuf, sizeof(resbuf));
      if (0 < ret) {
        printf("[%ld] send ok\n\n", KERNEL_TID);
      } else {
        ERR_print_errors_fp(stderr);
        printf("[%ld] send error: %d\n", KERNEL_TID, SSL_get_error(ssl, ret));
      }
    } else if (ret <= 0) {
      ERR_print_errors_fp(stderr);
      readerr = SSL_get_error(ssl, ret);
      switch(readerr)
      {
	case SSL_ERROR_NONE:
          break;
	case SSL_ERROR_WANT_READ:
	case SSL_ERROR_WANT_WRITE:
          printf("[%ld] read again\n", KERNEL_TID);
          usleep(100000);
          continue;
	
	case SSL_ERROR_ZERO_RETURN:
      	  printf("peer closed\n");
          break;
	default:
      	  printf("[%ld] read error %d, exit\n", KERNEL_TID, readerr);
          break;
      }
    } 
  } while (1);
  
  int sockfd = SSL_get_fd(ssl); /* get socket connection */
  SSL_free(ssl); /* release SSL state */
  close(sockfd); /* close connection */
         
  pthread_exit(NULL);

}


int OpenListener(int port)
{   
  int sd;
  struct sockaddr_in addr;
  sd = socket(PF_INET, SOCK_STREAM, 0);
  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;
  if (bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0)
  {
    perror("can't bind port");
    abort();
  }
  if ( listen(sd, 10) != 0 )
  {
    perror("can't configure listening port");
    abort();
  }
  return sd;
}


SSL_CTX* InitServerCTX(void)
{
  SSL_CTX *ctx = NULL;
#if OPENSSL_VERSION_NUMBER >= 0x10000000L
  const SSL_METHOD *method;
#else
  SSL_METHOD *method;
#endif

  SSL_library_init();
  OpenSSL_add_all_algorithms();  /* load & register all cryptos, etc. */
  SSL_load_error_strings();   /* load all error messages */
  method = TLSv1_server_method(); /* create new server-method instance */
  ctx = SSL_CTX_new(method);   /* create new context from method */
  if ( ctx == NULL )
  {
      ERR_print_errors_fp(stderr);
      abort();
  }
  return ctx;
}


void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile)
{
  /*get pem file path */
  char path[256] = {0};
  ssize_t  len = readlink("/proc/self/exe", path, sizeof(path));
  if (len < 0) {
    perror("readlink");
    abort();
  }
  if (sizeof(path) < strlen(path) + strlen(CertFile) - strlen("readlink")) {
    printf("insufficent buf\n");
    abort();
  }
  char* pos = strrchr(path, '/');
  memset(pos+1, 0, (char*)path + sizeof(path)-pos-1);
  memcpy(pos+1, CertFile, strlen(CertFile)+1);
  printf("[log] ca path:%s\n", path);
  if (access(path, F_OK) < 0) {
    printf("[log] ca file not found\n");
    abort();
  }

  if (SSL_CTX_load_verify_locations(ctx, path, NULL) != 1)
      ERR_print_errors_fp(stderr);
  

  if (SSL_CTX_set_default_verify_paths(ctx) != 1) 
      ERR_print_errors_fp(stderr);

  /* set the local certificate from CertFile */
  if ( SSL_CTX_use_certificate_file(ctx, path, SSL_FILETYPE_PEM) <= 0 )
  {
    ERR_print_errors_fp(stderr);
    abort();
  }
  /* set the private key from KeyFile (may be the same as CertFile) */
  if ( SSL_CTX_use_PrivateKey_file(ctx, path, SSL_FILETYPE_PEM) <= 0 )
  {
    ERR_print_errors_fp(stderr);
    abort();
  }
  /* verify private key */
  if ( !SSL_CTX_check_private_key(ctx) )
  {
    fprintf(stderr, "private key does not match the public certificaten");
    abort();
  }
  printf("[log] load cert complete ok\n");
}


void ShowCerts(SSL* ssl)
{   
  X509 *cert;
  char *line;
  cert = SSL_get_peer_certificate(ssl); /* Get certificates (if available) */
  if ( cert != NULL )
  {
    printf("[log] server certificates\n");
    line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
    printf("[log] subject: %s\n", line);
    free(line);
    line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
    printf("[log] issuer: %s\n", line);
    free(line);
    X509_free(cert);
  }
  else
    printf("[log] no cert\n");
}


void Servlet(SSL* ssl) /* Serve the connection -- threadable */
{   
  int ret = 0;
  pthread_t tid = 0;
  // const char* HTMLecho="<html><body><pre>%s</pre></body></html>nn";
  if ( SSL_accept(ssl) <= 0 )     /* do SSL-protocol accept */
    ERR_print_errors_fp(stderr);
  else
  { 
    printf("[log] connected with %s encryption\n", SSL_get_cipher(ssl));
    ShowCerts(ssl);        /* get any certificates */
    ret = pthread_create(&tid, NULL, DealData, ssl);
    if (ret != 0)
      printf("[log] create thread error\n");  
  }
}


int main(int argc, char **argv)
{   
  SSL_CTX *ctx;
  int server;
  char *portnum;
  if ( argc != 2 )
  {
    printf("Usage: %s <portnum>\n", argv[0]);
    exit(0);
  }
  SSL_library_init();
  printf("[log] ssl init done\n"); 
  portnum = argv[1];
  ctx = InitServerCTX();        /* initialize SSL */
  printf("[log] init ctx done\n");
  LoadCertificates(ctx, (char*)SSL_CERT_PATH, (char*)SSL_CERT_PATH);  /* load certs */
  printf("[log] load cert done\n");
  server = OpenListener(atoi(portnum));    /* create server socket */

  SSL *ssl = NULL;
  struct sockaddr_in addr;
  socklen_t len = sizeof(addr);
  int clientfd = 0;
  while (1)
  {   
    printf("[log] enter accept\n");
    clientfd = accept(server, (struct sockaddr*)&addr, &len);  /* accept connection as usual */
    printf("[log] connection: %s:%d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    ssl = SSL_new(ctx);              /* get new SSL state with context */
    SSL_set_fd(ssl, clientfd);       /* set connection socket to SSL state */
    Servlet(ssl);                    /* service connection */
  }

  close(server);            /* close server socket */
  SSL_CTX_free(ctx);         /* release context */
}




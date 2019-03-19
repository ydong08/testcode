
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
#include <openssl/ssl.h>
#include <openssl/err.h>

#define FAIL              -1
#define SSL_CERT_PATH     "/home/winter/Repo/testcode/basis/openssl/sslsocket/server.pem"

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
  if (SSL_CTX_load_verify_locations(ctx, CertFile, KeyFile) != 1)
      ERR_print_errors_fp(stderr);
  

  if (SSL_CTX_set_default_verify_paths(ctx) != 1) 
      ERR_print_errors_fp(stderr);

  /* set the local certificate from CertFile */
  if ( SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0 )
  {
    ERR_print_errors_fp(stderr);
    abort();
  }
  /* set the private key from KeyFile (may be the same as CertFile) */
  if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 )
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
  printf("load cert complete OK\n");
}


void ShowCerts(SSL* ssl)
{   
  X509 *cert;
  char *line;
  cert = SSL_get_peer_certificate(ssl); /* Get certificates (if available) */
  if ( cert != NULL )
  {
    printf("server certificates\n");
    line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
    printf("subject: %s\n", line);
    free(line);
    line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
    printf("issuer: %s\n", line);
    free(line);
    X509_free(cert);
  }
  else
    printf("no cert\n");
}


void Servlet(SSL* ssl) /* Serve the connection -- threadable */
{   
  char buf[1024];
  char reply[1024];
  int sd, bytes;
  const char* HTMLecho="<html><body><pre>%s</pre></body></html>nn";
  if ( SSL_accept(ssl) == FAIL )     /* do SSL-protocol accept */
    ERR_print_errors_fp(stderr);
  else
  {
	printf("connected with %s encryption\n", SSL_get_cipher(ssl));
    ShowCerts(ssl);        /* get any certificates */
    bytes = SSL_read(ssl, buf, sizeof(buf)); /* get request */
    if ( bytes > 0 )
    {
        buf[bytes] = 0;
        printf("client msg: %s\n", buf);
        sprintf(reply, HTMLecho, buf);   /* construct reply */
        SSL_write(ssl, reply, strlen(reply)); /* send reply */
    }
    else
        ERR_print_errors_fp(stderr);
  }
  sd = SSL_get_fd(ssl);       /* get socket connection */
  SSL_free(ssl);         /* release SSL state */
  close(sd);          /* close connection */
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
  printf("ssl init done\n"); 
  portnum = argv[1];
  ctx = InitServerCTX();        /* initialize SSL */
  printf("init ctx done\n");
  LoadCertificates(ctx, (char*)SSL_CERT_PATH, (char*)SSL_CERT_PATH);  /* load certs */
  printf("load cert done\n");
  server = OpenListener(atoi(portnum));    /* create server socket */

  SSL *ssl = NULL;
  struct sockaddr_in addr;
  socklen_t len = sizeof(addr);
  int clientfd = 0;
  while (1)
  {   
    printf("enter accept\n");
    clientfd = accept(server, (struct sockaddr*)&addr, &len);  /* accept connection   as usual */
    printf("connection: %s:%d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    ssl = SSL_new(ctx);              /* get new SSL state with context */
    SSL_set_fd(ssl, clientfd);      /* set connection socket to SSL state */
    Servlet(ssl);               /* service connection */
  }

  close(server);            /* close server socket */
  SSL_CTX_free(ctx);         /* release context */
}




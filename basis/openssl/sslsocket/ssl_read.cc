

ssize_t
ngx_ssl_recv(ngx_connection_t *c, u_char *buf, size_t size)
{
  int  n, bytes;
  if (c->ssl->last == NGX_ERROR) {
      c->read->error = 1;
      return NGX_ERROR;
  }
  if (c->ssl->last == NGX_DONE) {
      c->read->ready = 0;
      c->read->eof = 1;
      return 0;
  }

  bytes = 0;

  ngx_ssl_clear_error(c->log);

  /*
  * SSL_read() may return data in parts, so try to read
  * until SSL_read() would return no data
  */

  for ( ;; ) {
    n = SSL_read(c->ssl->connection, buf, size);
    if (n > 0) {
        bytes += n;
    }
    c->ssl->last = ngx_ssl_handle_recv(c, n);
    if (c->ssl->last == NGX_OK) {
        size -= n;
        if (size == 0) {
            c->read->ready = 1;
            return bytes;
        }
        buf += n;
        continue;
    }

    if (bytes) {
        if (c->ssl->last != NGX_AGAIN) {
            c->read->ready = 1;
        }
        return bytes;
    }

    switch (c->ssl->last) {
      case NGX_DONE:
          c->read->ready = 0;
          c->read->eof = 1;
          return 0;

      case NGX_ERROR:
          c->read->error = 1;

          /* fall through */

      case NGX_AGAIN:
          return c->ssl->last;
    }
  }
}



static ngx_int_t
ngx_ssl_handle_recv(ngx_connection_t *c, int n)
{
  int        sslerr;
  ngx_err_t  err;

  if (c->ssl->renegotiation) {
    /*
    * disable renegotiation (CVE-2009-3555):
    * OpenSSL (at least up to 0.9.8l) does not handle disabled
    * renegotiation gracefully, so drop connection here
    */

    ngx_log_error(NGX_LOG_NOTICE, c->log, 0, "SSL renegotiation disabled");

    while (ERR_peek_error()) {
        ngx_ssl_error(NGX_LOG_DEBUG, c->log, 0, "ignoring stale global SSL error");
    }

    ERR_clear_error();

    c->ssl->no_wait_shutdown = 1;
    c->ssl->no_send_shutdown = 1;

    return NGX_ERROR;
  }

  if (n > 0) {

    if (c->ssl->saved_write_handler) {

      c->write->handler = c->ssl->saved_write_handler;
      c->ssl->saved_write_handler = NULL;
      c->write->ready = 1;

      if (ngx_handle_write_event(c->write, 0) != NGX_OK) {
          return NGX_ERROR;
      }

      ngx_post_event(c->write, &ngx_posted_events);
    }

    return NGX_OK;
  }

  sslerr = SSL_get_error(c->ssl->connection, n);

  err = (sslerr == SSL_ERROR_SYSCALL) ? ngx_errno : 0;

  ngx_log_debug1(NGX_LOG_DEBUG_EVENT, c->log, 0, "SSL_get_error: %d", sslerr);

  if (sslerr == SSL_ERROR_WANT_READ) {
      c->read->ready = 0;
      return NGX_AGAIN;
  }

  if (sslerr == SSL_ERROR_WANT_WRITE) {

    ngx_log_error(NGX_LOG_INFO, c->log, 0,
                  "peer started SSL renegotiation");

    c->write->ready = 0;

    if (ngx_handle_write_event(c->write, 0) != NGX_OK) {
        return NGX_ERROR;
    }

    /*
    * we do not set the timer because there is already the read event timer
    */
    if (c->ssl->saved_write_handler == NULL) {
        c->ssl->saved_write_handler = c->write->handler;
        c->write->handler = ngx_ssl_write_handler;
    }

    return NGX_AGAIN;
  }

  c->ssl->no_wait_shutdown = 1;
  c->ssl->no_send_shutdown = 1;
  if (sslerr == SSL_ERROR_ZERO_RETURN || ERR_peek_error() == 0) {
    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, c->log, 0, "peer shutdown SSL cleanly");
    return NGX_DONE;
  }

  ngx_ssl_connection_error(c, sslerr, err, "SSL_read() failed");

  return NGX_ERROR;
}
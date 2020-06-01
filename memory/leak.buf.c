-----BEGIN PGP SIGNED MESSAGE-----

              BUFFER OVERRUN VULNERABILITIES IN KERBEROS

SUMMARY:

Serious buffer overrun vulnerabilities exist in many implementations
of Kerberos 4, including implementations included for backwards
compatibility in Kerberos 5 implementations.  Other less serious
buffer overrun vulnerabilites have also been discovered.  ALL KNOWN
KERBEROS 4 IMPLEMENTATIONS derived from MIT sources are believed to be
vulnerable.

IMPACT:

* A remote user may gain unauthorized root access to a machine running
  services authenticated with Kerberos 4.

* A remote user may gain unauthorized root access to a machine running
  krshd, regardless of whether the program is configured to accept
  Kerberos 4 authentication.

* A local user may gain unauthorized root access by exploiting v4rcp
  or ksu.

DETAILS:

The MIT Kerberos Team has been made aware of a security vulnerability
in the Kerberos 4 compatibility code contained within the MIT Kerberos
5 source distributions.  This vulnerability consists of a buffer
overrun in the krb_rd_req() function, which is used by essentially all
Kerberos-authenticated services that use Kerberos 4 for
authentication.  It is possible for an attacker to gain root access
over the network by exploiting this vulnerability.

An exploit is known to exist for the Kerberized Berkeley remote shell
daemon (krshd) for at least the i386-Linux platform, and possibly
others.  The extent of distribution of this exploit is unknown at this
time.

Other buffer overruns have been discovered as well, though with less
far-reaching impact.

The existing exploit does not directly use the buffer overrun in
krb_rd_req(); rather, it uses the buffer that was overrun by
krb_rd_req() to exploit a second overrun in krb425_conv_principal().
The krb_rd_req() code itself might not be exploitable once the overrun
in krb425_conv_principal() is repaired, though it is likely that some
other method of exploit may be found that does not require that an
overrun exist in krb425_conv_principal().

VULNERABLE DISTRIBUTIONS AND PROGRAMS:

Source distributions which may contain vulnerable code include:

       MIT Kerberos 5 releases krb5-1.0.x, krb5-1.1, krb5-1.1.1

       MIT Kerberos 4 patch 10, and likely earlier releases as well

       KerbNet (Cygnus implementation of Kerberos 5)

       Cygnus Network Security (CNS -- Cygnus implementation of
              Kerberos 4)

Daemons or services that may call krb_rd_req() and are thus vulnerable
to remote exploit include:

        krshd
        klogind (if accepting Kerberos 4 authentication)
        telnetd (if accepting Kerberos 4 authentication)
        ftpd (if accepting Kerberos 4 authentication)
        rkinitd
        kpopd

In addition, it is possible that the v4rcp program, which is usually
installed setuid to root, may be exploited by a local user to gain
root access by means of exploiting the krb_rd_req vulnerability.

The ksu program in some MIT Kerberos 5 releases has a vulnerability
that may result in unauthorized local root access.  This bug was fixed
in krb5-1.1.1, as well as in krb5-1.0.7-beta1.  Release krb5-1.1, as
well as krb5-1.0.6 and earlier, are believed to be vulnerable.

There is an unrelated buffer overrun in the krshd that is distributed
with at least the MIT Kerberos 5 source distributions.  It is not
known whether an exploit exists for this buffer overrun.  It is also
not known whether this buffer overrun is actually exploitable.

WORKAROUNDS:

Certain daemons that are called from inetd may be safe from
exploitation if their command line invocation is modified to exclude
the use of Kerberos 4 for authentication.  Please consult the manpages
or other documentation for your Kerberos distribution in order to
determine the correct command line for disabling Kerberos 4
authentication.  Daemons for which this approach may work include:

        krshd (*)
        klogind
        telnetd

(*) The krshd program may still be vulnerable to remote attack if
Kerberos 4 authentication is disabled, due to the unrelated buffer
overrun mentioned above.  It is best to disable the krshd program
completely until a patched version can be installed.

The v4rcp program should have its setuid permission removed, since it
may be possible to perform a local exploit against it.

The krb5 ksu program should have its setuid premission removed, if it
was not compiled from krb5-1.1.1, krb5-1.0.7-beta1, or later code.
Merely replacing the ksu binary with one compiled from krb5-1.1.1 or
krb5-1.0.7-beta1 should be safe, provided that it is not compiled with
shared libraries (the vulnerability is related to some library bugs).
If ksu was compiled with shared libraries, it may be best to install a
new release that has the library bug fixed.

In the MIT Kerberos 5 releases, it may not be possible to disable
Kerberos 4 authentication in the ftpd program.  Note that only
releases krb5-1.1 and later will have the ability to receive Kerberos
4 authentication.

FIXES:

The best course of action is to patch the code in the krb4 library, in
addition to patching the code in the krshd program.  The following
patches include some less essential patches that also affect buffer
overruns in potentially vulnerable code, but for which exploits are
somewhat more difficult to construct.

Please note that there are two sets of patches in this file that apply
against identically named files in two different releases.  You should
separate out the patch set that is relevant to you prior to applying
them; otherwise, you may inadvertently patch some files twice.

MIT will soon release krb5-1.2, which will have these changes
incorporated.

PATCHES AGAINST krb5-1.0.x:

The following are patches against 1.0.7-beta1 (roughly).  The most
critical ones are:

        appl/bsd/krshd.c
        lib/krb4/rd_req.c
        lib/krb5/krb/conv_princ.c

The rest are not as important but you may wish to apply them anyway
out of paranoia.  These patches may apply with a little bit of fuzz
against releases prior to krb5-1.0.7-beta1, but there likely have not
been significant changes in the affected code.  These patches may also
apply against KerbNet.  The lib/krb4/rd_req.c patch may also apply
against CNS and MIT Kerberos 4.

Index: appl/bsd/krshd.c
===================================================================
RCS file: /cvs/krbdev/krb5/src/appl/bsd/krshd.c,v
retrieving revision 5.66.2.6
diff -c -r5.66.2.6 krshd.c
*** krshd.c     1999/03/09 00:27:31     5.66.2.6
- --- krshd.c     2000/04/29 02:58:52
***************
*** 1469,1483 ****
          strcpy((char *) cmdbuf + offst, kprogdir);
        cp = copy + 3 + offst;

        if (auth_sys == KRB5_RECVAUTH_V4) {
!         strcat(cmdbuf, "/v4rcp");
        } else {
!         strcat(cmdbuf, "/rcp");
        }
        if (stat((char *)cmdbuf + offst, &s) >= 0)
!         strcat(cmdbuf, cp);
        else
!         strcpy(cmdbuf, copy);
        free(copy);
      }
  #endif
- --- 1469,1484 ----
          strcpy((char *) cmdbuf + offst, kprogdir);
        cp = copy + 3 + offst;

+       cmdbuf[sizeof(cmdbuf) - 1] = '\0';
        if (auth_sys == KRB5_RECVAUTH_V4) {
!         strncat(cmdbuf, "/v4rcp", sizeof(cmdbuf) - 1 - strlen(cmdbuf));
        } else {
!         strncat(cmdbuf, "/rcp", sizeof(cmdbuf) - 1 - strlen(cmdbuf));
        }
        if (stat((char *)cmdbuf + offst, &s) >= 0)
!         strncat(cmdbuf, cp, sizeof(cmdbuf) - 1 - strlen(cmdbuf));
        else
!         strncpy(cmdbuf, copy, sizeof(cmdbuf) - 1 - strlen(cmdbuf));
        free(copy);
      }
  #endif
Index: lib/krb4/kuserok.c
===================================================================
RCS file: /cvs/krbdev/krb5/src/lib/krb4/kuserok.c,v
retrieving revision 1.3
diff -c -r1.3 kuserok.c
*** kuserok.c   1996/01/27 06:06:22     1.3
- --- kuserok.c   2000/04/29 02:59:02
***************
*** 115,122 ****
      if ((pwd = getpwnam(luser)) == NULL) {
        return(NOTOK);
      }
!     (void) strcpy(pbuf, pwd->pw_dir);
!     (void) strcat(pbuf, "/.klogin");

      if (access(pbuf, F_OK)) {  /* not accessible */
        /*
- --- 115,125 ----
      if ((pwd = getpwnam(luser)) == NULL) {
        return(NOTOK);
      }
!     if (strlen (pwd->pw_dir) + sizeof ("/.klogin") >= sizeof (pbuf))
!       return NOTOK;
!     (void) strncpy(pbuf, pwd->pw_dir, sizeof(pbuf) - 1);
!     pbuf[sizeof(pbuf) - 1] = '\0';
!     (void) strncat(pbuf, "/.klogin", sizeof(pbuf) - 1 - strlen(pbuf));

      if (access(pbuf, F_OK)) {  /* not accessible */
        /*
Index: lib/krb4/rd_req.c
===================================================================
RCS file: /cvs/krbdev/krb5/src/lib/krb4/rd_req.c,v
retrieving revision 1.4
diff -c -r1.4 rd_req.c
*** rd_req.c    1996/02/24 14:29:26     1.4
- --- rd_req.c    2000/04/29 02:59:02
***************
*** 155,160 ****
- --- 155,162 ----
                                   Kerberos used to encrypt ticket */
      int status;

+     tkt->mbz = req_id->mbz = 0;
+
      if (authent->length <= 0)
        return(RD_AP_MODIFIED);

***************
*** 190,197 ****
          mutual = 0;
  #endif /* lint */
      s_kvno = *ptr++;          /* get server key version */
!     (void) strcpy(realm,ptr);   /* And the realm of the issuing KDC */
!     ptr += strlen(ptr) + 1;     /* skip the realm "hint" */

      /*
       * If "fn" is NULL, key info should already be set; don't
- --- 192,200 ----
          mutual = 0;
  #endif /* lint */
      s_kvno = *ptr++;          /* get server key version */
!     (void) strncpy(realm,ptr,REALM_SZ);       /* And the realm of the issuing KDC */
!     realm[REALM_SZ-1] = '\0';
!     ptr += strlen(realm) + 1; /* skip the realm "hint" */

      /*
       * If "fn" is NULL, key info should already be set; don't
***************
*** 277,289 ****
  #define check_ptr() if ((ptr - (char *) req_id->dat) > req_id->length) return(RD_AP_MODIFIED);

      ptr = (char *) req_id->dat;
!     (void) strcpy(r_aname,ptr);       /* Authentication name */
      ptr += strlen(r_aname)+1;
      check_ptr();
!     (void) strcpy(r_inst,ptr);        /* Authentication instance */
      ptr += strlen(r_inst)+1;
      check_ptr();
!     (void) strcpy(r_realm,ptr);       /* Authentication name */
      ptr += strlen(r_realm)+1;
      check_ptr();
      memcpy((char *)&ad->checksum, ptr, 4);    /* Checksum */
- --- 280,295 ----
  #define check_ptr() if ((ptr - (char *) req_id->dat) > req_id->length) return(RD_AP_MODIFIED);

      ptr = (char *) req_id->dat;
!     (void) strncpy(r_aname,ptr,ANAME_SZ); /* Authentication name */
!     r_aname[ANAME_SZ-1] = '\0';
      ptr += strlen(r_aname)+1;
      check_ptr();
!     (void) strncpy(r_inst,ptr,INST_SZ);       /* Authentication instance */
!     r_inst[INST_SZ-1] = '\0';
      ptr += strlen(r_inst)+1;
      check_ptr();
!     (void) strncpy(r_realm,ptr,REALM_SZ); /* Authentication name */
!     r_realm[REALM_SZ-1] = '\0';
      ptr += strlen(r_realm)+1;
      check_ptr();
      memcpy((char *)&ad->checksum, ptr, 4);    /* Checksum */
Index: lib/krb5/krb/conv_princ.c
===================================================================
RCS file: /cvs/krbdev/krb5/src/lib/krb5/krb/conv_princ.c,v
retrieving revision 1.19.8.1
diff -c -r1.19.8.1 conv_princ.c
*** conv_princ.c        1999/02/07 00:52:01     1.19.8.1
- --- conv_princ.c        2000/04/29 02:59:04
***************
*** 243,249 ****
              if (retval == 0 && full_name && full_name[0]) {
                  instance = full_name[0];
              } else {
!                 strcpy(buf, instance);
                  retval = krb5_get_realm_domain(context, realm, &domain);
                  if (retval)
                      return retval;
- --- 243,250 ----
              if (retval == 0 && full_name && full_name[0]) {
                  instance = full_name[0];
              } else {
!                 strncpy(buf, instance, sizeof(buf));
!                 buf[sizeof(buf) - 1] = '\0';
                  retval = krb5_get_realm_domain(context, realm, &domain);
                  if (retval)
                      return retval;
***************
*** 251,258 ****
                      for (cp = domain; *cp; cp++)
                          if (isupper(*cp))
                              *cp = tolower(*cp);
!                     strcat(buf, ".");
!                     strcat(buf, domain);
                      krb5_xfree(domain);
                  }
                  instance = buf;
- --- 252,259 ----
                      for (cp = domain; *cp; cp++)
                          if (isupper(*cp))
                              *cp = tolower(*cp);
!                     strncat(buf, ".", sizeof(buf) - 1 - strlen(buf));
!                     strncat(buf, domain, sizeof(buf) - 1 - strlen(buf));
                      krb5_xfree(domain);
                  }
                  instance = buf;
Index: lib/krb5/os/kuserok.c
===================================================================
RCS file: /cvs/krbdev/krb5/src/lib/krb5/os/kuserok.c,v
retrieving revision 5.19
diff -c -r5.19 kuserok.c
*** kuserok.c   1996/06/12 05:15:02     5.19
- --- kuserok.c   2000/04/29 02:59:04
***************
*** 77,84 ****
      if ((pwd = getpwnam(luser)) == NULL) {
        return(FALSE);
      }
!     (void) strcpy(pbuf, pwd->pw_dir);
!     (void) strcat(pbuf, "/.k5login");

      if (access(pbuf, F_OK)) {  /* not accessible */
        /*
- --- 77,85 ----
      if ((pwd = getpwnam(luser)) == NULL) {
        return(FALSE);
      }
!     (void) strncpy(pbuf, pwd->pw_dir, sizeof(pbuf) - 1);
!     pbuf[sizeof(pbuf) - 1] = '\0';
!     (void) strncat(pbuf, "/.k5login", sizeof(pbuf) - 1 - strlen(pbuf));

      if (access(pbuf, F_OK)) {  /* not accessible */
        /*
Index: lib/krb5/posix/syslog.c
===================================================================
RCS file: /cvs/krbdev/krb5/src/lib/krb5/posix/syslog.c,v
retrieving revision 5.7
diff -c -r5.7 syslog.c
*** syslog.c    1996/06/12 05:16:04     5.7
- --- syslog.c    2000/04/29 02:59:04
***************
*** 115,121 ****
        (void)sprintf(tbuf, "<%d>%.15s ", pri, ctime(&now) + 4);
        for (p = tbuf; *p; ++p);
        if (LogTag) {
!               (void)strcpy(p, LogTag);
                for (; *p; ++p);
        }
        if (LogStat & LOG_PID) {
- --- 115,121 ----
        (void)sprintf(tbuf, "<%d>%.15s ", pri, ctime(&now) + 4);
        for (p = tbuf; *p; ++p);
        if (LogTag) {
!               (void)strncpy(p, LogTag, sizeof(tbuf) - 1 - (p - tbuf));
                for (; *p; ++p);
        }
        if (LogStat & LOG_PID) {
***************
*** 146,151 ****
- --- 146,156 ----
        }

        (void)vsprintf(p, fmt_cpy, ap);
+       /* Bounds checking??  If a system doesn't have syslog, we
+          probably can't rely on it having vsnprintf either.  Try not
+          to let a buffer overrun be exploited.  */
+       if (strlen (tbuf) >= sizeof (tbuf))
+         abort ();

        /* output the message to the local logger */
        if (send(LogFile, tbuf, cnt = strlen(tbuf), 0) >= 0 ||
***************
*** 169,175 ****
                if ((fd = open(CONSOLE, O_WRONLY, 0)) < 0)
                        return;
                (void)alarm((u_int)0);
!               (void)strcat(tbuf, "\r");
                p = strchr(tbuf, '>') + 1;
                (void)write(fd, p, cnt + 1 - (p - tbuf));
                (void)close(fd);
- --- 174,181 ----
                if ((fd = open(CONSOLE, O_WRONLY, 0)) < 0)
                        return;
                (void)alarm((u_int)0);
!               tbuf[sizeof(tbuf) - 1] = '\0';
!               (void)strncat(tbuf, "\r", sizeof(tbuf) - 1 - strlen(tbuf));
                p = strchr(tbuf, '>') + 1;
                (void)write(fd, p, cnt + 1 - (p - tbuf));
                (void)close(fd);

PATCHES AGAINST krb5-1.1.1:

The following are patches against 1.1.1.  The most critical ones are:

        appl/bsd/krshd.c
        lib/krb4/rd_req.c
        lib/krb5/krb/conv_princ.c

The rest are not as important but you may wish to apply them anyway
out of paranoia.

Index: appl/bsd/krshd.c
===================================================================
RCS file: /cvs/krbdev/krb5/src/appl/bsd/krshd.c,v
retrieving revision 5.79.2.1
diff -c -r5.79.2.1 krshd.c
*** krshd.c     1999/08/23 18:55:10     5.79.2.1
- --- krshd.c     2000/04/29 03:00:38
***************
*** 1468,1482 ****
          strcpy((char *) cmdbuf + offst, kprogdir);
        cp = copy + 3 + offst;

        if (auth_sys == KRB5_RECVAUTH_V4) {
!         strcat(cmdbuf, "/v4rcp");
        } else {
!         strcat(cmdbuf, "/rcp");
        }
        if (stat((char *)cmdbuf + offst, &s) >= 0)
!         strcat(cmdbuf, cp);
        else
!         strcpy(cmdbuf, copy);
        free(copy);
      }
  #endif
- --- 1468,1483 ----
          strcpy((char *) cmdbuf + offst, kprogdir);
        cp = copy + 3 + offst;

+       cmdbuf[sizeof(cmdbuf) - 1] = '\0';
        if (auth_sys == KRB5_RECVAUTH_V4) {
!         strncat(cmdbuf, "/v4rcp", sizeof(cmdbuf) - 1 - strlen(cmdbuf));
        } else {
!         strncat(cmdbuf, "/rcp", sizeof(cmdbuf) - 1 - strlen(cmdbuf));
        }
        if (stat((char *)cmdbuf + offst, &s) >= 0)
!         strncat(cmdbuf, cp, sizeof(cmdbuf) - 1 - strlen(cmdbuf));
        else
!         strncpy(cmdbuf, copy, sizeof(cmdbuf) - 1 - strlen(cmdbuf));
        free(copy);
      }
  #endif
Index: lib/krb4/kuserok.c
===================================================================
RCS file: /cvs/krbdev/krb5/src/lib/krb4/kuserok.c,v
retrieving revision 1.5
diff -c -r1.5 kuserok.c
*** kuserok.c   1997/09/26 02:41:41     1.5
- --- kuserok.c   2000/04/29 03:00:53
***************
*** 118,125 ****
      if ((pwd = getpwnam(luser)) == NULL) {
        return(NOTOK);
      }
!     (void) strcpy(pbuf, pwd->pw_dir);
!     (void) strcat(pbuf, "/.klogin");

      if (access(pbuf, F_OK)) {  /* not accessible */
        /*
- --- 118,128 ----
      if ((pwd = getpwnam(luser)) == NULL) {
        return(NOTOK);
      }
!     if (strlen (pwd->pw_dir) + sizeof ("/.klogin") >= sizeof (pbuf))
!       return NOTOK;
!     (void) strncpy(pbuf, pwd->pw_dir, sizeof(pbuf) - 1);
!     pbuf[sizeof(pbuf) - 1] = '\0';
!     (void) strncat(pbuf, "/.klogin", sizeof(pbuf) - 1 - strlen(pbuf));

      if (access(pbuf, F_OK)) {  /* not accessible */
        /*
Index: lib/krb4/rd_req.c
===================================================================
RCS file: /cvs/krbdev/krb5/src/lib/krb4/rd_req.c,v
retrieving revision 1.9
diff -c -r1.9 rd_req.c
*** rd_req.c    1999/02/09 02:57:14     1.9
- --- rd_req.c    2000/04/29 03:00:53
***************
*** 184,189 ****
- --- 184,191 ----
      krb5_keyblock keyblock;
      int status;

+     tkt->mbz = req_id->mbz = 0;
+
      if (authent->length <= 0)
        return(RD_AP_MODIFIED);

***************
*** 219,226 ****
          mutual = 0;
  #endif /* lint */
      s_kvno = *ptr++;          /* get server key version */
!     (void) strcpy(realm,ptr);   /* And the realm of the issuing KDC */
!     ptr += strlen(ptr) + 1;     /* skip the realm "hint" */

      /*
       * If "fn" is NULL, key info should already be set; don't
- --- 221,229 ----
          mutual = 0;
  #endif /* lint */
      s_kvno = *ptr++;          /* get server key version */
!     (void) strncpy(realm,ptr,REALM_SZ);       /* And the realm of the issuing KDC */
!     realm[REALM_SZ-1] = '\0';
!     ptr += strlen(realm) + 1; /* skip the realm "hint" */

      /*
       * If "fn" is NULL, key info should already be set; don't
***************
*** 324,336 ****
  #define check_ptr() if ((ptr - (char *) req_id->dat) > req_id->length) return(RD_AP_MODIFIED);

      ptr = (char *) req_id->dat;
!     (void) strcpy(r_aname,ptr);       /* Authentication name */
      ptr += strlen(r_aname)+1;
      check_ptr();
!     (void) strcpy(r_inst,ptr);        /* Authentication instance */
      ptr += strlen(r_inst)+1;
      check_ptr();
!     (void) strcpy(r_realm,ptr);       /* Authentication name */
      ptr += strlen(r_realm)+1;
      check_ptr();
      memcpy((char *)&ad->checksum, ptr, 4);    /* Checksum */
- --- 327,342 ----
  #define check_ptr() if ((ptr - (char *) req_id->dat) > req_id->length) return(RD_AP_MODIFIED);

      ptr = (char *) req_id->dat;
!     (void) strncpy(r_aname,ptr,ANAME_SZ); /* Authentication name */
!     r_aname[ANAME_SZ-1] = '\0';
      ptr += strlen(r_aname)+1;
      check_ptr();
!     (void) strncpy(r_inst,ptr,INST_SZ);       /* Authentication instance */
!     r_inst[INST_SZ-1] = '\0';
      ptr += strlen(r_inst)+1;
      check_ptr();
!     (void) strncpy(r_realm,ptr,REALM_SZ); /* Authentication name */
!     r_realm[REALM_SZ-1] = '\0';
      ptr += strlen(r_realm)+1;
      check_ptr();
      memcpy((char *)&ad->checksum, ptr, 4);    /* Checksum */
Index: lib/krb5/krb/conv_princ.c
===================================================================
RCS file: /cvs/krbdev/krb5/src/lib/krb5/krb/conv_princ.c,v
retrieving revision 1.23.2.2
diff -c -r1.23.2.2 conv_princ.c
*** conv_princ.c        1999/10/12 23:16:58     1.23.2.2
- --- conv_princ.c        2000/04/29 03:00:55
***************
*** 234,240 ****
              if (retval == 0 && full_name && full_name[0]) {
                  instance = full_name[0];
              } else {
!                 strcpy(buf, instance);
                  retval = krb5_get_realm_domain(context, realm, &domain);
                  if (retval)
                      return retval;
- --- 234,241 ----
              if (retval == 0 && full_name && full_name[0]) {
                  instance = full_name[0];
              } else {
!                 strncpy(buf, instance, sizeof(buf));
!                 buf[sizeof(buf) - 1] = '\0';
                  retval = krb5_get_realm_domain(context, realm, &domain);
                  if (retval)
                      return retval;
***************
*** 242,249 ****
                      for (cp = domain; *cp; cp++)
                          if (isupper(*cp))
                              *cp = tolower(*cp);
!                     strcat(buf, ".");
!                     strcat(buf, domain);
                      krb5_xfree(domain);
                  }
                  instance = buf;
- --- 243,250 ----
                      for (cp = domain; *cp; cp++)
                          if (isupper(*cp))
                              *cp = tolower(*cp);
!                     strncat(buf, ".", sizeof(buf) - 1 - strlen(buf));
!                     strncat(buf, domain, sizeof(buf) - 1 - strlen(buf));
                      krb5_xfree(domain);
                  }
                  instance = buf;
Index: lib/krb5/os/kuserok.c
===================================================================
RCS file: /cvs/krbdev/krb5/src/lib/krb5/os/kuserok.c,v
retrieving revision 5.20.4.2
diff -c -r5.20.4.2 kuserok.c
*** kuserok.c   1999/09/23 00:50:45     5.20.4.2
- --- kuserok.c   2000/04/29 03:00:55
***************
*** 80,87 ****
      if ((pwd = getpwnam(luser)) == NULL) {
        return(FALSE);
      }
!     (void) strcpy(pbuf, pwd->pw_dir);
!     (void) strcat(pbuf, "/.k5login");

      if (access(pbuf, F_OK)) {  /* not accessible */
        /*
- --- 80,88 ----
      if ((pwd = getpwnam(luser)) == NULL) {
        return(FALSE);
      }
!     (void) strncpy(pbuf, pwd->pw_dir, sizeof(pbuf) - 1);
!     pbuf[sizeof(pbuf) - 1] = '\0';
!     (void) strncat(pbuf, "/.k5login", sizeof(pbuf) - 1 - strlen(pbuf));

      if (access(pbuf, F_OK)) {  /* not accessible */
        /*
Index: lib/krb5/posix/syslog.c
===================================================================
RCS file: /cvs/krbdev/krb5/src/lib/krb5/posix/syslog.c,v
retrieving revision 5.8
diff -c -r5.8 syslog.c
*** syslog.c    1998/07/17 20:39:43     5.8
- --- syslog.c    2000/04/29 03:00:55
***************
*** 115,121 ****
        (void)sprintf(tbuf, "<%d>%.15s ", pri, ctime(&now) + 4);
        for (p = tbuf; *p; ++p);
        if (LogTag) {
!               (void)strcpy(p, LogTag);
                for (; *p; ++p);
        }
        if (LogStat & LOG_PID) {
- --- 115,121 ----
        (void)sprintf(tbuf, "<%d>%.15s ", pri, ctime(&now) + 4);
        for (p = tbuf; *p; ++p);
        if (LogTag) {
!               (void)strncpy(p, LogTag, sizeof(tbuf) - 1 - (p - tbuf));
                for (; *p; ++p);
        }
        if (LogStat & LOG_PID) {
***************
*** 146,151 ****
- --- 146,156 ----
        }

        (void)vsprintf(p, fmt_cpy, ap);
+       /* Bounds checking??  If a system doesn't have syslog, we
+          probably can't rely on it having vsnprintf either.  Try not
+          to let a buffer overrun be exploited.  */
+       if (strlen (tbuf) >= sizeof (tbuf))
+         abort ();

        /* output the message to the local logger */
        if (send(LogFile, tbuf, cnt = strlen(tbuf), 0) >= 0 ||
***************
*** 169,175 ****
                if ((fd = open(CONSOLE, O_WRONLY, 0)) < 0)
                        return;
                (void)alarm((u_int)0);
!               (void)strcat(tbuf, "\r");
                p = strchr(tbuf, '>') + 1;
                (void)write(fd, p, cnt + 1 - (p - tbuf));
                (void)close(fd);
- --- 174,181 ----
                if ((fd = open(CONSOLE, O_WRONLY, 0)) < 0)
                        return;
                (void)alarm((u_int)0);
!               tbuf[sizeof(tbuf) - 1] = '\0';
!               (void)strncat(tbuf, "\r", sizeof(tbuf) - 1 - strlen(tbuf));
                p = strchr(tbuf, '>') + 1;
                (void)write(fd, p, cnt + 1 - (p - tbuf));
                (void)close(fd);

ACKNOWLEDGEMENTS:

Thanks to Jim Paris <jim@jtan.com> (MIT class of 2003) for pointing
out the krb_rd_req() vulnerability.

Thanks to Nalin Dahyabhai of Redhat for pointing out some other buffer
overruns and coming up with patches.

-----BEGIN PGP SIGNATURE-----
Version: PGP for Personal Privacy 5.0
Comment: Processed by Mailcrypt 3.5b6, an Emacs/PGP interface
Charset: noconv

iQCVAwUBOSGcyMUtR20Nv5BtAQEMxgP9EE0BPsV74PIzQsuf0YXK41x4eeMzAwJ0
IDn/PyfqvdAu6pdhf9CaQSMEDCdfdnsP/TYe7Kj3dmI3ijJUsSbKiovBF1pUTWat
tcQebpaL/z9AfVvROXGhINifXyHv2FlQ6sLbfC+1N4XFafDIs2j1EaPsp+Mk83mY
uy8Ud3DQ6FY=
=bHIH
-----END PGP SIGNATURE-----
#ifndef PDNC
// dnc includes dns (currently commented out)
#include "code.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "fvar.h"

#define BRN	"<br>\n"

CH streq(CH *str1, CH *str2, CH trigger) {
  IF (!str1 OR !str2) { RT 'N'; } // null string not equal
  WI (str1[0] NQ '\0' OR str2[0] NQ '\0') {
    IF (OREQ2(trigger, str1[0], str2[0])) { BK; }
    IF (str1[0] EQ '\0') { RT '1'; }  // 1 too short
    EF (str2[0] EQ '\0') { RT '2'; }  // 2 too short
    IF (str1[0] NQ str2[0]) { RT 'C'; }    // unmatching char
    INC str1; INC str2;
  }
  IF (OREQ2(str1[0], NUL, trigger) AND OREQ2(str2[0], NUL, trigger))
    { RT '='; } EL { RT '?'; }
}


#include "hash64.c"
#define DNSHASHINITSIZE		((u_int64_t)14695981039346656037ULL)
#define DNSHASHTRUESIZE		((u_int64_t)218340105584896ULL)
#define DNSLABELSIZE		8
#define DNSVALUESIZE		64

// label hashes are 8-byte, 62 bit-combinations per byte
// labelout should be exactly 9 characters wide, with [8] = '\0'
CH dnstoken(CH *valuein, CH *tokenout) {
  CH *charset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  IN valuelen = 0; WI (valuein[valuelen] NQ '\0') { valuelen++; }
  u_int64_t hashtotal = fnv_64_buf(valuein, valuelen, DNSHASHINITSIZE);
  hashtotal %= DNSHASHTRUESIZE;
  CH hashlabel[DNSLABELSIZE WITHNUL];
  hashlabel[DNSLABELSIZE] = '\0';
  hashlabel[0] = hashlabel[1] = hashlabel[2] = hashlabel[3] = '@';
  hashlabel[4] = hashlabel[5] = hashlabel[6] = hashlabel[7] = '@';
  IN hashindex = DNSLABELSIZE, hashdiff = 0;
  WI (--hashindex >= 0) {
    hashdiff = (int)(hashtotal % 62);
    hashlabel[hashindex] = charset[hashdiff];
    tokenout[hashindex] = charset[hashdiff];
    hashtotal /= 62;
  }
  tokenout[DNSLABELSIZE] = '\0';
  RT (hashtotal == 0) ? 'l' : '-';
}

CH dnsrotate(CH *tokenin, CH *host, SCH dir, CH *tokenout) {
  CH *charset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  IN charsetlen = 62;
  IN nibcount = 0;
  IN tix = 0; // token index
  IN hix = -1; // host index
  WI (INC hix GQ 0) {
    IF (host[hix] EQ '.' OR host[hix] EQNUL) {
      IN csi = 0; // charset index
      CH setchar = charset[csi];
      WI (setchar NQNUL AND setchar NQ tokenin[tix])
        { setchar = charset[INC csi]; }
      IF (dir EQ -1) {
        csi = (csi + charsetlen - (nibcount % charsetlen)) % charsetlen;
      } EL { csi = (csi + (nibcount % charsetlen)) % charsetlen; }
      tokenout[tix] = charset[csi]; 
      tokenout[tix + 1] = '\0';
      nibcount = 0;
      IF (INC tix GT DNSLABELSIZE) { RT '!'; }
    } EF INRANGE(host[hix], A0, A9) {
      nibcount = (nibcount * 10) + (host[hix] - A0);
    } EL { tokenout[0] = '!'; tokenout[1] = NUL; RT '!'; }
    IF (host[hix] EQNUL) { BK; }
  }
  RT Y;
}

VD timestr(CH *timebuf, IN buflen) {
  struct timeval tmnow;
  struct tm *tmloc;
  gettimeofday(&tmnow, NULL);
  tmloc = localtime(&tmnow.tv_sec);
  strftime(timebuf, buflen, "%Y.%m.%d.%H.%M.%S", tmloc);
  IN tlen = -1; WI (timebuf[INC tlen] NQNUL);
  sprintf(&timebuf[tlen], ".%06d", (int)tmnow.tv_usec);
  timebuf[tlen + 7] = timebuf[tlen + 6];
  timebuf[tlen + 6] = timebuf[tlen + 5];
  timebuf[tlen + 5] = timebuf[tlen + 4];
  timebuf[tlen + 4] = '.';
  timebuf[tlen + 8] = '\0';
}
// ^ map function in/out history (in, out <- timebuf, in/out)

#define FVARPATHLEN	256
#define FVARBUFFERLEN	256

CH *homepath = ""; // domainregion
// ^^ consider including ~/ as prefix or hard equivalent
// currently runs as root user

#include "PRUNPORT.h"
CH *storagepath = PRUNDNSSTORAGEPATH; // webmap directory
CH *memorypath  = PRUNDNSMEMORYPATH;  // memory directory

#ifndef PDNC
IN main(IN argc, CH **argv) {
  // -- REQUEST AND CONTEXT --------------------------------------------
  CH *gateway = getenv("GATEWAY_INTERFACE");
  CH *httpaccept = getenv("HTTP_ACCEPT");
  CH *httpuseragent = "N/A"; // getenv("HTTP_USER_AGENT");
// ^ comment a declaration and all lines that refer to it
  IF (!httpuseragent) { httpuseragent = "N/A"; }
  CH *path = getenv("PATH");
  CH *remoteaddr = getenv("REMOTE_ADDR");
  IF (!remoteaddr) { remoteaddr = "127.0.0.1"; }
  CH *remotehost = ""; // getenv("REMOTE_HOST");
  IF (!remotehost) { remotehost = ""; }
//  CH *requestmethod = getenv("REQUEST_METHOD");
//  IF (!requestmethod) { requestmethod = "CMD"; }
//  CH *scriptname = getenv("SCRIPT_NAME");
//  CH *servername = getenv("SERVER_NAME");
//  CH *serverport = getenv("SERVER_PORT");
//  CH *serverprotocol = getenv("SERVER_PROTOCOL");
//  CH *serversoftware = getenv("SERVER_SOFTWARE");
  // querystring = name[&newtoken[&suppdata]]
  CH *querystring = (argc > 1) ? argv[1] : NULL;
  IF (!querystring) { querystring = getenv("QUERY_STRING"); }
  IF (!querystring) { querystring = ""; }
  CH *querystrend = querystring;
  INCONTO(querystrend, '\n');
  IF (querystrend[0] EQ '\n')
    { querystrend[0] = NUL; }
  CH *suppdata = "";
  CH *newtoken = querystring;
  INCONTO(newtoken, '&');
  IF (newtoken[0] EQ '&')
    { newtoken[0] = NUL; }
  CH *tokenend = newtoken;
  INCONTO(tokenend, '&');
  IN tokenlen = tokenend - newtoken;
  IF (tokenend[0] EQ '&') {
    tokenend[0] = NUL;
    suppdata = &tokenend[1];
  }
  // querytime = yyyy.mm.dd.hh.nn.ss.lll.uuu
  // Years.Months.Days.Hours.miNutes.Seconds.miLliseconds.micrUseconds
  CH querytime[DNSVALUESIZE];
  timestr(querytime, DNSVALUESIZE);
  CH *contentlength = getenv("CONTENT_LENGTH");
  IN contentsize = (contentlength) ? atoi(contentlength) : 0;
  LOG("Content-Type: text/html\r\n");
  LOG("Cache-Control: no-cache, no-store, must-revalidate\r\n");
//  LOG("Content-Encoding: identity\r\n"); breaks some devices
  LOG("Pragma: no-cache\r\n");
  LOG("Expires: 0\r\n\r\n");
  LOG("<html><body>\n");
//  LOG1("{ THIS }%s", BRN);
//  LOG2("{ path: \"%s\" }%s", path, BRN);
//  LOG4("{ servername: \"%s:%s%s\" }%s", servername, serverport, scriptname, BRN);
//  LOG4("{ servergate: \"%s/%s/%s\" }%s", serversoftware, serverprotocol, gateway, BRN);
//  LOG2("{ httpaccept: \"%s\" }%s", httpaccept, BRN);
//  LOG1("{ THAT }%s", BRN);
//  LOG2("{ httpuagent: \"%s\" }%s", httpuseragent, BRN);
//  LOG2("{ remoteaddr: \"%s\" }%s", remoteaddr, BRN);
//  LOG2("{ remotehost: \"%s\" }%s", remotehost, BRN);
//  LOG2("{ requmethod: \"%s\" }%s", requestmethod, BRN);
//  LOG2("{ queryvalue: \"%s\" }%s", querystring, BRN);
  LOG2("{ hooktime: \"%s\" }%s", querytime, BRN);
//  LOG1("{ THEN }%s", BRN);
//  IN qse = -1; IN qset = 0;
//  WI (querystring[++qse] NQNUL)
//    { IF (querystring[qse] EQ '=') { qset = 1; BK; } }
//  IF (qset EQ 1) { querystring[qse] = '\0'; }
  CH memopath[FVARPATHLEN];
  CH memodata[FVARBUFFERLEN];
  CH filepath[FVARPATHLEN];
  CH filedata[FVARBUFFERLEN];
  CH nametoken[DNSLABELSIZE WITHNUL];
  CH sendtoken[DNSLABELSIZE WITHNUL];
  CH savetoken[FVARBUFFERLEN];
  CH tokendata[FVARBUFFERLEN];

// handle new value assignment/update, then display map
  IF (querystring[0] NQNUL) {
    STR3(filepath, homepath, storagepath, querystring);
    LOG2("{ filepath: \"%s\" }%s", filepath, BRN);
//  IF (qset EQ 1) {
//    IF (!fvarset(filepath, &querystring[qse + 1]))
//      { RLOG(">!"); }
//  }
  // -- HANDLING AND RESPONSE ----------red/blue--------------------------
// COMMENT/code , comment/CODE (shift+tab to change mode)
// split code mode, but also nested code mode more importantly
// standard information:
//   dnshostname, remoteaddr, token, time, dnshostdomain, dnshostaddress, access status/history
// context:
//   i am here and i rotate my token with each response
// possible responses:
//   you are not recognised, but here is a token
//   you are recognised but tokenless, here is a token
//  ^^ but actually don't tell me that token again. I will find it from remoteaddr->dnshostname
//  ^^ no, send me a rotated token so that I can unrotate to find the old key to .. the time..
//
//   you are recognised, and your rotated token looks good
//   you are recognised, but there is a* problem(s) with your token (* are many)
//   you are recognised, and the given address assignment has been activated
//   you are recognised, but you do not have access to that domain
// note: to circumvent, simply guess the 8 character access address, and rotate
//       your token accordingly to match the server's rotation
//   ->token=...currently querystring(->)time (use remoteaddr), saved at /web/dns/host/name
//   <-rotate by time of new request, send difference from last response to match
// server should remember previous request time at old token value
// server rotates difference forward to check new token value
// server rotates difference backwards to ....................
// server needs to remember the time of previous response
//   prevtime+difference=newtime
//   remoteaddr->dnshostname<-newtime=
// dnes domain name entanglement server
// no address - give token, remember address
//    address - check/update address, deny token
    IF (!fvarget(filepath, filedata)) {
      // no address, token available
      IF (newtoken[0] EQNUL) {
        // no current token
        dnstoken(querystring, nametoken);
        dnsrotate(nametoken, querytime, -1, sendtoken);
        dnsrotate(sendtoken, querytime, -1, savetoken);
        STR3(memopath, homepath, memorypath, savetoken);
        LOG2("{ memopath: \"%s\" }%s", memopath, BRN);
        IF (fvarset(memopath, querytime)) {
          IF (fvarset(filepath, remoteaddr)) {
            LOG2("{ newtoken: \"%s\" }%s", sendtoken, BRN);
          } EL { LOG("Failed to assign.\n"); }
        } EL { LOG("Failed to remember.\n"); }
      } EL {
        LOG1("Token where no token. %s\n", newtoken);
      }
    } EL {
      // address exists, read or update
      IF (newtoken[0] EQNUL) {
        // no current token <--- allow if not secured
        LOG("{ newtoken: \"notoken\" }\n");
      } EF (tokenlen NQ DNSLABELSIZE) {
        LOG("{ newtoken: \"badtoken\" }\n");
      } EL {
        // could retrieve moAr info by post [ fgetc(stdin) ]	
        STR3(memopath, homepath, memorypath, newtoken);
        LOG2("{ remopath: \"%s\" }%s", memopath, BRN);
        IF (!fvarget(filepath, memodata))
          { LOG("Fail token.\n"); }
        EL {
          // use memodata to check expiry
          LOG2("{ lastupdate: %s }%s", memodata, BRN);
          LOG2("{ thisupdate: %s }%s", querytime, BRN);
          CH *lyc = memodata;               IN ly = atoi(lyc);
          CH *lmc = lyc; INCTHRU(lmc, '.'); IN lm = atoi(lmc);
          CH *ldc = lmc; INCTHRU(ldc, '.'); IN ld = atoi(ldc);
          CH *lhc = ldc; INCTHRU(lhc, '.'); IN lh = atoi(lhc);
          CH *lnc = lhc; INCTHRU(lnc, '.'); IN ln = atoi(lnc);
          CH *lsc = lnc; INCTHRU(lsc, '.'); IN ls = atoi(lsc);
          CH *llc = lsc; INCTHRU(llc, '.'); IN ll = atoi(llc);
          CH *luc = llc; INCTHRU(luc, '.'); IN lu = atoi(luc);
          CH *tyc = querytime;              IN ty = atoi(tyc);
          CH *tmc = tyc; INCTHRU(tmc, '.'); IN tm = atoi(tmc);
          CH *tdc = tmc; INCTHRU(tdc, '.'); IN td = atoi(tdc);
          CH *thc = tdc; INCTHRU(thc, '.'); IN th = atoi(thc);
          CH *tnc = thc; INCTHRU(tnc, '.'); IN tn = atoi(tnc);
          CH *tsc = tnc; INCTHRU(tsc, '.'); IN ts = atoi(tsc);
          CH *tlc = tsc; INCTHRU(tlc, '.'); IN tl = atoi(tlc);
          CH *tuc = tlc; INCTHRU(tuc, '.'); IN tu = atoi(tuc);
          IF (ty EQ ly OR (tm EQ 1 AND ty EQ ly + 1)) {
            IF (tm EQ lm OR (td EQ 1 AND tm EQ lm + 1)) {
              IF (td EQ ld OR (th EQ 0 AND td EQ ld + 1)) {
                IF (th EQ lh AND tn - ln LQ 2) {
                  IF (ts LQ ls) {
                    LOG("Valid.\n");
                  } EL { LOG("Expired minute.\n"); }
                } EF (th EQ lh + 1 AND tn - (ln + 60) LQ 2) {
                  IF (ts LQ ls) {
                    LOG("Valid-l.\n");
                  } EL { LOG("Expired l-minute.\n"); }
                } EL { LOG("Bad hour.\n"); }
              } EL { LOG("Bad day.\n"); }
            } EL { LOG("Bad month.\n"); }
          } EL { LOG("Bad year.\n"); }
        }
      }
    }
//    CH *databufend = databuffer;
//    INCONTO(databufend, '\n');
//    IF (databufend EQ '\n') { databufend[0] = NUL; }
//    LOG2("{ '%s': '%s' }\n", querystring, databuffer);
  }
  // -- DISPLAY MAP -------------------------------------------------------
//  IF (querystring[0] EQNUL) {
    STR2(filepath, homepath, storagepath);
    DIR *dfd;
    IF ((dfd = opendir(filepath)) EQNULL) {
      LOG1("{ nodelist: \"\" }%s", BRN);
    } EL {
      LOG1("{ nodelist: {%s", BRN);
      CH filename[FVARPATHLEN + FVARPATHLEN]; // merging of path and name
      struct dirent *dp;
      WI ((dp = readdir(dfd)) NQNULL) {
        struct stat stbuf;
        STR2(filename, filepath, dp->d_name);
        IF (stat(filename, &stbuf) EQ -1) {
//          LOG2(": \"%s\"%s", filename, BRN);
        } EF (dp->d_name[0] EQ '.') {
        } EF ((stbuf.st_mode & S_IFMT) EQ S_IFDIR) {
          LOG2("<b>%s/</b>%s", dp->d_name, BRN);
        } EL {
          LOG2("%s%s", dp->d_name, BRN);
          IF (fvarget(filename, filedata)) {
            IF (streq(filedata, remoteaddr, '\n') EQ '=')
              { LOG2("=<b>%s</b>%s", filedata, BRN); }
            EL { LOG2("=%s%s", filedata, BRN); }
          }
        }
      }
      LOG1("} }%s", BRN);
    }
//    LOG("</body></html>\n");
//    RT 0;
//  }

  LOG("</body></html>\n");
  RT 0;
}
#endif

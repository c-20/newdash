#include <stdio.h>
#include <stdlib.h>

#include <string.h> // required for strlen

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>

using namespace std;

//#define Rcommand  "./R"

//#define RUNBUFFERLEN  16
//static char RUNBUFFER[RUNBUFFERLEN]; // fails on long directories !
static string runbuffer = "";
static string runpath = "/"; // default is / for readpage ... GET /?/ should do same I guess
#include "PRUNPORT.h"
#define RUNCOMMAND PRUNCOMMAND
#define RUNPORT    PRUNPORT

//#define RUNPORT     8080
static bool SETRUNBUFFER(string cmd) { // , string run) {
  runbuffer = cmd;
  return true;
}
/*
// what happens if i run it more than once ?
// doesn't happen !
// testing if length > 0 to prevent
  if (runbuffer.length() > 0) {
    cout << "ONLY EXPECTED TO RUN SETRUNBUFFER ONCE\n";
    // return false; // 
    exit(1);
  }
//cout << "SETRUNBUFFER\n";  
  runbuffer += cmd;
  int ebstix = runbuffer.length() - 1;
  if (runbuffer.at(ebstix) == '.' &&
      (ebstix == 0 || runbuffer.at(ebstix - 1) == '/')) {
    // it's important to note that a cmd ending in /. can be concatenated
    // with the run command ./dash to create /../dash, thereby allowing
    // a command in the parent directory to be run. this is unsafe!
    // but then maybe extracting the path for local context is too.
    return false;
  } else if (runbuffer.at(ebstix) == '/') {
    // cmd string ends with a /
    cout << "running a directory was presumed impossible\n";
  } else {
    // find path component end
    bool hasspacesincmdname = false;
    bool hasspacesincmdpath = false; // currently not checked !!
    int pcestix = ebstix;
    while (pcestix >= 0 && runbuffer.at(pcestix) != '/') {
      // first iteration should be true (not /) as per earlier condition
      if (runbuffer.at(pcestix) == ' ') { hasspacesincmdname = true; }
      pcestix--;
    }
    if (pcestix < 0) {
      cout << "no path component found; it is required\n";
      // todo: check $PATH to identify? use which ? (if no spaces)
      return false;
    } else if (runbuffer.at(pcestix) != '/') {
      cout << "this condition should not be true; expected a /\n";
      return false;
    } else {
      runpath = runbuffer.substr(0, pcestix);
cout << "RUNPATH: " << runpath << "\n";
      // todo: expect ./ prefix if only local commands are allowed?
      runbuffer = runpath.append("/" + run);
cout << "RUNBUFFER: " << runbuffer << "\n";
      return true;
    }
  }
  return false; // failed to fail to fail
}
*/
/*
static bool ADDRUNPARAM(string param) {
  if (runpath.length()) {
    runpath = runpath.append(" " + param);
    return true;
  } else {
    cout << "cannot add param without a command\n";
    // todo: make sure command isn't all spaces? check for validity?
    return false;
  }
}
*/ // run params are added later with each instantiation
//#define CHECKCOMMAND "./isdash"
// ^ using internal function call but external programs should be considered

#include "isdash.h"
static bool checkcommand() {
  return isdash();
}
//#define checkcommand isdash

#define FAILPAGEHTML  \
  "<html><head><title>fail</title></head>" \
    "<body>failed.</body></html>";

#define RT  return       // RETURNTOCALLINGFUNCTION
#define SI  int          // INTEGER (SIGNED)
#define ST  static
#define VD  void
#define VP  void *       // VOIDPOINTER
#define VPP void **      // VOIDPOINTERPOINTER
#define CH  char
#define CS  CH *
#define CCS const CH * // CONSTCHARSTRING
#define SIZEP size_t *   // SIZEPOINTER
#define DEMON struct MHD_Daemon *     // THE DAEMON
#define DATAP struct MHD_Connection * // THE CONNECTION
#define RESPP struct MHD_Response *   // THE RESPONSE
#define RESUP enum MHD_Result         // THE RESULT
#define LOG printf       // LOGMESSAGE
#define IF if            // CONDITION


RESUP geturlargs(VP cls, MHD_ValueKind kind, CCS key, CCS value) {
//  vector<pair<string, string>> *urlargs = static_cast<vector<pair<string, string>> *>(cls);
  map<string, string> *urlargs = static_cast<map<string, string> *>(cls);
  if (!value) { // this indicates no = ... maybe more than one?
    SI fileindex = 1;
    string filekey = ":file";
    string newkey = filekey + to_string(fileindex);
    while (urlargs->find(newkey) != urlargs->end())
      { newkey = filekey + to_string(++fileindex); }
    (*urlargs)[newkey] = key;
  } else if (urlargs->find(key) == urlargs->end()) {
    (*urlargs)[key] = value;
  } else {
    (*urlargs)[key] = "";
  }
  RT MHD_YES;
}

SI geturlgetargs(DATAP connection, MHD_KeyValueIterator iterator, VP itercls) {
  enum MHD_ValueKind kind = MHD_GET_ARGUMENT_KIND;
  SI ret = MHD_get_connection_values(connection, kind, iterator, itercls);
  RT (ret >= 0) ? 1 : 0; // true on success, false on fail
}

string urlsubtext(string url) {
  size_t contextend = url.find_last_of('-');
  size_t dirpathend = url.find_last_of('/');
  if (dirpathend == string::npos) {
    return ""; // no / (not expected because // prefix
  } else if (contextend == string::npos) {
    return ""; // no - in context -- //this/seems/to/be/a/path
  } else if (dirpathend > contextend) {
    return url.substr(dirpathend + 1);
  } else { return ""; } // last / is before last -
}

string commandresult(string cmd) {
  string response = "";
//  cmd.append(" 2>&1");
  const char *cmdstr = cmd.c_str();
  cout << "CMDSTR: " << cmdstr << "\n";

  FILE *stream = popen(cmdstr, "r");
  if (!stream) { RT "commandresult fail\n"; }
  while (1) { // !feof(stream)) {
    SI fchint = fgetc(stream);
    if (fchint == EOF) { break; } // shouldn't happen .. actually ..
    CH fch = (char)fchint;
    response += fch;
  }
  pclose(stream);
  cout << "RESPONSE[" << response.length() << "]: " << response << "\n";
  return response; // returning a local string variable seems to be ok ...?
}

RESUP sendfail(DATAP connection) {
  CS failpage = (CS)FAILPAGEHTML;
  RESPP response = MHD_create_response_from_buffer(strlen(failpage), failpage, MHD_RESPMEM_PERSISTENT);
  if (!response) { RT MHD_NO; } // cannot create response
  RESUP retval = MHD_queue_response(connection, MHD_HTTP_OK, response);
  MHD_destroy_response(response);
  RT retval;
}

RESUP answercall(VP cls, DATAP connection, CCS ccsurl, CCS method, CCS version, CCS data, SIZEP datasize, VPP concls) {
cout << "___________Answering call......_______________\n";

// todo: check if method is GET or POST
// 
//  CS page = (CS)malloc(100);

//  static int aptr;
//  if (&aptr != *concls) {
//    *ptr = &concls;
//    cout << "CONCLSEXIT\n";
//    return MHD_YES;
//  } // ignore followup requests ......? seems it'll first reply blank
// ^ something to prevent broken connections? set *ptr = NULL; when done
  // required $0 (argv[0]) so have to build in main
  string cmd = "" + runbuffer; //RUNBUFFER; //RUNCOMMAND;
//  cout << "CMD: " << cmd << "\n";
  map<string, string> getmap;
//  cout << "TEST A\n";
  if (!geturlgetargs(connection, geturlargs, &getmap))
    { RT sendfail(connection); }
  else { cout << "URLARGS GETMAP ACQUIRED\n"; }
//  cout << "TEST B\n";
  string url = string(ccsurl); // string conversion
  string ixurl = "/500.ico"; // server error by default
  string file1url = "";
  string file1set = "";
  map<string, string>::iterator getit = getmap.begin();
  if (getit == getmap.end()) {
    cout << "GETMAP IS EMPTY\n";
    cout << "URL: " << url << "\n";
    // THIS IS A GET SWITCH, NO DATA WAS POSTED. HOWTO POST ?
    size_t varlocation = url.find_first_of("/""/");
    // returns true if url is "/"
    if (url.length() > 1 && varlocation == 0) {
      string vrl = url.substr(2);
      size_t contextlocation = vrl.find_first_of('-');
      size_t valuelocation = vrl.find_first_of('/');
      if (contextlocation < 1) {
        cout << "INVALID CONTEXT LOCATION\n";
      } else if (valuelocation == string::npos) {
        file1url = vrl;
      } else if (valuelocation < contextlocation) {
        cout << "VARNAME CONTAINS /\n";
      } else {
        file1set = vrl.substr(valuelocation + 1);
        file1url = vrl.substr(0, valuelocation);
      }
    } else if (url.length() == 1 && url.at(0) == '/') {
      cout << "INDEX PAGE REQUEST\n";
      ixurl = "/";
    } else {
      cout << "INVALID VAR PREFIX\n";
      ixurl = "/404.ico";
    }
  }
  while (getit != getmap.end()) {
    string key = getit->first;
    string value = getit->second;
    if (key.at(0) == ':') {
      file1url = string(value);
cout << "-FILE1URL: " << file1url << "\n";
      file1set = urlsubtext(file1url);
cout << "-FILE1SET: " << file1set << "\n";

      if (file1set.length() > 0) {
        int file1urlsize = file1url.length() - (file1set.length() + 1);
        file1url = file1url.substr(0, file1urlsize);
      } // snip valueset suffix if detected
cout << "FILE1URL: " << file1url << "\n";
cout << "FILE1SET: " << file1set << "\n";
//cout << "url set by " << key << " (expect file1 for now)\n"; // currently multiple overwrites instead of multicall
//cout << ":PREFIX PARAM " << key << " = ---" << value << "---\n";
//      cmd = cmd + " [" + value + "]";
    // consider unicode keys ?
    } else { // key-value pairs are for parameters
      cout << "PARAM: " << key << ": " << value << "\n";
    }
    getit++;
  }
  // check url first?
  cout << "INPUT: " << method << " " << url << "\n";
  cout << "DATASIZE: " << *datasize << "\n";
  // cmd starts as runbuffer
  if (file1url.length() > 0) {
    if (file1set.length() > 0) {
      cmd = cmd + " /""/" + file1url + "=" + file1set;
    } else {
      cmd = cmd + " /""/" + file1url;
    } // run inputs //CON-TEXT/VALUE, passes //CON-TEXT=VALUE
  } else {
    // can be index, or value errors redirect to /404 or other error code
    cmd = cmd + " " + ixurl;
// this is the index page
// (don't write to iframe -- cannot detect tho, can only delegate css/js)
// valid get/set should not match here
//cout << "classic url input no longer used\n";
//exit(1);
    // classic url input no longer used
  }
cout << "======\n";
  cout << "URLCMD: " << cmd << "\n";
  string src = commandresult(cmd); // ./");
//  cout << "RESULTPAGE: " << src << "\n";
  cout << "SRCLEN: " << src.length() << "\n";
//  map <string, string, noordering> headers;
  vector<pair<string, string>> headers;
  string linedelim = "\r\n";
  string pairdelim = ": ";
  size_t pos = 0;
  string token;
  while ((pos = src.find(linedelim)) != string::npos) {
    token = src.substr(0, pos);
    size_t splitpos = token.find(pairdelim);
    if (splitpos != string::npos) {
      string tokenname = token.substr(0, splitpos);
      string tokenvalue = token.substr(tokenname.length() + pairdelim.length());
      headers.push_back(make_pair(tokenname, tokenvalue));
    } else if (token.length() == 0) {
      cout << "BLANKHEADERLINE\n";
    } else {
      cout << "INVALIDTOKENORrnINFILE\n";
      break;
    }
    src.erase(0, pos + linedelim.length());
  }
//  cout << "PAGE: " << src << "\n";

  CCS srcpage = src.c_str();
  SI srcpagelen = src.length();
//  cout << "SRCPAGE: " << srcpage << "\n";
//  SI pagelen = strlen(srcpage);
  cout << "SRCPAGELEN: " << srcpagelen << "\n";
  VP page = (VP)srcpage;
  enum MHD_ResponseMemoryMode memmode = MHD_RESPMEM_MUST_COPY;
  RESPP response = MHD_create_response_from_buffer(srcpagelen, page, memmode);

  vector<pair<string, string>>::iterator rh = headers.begin();
  if (rh == headers.end()) {
    cout << "NO HEADERS OR LAST HEADER";
    cout << "SRCPAGE[" << srcpagelen << "]: " << srcpage << "\n----\n";
  }
  while (rh != headers.end()) {
    cout << "HEADER: " << rh->first << " = " << rh->second << "\n";
    MHD_add_response_header(response, rh->first.c_str(), rh->second.c_str());
    rh++;
  }
//  MHD_add_response_header(response, "Content-Type",  "text/html; charset=utf-8");
//  MHD_add_response_header(response, "Cache-Control", "max-age=0, must-revalidate");
  RESUP retval = MHD_queue_response(connection, MHD_HTTP_OK, response);
  MHD_destroy_response(response);
//  free(page);
  RT retval;
}

SI main(SI argc, CS *argv) {
  if (checkcommand()) {
    cout << "apparently the daemon is already running\n";
    RT 1; // error code #1
  }
  string dollarzero(argv[0]);
cout << "DOLLARZERO: " << dollarzero << "\n";
//  if (!SETRUNBUFFER(strinargv[0], RUNCOMMAND)) {
  if (!SETRUNBUFFER(RUNCOMMAND)) { // dollarzero, RUNCOMMAND)) {
    cout << "failed to build cmd path\n";
//    RT sendfail(connection);
    RT 2; // error code #2
  } else if (!runpath.length()) {
    cout << "run path was expected... check using /which/ command?\n";
    RT 2;
  }
//  if (!ADDRUNPARAM(
  cout << "RUNBUFFER: " << runbuffer << "\n";
  DEMON daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD,
    RUNPORT, NULL, NULL, &answercall, NULL, MHD_OPTION_END);
  IF (!daemon) {
    LOG("daemon fail!\n");
    RT 1;
  }
  getchar();
  MHD_stop_daemon(daemon);
  RT 0;
}

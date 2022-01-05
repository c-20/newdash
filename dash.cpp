#include <iostream>
#include <stdio.h>

using namespace std;

#define SI   int
#define CH   char
#define CS   CH *
#define CCS  const CS

#include "html.h"

#define DASHTITLE  "dash"
void maintitle() {
  cout << "    <title>" << DASHTITLE << "</title>\n";
}
//#define dashswitch    "/DASH/SWITCH"
//#define dashswitchon  "/DASH/SWITCH=ON"
//#define dashswitchoff "/DASH/SWITCH=OFF"
#define dashswitch      "/DASH-SWITCH"
#define dashswitchon    "/DASH-SWITCH/ON"
#define dashswitchoff   "/DASH-SWITCH/OFF"

void mainstyle() {
  cout << "    <style type='text/css'>\n";
  cout << "      html, body { width: 95%; height: 95%; }\n";
  cout << "      body { position: relative; }\n";
  cout << "      #box { position: absolute;\n";
  cout << "        top: 10%; left: 10%; right: 2.5%; bottom: 2.5%;\n";
//  cout << "        background-color: #226;\n";
  cout << "        border: #226 solid 4px; border-radius: 16px; }\n";
  cout << "      #switch { position: absolute;\n";
  cout << "        top: 32px; left: 32px;\n"; //right: 32px; bottom: 32px;\n";
  cout << "        width: 196px; height: 196px;\n";
  cout << "        border: #000 solid 1px; border-radius: 8px;\n";
  cout << "        text-align: center; background-color: #555; }\n";
  cout << "      #switchbox { position: absolute;\n";
  cout << "        top: 32px; left: 232px;\n"; //right: 32px; bottom: 32px;\n";
  cout << "        width: 188px; height: 196px;\n";
  cout << "        border: #000 solid 1px; border-radius: 8px;\n";
  cout << "        text-align: center; background-color: #555; }\n";
  cout << "      #switchsync { position: absolute;\n";
  cout << "        top: 28px; left: 28px;\n"; //right: 32px; bottom: 32px;\n";
  cout << "        width: 396px; height: 204px;\n";
  cout << "        border: #000 solid 1px; border-radius: 8px;\n";
  cout << "        text-align: center; background-color: #555; }\n";
  cout << "      #switchsync.syncd { background-color: #02d; }\n";
  cout << "      #switchsync.recvd { background-color: #03f; }\n";
  cout << "      #switchsync.error { background-color: #f33; }\n";
  cout << "      #switch.myswitchon  { background-color: #8e8; }\n";
  cout << "      #switch.myswitchoff { background-color: #e88; }\n";
  cout << "      #switch.takeover    { background-color: #e8e; }\n";
  cout << "      #switch.mismatch    { background-color: #e00; }\n";
  cout << "      #switch .on { position: absolute;\n";
  cout << "        top: 5%; left: 5%; right: 5%; bottom: 55%;\n";
  cout << "        border: #000 solid 3px; border-radius: 3px;\n";
  cout << "        background-color: #8e8; font: #000 16px Verdana; }\n";
  cout << "      #switch .on:hover { background-color: #5f5; cursor: pointer; }\n";
  cout << "      #switch .noton { position: absolute;\n";
  cout << "        top: 7%; left: 7%; right: 7%; bottom: 57%;\n";
  cout << "        border: #000 solid 1px; border-radius: 3px;\n";
  cout << "        background-color: #393; font: #000 16px Verdana; }\n";
  cout << "      #switch .noton:hover { border: #5f5 solid 4px; cursor: pointer; }\n";
  cout << "      #switch .off { position: absolute;\n";
  cout << "        top: 55%; left: 5%; right: 5%; bottom: 5%;\n";
  cout << "        border: #000 solid 3px; border-radius: 3px;\n";
  cout << "        background-color: #e88; }\n";
  cout << "      #switch .off:hover { background-color: #f55; cursor: pointer; }\n";
  cout << "      #switch .notoff { position: absolute;\n";
  cout << "        top: 57%; left: 7%; right: 7%; bottom: 7%;\n";
  cout << "        border: #000 solid 1px; border-radius: 3px;\n";
  cout << "        background-color: #933; }\n";
  cout << "      #switch .notoff:hover { border: #f55 solid 4px; cursor: pointer; }\n";
  cout << "      #switch .onshadow { position: absolute;\n";
  cout << "        top: 5%; left: 5%; right: 5%; bottom: 55%;\n";
  cout << "        border: #777 solid 1px; border-radius: 3px;\n";
  cout << "        background-color: #444; opacity: 40%; }\n";
  cout << "      #switch .offshadow { position: absolute;\n";
  cout << "        top: 55%; left: 5%; right: 5%; bottom: 5%;\n";
  cout << "        border: #444 solid 1px; border-radius: 3px;\n";
  cout << "        background-color: #444; opacity: 60%; }\n";
  cout << "      #switch .off .text { position: absolute; bottom: 0; left: 0; right: 0;\n";
  cout << "        font: #000 12px Verdana; text-align: center; }\n";
  cout << "      #switch .on  .text { position: absolute; top:    0; left: 0; right: 0;\n";
  cout << "        font: #000 16px Verdana; text-align: center; }\n";
  cout << "      #switch .line { position: absolute;\n";
  cout << "        top: 48%; left: 4%; right: 4%; height: 3%;\n";
  cout << "        border: #555 solid 1px; border-radius: 3px;\n";
  cout << "        background-color: #444; opacity: 50%; }\n";
  cout << "    </style>\n";
  cout << "    <script type='text/javascript'>\n";
  cout << "      var syncswitchdelay = 0;\n"; // 1000ms on init, / 3000 / 1000 on toggle
  cout << "      var syncswitchtimer = undefined\n";
  cout << "      function syncswitch() {\n";
  cout << "        var box = document.getElementById('switchbox');\n";
  cout << "        var sync = document.getElementById('switchsync');\n";
  cout << "        sync.className = 'syncd'\n";
  cout << "        var boxhost = document.location.href;\n"; // expects it to end in /
  cout << "        box.src = boxhost + '" << dashswitch << "';\n";
  cout << "      }\n";
  cout << "      function syncswitchtoggle() {\n";
  cout << "        if (syncswitchtimer !== undefined) {\n";
  cout << "          clearInterval(syncswitchtimer);\n";
  cout << "          syncswitchtimer = undefined;\n";
  cout << "        }\n";
  cout << "        if (syncswitchdelay == 1000) {\n";
  cout << "          syncswitchdelay = 3000;\n";
  cout << "        } else { syncswitchdelay = 1000; }\n";
  cout << "        console.log('syncswitchdelay: ' + syncswitchdelay);\n";
  cout << "        syncswitchtimer = setInterval(function() { syncswitch(); }, syncswitchdelay);\n";
  cout << "      }\n";
/* flickswitchtoggle()
  cout << "      function flickswitch() {\n";
  cout << "        var switchobj = document.getElementById('switch');\n";
  cout << "        var obj = switchobj.lastElementChild;\n";
  cout << "        var box = document.getElementById('switchbox');\n";
  cout << "        if (obj && switchobj) {\n"; // had better exist .....
//  cout << "          if (obj.className == 'off' || obj.className == 'OFF') {\n";
//  cout << "          if (obj.className == 'OFF') {\n"; // if refusing to reswitch mid-switch
  cout << "          if (switchobj.className == 'off') {\n"; // || obj.className == 'OFF') {\n";
  cout << "            obj.className = 'on';\n";
  cout << "            obj.innerHTML = '<div class=\\'text\\'>on</div>';\n";
  cout << "            var boxhost = document.location.href;\n"; // expects it to end in /
  cout << "            box.src = boxhost + '/DASH/SWITCH=ON';\n";
  cout << "            switchobj.className = 'myswitchon';\n";
  cout << "          } else {\n"; // consider more than 'on' and 'ON' if refusing to reswitch mid-switch
  cout << "            obj.className = 'off';\n";
  cout << "            obj.innerHTML = '<div class=\\'text\\'>off</div>';\n";
  cout << "            var boxhost = document.location.href;\n"; // expects it to end in /
  cout << "            box.src = boxhost + '/DASH/SWITCH=OFF';\n";
  cout << "            switchobj.className = 'myswitchoff';\n";
  cout << "          }\n";
  cout << "        }\n";
  cout << "      }\n";
*/
  cout << "      function flickswitchon() {\n";
  cout << "        var switchobj = document.getElementById('switch');\n";
  cout << "        var onobj  = document.getElementById('switchon');\n";
  cout << "        var offobj = document.getElementById('switchoff');\n";
  cout << "        var box = document.getElementById('switchbox');\n";
  cout << "        if (switchobj && onobj && offobj && box) {\n"; // had better exist .....
  cout << "          onobj.className = 'on';\n";
  cout << "          onobj.innerHTML = '<div class=\\'text\\'>on</div>';\n";
  cout << "          offobj.className = 'notoff';\n";
  cout << "          offobj.innerHTML = '';\n";
  cout << "          var boxhost = document.location.href;\n"; // expects it to end in /
  cout << "          box.src = boxhost + '" << dashswitchon << "';\n";
  cout << "          switchobj.className = 'myswitchon';\n";
  cout << "        }\n";
  cout << "      }\n";
  cout << "      function flickswitchoff() {\n";
  cout << "        var switchobj = document.getElementById('switch');\n";
  cout << "        var onobj  = document.getElementById('switchon');\n";
  cout << "        var offobj = document.getElementById('switchoff');\n";
  cout << "        var box = document.getElementById('switchbox');\n";
  cout << "        if (switchobj && onobj && offobj && box) {\n"; // had better exist .....
  cout << "          onobj.className = 'noton';\n";
  cout << "          onobj.innerHTML = '';\n";
  cout << "          offobj.className = 'off';\n";
  cout << "          offobj.innerHTML = '<div class=\\'text\\'>off</div>';\n";
  cout << "          var boxhost = document.location.href;\n"; // expects it to end in /
  cout << "          box.src = boxhost + '" << dashswitchoff << "';\n";
  cout << "          switchobj.className = 'myswitchoff';\n";
  cout << "        }\n";
  cout << "      }\n";
  cout << "      function readswitch() {\n";
  cout << "        var sync = document.getElementById('switchsync');\n";
  cout << "        sync.className = 'recvd'\n";
  cout << "        var switchobj = document.getElementById('switch');\n";
  cout << "        var onobj  = document.getElementById('switchon');\n";
  cout << "        var offobj = document.getElementById('switchoff');\n";
  cout << "        var box = document.getElementById('switchbox');\n";
  cout << "        var val = box.contentDocument.documentElement.lastElementChild.innerHTML;\n";
  cout << "        var currentfieldname = 'CURRENTVALUE: ';\n";
  cout << "        var currentfieldindex = val.indexOf(currentfieldname);\n";
  cout << "        var newfieldname = 'NEWVALUE: ';\n";
  cout << "        var newfieldindex = val.indexOf(newfieldname);\n";
  // if both, consider the order that they are in (compare which is lower) ... end of \nEOF or \nFIELD: 
  // only searching for current and new keywords by indexOf ..........
  // also searching for \n as value delimiter except for multiline value
  // assumes switchobj && onobj && offobj && box are objects
  cout << "        if (newfieldindex != -1) {\n";
  cout << "          var newvalue = val.substr(newfieldindex + newfieldname.length);\n";
  cout << "          if (newvalue.indexOf('\\n') != -1)\n"; // expect this
  cout << "            { newvalue = newvalue.substr(0, newvalue.indexOf('\\n')); }\n";
  cout << "          else { newvalue = '! ?'; }\n"; // incomplete value string
//  cout << "          onobj.className = 'noton';\n";
//  cout << "          onobj.innerHTML = '';\n";
//  cout << "          offobj.className = 'off';\n";
//  cout << "          offobj.innerHTML = '<div class=\\'text\\'>off</div>';\n";
//  cout << "          if (newvalue.toLowerCase() != switchobj.className)\n";
  // expect switchobj.className to be myswitchon or myswitchoff... unless some kind of out of sync switching..
  // can that even happen or is this one thread only? should be .......;
  cout << "          if (switchobj.className != 'myswitch' + newvalue.toLowerCase()) {\n";
  cout << "            switchobj.className = 'mismatch';\n";
  cout << "          } else if (switchobj.className == 'myswitchon') {\n";
  cout << "            onobj.innerHTML = '<div class=\\'text\\'>' + newvalue + '</div>';\n";
  cout << "            onobj.className = newvalue;\n"; // ON
  cout << "          } else if (switchobj.className == 'myswitchoff') {\n";
  cout << "            offobj.innerHTML = '<div class=\\'text\\'>' + newvalue + '</div>';\n";
  cout << "            offobj.className = newvalue;\n"; // OFF
  cout << "          }\n";
// obj.className should match switch.className but obj.className in CAPS and switch.className not
  cout << "        } else if (currentfieldindex != -1) {\n";
  cout << "          var currentvalue = val.substr(currentfieldindex + currentfieldname.length);\n";
  cout << "          if (currentvalue.indexOf('\\n') != -1)\n"; // expect this
  cout << "            { currentvalue = currentvalue.substr(0, currentvalue.indexOf('\\n')); }\n";
  cout << "          else { currentvalue = '? ?'; }\n"; // incomplete value string
  cout << "          //obj.innerHTML = '<div class=\\'text\\'>' + currentvalue + '</div>';\n";
  cout << "          //obj.className = currentvalue;\n";
//  cout << "          else if (switchobj.className == 'myswitchon')\n"; IF TAKEOVER, INHERIT ?
  cout << "          if (switchobj.className == 'takeover') {\n";
  cout << "            var uppervalue = currentvalue;\n"; // 'ON' or 'OFF'
  cout << "            if (uppervalue == 'ON' && onobj.className == 'on') {\n";
  cout << "              onobj.className = currentvalue;\n";  // 'ON'
  cout << "              onobj.innerHTML = '<div class=\\'text\\'>' + currentvalue + '</div>';\n";
  cout << "              switchobj.className = 'myswitchon';\n";
  cout << "            } else if (uppervalue == 'OFF' && offobj.className == 'off') {\n";
  cout << "              offobj.className = currentvalue;\n";  // 'OFF'
  cout << "              offobj.innerHTML = '<div class=\\'text\\'>' + currentvalue + '</div>';\n";
  cout << "              switchobj.className = 'myswitchoff';\n";
  cout << "            } else {\n"; // else if in takeover mode but value and class don't match
  cout << "              switchobj.className = 'takeover';\n"; // re-takeover from a take-over
  cout << "              var lowervalue = currentvalue.toLowerCase();\n";
  cout << "              if (lowervalue == 'on') {\n";
  cout << "                onobj.className = lowervalue;\n";  // 'on'
  cout << "                onobj.innerHTML = '<div class=\\'text\\'>' + lowervalue + '</div>';\n";
  cout << "                offobj.className = 'notoff';\n";
  cout << "                offobj.innerHTML = '';\n";
  cout << "              } else if (lowervalue == 'off') {\n";
  cout << "                onobj.className = 'noton';\n";
  cout << "                onobj.innerHTML = '';\n";
  cout << "                offobj.className = lowervalue;\n"; // 'off'
  cout << "                offobj.innerHTML = '<div class=\\'text\\'>' + lowervalue + '</div>';\n";
  cout << "              } else { switchobj.className = 'mismatch'; }\n";
  cout << "            }\n";
// IF TAKEOVER MODE, CLASS SHOULD BE LOWERCASE .... GO BBY CLASSNAMENOT LOWERVALUE ?
// TAKEOVER TO LOWERCASE THEN UPPERCASE IN NEXT FIX... CHANGE CLASSNAME ? SHOULDN'T HAVE TO WHY THE LOOPING
// ACTUALLY DO HAVE TO
  cout << "          } else if (switchobj.className != 'myswitch' + currentvalue.toLowerCase()) {\n";
  cout << "            if (switchobj.className == 'mismatch') {\n";
  cout << "              onobj.className = 'noton';\n";
  cout << "              offobj.className = 'notoff';\n";
  cout << "            } else {\n"; // re-takeover doesn't end up here because of earlier condition
  cout << "              switchobj.className = 'takeover';\n";
  cout << "              var lowervalue = currentvalue.toLowerCase();\n";
  cout << "              if (lowervalue == 'on') {\n";
  cout << "                onobj.className = lowervalue;\n";  // 'on'
  cout << "                onobj.innerHTML = '<div class=\\'text\\'>' + lowervalue + '</div>';\n";
  cout << "                offobj.className = 'notoff';\n";
  cout << "                offobj.innerHTML = '';\n";
  cout << "              } else if (lowervalue == 'off') {\n";
  cout << "                onobj.className = 'noton';\n";
  cout << "                onobj.innerHTML = '';\n";
  cout << "                offobj.className = lowervalue;\n"; // 'off'
  cout << "                offobj.innerHTML = '<div class=\\'text\\'>' + lowervalue + '</div>';\n";
  cout << "              } else { switchobj.className = 'mismatch'; }\n";
  cout << "            }\n";
// TODO: IF TAKEOVERING, SET CURRENTVALUE OF ON/OFF IF TAKEOVERED, SET TO MYSWITCH ONOROFF
  cout << "          } else if (switchobj.className == 'myswitchon') {\n";
  cout << "            onobj.innerHTML = '<div class=\\'text\\'>' + currentvalue + '</div>';\n";
  cout << "          } else if (switchobj.className == 'myswitchoff') {\n";
  cout << "            offobj.innerHTML = '<div class=\\'text\\'>' + currentvalue + '</div>';\n";
  cout << "          }\n";
//  cout << "          if (currentvalue.toLowerCase() != switchobj.className)\n";
//  cout << "            { switchobj.className = 'takeover'; }\n"; // sets classname repeatedly each time
// ^^^ if currentvalue differs from the current currentvalue, flash three times !!!!!!
// thus delaying further checks by an extra second as an acknowledgement that the change
// was noted ...
  cout << "        }\n";
  cout << "      }\n";
  cout << "    </script>\n";
}

void mainswitch() {
  cout << "<div id='switchsync' onclick='syncswitchtoggle();'></div>\n";
  cout << "<div id='switch'>\n";
  cout << "  <div class='line'></div>\n";
  cout << "  <div class='onshadow'></div>\n";
  cout << "  <div class='offshadow'></div>\n";
  cout << "  <div id='switchon' class='noton' onclick='flickswitchon();'><div class='text'>? ?</div></div>\n";
  cout << "  <div id='switchoff' class='notoff' onclick='flickswitchoff();'><div class='text'>? ?</div></div>\n";
  cout << "</div>\n";
  cout << "<iframe id='switchbox' onload='readswitch();' src=''>\n";
  cout << "</iframe>\n";
}

int main(int argc, char **argv) {
  string url = "";
  if (argc < 2) {
    cout << "no url given.\n";
    url = "";
    return 0;
  } else if (argc == 2) {
    url = string(argv[1]);
//    if (url.at(0) == '\\' || (url.at(0) == '/' && url.at(1) == '\\')) {
      // url strings cannot contain \ so //VAR=VALUE syntax is used in shell + web
    if (url.at(0) == '/' && url.length() > 1 && url.at(1) == '/') {
      // backslash input prefix for value read/write, allow web get/set
      string varname = url.substr((url.at(0) == '/') ? 2 : 1);
      string::size_type varsplit = string::npos;
      if ((varsplit = varname.find("=", 0)) != string::npos) { // = means set a value
        string varvalue = varname.substr(varsplit + 1);
        varname = varname.substr(0, varsplit);
        bool isuppercase = (varname.length()) ? true : false; // empty = not uppercase
        string errorwords = "";
        int contextstatus = 0; // only write *-*
        int vari = -1;
        while (++vari < varname.length()) {
          if (varname.at(vari) == '/') {
            if (vari == 0) {
              errorwords = "UPPERCASE VAR DIRS ARE RELATIVE\n";
//              cout << "PROTECTEDVARNAME: " << varname << "\n";
//              cout << "REJECTEDVARVALUE: " << varvalue << "\n";
              break; // return 0;
            } else if (varname.at(vari - 1) == '/') {
              errorwords = "BLANK VAR DIRS / / ARE VOID\n";
//              cout << "PROTECTEDVARNAME: " << varname << "\n";
//              cout << "REJECTEDVARVALUE: " << varvalue << "\n";
              break; // return 0;
            }
          } else if (varname.at(vari) == '-') {
            contextstatus++;
          } else if (varname.at(vari) < 'A' || varname.at(vari) > 'Z') {
            isuppercase = false;
            errorwords = "ONLY UPPERCASE VARS ARE MUTABLE\n";
//            cout << "PROTECTEDVARNAME: " << varname << "\n";
//            cout << "REJECTEDVARVALUE: " << varvalue << "\n";
            break; // return 0;
          }
        }
        bool writefailed = false;
        bool readfailed = false;
        size_t firstslash = varname.find_first_of('/');
        string uppercasevalue = "";
        if (errorwords.length()) {
          // continue
        // } else if ,, need to check for any /, but not any \/
        //    but \\/ still counts .. for now no \, pass another way
        } else if (varname.length() < 1) {
          errorwords = "VARNAME IS BLANK\n";
        } else if (firstslash != string::npos) {
          errorwords = "VARNAME CONTAINS /\n";
        } else if (!contextstatus) {
          errorwords = "ONLY *-* VARS ARE MUTABLE\n";
        } else if (isuppercase && contextstatus) { // NOTE: THE CURRENT FILE IS BEING OVERWRITTEN
          CCS uppercasefilename = varname.c_str();
// affects Content-Type header
//cout << "UPPERCASEFILENAMEFORWRITE: " << uppercasefilename << "\n";
          FILE *uppercasefilew = fopen(uppercasefilename, "w");
          if (!uppercasefilew) {
            writefailed = true;
          } else {
            int varvalix = -1;
            while (++varvalix < varvalue.length()) {
              fputc(varvalue.at(varvalix), uppercasefilew);
              // check for errors with each byte write ?
            }
            fclose(uppercasefilew);
          }
          FILE *uppercasefiler = fopen(uppercasefilename, "r");
          if (!uppercasefiler) {
            readfailed = true;
          } else {
            // if write failed but read succeeds, file might be read-only
            while (1) {
              SI ucchint = fgetc(uppercasefiler);
              if (ucchint == EOF) { break; } // done
              CH ucch = (CH)ucchint;
              uppercasevalue += ucch;
            }
            fclose(uppercasefiler);
          }
        }
        valueheader();
        htmlhead();
          metacharset();
        headbodysmalltext();
        cout << "VAR: " << varname << "\n";
        if (!writefailed && !readfailed) {
          cout << "NEWVALUE: " << uppercasevalue << "\n";
          if (errorwords.length()) { cout << "ERRORONSUCCESS: " << errorwords; }
         } else if (writefailed && !readfailed) {
          cout << "ROMVALUE: " << uppercasevalue << "\n";
        } else { // both failed or read failed ... expect blank value
          cout << "NULVALUE: " << uppercasevalue << "\n";
        }
        if (errorwords.length()) {
          cout << "ERROR: " << errorwords << "\n";
        }
        bodyhtml();
        return 0;
      } else { // no = means read a value
        bool isuppercase = true;
        int contextstatus = 0; // only read *-*
        int vari = -1;
        while (++vari < varname.length()) {
          if (varname.at(vari) == '/') {
            if (vari == 0) {
              cout << "UPPERCASE VAR DIRS ARE RELATIVE\n";
              cout << "PROTECTEDVARNAME: " << varname << "\n";
              break; // return 0;
            } else if (varname.at(vari - 1) == '/') {
              cout << "BLANK VAR DIRS / / ARE VOID\n";
              cout << "PROTECTEDVARNAME: " << varname << "\n";
              break; // return 0;
            }
          } else if (varname.at(vari) == '-') {
            contextstatus++;
          } else if (varname.at(vari) < 'A' || varname.at(vari) > 'Z') {
            isuppercase = false;
            cout << "ONLY UPPERCASE VARS ARE READABLE\n";
            cout << "PROTECTEDVARNAME: " << varname << "\n";
            break; // return 0;
          }
        }
        bool readfailed = false;
        string uppercasevalue = "";
        if (!contextstatus) {
          cout << "ONLY *-* VARS ARE READABLE -- MANDATORY CONTEXT\n";
        } else if (isuppercase && contextstatus) { // NOTE: THE CURRENT FILE IS BEING READ
          CCS uppercasefilename = varname.c_str();
// affects Content-Type header
//cout << "UPPERCASEFILENAMEFORREAD: " << uppercasefilename << "\n";
          FILE *uppercasefile = fopen(uppercasefilename, "r");
          if (!uppercasefile) {
            readfailed = true;
          } else {
            while (1) {
              SI ucchint = fgetc(uppercasefile);
              if (ucchint == EOF) { break; } // done
              CH ucch = (CH)ucchint;
              uppercasevalue += ucch;
            }
            fclose(uppercasefile);
          }
        }
        valueheader();
        htmlhead();
          metacharset();
        headbodysmalltext();
        cout << "VAR: " << varname << "\n";
        if (!readfailed) { // if read didn't failed. the value was read
          cout << "CURRENTVALUE: " << uppercasevalue << "\n";
        } else { // expect blank
          cout << "FAILEDVALUE: " << uppercasevalue << "\n";
        }
        bodyhtml();
        return 0;
      }
    } else if (url == "/favicon.ico") {
      iconheader();
      FILE *fico = fopen("./favicon.ico", "r");
      if (!fico) {
        // cout << "NO FAVICON\n";
        return 0; // send header and close
      } else {
        while (1) {
          int fc = fgetc(fico);
          if (fc == EOF) { break; }
// ^ still need to send EOF ?
          char fch = (char)fc;
          fputc(fc, stdout);
//          cout << fch;
        }
        fclose(fico);
//        cout << "\n"; // extra linebreak ok ? or no?
// The image cannot be displayed because it contains errors.
      }
      return 0;
    } else if (url != "/") {
      cout << "IGNOREDURL: " << url << "\n";
      return 0; // no response page
    } // else fall through to index page html
  } else {
    cout << "too many arguments.\n";
    return 0;
  }
  mainheader();
  htmlhead();
    metacharset();
    maintitle();
    mainstyle();
  headbody();

  newdiv("box");
    mainswitch();
  enddiv();
  newscript();
    cout << "syncswitchtoggle();\n";
  endscript();
  bodyhtml();


  return 0;
}


#include "code.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#define PDNC
//#include "dns.c"

//#include <stdint.h>
//typedef int      u_int32_t;
//typedef long int u_int64_t;
// suddenly works after fixing/updating code.h
#include "token.h"

//#define DNCCOMMANDLENGTH	128
// length 117, nowtoken asked for 127
#define DNCCOMMANDLENGTH	256
#define DNCVALUELENGTH		128


//CS host = [serverip];
CS host = "10.10.10.1";
CS name = "test";
// [serverip] [hostname];
CS labels[] = {
  "{ hooktime: \"",
  "{ newtoken: \"", NULL };

// CSCS SleepCommand = "sleep 5";
CS SleepCommand = "sleep 5";

IN main(IN argc, CH **argv) {
  IF (argc > 1) { name = argv[1]; }
  CH dnccommand[DNCCOMMANDLENGTH];
  CH dncvalue[DNCVALUELENGTH];
  CH remotime[DNCVALUELENGTH];
  CH nowtoken[DNCVALUELENGTH];
  CH oldtoken[DNCVALUELENGTH];
  oldtoken CLEAR;
  nowtoken CLEAR;
  IF (1) {
    WI (1) {
      IF (oldtoken[0] NQNUL) {
        STRF(dnccommand, "dlraw \"%s/n#%s--%s\"", host, name, oldtoken);
      } EF (nowtoken[0] NQNUL) {
        STRF(dnccommand, "dlraw \"%s/n#%s-%s\"", host, name, nowtoken);
      } EL { STRF(dnccommand, "dlraw \"%s/n#%s\"", host, name); }
      MLOG1("%s\n", dnccommand);
      FILE* outM = popen("_ - -Y", "w"); // 1>&2", "w"); <---- NOTE: this is not standard out!
      FILE* cmd = popen(dnccommand, "r");
      IF (outM && cmd) {
        IN ch = NUL;
        IN lch = ch;
        IN llch = lch;
        IN lllch = llch;
        IN llllch = lllch;
        IN lllllch = llllch;
        IN ci = 0;
        IN li = 0;
        CH lc = NUL;
        IN lbi = 0;
        CH *label = labels[lbi];
        CH intag = N;
        CH tagch = NUL;
        CS tagpf = "";
        CH headmute = Y;
        CH eatbr = N;
        WI (ch NQEOF) {
          lllllch = llllch;
          llllch = lllch;
          lllch = llch;
          llch = lch;
          lch = ch;
// ch[4] = ch[3]; ch[3] = ch[2]; ch[2] = ch[1]; ch[1] = ch[0];
          ch = fgetc(cmd);
//IF (intag YES) { Mc(ch); } EL { Bc(ch); }
          IF (eatbr EQ Y) {
            eatbr = N;
            IF (ch EQ '\n') { CT; }
          }
          IF (ch EQEOF) { ci = -1; BK; }
          EF (ch EQ '<') {
            intag  = Y; tagch = '-'; tagpf = "";
// ------------------------------------------------------------------------------------
//          } EF (intag EQ Y) {
//            IF (ch EQ ' ')
//              { fprintf(outM, "< "); Ms("THIS"); intag = N; }
//            EL { tagch = ch; intag = Y; }
          } EF (intag YES) { // AND headmute NO) {
            IF (ch EQ '>' OR ch EQ ' ') {
              IF (llllch EQ 'b' AND lllch EQ 'o' AND llch EQ 'd' AND lch EQ 'y') {
                IF (lllllch EQ '<')
                  { headmute = N; eatbr = Y; } // Rs("NOHEADMUTE"); }
                EF (lllllch EQ '/')
                  { headmute = Y; } // Rs("TAILMUTE"); }
//                EL { headmute = Y; Rs("HEADMUTE"); } // IF lllllch EQ '/'
                EL { Rs("Unexpected *body>"); }
              } EF (lllch EQ 'd' AND llch EQ 'i' AND lch EQ 'v') {
                IF (llllch EQ '<')
                  { headmute = Y; } // Rs("DIVMUTE. eatbr?"); } // eatbr = Y; Rs("NOHEADMUTE"); }
                EF (llllch EQ '/' AND headmute NO) // tailmute should have started
                  { Rs("Unexpected </div> in body."); }
              }
              IF (tagch NQ '-')
                { fprintf(outM, "<%s%c>", tagpf, tagch); }
              intag = N; // Cs("ENDTAG");
            } EF (tagch EQ '/') { tagpf = "/"; tagch = ch; }
//            EF (tagch EQ ' ') { intag = N; }
            EF (tagch NQ '-') { tagch = '-'; }
// ------------------------------------------------------------------------------------
         } EF (headmute EQ Y) {
           headmute = Y;
         } EF (lc EQNUL AND li GQ 0 AND ch NQ label[li]) {
            li = 0; fprintf(outM, "<C>%c", ch); // no match
          } EF (lc EQNUL AND li GQ 0 AND ch EQ label[li]) {
            IF (label[li + 1] EQNUL)
              { lc = label[li]; li = 0;
                fprintf(outM, "<W>%c", '"'); } // enable read
            EL { INC li; fprintf(outM, "<Y>%c", ch); }

//Yc(ch); }


          } EF (lc NQNUL AND li GQ 0 AND ch NQ lc) {
            dncvalue[li] = ch;
            dncvalue[INC li] = NUL;
            fprintf(outM, "<Y>%c", ch);
          } EF (lc NQNUL AND li GQ 0 AND ch EQ lc) {
            fprintf(outM, "<W>%c", '"');
            IF (label EQ labels[0])
              { STRF(remotime, "%s", dncvalue); }
            EF (label EQ labels[1]) {
              CS dnspace = dncvalue; INCONTO(dnspace, ' ');
              IF (dnspace[0] EQ ' ') {
                STR1(oldtoken, nowtoken);
                fprintf(outM, "<R>%s</R><C>--%s</C>", dncvalue, oldtoken);
              } EL {
                dnsrotate(dncvalue, remotime, -1, nowtoken);
                fprintf(outM, "<W> -> ");
                fprintf(outM, "<G>%s", nowtoken);
              }
            }
            IF (labels[lbi + 1] EQ NULL) { li = -1; }
            EL { label = labels[INC lbi]; li = 0; lc = NUL; }
          } EF (li EQ -1) {
            fprintf(outM, "<C>%c", ch); // finish matches
            IF (ch EQ '\n') { fprintf(outM, "<R>"); li = -2; }
          } EF (li EQ -2) {
            fprintf(outM, "%c", ch); // no more matches
          }
          INC ci;
        }
        pclose(cmd);
        pclose(outM);
      } EL { RLOG("Output failed."); }
      Ws(" ("); YXC(167); Cc('1'); Ws(") ");
      FILE *slpA = popen(SleepCommand, "r");
      IF (slpA) {
  //      CLOG(" ");
        pclose(slpA);
      } EL { RLOG("Cannot sleep!\n"); RT 2; }
      FILE *wfi = popen("bash wifi1 seek pbc0", "r");
      IF (wfi) {
        WI (!feof(wfi)) {
          IN ch = fgetc(wfi);
          IF (ch EQEOF) { BK; }
          EL { LOG1("%c", ch); }
        }
        pclose(wfi);
      } EL { RLOG("Cannot evaluate!\n"); RT 3; }
      Ws(" ("); YXC(167); Cc('1'); Ws(") ");
      FILE *slpB = popen(SleepCommand, "r");
      IF (slpB) {
//        CLOG(" ");
        pclose(slpB);
      } EL { RLOG("Cannot sleep!\n"); RT 2; }
    }
  } EL { RLOG("Cannot colour..."); RT 1; }
  RT 0;
}

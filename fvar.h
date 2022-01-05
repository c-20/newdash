
IN fvarget(CH *path, CH *buffer) {
  FILE *vf = fopen(path, "r");
  IF (!vf) { RT 0; }
  IN valcount = 0;
  IN inch = NUL;
  WI (inch NQEOF) {
    inch = fgetc(vf);
    IF (inch EQEOF) { BK; }
    IF (inch EQ '\n') { INC valcount; }
    buffer[0] = inch;
    buffer[1] = '\0';
    INC buffer;
  }
  fclose(vf);
  RT valcount;
}

IN fvarset(CH *path, CH *newvalue) {
  FILE *vf = fopen(path, "w");
  IF (!vf) { RT 0; }
  IN valcount = 0;
  IN inch = newvalue[0];
  WI (inch NQNUL) {
//    IF (inch EQNUL) { BK; }
    IF (inch EQ '\n') { INC valcount; }
    fputc(inch, vf);
    INC newvalue;
    inch = newvalue[0];
  }
  IF (inch NQ '\n') {
    inch = '\n';
    fputc(inch, vf);
    valcount++;
  }
  fclose(vf);
  RT valcount;  
}

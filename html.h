void mainheader() {
  cout << "Content-Type: text/html; charset=utf-8\r\n";
  cout << "Cache-Control: private, no-cache, no-store, must-revalidate\r\n";
  cout << "\r\n";
}


void valueheader() {
  cout << "Content-Type: text/html\r\n";
  cout << "Cache-Control: private, no-cache, no-store, must-revalidate\r\n";
  cout << "\r\n";
}

void iconheader() {
//  cout << "Content-Type: image/x-icon\r\n";
  cout << "Content-Type: image/x-icon\r\n";
  cout << "Cache-Control: private, no-cache, no-store, must-revalidate\r\n";
  cout << "\r\n";
}

void htmlhead() {
  cout << "<html>\n";
  cout << "  <head>\n";
}

void metacharset() {
  cout << "    <meta charset='UTF-8'>\n";
}

void headbody() {
  cout << "  </head>\n";
  cout << "  <body>\n";
}

void headbodysmalltext() {
  cout << "  </head>\n";
  cout << "  <body style='font: bold 12px Verdana; color: #fc3;";
  cout <<               " text-align: center; line-height: 3em;'>\n";
}

void newdiv(string id) {
  cout << "    <div id='" << id << "'>\n";
}

void enddiv() {
  cout << "    </div>\n";
}

void newscript() {
  cout << "<script type='text/javascript'>\n";
}
void endscript() {
  cout << "</script>\n";
}

void bodyhtml() {
  cout << "  </body>\n";
  cout << "</html>\n";
}

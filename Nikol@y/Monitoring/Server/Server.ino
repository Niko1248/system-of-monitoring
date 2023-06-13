
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#define REQ_BUF_SZ 128
#define FIRST_PIN 40
#define LAST_PIN 51
#define FREE_SOCKETS 1

File logFile;
File sendLog;
File webFile;
File sisFile;
EthernetClient client;

#define MAX_BUFFER_SIZE 1024
uint16_t rsize;
uint8_t buff[MAX_BUFFER_SIZE];
char HTTP_req[REQ_BUF_SZ] = { 0 };  // buffered HTTP request stored as null terminated string
char req_index = 0;                 // index into HTTP_req buffer


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 110);
EthernetServer server(80);

void setup() {
  Serial.begin(9600);

  for (int i = FIRST_PIN; i <= LAST_PIN; i++) {  // Установил все пины в input
    pinMode(i, INPUT);
  }
  SD.begin(4);
  Ethernet.begin(mac, ip);
  server.begin();
  digitalWrite(10,HIGH);

}
void loop() {
  serverWorks();
}

void serverWorks2(EthernetClient sclient) {

  if (sclient) {
    // если сервер поднят
    boolean currentLineIsBlank = true;
    while (sclient.connected()) {  // и мы подключились
      if (sclient.available()) {
        char c = sclient.read();  //присваиваем реквесты переменной с
        if (req_index < (REQ_BUF_SZ - 1)) {
          HTTP_req[req_index] = c;  // читаем посимвольно запрос
          req_index++;
        }
        if (c == '\n' && currentLineIsBlank) {                                               // если символы в запросе кончились то
          if (StrContains(HTTP_req, "GET / ") || StrContains(HTTP_req, "GET /user.htm")) {  // содердит ли реквест GET /index.htm ?
            sclient.println("HTTP/1.1 200 OK");
            sclient.println("Content-Type: text/html");
            sclient.println("Connnection: close");
            sclient.println();
            webFile = SD.open("user.htm");
          }
          else if (StrContains(HTTP_req, "GET /admin" || StrContains(HTTP_req, "GET /admin.htm")) {
            sclient.println("HTTP/1.1 200 OK");
            sclient.println("Content-Type: text/html");
            sclient.println("Connnection: close");
            sclient.println();
            webFile = SD.open("admin.htm");
          }
          else if (StrContains(HTTP_req, "GET /user.css")) {
            webFile = SD.open("user.css");
            if (webFile) {
              sclient.println("HTTP/1.1 200 OK");
              sclient.println();
            }
          }
          else if (StrContains(HTTP_req, "GET /admin.css")) {
            webFile = SD.open("admin.css");
            if (webFile) {
              sclient.println("HTTP/1.1 200 OK");
              sclient.println();
            }
          }
          else if (StrContains(HTTP_req, "GET /sis.jso")) {
            webFile = SD.open("sis.jso");
            if (webFile) {
              sclient.println("HTTP/1.1 200 OK");
              sclient.println();
            }
          }
          else if (StrContains(HTTP_req, "GET /favicon.ico")) {
            webFile = SD.open("favicon.ico");
            if (webFile) {
              sclient.println("HTTP/1.1 200 OK");
              sclient.println("Cache-Control: public, max-age=31536000");
              sclient.println();
            }
          }
          else if (StrContains(HTTP_req, "GET /alert.ogg")) {
            webFile = SD.open("alert.ogg");
            if (webFile) {
              sclient.println("HTTP/1.1 200 OK");
              sclient.println("Cache-Control: public, max-age=31536000");
              sclient.println();
            }
          }
          else if (StrContains(HTTP_req, "GET /R_EL.wof")) {
            webFile = SD.open("R_EL.wof");
            if (webFile) {
              sclient.println("HTTP/1.1 200 OK");
              sclient.println("Cache-Control: public, max-age=31536000");
              sclient.println();
            }
          }
          else if (StrContains(HTTP_req, "GET /R_L.wof")) {
            webFile = SD.open("R_L.wof");
            if (webFile) {
              sclient.println("HTTP/1.1 200 OK");
              sclient.println("Cache-Control: public, max-age=31536000");
              sclient.println();
            }
          }
          else if (StrContains(HTTP_req, "GET /R_M.wof")) {
            webFile = SD.open("R_M.wof");
            if (webFile) {
              sclient.println("HTTP/1.1 200 OK");
              sclient.println("Cache-Control: public, max-age=31536000");
              sclient.println();
            }
          }
          /////////////////////////////////////Вот тут начинается Ajax///////////////////////////////////////////////
          else if (StrContains(HTTP_req, "readPinInfo")) {  //если реквест содержит запрос readPinInfo то...     кстати, readPinInfo это и есть функция ответa ajax на js в index.htm

            for (int i = FIRST_PIN; i <= LAST_PIN; i++) {
              bool Dr = digitalRead(i);
              sclient.print(Dr);
            }
          } 
          else if (StrContains(HTTP_req, "POST /get_access_admin")) {
            String pass = "";
            while(sclient.connected()){
              while(sclient.available()){       
                char c = sclient.read();
                pass += c;
              }   
              req_index = 0;
              StrClear(HTTP_req, REQ_BUF_SZ);
              break;
            }
            if(pass == "adminqwe123"){
              sclient.print("ok");
            }else if (pass != "adminqwe123"){
              sclient.print("no");
            }
          }
          else if (StrContains(HTTP_req, "POST /get_access_user")) {
            String pass = "";
            while(sclient.connected()){
              while(sclient.available()){       
                char c = sclient.read();
                pass += c;
              }   
              req_index = 0;
              StrClear(HTTP_req, REQ_BUF_SZ);
              break;
            }
            if(pass == "userqwe123"){
              sclient.print("ok");
            }else if (pass != "userqwe123"){
              sclient.print("no");
            }
          }
          else if (StrContains(HTTP_req, "POST /log_write")) {
            if (!SD.exists("log.xml")){
              logFile = SD.open("log.xml", FILE_WRITE);
              logFile.write("[");
              logFile.close();
            } else{
              logFile = SD.open("log.xml",FILE_WRITE);
            }
            while(sclient.connected()){
              while(sclient.available()){       
                char c = sclient.read();
                logFile.write(c);
              }   
              
              logFile.close();
              req_index = 0;
              StrClear(HTTP_req, REQ_BUF_SZ);
              break;
            }
            sclient.println("HTTP/1.1 200 OK");
          }
          else if (StrContains(HTTP_req, "GET /remove_log")){
            sclient.println("HTTP/1.1 200 OK");
            if (SD.exists("log.xml")){
              SD.remove("log.xml");
            }
          }
          else if (StrContains(HTTP_req, "GET /log.xml")){ 
            webFile = SD.open("log.xml");
            if (webFile) {
              sclient.println("HTTP/1.1 200 OK");
              sclient.println();
            }
          }
          else if (StrContains(HTTP_req, "POST /editJSON")) {
            if (SD.exists("sis.jso")){
              SD.remove("sis.jso");
            }
            sisFile = SD.open("sis.jso",FILE_WRITE);

            while(sclient.connected()){
              sclient.println("HTTP/1.1 200 OK");
              while(sclient.available()){       
                char f = sclient.read();                
                sisFile.write(f);
                Serial.write(f);
                delay(4);
              } 
              req_index = 0;
              StrClear(HTTP_req, REQ_BUF_SZ);
              break;
            }
            sisFile.close(); 
          }

          /////////////////////////////////////Вот заканчивается Ajax///////////////////////////////////////////////
          if (webFile) {
            while (webFile.available()) {
              sclient.write(webFile.read());  // отправляем готовые данные на страницу браузера
            }
            webFile.close();
          }
          req_index = 0;
          StrClear(HTTP_req, REQ_BUF_SZ);
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    sclient.stop();
  }
}

void StrClear(char *str, char length) {
  for (int i = 0; i < length; i++) {
    str[i] = 0;
  }
}

char StrContains(char *str, char *sfind) {
  char found = 0;
  char index = 0;
  char len;
  len = strlen(str);
  if (strlen(sfind) > len) {
    return 0;
  }
  while (index < len) {
    if (str[index] == sfind[found]) {
      found++;
      if (strlen(sfind) == found) {
        return 1;
      }
    } else {
      found = 0;
    }
    index++;
  }
  return 0;
}
void serverWorks() {
  for (int sock = 0; sock < MAX_SOCK_NUM - FREE_SOCKETS; sock++) {
    EthernetClient sclient = server.available_(sock);
    serverWorks2(sclient);
  }
  /*
  EthernetClient sclient = server.available();
  serverWorks2(sclient);
  */
}

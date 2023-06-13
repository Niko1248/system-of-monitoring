#define Version 1.08

<<<<<<< HEAD
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#define REQ_BUF_SZ 128
#define FIRST_PIN 40
#define LAST_PIN 44
#define FREE_SOCKETS 1
=======
#define SIGRD 5
>>>>>>> 56ba87b99a64b5147f2befaada0b09320b1930bf

#include <avr/boot.h>
#include <ArduinoUniqueID.h>


typedef struct {
  byte sig[3];
  const char *desc;
} signatureType;

int foundSig = -1;
unsigned int tik = 0;
byte freq, count = 0;

volatile unsigned int int_tic;
volatile unsigned long tic;
bool readwdt = 0;
unsigned int n = 0;

void setWDT() {
  TCCR1B = 0; TCCR1A = 0; TCNT1 = 0;
  TIMSK1 = 1 << TOIE1;
  TCCR1B = 1 << CS10;
  WDTCSR = (1 << WDCE) | (1 << WDE); //установить биты WDCE WDE (что б разрешить запись в другие биты
  WDTCSR = (1 << WDIE) | (1 << WDP2) | (1 << WDP1); // разрешение прерывания + выдержка 1 секунда(55 страница даташита)
}

ISR (TIMER1_OVF_vect) { //прерывания счёта по переполнению uint
  int_tic++; //считать переполнения через 65536 тактов
}

void setup()
{
  Serial.begin(115200);
  Serial.print(F("\nSysInfo for Arduino version "));
  Serial.println(Version);
  Serial.println();
  
  //UniqueIDdump(Serial);
  Serial.print("UniqueID: ");
  for (size_t i = 0; i < UniqueIDsize; i++)
  {
    if (UniqueID[i] < 0x10)
      Serial.print("0");
    Serial.print(UniqueID[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  
  delay(1000);

  Frequency();
  Serial.println();
  Serial.print(F("F_CPU = "));
  Serial.println(F_CPU);

  Serial.print(F("Frequency = "));
  Serial.print(freq);
  Serial.println(F(" MHz"));
  Serial.println();

  CPU();
  Memory();
  Signature();
  VCC();
  Temperature();
  TestPins();
  int_tic = 0;
  readwdt = true;
  setWDT();
}

void loop() {}

<<<<<<< HEAD
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
          else if (StrContains(HTTP_req, "GET /admin") || StrContains(HTTP_req, "GET /admin.htm")) {
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
              sclient.println("Cache-Control: public, max-age=31536000");
              sclient.println();
            }
          }
          else if (StrContains(HTTP_req, "GET /admstyle.css")) {
            webFile = SD.open("admstyle.css");
            if (webFile) {
              sclient.println("HTTP/1.1 200 OK");
              sclient.println("Cache-Control: public, max-age=31536000");
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
          else if (StrContains(HTTP_req, "GET /lupa.png")) {
            webFile = SD.open("lupa.png");
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
            if (!webFile){
              sclient.println("HTTP/1.1 404 Not Found");
            }
          } 
          else if (StrContains(HTTP_req, "GET /g")){ 
            for (int i = FIRST_PIN; i <= LAST_PIN; i++) {  // Установил все пины в input
              pinMode(i, OUTPUT);
              digitalWrite(i, 1);
            }
          }
          /////////////////////////////////////Вот заканчивается Ajax///////////////////////////////////////////////
          if (webFile) {
            while (webFile.available()) {
              sclient.write(webFile.read());  // отправляем готовые данные на страницу браузера
            }
            webFile.close();
          }
=======
void Frequency()
{
  byte fcpu;

  TCCR1A = 0;
  TCCR1B = 1 << CS10 | 1 << CS12; // clk/1024
  TCNT1 = 0;
>>>>>>> 56ba87b99a64b5147f2befaada0b09320b1930bf

  WDTCSR = (1 << WDCE) | (1 << WDE);
  WDTCSR = (1 << WDIE); // enable WDT delay 16ms

  while (count < 3) delay(1);

  WDTCSR = (1 << WDCE); // disable WDT

  if (tik <= 30) freq = 1;
  if ((tik > 30)  && (tik <= 100)) freq = 4;
  if ((tik > 100) && (tik <= 170)) freq = 8;
  if ((tik > 170) && (tik <= 240)) freq = 12;
  if ((tik > 240) && (tik <= 320)) freq = 16;
  if (tik > 320) freq = 20;

  fcpu = F_CPU / 1000000L;

  if ((fcpu == 16) && (freq == 8)) Serial.begin(19200);
}

ISR (WDT_vect) {
  if (!readwdt) {
    tik = TCNT1;
    count++;
    TCNT1 = 0;
  } else {
    if (n < 10) {
      n++;
      tic = ((uint32_t)int_tic << 16) | TCNT1 ; //подсчёт тиков
      Serial.print(" 1 Sec WDT= ");
      Serial.print( (float) tic * 625E-10 , 6 );
      ICR1 = 0; int_tic = 0; TCNT1 = 0;
      Serial.print(' ');
      Serial.println("Seconds");
    }
  }
}

void CPU()
{
  Serial.print(F("CPU_IDE = "));

#if   defined(__AVR_ATmega328P__)
  Serial.println(F("ATmega328P"));
#elif defined(__AVR_ATmega48__)
  Serial.println(F("ATmega48"));
#elif defined(__AVR_ATmega48P__)
  Serial.println(F("ATmega48P"));
#elif defined(__AVR_ATmega88__)
  Serial.println(F("ATmega88"));
#elif defined(__AVR_ATmega88P__)
  Serial.println(F("ATmega88P"));
#elif defined(__AVR_ATmega168__)
  Serial.println(F("ATmega168"));
#elif defined(__AVR_ATmega168P__)
  Serial.println(F("ATmega168P"));
#elif defined(__AVR_ATmega328__)
  Serial.println(F("ATmega328"));
#elif defined(__AVR_ATmega164__)
  Serial.println(F("ATmega164"));
#elif defined(__AVR_ATmega164P__)
  Serial.println(F("ATmega164P"));
#elif defined(__AVR_ATmega324__)
  Serial.println(F("ATmega324"));
#elif defined(__AVR_ATmega324P__)
  Serial.println(F("ATmega324P"));
#elif defined(__AVR_ATmega644__)
  Serial.println(F("ATmega644"));
#elif defined(__AVR_ATmega644P__)
  Serial.println(F("ATmega644P"));
#elif defined(__AVR_ATmega1284__)
  Serial.println(F("ATmega1284"));
#elif defined(__AVR_ATmega1284P__)
  Serial.println(F("ATmega1284P"));
#elif defined(__AVR_ATmega640__)
  Serial.println(F("ATmega640"));
#elif defined(__AVR_ATmega1280__)
  Serial.println(F("ATmega1280"));
#elif defined(__AVR_ATmega1281__)
  Serial.println(F("ATmega1281"));
#elif defined(__AVR_ATmega2560__)
  Serial.println(F("ATmega2560"));
#elif defined(__AVR_ATmega2561__)
  Serial.println(F("ATmega2561"));
#elif defined(__AVR_ATmega8U2__)
  Serial.println(F("ATmega8U2"));
#elif defined(__AVR_ATmega16U2__)
  Serial.println(F("ATmega16U2"));
#elif defined(__AVR_ATmega32U2__)
  Serial.println(F("ATmega32U2"));
#elif defined(__AVR_ATmega16U4__)
  Serial.println(F("ATmega16U4"));
#elif defined(__AVR_ATmega32U4__)
  Serial.println(F("ATmega32U4"));
#elif defined(__AVR_AT90USB82__)
  Serial.println(F("AT90USB82"));
#elif defined(__AVR_AT90USB162__)
  Serial.println(F("AT90USB162"));
#elif defined(__AVR_ATtiny24__)
  Serial.println(F("ATtiny24"));
#elif defined(__AVR_ATtiny44__)
  Serial.println(F("ATtiny44"));
#elif defined(__AVR_ATtiny84__)
  Serial.println(F("ATtiny84"));
#elif defined(__AVR_ATtiny25__)
  Serial.println(F("ATtiny25"));
#elif defined(__AVR_ATtiny45__)
  Serial.println(F("ATtiny45"));
#elif defined(__AVR_ATtiny85__)
  Serial.println(F("ATtiny85"));
#elif defined(__AVR_ATtiny13__)
  Serial.println(F("ATtiny13"));
#elif defined(__AVR_ATtiny13A__)
  Serial.println(F("ATtiny13A"));
#elif defined(__AVR_ATtiny2313__)
  Serial.println(F("ATtiny2313"));
#elif defined(__AVR_ATtiny2313A__)
  Serial.println(F("ATtiny2313A"));
#elif defined(__AVR_ATtiny4313__)
  Serial.println(F("ATtiny4313"));
#elif defined(__AVR_ATmega8__)
  Serial.println(F("ATmega8"));
#elif defined(__AVR_ATmega8A__)
  Serial.println(F("ATmega8A"));
#else
  Serial.println(F("Unknown"));
#endif

  Serial.println();
}

void Memory()
{
  extern int __bss_end, *__brkval;
  int freeRam;

  Serial.print(F("Flash Memory = "));
  Serial.print(FLASHEND);
  Serial.println(F(" bytes"));

  if ((int)__brkval == 0)
    freeRam = ((int)&freeRam) - ((int)&__bss_end);
  else
    freeRam = ((int)&freeRam) - ((int)__brkval);

  Serial.print(F("Free RAM memory = "));
  Serial.print(freeRam);
  Serial.println(F(" bytes"));

  Serial.println();
}

void Signature()
{
  const signatureType signatures [] = {
    { { 0x1E, 0x95, 0x0F }, "ATmega328P",  },    //  0
    { { 0x1E, 0x92, 0x05 }, "ATmega48A",   },    //  1
    { { 0x1E, 0x92, 0x0A }, "ATmega48PA",  },    //  2
    { { 0x1E, 0x93, 0x0A }, "ATmega88A",   },    //  3
    { { 0x1E, 0x93, 0x0F }, "ATmega88PA",  },    //  4
    { { 0x1E, 0x94, 0x06 }, "ATmega168A",  },    //  5
    { { 0x1E, 0x94, 0x0B }, "ATmega168PA", },    //  6
    { { 0x1E, 0x95, 0x14 }, "ATmega328",   },    //  7
    { { 0x1E, 0x95, 0x16 }, "ATmega328PB", },    //  8
    { { 0x1E, 0x94, 0x0A }, "ATmega164P",  },    //  9
    { { 0x1E, 0x95, 0x08 }, "ATmega324P",  },    // 10
    { { 0x1E, 0x96, 0x0A }, "ATmega644P",  },    // 11
    { { 0x1E, 0x97, 0x05 }, "ATmega1284P", },    // 12
    { { 0x1E, 0x97, 0x06 }, "ATmega1284",  },    // 13
    { { 0x1E, 0x96, 0x08 }, "ATmega640",   },    // 14
    { { 0x1E, 0x97, 0x03 }, "ATmega1280",  },    // 15
    { { 0x1E, 0x97, 0x04 }, "ATmega1281",  },    // 16
    { { 0x1E, 0x98, 0x01 }, "ATmega2560",  },    // 17
    { { 0x1E, 0x98, 0x02 }, "ATmega2561",  },    // 18
    { { 0x1E, 0x93, 0x89 }, "ATmega8U2",   },    // 19
    { { 0x1E, 0x94, 0x89 }, "ATmega16U2",  },    // 20
    { { 0x1E, 0x95, 0x8A }, "ATmega32U2",  },    // 21
    { { 0x1E, 0x94, 0x88 }, "ATmega16U4",  },    // 22
    { { 0x1E, 0x95, 0x87 }, "ATmega32U4",  },    // 23
    { { 0x1E, 0x93, 0x82 }, "At90USB82",   },    // 24
    { { 0x1E, 0x94, 0x82 }, "At90USB162",  },    // 25
    { { 0x1E, 0x91, 0x0B }, "ATtiny24",    },    // 26
    { { 0x1E, 0x92, 0x07 }, "ATtiny44",    },    // 27
    { { 0x1E, 0x93, 0x0C }, "ATtiny84",    },    // 28
    { { 0x1E, 0x91, 0x08 }, "ATtiny25",    },    // 29
    { { 0x1E, 0x92, 0x06 }, "ATtiny45",    },    // 30
    { { 0x1E, 0x93, 0x0B }, "ATtiny85",    },    // 31
    { { 0x1E, 0x91, 0x0A }, "ATtiny2313A", },    // 32
    { { 0x1E, 0x92, 0x0D }, "ATtiny4313",  },    // 33
    { { 0x1E, 0x90, 0x07 }, "ATtiny13A",   },    // 34
    { { 0x1E, 0x93, 0x07 }, "ATmega8A",    }     // 35
  };

  int flashSize, NumSig = 36;
  byte sig[3], fuse;

  Serial.print(F("Signature = "));

  sig[0] = boot_signature_byte_get(0);
  if (sig[0] < 16) Serial.print("0");
  Serial.print(sig[0], HEX);
  Serial.print(" ");

  sig[1] = boot_signature_byte_get(2);
  if (sig[1] < 16) Serial.print("0");
  Serial.print(sig[1], HEX);
  Serial.print(" ");

  sig[2] = boot_signature_byte_get(4);
  if (sig[2] < 16) Serial.print("0");
  Serial.println(sig[2], HEX);

  Serial.print(F("Fuses (Low/High/Ext/Lock) = "));

  fuse = boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS);
  if (fuse < 16) Serial.print("0");
  Serial.print(fuse, HEX);
  Serial.print(" ");

  fuse = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
  if (fuse < 16) Serial.print("0");
  Serial.print(fuse, HEX);
  Serial.print(" ");

  fuse = boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS);
  if (fuse < 16) Serial.print("0");
  Serial.print(fuse, HEX);
  Serial.print(" ");

  fuse = boot_lock_fuse_bits_get(GET_LOCK_BITS);
  if (fuse < 16) Serial.print("0");
  Serial.println(fuse, HEX);

  flashSize = 1 << (sig[1] & 0x0F);

  for (int j = 0; j < NumSig; j++)
  {
    if (memcmp(sig, signatures[j].sig, 3) == 0)
    {
      foundSig = j;
      Serial.print(F("Processor = "));
      Serial.println(signatures[j].desc);

      Serial.print(F("Flash memory size = "));
      Serial.print(flashSize, DEC);
      Serial.println(F(" kB"));
      break;
    }
  }

  if (foundSig < 0) Serial.println(F("Unrecogized signature"));

  Serial.println();
}

void VCC()
{
#define Vref 1100
  int mvVcc;

  if ((foundSig >= 0) && (foundSig <= 8)) {

    ADMUX = (1 << REFS0) | 0x0E;
    ADCSRB = 0;
    ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADSC) | 0x05;
    delay(1);
    mvVcc = (1023L * Vref) / ADC;

    Serial.print(F("VCC = "));
    Serial.print(mvVcc);
    Serial.println(F(" mV"));
    Serial.println();
  }

  if ((foundSig >= 14) && (foundSig <= 18)) {

    ADMUX = (1 << REFS0) | 0x1E;
    ADCSRB = 0;
    ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADSC) | 0x05;
    delay(1);
    mvVcc = (1023L * Vref) / ADC;

    Serial.print(F("VCC = "));
    Serial.print(mvVcc);
    Serial.println(F(" mV"));
    Serial.println();
  }

  if ((foundSig >= 22) && (foundSig <= 23)) {

    ADMUX = (1 << REFS0) | 0x1E;
    ADCSRB = 0;
    ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADSC) | 0x05;
    delay(1);
    mvVcc = (1023L * Vref) / ADC;

    Serial.print(F("VCC = "));
    Serial.print(mvVcc);
    Serial.println(F(" mV"));
    Serial.println();
  }

  if ((foundSig >= 29) && (foundSig <= 31)) {

    ADMUX = 0x0E;
    ADCSRB = 0;
    ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADSC) | 0x05;
    delay(1);
    mvVcc = (1023L * Vref) / ADC;

    Serial.print(F("VCC = "));
    Serial.print(mvVcc);
    Serial.println(F(" mV"));
    Serial.println();
  }
}

void Temperature()
{
  float temperature;

  if ((foundSig >= 0) && (foundSig <= 8)) {

    ADMUX = (1 << REFS1) | (1 << REFS0) | (1 << MUX3);
    ADCSRA |= (1 << ADEN);
    delay(20);
    ADCSRA |= (1 << ADSC);
    delay(1);

    temperature = (ADC - 324.31) / 1.22;

    Serial.print(F("Internal Temperature = "));
    Serial.print(temperature, 1);
    Serial.println(F(" C"));
    Serial.println();
  }

  if ((foundSig >= 22) && (foundSig <= 23)) {

    ADMUX = (1 << REFS1) | (1 << REFS0) | 0x07;
    ADCSRB = 0x20;
    ADCSRA |= (1 << ADEN);
    delay(20);
    ADCSRA |= (1 << ADSC);
    delay(1);

    temperature = (ADC - 324.31) / 1.22;

    Serial.print(F("Internal Temperature = "));
    Serial.print(temperature, 1);
    Serial.println(F(" C"));
    Serial.println();
  }

  if ((foundSig >= 29) && (foundSig <= 31)) {

    ADMUX = (1 << REFS1) | 0x0F;
    ADCSRA |= (1 << ADEN);
    delay(20);
    ADCSRA |= (1 << ADSC);
    delay(1);

    temperature = (ADC - 324.31) / 1.22;

    Serial.print(F("Internal Temperature = "));
    Serial.print(temperature, 1);
    Serial.println(F(" C"));
    Serial.println();
  }
}

void TestPins()
{
#define FIRST_PIN 0
#define LAST_PIN 19

  Serial.println(F("Test of short circuit on GND or VCC:"));

  for (byte pin = FIRST_PIN; pin <= LAST_PIN; pin++)
  {
    if (pin < 10) Serial.print(F("Pin:  "));
    else Serial.print(F("Pin: "));
    Serial.print(pin);

    pinMode(pin, OUTPUT);
    digitalWrite(pin, 0);
    Serial.print(F("    Low: "));
    if (!digitalRead(pin)) Serial.print(F("Ok  "));
    else Serial.print(F("Fail"));

    digitalWrite(pin, 1);
    Serial.print(F("  High: "));
    if (digitalRead(pin)) Serial.print(F("Ok  "));
    else Serial.print(F("Fail"));

    pinMode(pin, INPUT_PULLUP);
    Serial.print(F("  Pull Up: "));
    if (digitalRead(pin)) Serial.print(F("Ok  "));
    else Serial.print(F("Fail"));

    Serial.println();
    pinMode(pin, INPUT);
  }

  Serial.println();
}
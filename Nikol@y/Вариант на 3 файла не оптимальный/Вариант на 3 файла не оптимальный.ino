#include <SD.h>
#include <SPI.h>
#include <Ethernet.h>
// comment out next line to write to SD from FTP server
// #define FTPWRITE

// this must be unique
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x59, 0x67 };  

// change to your network settings
IPAddress ip( 192, 168, 0, 110 );    
IPAddress gateway( 192, 168, 0, 1 );
IPAddress subnet( 255, 255, 255, 0 );

// change to your server
IPAddress server( 5, 101, 152, 139 );

EthernetClient client;
EthernetClient dclient;

char outBuf[128];
char outCount;

// change fileName to your file (8.3 format!)
char fileName[13] = "index.htm";
char fileName2[13] = "sis.xml";
char fileName3[13] = "style.css";

void setup()
{
  Serial.begin(9600);

  digitalWrite(10,HIGH);

  if(SD.begin(4) == 0)
  {
    Serial.println(F("SD init fail"));          
  }

  Ethernet.begin(mac, ip, gateway, gateway, subnet); 
  digitalWrite(10,HIGH);
  delay(1000);
  Serial.println(F("Ready. Press a =all; h=HTML; x=XML; C=CSS"));
}

void loop()
{
  byte inChar;

  inChar = Serial.read();
    if(inChar == 'a')
  {
    if(doFTP1()) Serial.println(F("HTML OK"));
    if(doFTP2()) Serial.println(F("XML OK"));
    if(doFTP3()) Serial.println(F("CSS OK"));
    else Serial.println(F("FTP FAIL"));
  }
  if(inChar == 'h')
  {
    if(doFTP1()) Serial.println(F("HTML OK"));
    else Serial.println(F("FTP FAIL"));
  }
  if(inChar == 'x')
  {
    if(doFTP2()) Serial.println(F("XML OK"));
    else Serial.println(F("FTP FAIL"));
  }
  if(inChar == 'c')
  {
    if(doFTP3()) Serial.println(F("CSS OK"));
    else Serial.println(F("FTP FAIL"));
  }


}

File fh;
File fh2;
File fh3;

byte doFTP1()
{
  SD.remove(fileName);
  fh = SD.open(fileName,FILE_WRITE);

  if(!fh)
  {
    Serial.println(F("SD open fail"));
    return 0;    
  }

  Serial.println(F("SD opened"));

  if (client.connect(server,21)) {
    Serial.println(F("Command connected"));
  } 
  else {
    fh.close();
    Serial.println(F("Command connection failed"));
    return 0;
  }

  if(!eRcv()) return 0;

  client.println(F("USER siriusfa"));

  if(!eRcv()) return 0;

  client.println(F("PASS 8jCCEKc1"));

  if(!eRcv()) return 0;

  client.println(F("SYST"));

  if(!eRcv()) return 0;

  client.println(F("Type I"));

  if(!eRcv()) return 0;

  client.println(F("PASV"));

  if(!eRcv()) return 0;

  char *tStr = strtok(outBuf,"(,");
  int array_pasv[6];
  for ( int i = 0; i < 6; i++) {
    tStr = strtok(NULL,"(,");
    array_pasv[i] = atoi(tStr);
    if(tStr == NULL)
    {
      Serial.println(F("Bad PASV Answer"));    

    }
  }

  unsigned int hiPort,loPort;

  hiPort = array_pasv[4] << 8;
  loPort = array_pasv[5] & 255;

  Serial.print(F("Data port: "));
  hiPort = hiPort | loPort;
  Serial.println(hiPort);

  if (dclient.connect(server,hiPort)) {
    Serial.println(F("Data connected"));
  } 
  else {
    Serial.println(F("Data connection failed"));
    client.stop();
    fh.close();
    return 0;
  }

  client.print(F("RETR "));
  client.println(fileName);

  if(!eRcv())
  {
    dclient.stop();
    return 0;
  }


  while(dclient.connected())
  {
    while(dclient.available())
    {
      char c = dclient.read();
      fh.write(c);      
      Serial.write(c); 
    }
  }

  dclient.stop();
  Serial.println(F("Data disconnected"));


  client.println(F("QUIT"));


  client.stop();
  Serial.println(F("Command disconnected"));

  fh.close();
  Serial.println(F("SD closed"));
  delay(4000);
  return 1;
}


byte doFTP2()
{
  SD.remove(fileName2);
  fh2 = SD.open(fileName2,FILE_WRITE);

  if(!fh2)
  {
    Serial.println(F("SD open fail"));
    return 0;    
  }

  Serial.println(F("SD opened"));

  if (client.connect(server,21)) {
    Serial.println(F("Command connected"));
  } 
  else {
    fh2.close();
    Serial.println(F("Command connection failed"));
    return 0;
  }

  if(!eRcv()) return 0;

  client.println(F("USER siriusfa"));

  if(!eRcv()) return 0;

  client.println(F("PASS 8jCCEKc1"));

  if(!eRcv()) return 0;

  client.println(F("SYST"));

  if(!eRcv()) return 0;

  client.println(F("Type I"));

  if(!eRcv()) return 0;

  client.println(F("PASV"));

  if(!eRcv()) return 0;

  char *tStr = strtok(outBuf,"(,");
  int array_pasv[6];
  for ( int i = 0; i < 6; i++) {
    tStr = strtok(NULL,"(,");
    array_pasv[i] = atoi(tStr);
    if(tStr == NULL)
    {
      Serial.println(F("Bad PASV Answer"));    

    }
  }

  unsigned int hiPort,loPort;

  hiPort = array_pasv[4] << 8;
  loPort = array_pasv[5] & 255;

  Serial.print(F("Data port: "));
  hiPort = hiPort | loPort;
  Serial.println(hiPort);

  if (dclient.connect(server,hiPort)) {
    Serial.println(F("Data connected"));
  } 
  else {
    Serial.println(F("Data connection failed"));
    client.stop();
    fh2.close();
    return 0;
  }

  client.print(F("RETR "));
  client.println(fileName2);

  if(!eRcv())
  {
    dclient.stop();
    return 0;
  }


  while(dclient.connected())
  {
    while(dclient.available())
    {
      char c = dclient.read();
      fh2.write(c);      
      Serial.write(c); 
    }
  }

  dclient.stop();
  Serial.println(F("Data disconnected"));

  client.println(F("QUIT"));

  client.stop();
  Serial.println(F("Command disconnected"));

  fh2.close();
  Serial.println(F("SD closed"));
  delay(4000);
  return 1;
}

byte doFTP3()
{
  SD.remove(fileName3);
  fh3 = SD.open(fileName3,FILE_WRITE);

  if(!fh3)
  {
    Serial.println(F("SD open fail"));
    return 0;    
  }

  Serial.println(F("SD opened"));

  if (client.connect(server,21)) {
    Serial.println(F("Command connected"));
  } 
  else {
    fh3.close();
    Serial.println(F("Command connection failed"));
    return 0;
  }

  if(!eRcv()) return 0;

  client.println(F("USER siriusfa"));

  if(!eRcv()) return 0;

  client.println(F("PASS 8jCCEKc1"));

  if(!eRcv()) return 0;

  client.println(F("SYST"));

  if(!eRcv()) return 0;

  client.println(F("Type I"));

  if(!eRcv()) return 0;

  client.println(F("PASV"));

  if(!eRcv()) return 0;

  char *tStr = strtok(outBuf,"(,");
  int array_pasv[6];
  for ( int i = 0; i < 6; i++) {
    tStr = strtok(NULL,"(,");
    array_pasv[i] = atoi(tStr);
    if(tStr == NULL)
    {
      Serial.println(F("Bad PASV Answer"));    

    }
  }

  unsigned int hiPort,loPort;

  hiPort = array_pasv[4] << 8;
  loPort = array_pasv[5] & 255;

  Serial.print(F("Data port: "));
  hiPort = hiPort | loPort;
  Serial.println(hiPort);

  if (dclient.connect(server,hiPort)) {
    Serial.println(F("Data connected"));
  } 
  else {
    Serial.println(F("Data connection failed"));
    client.stop();
    fh3.close();
    return 0;
  }

  client.print(F("RETR "));
  client.println(fileName3);

  if(!eRcv())
  {
    dclient.stop();
    return 0;
  }


  while(dclient.connected())
  {
    while(dclient.available())
    {
      char c = dclient.read();
      fh3.write(c);      
      Serial.write(c); 
    }
  }

  dclient.stop();
  Serial.println(F("Data disconnected"));

  if(!eRcv()) return 0;

  client.println(F("QUIT"));

  if(!eRcv()) return 0;

  client.stop();
  Serial.println(F("Command disconnected"));

  fh3.close();
  Serial.println(F("SD closed"));
  return 1;
}


byte eRcv()
{
  byte respCode;
  byte thisByte;

  while(!client.available()) delay(1);

  respCode = client.peek();

  outCount = 0;

  while(client.available())
  {  
    thisByte = client.read();    
    Serial.write(thisByte);

    if(outCount < 127)
    {
      outBuf[outCount] = thisByte;
      outCount++;      
      outBuf[outCount] = 0;
    }
  }

  if(respCode >= '4')
  {
    efail();
    return 0;  
  }

  return 1;
}


void efail()
{
  byte thisByte = 0;

  client.println(F("QUIT"));

  while(!client.available()) delay(1);

  while(client.available())
  {  
    thisByte = client.read();    
    Serial.write(thisByte);
  }

  client.stop();
  Serial.println(F("Command disconnected"));
  fh.close();
  Serial.println(F("SD closed"));
}

#define FIRST_PIN 3 // первый вывод
#define LAST_PIN 53// последний вывод

void Test1(byte pin)
{
  if(pin < 10) Serial.print(" PIN:  ");
  else Serial.print(" PIN: ");
  Serial.print(pin);
  pinMode(pin, OUTPUT);
  digitalWrite(pin, 0);
  Serial.print("    LOW: ");
  if(!digitalRead(pin)) Serial.print("OK  ");
  else Serial.print("FAIL");
  digitalWrite(pin, 1);
  Serial.print("  HIGH: ");
  if(digitalRead(pin)) Serial.print("OK  "); 
  else Serial.print("FAIL");
  pinMode(pin, INPUT);
  Serial.print("  PULL UP: ");
  if(digitalRead(pin)) Serial.print("OK  ");
  else Serial.print("FAIL");
  digitalWrite(pin, 0);
}

void Test2(byte pin)
{
  Serial.print("     ");
  pinMode(pin, OUTPUT);
  digitalWrite(pin, 1);
  delay(5);
  if(!digitalRead(pin))Serial.println("SHORT");
  else Serial.println("OK");
  pinMode(pin, INPUT); 
  digitalWrite(pin, 0);

}

void setup() {
  Serial.begin(9600); 

  Serial.println("Test of short circuit on GND or VCC and between pins:");
  Serial.println();
  for(byte i = FIRST_PIN; i <= LAST_PIN; i++) 
  {
    for(byte j = FIRST_PIN; j <= LAST_PIN; j++) 
    {
      pinMode(j, INPUT);
      digitalWrite(j, 0);
    }
    Test1(i);

    for(byte j = FIRST_PIN; j <= LAST_PIN; j++) 
    {
      pinMode(j, OUTPUT);
      digitalWrite(j, 0);
    }
    Test2(i);
  }

  for(byte j = FIRST_PIN; j <= LAST_PIN; j++) 
  {
    pinMode(j, INPUT);
    digitalWrite(j, 0);
  }
}

void loop() {}
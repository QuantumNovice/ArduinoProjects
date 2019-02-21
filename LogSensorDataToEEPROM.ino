#include <EEPROM.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (analogRead(A0) == 0){
    logEEPROM(100);
    while (1){
      Blink(100);
    }
  }
  if (Serial.available()){
    readEEPROM();
  }
  //Serial.println(analogRead(A0));
}

void logEEPROM(int wait){
  int addr = 0;
  while (!Serial.available()){
    int reading = analogRead(A1);
    EEPROM.write(addr, reading);
    addr = addr + 1;
    if (addr == EEPROM.length()){
      addr = 0;
      break;
    }
    delay(wait);
  }
}

void readEEPROM(){
  int addr = 0;
  byte value;
  while (1){
    value = EEPROM.read(addr);
    Serial.print(addr);
    Serial.print(",");
    Serial.print(value, DEC);
    Serial.println();
    addr = addr + 1;
    if (addr == EEPROM.length()){
      addr = 0;
      break;
    }
  }
}

void Blink(int wait){
  delay(wait/2);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(wait/2);
  digitalWrite(LED_BUILTIN, LOW);
}

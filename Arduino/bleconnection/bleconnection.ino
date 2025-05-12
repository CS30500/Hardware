#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2,3);

void setup(){
  Serial.begin(9600);
  BTSerial.begin(9600);
  Serial.println("HM-10 Bluetooth started");
}

void loop(){
  if (BTSerial.available()){
    char c = BTSerial.read();
    Serial.print("From BLE : ");
    Serial.println(c);
  }

  if(Serial.available()){
    char c = Serial.read();
    BTSerial.write(c);
  }
}


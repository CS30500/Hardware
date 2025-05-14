#include <Wire.h>
#include <U8g2lib.h>
#include <SoftwareSerial.h>

// OLED 디스플레이 (I2C, SH1106)
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

// 블루투스 모듈 (HM-10)
SoftwareSerial BTSerial(2, 3); // D2: RX, D3: TX

// 센서 핀
const int tempPin = A0; // Todo : 추후 수정 요망
const int pressurePin = A1; // Todo : 추후 수정 요망
const int trigPin = 4; const int echoPin = 5;

// 출력 핀
const int ledPin = 8; // Todo : 추후 수정 요망
const int buzzerPin = 11;

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.clearBuffer();
  u8g2.drawStr(0, 24, "SmartBottle Ready!");
  u8g2.sendBuffer();
}

void loop() {
  // 1. 센서 값 읽기
  float temperature = getTemperature();
  long distance = getDistance();
  int pressure = analogRead(pressurePin);

  // 2. 블루투스로 전송
  sendSensorData(temperature, distance, pressure);

  // 3. 명령 수신 처리
  if (BTSerial.available()) {
    String cmd = BTSerial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "LED_ON") {
      digitalWrite(ledPin, HIGH);
    } else if (cmd == "LED_OFF") {
      digitalWrite(ledPin, LOW);
    } else if (cmd == "BUZZ_ON") {
      digitalWrite(buzzerPin, HIGH);
    } else if (cmd == "BUZZ_OFF") {
      digitalWrite(buzzerPin, LOW);
    } else if (cmd.startsWith("LCD:")) {
      String message = cmd.substring(4);
      showLCDMessage(message);
    }
  }

  delay(1000); // 1초 간격
}

// --- 센서 처리 함수 ---

float getTemperature() {
  int val = analogRead(tempPin);
  float voltage = val * (5.0 / 1023.0);
  return (voltage - 0.5) * 100;
}

long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH) * 0.034 / 2;
}

void sendSensorData(float temp, long dist, int press) {
  BTSerial.print("TEMP:");
  BTSerial.println(temp);
  BTSerial.print("DIST:");
  BTSerial.println(dist);
  BTSerial.print("PRESS:");
  BTSerial.println(press);
}

void showLCDMessage(String msg) {
  u8g2.clearBuffer();
  u8g2.setCursor(0, 24);
  u8g2.print(msg);
  u8g2.sendBuffer();
}
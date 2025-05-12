#include <Wire.h>
#include <U8g2lib.h>
#include <SoftwareSerial.h>

// OLED 디스플레이 (SH1106, I2C)
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

// 초음파 핀
const int trigPin = 4;
const int echoPin = 5;

// 부저 핀
const int buzzerPin = 11;

// 블루투스 (HM-10)
SoftwareSerial BTSerial(2, 3); // D2 = RX, D3 = TX

void setup() {
  // 시리얼 통신 시작
  Serial.begin(9600);
  BTSerial.begin(9600);

  // 초기화 메시지 한 번만 출력
  Serial.println("✅ HM-10 Bluetooth started");

  // OLED 초기화 + 고정 메시지 출력
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 24);
  u8g2.print("                  ^_^");
  u8g2.sendBuffer();

  // 핀 설정
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // 1. 초음파 거리 측정
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;

  // 2. 거리값 시리얼 모니터로만 출력
  Serial.print("📏 거리: ");
  Serial.print(distance);
  Serial.println(" cm");

  // 3. 부저 제어 (10cm 미만일 때 울림)
  if (distance < 10.0) {
    tone(buzzerPin, 1000);
  } else {
    noTone(buzzerPin);
  }

  // 4. BLE ↔ Serial 중계
  if (BTSerial.available()) {
    char c = BTSerial.read();
    Serial.print("📥 From BLE: ");
    Serial.println(c);
  }

  if (Serial.available()) {
    char c = Serial.read();
    BTSerial.write(c);
  }

  delay(500);
}

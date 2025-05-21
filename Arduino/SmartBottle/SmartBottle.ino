
#include <Wire.h>
#include <U8g2lib.h>
#include <SoftwareSerial.h>
#include <FastLED.h>

// --- OLED 디스플레이 ---
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0);

// --- 블루투스 ---
SoftwareSerial BTSerial(2, 3); // D2: RX, D3: TX

// --- 센서 핀 ---
const int tempPin = A1;
const int pressurePin = A0;
const int trigPin = 4;
const int echoPin = 5;

// --- 출력 핀 ---
#define LED_PIN     6
#define NUM_LEDS    10
#define BRIGHTNESS  100
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
const int buzzerPin = 11;

// --- LED 배열 선언 ---
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.clearBuffer();
  u8g2.drawStr(0, 24, "SmartBottle Ready!");
  u8g2.sendBuffer();

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
}

void loop() {
  float temperature = getTemperature();
  float distance = getDistance();  // cm
  int pressure = analogRead(pressurePin);

  // BLE 전송
  sendSensorData(temperature, distance, pressure);

  // LED 표시
  showWaterLevel(distance);

  // 명령 수신
  if (BTSerial.available()) {
    String cmd = BTSerial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "BUZZ_ON") {
      digitalWrite(buzzerPin, HIGH);
      showLCDMessage("Buzzer ON");
    } else if (cmd == "BUZZ_OFF") {
      digitalWrite(buzzerPin, LOW);
      showLCDMessage("Buzzer OFF");
    } else if (cmd.startsWith("LCD:")) {
      String message = cmd.substring(4);
      showLCDMessage(message);
    }
  }

  delay(1000);
}

// --- 센서 함수 ---
float getTemperature() {
  int val = analogRead(tempPin);
  float voltage = val * (5.0 / 1023.0);
  return (voltage - 0.5) * 100;
}

float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH) * 0.034 / 2;
}

// --- 데이터 전송 ---
void sendSensorData(float temp, float dist, int press) {
  BTSerial.print("TEMP:");
  BTSerial.println(temp);
  BTSerial.print("DIST:");
  BTSerial.println(dist);
  // BTSerial.print("PRESS:");
  // BTSerial.println(press);
}

// --- LCD 메시지 표시 ---
void showLCDMessage(String msg) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.setCursor(0, 24);
    u8g2.print(msg);
  } while (u8g2.nextPage());
}


// --- 물 높이 표시 (LED) ---
void showWaterLevel(float distance) {
  const float maxDistance = 20; // 컵 최대 높이 (cm 기준)
  const float minDistance = 3;  // 물이 꽉 찼을 때의 거리

  float  capped = constrain(distance, minDistance, maxDistance);
  float fillRatio = (maxDistance - capped) * 1.0 / (maxDistance - minDistance);
  int ledCount = round(fillRatio * NUM_LEDS);

  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < ledCount) {
      leds[i] = CRGB::Red;
    } else {
      leds[i] = CRGB::Black;
    }
  }

  FastLED.show();
}

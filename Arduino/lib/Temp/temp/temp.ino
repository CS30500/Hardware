#include <SoftwareSerial.h>

void setup() {
  Serial.begin(9600); // 시리얼 통신 시작 (9600bps)
}

void loop() {
  int val = analogRead(A1);  // A1 핀에서 아날로그 값 읽기 (0~1023)
  float voltage = (val * 5.0) / 1023.0; // 0~5V로 환산
  Serial.print("Voltage: ");
  Serial.print(voltage, 6);  // 소수점 6자리까지 출력
  Serial.println(" V");
  delay(500);  // 0.5초마다 측정
}

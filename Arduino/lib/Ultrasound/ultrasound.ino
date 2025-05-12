// smartbottle.ino 내에 아래처럼 통합

// 초음파 센서 핀
const int trigPin = 4;
const int echoPin = 5;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // 여기에 다른 장치 초기화도 함께
}

void loop() {
  // 초음파 센서 읽기
  long duration;
  float distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("측정 거리: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);
}

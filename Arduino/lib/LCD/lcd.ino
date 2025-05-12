#include <Wire.h>
#include <U8g2lib.h>

// ✅ 정확한 클래스 이름 (SH1106)
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

void setup() {
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 24, "Hello, Team1234");
  u8g2.sendBuffer();
}

void loop() {
  // 반복 동작 없음
}

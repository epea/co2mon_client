#include <M5StickCPlus.h>
#include <MHZ19.h>

#include "BluetoothSerial.h"

MHZ19 myMHZ19;
BluetoothSerial SerialBT;

//画面の明るさ
bool lcdOn = false;

void setup() {
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextSize(5);  
  //MH-Z19用シリアル
  Serial1.begin(9600, SERIAL_8N1, 26, 0);
  myMHZ19.begin(Serial1);
  myMHZ19.autoCalibration(false);
  delay(1000);
  
  //デバッグ用シリアル
  Serial.begin(115200);
  Serial.println("MH-Z19 test");

  SerialBT.begin("CO2Sensor");
}
int co2 =0;
void loop() {
  M5.Lcd.setCursor(0, 0, 1);
  co2 = myMHZ19.getCO2();
  Serial.println(co2);
  M5.Lcd.print(co2);
  SerialBT.println(co2);
  delay(1000);
}

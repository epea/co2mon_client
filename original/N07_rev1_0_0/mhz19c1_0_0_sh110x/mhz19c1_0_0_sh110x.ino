// 初期表示スプラッシュ抑制
#define SH110X_NO_SPLASH

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3c 

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// https://github.com/nara256/mhz19_uart　から呼んでいない項目を削除
// Adafruit_SH110XのAdafruit_SH1106Gに変更したところメモリがそのままでは足りない。
// 20バイト前後の余力しかないのでprintln追加も不安定になるはず
// Aliexpress1.3inchのOLEDのうち背面の型番がGME12864-77は本来のドライバがssd1315の1.29inchの模様。
// Adafruit_SH1106ライブラリと互換Nanoでは動作がしないもしくは不安定
// https://www.rocher.kyoto.jp/arbr/?p=2108
#include "MHZ19_uart.h"

#define RX_PIN 2
#define TX_PIN 3
#define INTERVAL 6

MHZ19_uart mhz19;

void setup()
{
  Serial.begin(115200);
  Serial.println("start");
  delay(250);
  
  display.begin(i2c_Address, true);
  delay(250);
  
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  delay(250);
  
  displayLCD("start");
  
  mhz19.begin(RX_PIN, TX_PIN);
  mhz19.setAutoCalibration(false);
  delay(1000);
}

byte count = 0;
int co2 = 0;
void loop()
{
  if ((count % INTERVAL) == 0 ) {
    co2 = mhz19.getCO2PPM();

    char buff[9];
    sprintf(buff, "{\"CO2\":%d}", co2);
    Serial.println(buff);
  }
  displayCo2(co2, (count % 2));
  count++;
  delay(1000);
}

void displayLCD(String message) {
  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(0,0);
  display.println(message);
  display.display();
}

void displayCo2(int co2, bool isPiriod) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print("co2 ppm");
  if (isPiriod){
    display.println(" .");
  } else {
    display.println("");
  }
  display.setTextSize(1);
  display.setCursor(0,24);
  display.setTextSize(5);
  char buff[4];
  sprintf(buff, "%4d", co2);
  display.println(buff);
  display.display();
}

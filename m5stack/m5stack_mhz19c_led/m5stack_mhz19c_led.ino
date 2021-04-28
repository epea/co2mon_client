#include <M5Stack.h>
#include <MHZ19_uart.h>
#include <ArduinoJson.h>

#define RX 16
#define TX 17
#define INTERVAL 6

#define LED_PIN 22

MHZ19_uart mhz19;

void setup(){
  M5.begin();
  M5.Lcd.setTextSize(5);
  M5.Lcd.print("start");

  Serial.begin(9600);
  /*
   * ライブラリデフォルトのHardwareSerialNoは2
   * M5Stackの何かの機能で重なったら変更必要かもしれない
   */
  //mhz19.setHardwareSerialNo(1);
  mhz19.begin(RX, TX);
  mhz19.setAutoCalibration(false);
  
  Serial.println("start");

  delay(2000);
  pinMode(LED_PIN, OUTPUT);
}

int count = 0;
int co2 = 0;
void loop()
{
  if ((count % INTERVAL) == 0 ) {
    co2 = mhz19.getCO2PPM();
    displayCo2(co2);
    writeJson(co2);
    digitalWrite(LED_PIN,HIGH);
    m5.update();
  } else {
     M5.Lcd.print(".");
     digitalWrite(LED_PIN,LOW);
     m5.update();
  }
  count++;
  delay(1000);
}

void displayCo2(int co2) {
  M5.Lcd.clear(); 
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("CO2 ppm");
  M5.Lcd.print(co2);
}

void writeJson(int co2){
  const int capacity = JSON_OBJECT_SIZE(1);
  StaticJsonDocument<capacity> json;
  json["CO2"] = co2;
  serializeJson(json, Serial);
  Serial.println();
}

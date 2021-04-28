#include <M5Stack.h>
#include <MHZ19_uart.h>
#include <ArduinoJson.h>

#define RX 16
#define TX 17
#define INTERVAL 6

#define LED_PIN 22

MHZ19_uart mhz19;

/*
 * 濃度警告の閾値と切り替えフラグ
 * 以上じゃなくて「大なり」判定
 */
#define CO2_WARNING_PPM 1000
bool _warnFlg = false;

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

  xTaskCreatePinnedToCore(ledControlTask, "ledControlTask", 1024, NULL, 0, NULL, 0);
}

int count = 0;
int co2 = 0;
void loop()
{
  if ((count % INTERVAL) == 0 ) {
    co2 = mhz19.getCO2PPM();
    displayCo2(co2);
    writeJson(co2);
    _warnFlg = (co2 > CO2_WARNING_PPM);
    m5.update();
  } else {
     M5.Lcd.print(".");
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

void ledControlTask(void *pvParameters)
{
    while (1) {
      ledWarning(_warnFlg);
    }
}

void ledWarning(bool _warnFlg)
{
  if( _warnFlg ){
    for(int i=0;i<10;i++){
          digitalWrite(LED_PIN,HIGH);
          delay(250);
          digitalWrite(LED_PIN,LOW);
          delay(250);
    }
  } else {
    digitalWrite(LED_PIN,LOW);
    delay(5000);
  }
}

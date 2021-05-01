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

void setup()
{
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
  /* 
   * 自動キャリブレーションは切っている 
   * 毎日外気に当てていても電圧安定していない時があるとと結構ズレる。
   * OFFにしたら感覚的に精度±100ppm ONにしたら感覚的に精度±200ppmぐらい
   */
  mhz19.setAutoCalibration(false);

  Serial.println("start");

  delay(2000);
  pinMode(LED_PIN, OUTPUT);

  xTaskCreatePinnedToCore(ledControlTask, "ledControlTask", 1024, NULL, 0, NULL, 0);
}

int count = 0;
int co2 = 0;
bool inCaribrationProcess = false;
void loop()
{
  M5.update();
  // A,C両ボタン長押しでキャリブレーション
  if (M5.BtnA.pressedFor(1000) && M5.BtnB.releasedFor(1000) && M5.BtnC.pressedFor(1000))
  {
    if(inCaribrationProcess){
      delay(5000);
    } else {
      inCaribrationProcess = true;
      M5.Lcd.clear();
      mhz19.calibrateZero();
      M5.Lcd.setCursor(0, 0);
      M5.Lcd.println("finish");
      M5.Lcd.println("calibration");
    }
  }
  else
  {
    inCaribrationProcess = false;
    doNormalProcess();
  }
}

void doNormalProcess()
{
  if ((count % INTERVAL) == 0)
  {
    co2 = mhz19.getCO2PPM();
    displayCo2(co2);
    writeJson(co2);
    _warnFlg = (co2 > CO2_WARNING_PPM);
  }
  else
  {
    M5.Lcd.print(".");
  }
  count++;
  delay(1000);
}

void displayCo2(int co2)
{
  M5.Lcd.clear();
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("CO2 ppm");
  M5.Lcd.print(co2);
}

void writeJson(int co2)
{
  const int capacity = JSON_OBJECT_SIZE(1);
  StaticJsonDocument<capacity> json;
  json["CO2"] = co2;
  serializeJson(json, Serial);
  Serial.println();
}

void ledControlTask(void *pvParameters)
{
  while (1)
  {
    ledWarning(_warnFlg);
  }
}

void ledWarning(bool _warnFlg)
{
  if (_warnFlg)
  {
    for (int i = 0; i < 10; i++)
    {
      digitalWrite(LED_PIN, HIGH);
      delay(250);
      digitalWrite(LED_PIN, LOW);
      delay(250);
    }
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
    delay(5000);
  }
}

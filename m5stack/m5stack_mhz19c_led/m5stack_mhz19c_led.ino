#include <M5Stack.h>

#include <MHZ19_uart.h>

#include <BluetoothSerial.h>

#include <WiFi.h>
#include <WebServer.h>
#include <esp_wifi.h>

#include <ArduinoJson.h>

#define RX 16
#define TX 17
#define INTERVAL 6

#define LED_PIN 22

MHZ19_uart mhz19;

/*
   濃度警告の閾値と切り替えフラグ
   以上じゃなくて「大なり」判定
*/
#define CO2_WARNING_PPM 1000
bool _warnFlg = false;

// wifi起動フラグ
bool _wifiFlg = false;
WebServer server(80);
String serverResponse;

// BLE送信フラグ
bool _bleFlg = false;
BluetoothSerial SerialBT;

void setup()
{
  M5.begin();

  M5.Lcd.setBrightness(200);
  displayLCD("Start");

  Serial.begin(9600);
  Serial.println("Start");

  /*
     この中で呼んでいるESP::restart()はI2Cの再接続がうまくいかない。
     mhzBegin()より前で呼んでいる。
  */
  doSettingProcess();

  delay(2000);
  // LED用スレッド起動
  pinMode(LED_PIN, OUTPUT);
  xTaskCreatePinnedToCore(ledControlTask, "ledControlTask", 1024, NULL, 0, NULL, 1);

  // MHZ起動
  mhzBegin();
}

void doSettingProcess() {
  Serial.println("doSettingProcess");
  M5.update();
  if (M5.BtnA.isPressed())
  {
    startWifi();
    delay(1000);
  }
  if (M5.BtnB.isPressed())
  {
    startBLE();
    delay(1000);
  }
}


void startWifi() {
  Serial.println("Wifi Starting");
  displayLCD("Wifi Starting");
  _wifiFlg = true;

  connectWifi(10000, false);

  Serial.println("");

  if ( WiFi.status() != WL_CONNECTED ) {
    WiFi.mode(WIFI_AP_STA);
    WiFi.beginSmartConfig();
    Serial.println("Waiting for SmartConfig");
    while (!WiFi.smartConfigDone()) {
      delay(500);
      Serial.print(".");
      if ( 30000 < millis() ) {
        Serial.println("");
        Serial.println("Reset");
        ESP.restart();
      }
    }
    Serial.println("SmartConfig received.");
    Serial.println("Waiting for WiFi");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    // 接続成功
    Serial.println("WiFi Connected.");
    displayWifi();
  }
}

void connectWifi(long waitTime, bool shouldConnect) {
  esp_wifi_disconnect();
  Serial.println("connectWifi");
  WiFi.begin();

  long processStartTime = millis();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    if ( waitTime < ( millis() - processStartTime) ) {
      Serial.println("");
      if (shouldConnect) {
        Serial.println("Reset");
        ESP.restart();
      } else {
        Serial.println("break");
        break;
      }
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    displayWifi();
    startServer();
  }
}

void displayWifi() {
  Serial.println("");
  Serial.println("WiFi Connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  displayLCD(ipAddress2String(WiFi.localIP()));
  delay(15000);
}

void startServer() {
  Serial.println("start api server");
  server.on("/", handleAPI);
  server.begin();
}

void handleAPI() {
  Serial.println("handleAPI");
  server.send(200, "application/json", serverResponse);
}


String ipAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") + \
         String(ipAddress[1]) + String(".") + \
         String(ipAddress[2]) + String(".") + \
         String(ipAddress[3])  ;
}

void startBLE() {
  Serial.println("BLE Starting");
  displayLCD("BLE Starting");
  _bleFlg = true;
  SerialBT.begin("CO2Sensor");
}


void mhzBegin() {
  Serial.println("mhzBegin");
  /*
     ライブラリデフォルトのHardwareSerialNoは2
     M5Stackの何かの機能で重なったら変更必要かもしれない
  */
  //mhz19.setHardwareSerialNo(1);
  mhz19.begin(RX, TX);
  /*
     自動キャリブレーションは切っている
     毎日外気に当てていても電圧安定していない時があるとと結構ズレる。
     OFFにしたら感覚的に精度±100ppm ONにしたら感覚的に精度±200ppmぐらい
  */
  mhz19.setAutoCalibration(false);
}


int count = 0;
int co2 = 0;
bool inCaribrationProcess = false;
void loop()
{
  if (_wifiFlg) server.handleClient();

  M5.update();
  // A,C両ボタン長押しでキャリブレーション
  if (M5.BtnA.pressedFor(1000) && M5.BtnB.releasedFor(1000) && M5.BtnC.pressedFor(1000))
  {
    if (inCaribrationProcess) {
      delay(5000);
    } else {
      inCaribrationProcess = true;
      mhz19.calibrateZero();
      displayLCD("finish calibration");
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
    String json = toJson(co2);

    Serial.println(json);


    _warnFlg = (co2 > CO2_WARNING_PPM);

    if (_bleFlg) {
      SerialBT.println(co2);
    }
    if ( _wifiFlg ) {
      serverResponse = json;
    }
  }
  else
  {
    M5.Lcd.print(".");
  }
  count++;
  delay(1000);
}

void displayLCD(String message) {
  M5.Lcd.clear();
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.setTextColor(BLACK, WHITE);
  M5.Lcd.setTextSize(15);
  M5.Lcd.print(message);
}

void displayCo2(int co2)
{
  M5.Lcd.clear();
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.setTextColor(BLACK, WHITE);
  M5.Lcd.setTextSize(15);
  M5.Lcd.println("CO2 ppm");
  M5.Lcd.print(co2);
}

String toJson(int co2)
{
  String result;
  const int capacity = JSON_OBJECT_SIZE(1);
  StaticJsonDocument<capacity> json;
  json["CO2"] = co2;
  serializeJson(json, result);
  return result;
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

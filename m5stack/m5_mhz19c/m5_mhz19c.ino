#include <M5Stack.h>
#include <MHZ19_uart.h>

#define MHZ_RX 16
#define MHZ_TX 17

MHZ19_uart mhz19;

File file;

// 保存するファイル名
const char* fname = "/test_log.csv";

void setup()
{
  Serial.begin(9600);
  mhz19.begin(MHZ_RX, MHZ_TX);
  mhz19.setAutoCalibration(false);

  M5.begin();
  M5.Lcd.setBrightness(200);
  M5.Lcd.fillScreen(WHITE);

  
  Serial.println("MH-Z19 is warming up now.");
  delay(10 * 1000); //
}

void loop()
{
  int co2ppm = mhz19.getCO2PPM();
  int temp = mhz19.getTemperature();

  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.setTextColor(BLACK, WHITE);
  M5.Lcd.setTextSize(15);
  M5.Lcd.printf("CO2:%d\n",co2ppm);

  writeData(co2ppm);
  
  Serial.print("co2: ");
  Serial.println(co2ppm);
  Serial.print("temp: ");
  Serial.println(temp);

  if(co2ppm>500){
  M5.Speaker.setVolume(1);
  M5.Speaker.tone(440, 200);
  delay(1000);
  M5.Speaker.mute();
  }
  delay(5000);
}

void writeData(int param) {
  file = SD.open(fname, FILE_APPEND);
  file.printf("hoge,%d\r",param);
  file.close();
}

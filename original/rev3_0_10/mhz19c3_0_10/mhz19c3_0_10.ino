#include <FaBoLCDmini_AQM0802A.h>
#include <Wire.h>
// https://github.com/nara256/mhz19_uart
#include <MHZ19_uart.h>

#define RX_PIN 2
#define TX_PIN 3
#define INTERVAL 6

MHZ19_uart mhz19;

FaBoLCDmini_AQM0802A lcd;

void setup()
{
  checkLED(1000);
  
  Wire.begin();
  delay(100);
  lcd.begin();
  delay(100);
  displayLCD("Start");
  delay(1000);
  
  checkLED(500);
  
  delay(100);
  mhz19.begin(RX_PIN, TX_PIN);
  mhz19.setAutoCalibration(false);
  delay(1000);
}

void checkLED(int mills) {
  int PIN = 13;
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, HIGH);
  delay(1000);
  digitalWrite(PIN, LOW);
  delay(mills);
}

int count = 0;
int co2 = 0;
void loop()
{
  if ((count % INTERVAL) == 0 ) {
    co2 = mhz19.getCO2PPM();
  }
  displayCo2(co2, (count % 2));
  count++;
  delay(1000);
}

void displayLCD(String message) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);
}

void displayCo2(int co2, bool isPresiod) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CO2 ppm");
  lcd.setCursor(0, 1);
  lcd.print(co2);
  if (isPresiod) {
    lcd.print(" .");
  }
}

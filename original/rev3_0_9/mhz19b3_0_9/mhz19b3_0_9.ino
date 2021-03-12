#include <FaBoLCDmini_AQM0802A.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MHZ19.h>

#define SRX 0
#define STX 1
#define INTERVAL 6

MHZ19 myMHZ19;

SoftwareSerial softSerial(SRX, STX);
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
  
  softSerial.begin(9600);
  delay(100);
  myMHZ19.begin(softSerial);
  myMHZ19.autoCalibration(false);
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
    co2 = myMHZ19.getCO2();
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

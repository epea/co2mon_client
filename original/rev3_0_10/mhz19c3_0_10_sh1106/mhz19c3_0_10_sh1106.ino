#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

// https://github.com/nara256/mhz19_uart
#include <MHZ19_uart.h>

#define RX_PIN 2
#define TX_PIN 3
#define INTERVAL 6

Adafruit_SH1106 display(-1);

MHZ19_uart mhz19;


void setup()
{
  Serial.begin(115200);
  checkLED(1000);
  
  Wire.begin();
  delay(100);

  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  
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
    Serial.println(co2);
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
  display.println("");


  display.setTextSize(5);
  char buff[4];
  sprintf(buff, "%4d", co2);
  display.println(buff);
  display.display();
}

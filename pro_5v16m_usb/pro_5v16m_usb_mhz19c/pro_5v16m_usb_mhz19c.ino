#include <Wire.h>
// https://github.com/nara256/mhz19_uart
#include <MHZ19_uart.h>

#define RX_PIN 2
#define TX_PIN 3
#define INTERVAL 6

MHZ19_uart mhz19;

void setup()
{
  checkLED(1000);

  Serial.begin(9600);
  
  Wire.begin();
  delay(1000);
  
  checkLED(500);
  
  delay(100);
  mhz19.begin(RX_PIN, TX_PIN);
  mhz19.setAutoCalibration(false);
  delay(1000);
}

void checkLED(int mills) {
  int PIN = 13;
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
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
  
  count++;
  delay(1000);
}

#include <M5Stack.h>
#include <MHZ19_uart.h>

#define RX 16
#define TX 17
#define INTERVAL 2

#define LED_PIN 22

int ledState = LOW;


MHZ19_uart mhz19;

void setup(){
  M5.begin();
  M5.Lcd.setTextSize(5);
  M5.Lcd.print("start");

  Serial.begin(9600);
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
    //co2=count;
    displayCo2(co2);
    Serial.println("LED_ALERT HIGH");
    Serial.println(co2);
    ledState = HIGH;
    Serial.println(ledState);
    digitalWrite(LED_PIN,ledState);
    m5.update();
  } else {
     M5.Lcd.print(".");
     Serial.println("LED_ALERT LOW");
     ledState = LOW;
     Serial.println(ledState);
     digitalWrite(LED_PIN,ledState);
     m5.update();
  }
  count++;
  delay(1000);
  

}

void displayCo2(int co2) {
  Serial.println("displayCo2");
  M5.Lcd.clear(); 
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("CO2 ppm");
  M5.Lcd.print(co2);
}

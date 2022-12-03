#include <Wire.h>

#include <Adafruit_GFX.h>

// from library maneger
#include <Adafruit_SH110X.h>

#define DISPLAY_ADDRESS 0x3c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Wire.setSDA(16);
  Wire.setSCL(17);
  Wire.begin();                // join i2c bus (address optional for master)
  Serial.begin(9600);          // start serial communication at 9600bps

  display.begin(DISPLAY_ADDRESS, true);
  display.setTextColor(SH110X_WHITE);
  delay(100);
  displayLCD("Start");
  delay(3000);
}

int reading = 0;
float temp=0;

void loop() {
  // step 1: wake sensor
  Wire.beginTransmission(byte(0x68)); // transmit to device 0x68=104
  Wire.endTransmission();      // stop transmitting

  delay(5);       //max 15ms            

  // step 3: instruct sensor to return a particular echo reading
  Wire.beginTransmission(byte(0x68)); // transmit to device #104
  Wire.write(byte(0x06));      // sets register pointer to echo #1 register (0x06)
  Wire.endTransmission();      // stop transmitting

  // step 4: request reading from sensor
  Wire.requestFrom(byte(0x68), 2);    // request 2 bytes from slave device #104

  // step 5: receive reading CO2 from sensor
  if (2 <= Wire.available()) { // if two bytes were received
    reading = Wire.read();    // receive high byte (overwrites previous reading)
    reading = reading << 8;    // shift high byte to be high 8 bits
    reading |= Wire.read();   // receive low byte as lower 8 bits
    displayCo2(reading);
    Serial.print(reading);   // print the reading
    Serial.println(" CO2");
  }
  
 // step 6: read temperature
  Wire.beginTransmission(byte(0x68)); // transmit to device 
  Wire.write(byte(0x08));      // sets register pointer to echo #1 register (0x08)
  Wire.endTransmission();      // stop transmitting
  Wire.requestFrom(byte(0x68), 2);    // request 2 bytes from slave device 
   if (2 <= Wire.available()) { // if two bytes were received
    reading = Wire.read();    // receive high byte (overwrites previous reading)
    reading = reading << 8;    // shift high byte to be high 8 bits
    reading |= Wire.read();   // receive low byte as lower 8 bits
    temp=reading;
    Serial.print((temp/100));   // print the reading
    Serial.println(" C");
  }
  delay(5000);                  // wait a bit since people have to read the output :)
}

void displayLCD(String message) {
  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(0, 0);
  display.println(message);
  display.display();
}

void displayCo2(int co2) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("co2 ppm");
  display.println("");

  display.setTextSize(1);
  display.setCursor(0, 24);

  display.setTextSize(5);
  char buff[4];
  sprintf(buff, "%4d", co2);
  display.println(buff);
  display.display();
}

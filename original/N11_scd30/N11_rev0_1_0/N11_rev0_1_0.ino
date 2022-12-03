#include <Wire.h>

#include <Adafruit_GFX.h>

// from library maneger
#include <Adafruit_SH110X.h>

// from library maneger
#include <Adafruit_SCD30.h>

#define DISPLAY_ADDRESS 0x3c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_SCD30  scd30;

void setup() {
  Wire.setSDA(16);
  Wire.setSCL(17);
  Wire.begin();                // join i2c bus (address optional for master)
  Serial.begin(9600);          // start serial communication at 9600bps

  display.begin(DISPLAY_ADDRESS, true);
  display.setTextColor(SH110X_WHITE);
  delay(100);
  displayLCD("Start");

  if (!scd30.begin()) {
    Serial.println("Failed to find SCD30 chip");
    while (1) { delay(10); }
  }
  Serial.println("SCD30 Found!");
  delay(3000);

  Serial.print("Measurement Interval: "); 
  Serial.print(scd30.getMeasurementInterval()); 
  Serial.println(" seconds");
}


int reading = 0;
float temp = 0;

void loop() {
  if (scd30.dataReady()){
    Serial.println("Data available!");

    if (!scd30.read()){ Serial.println("Error reading sensor data"); return; }

    Serial.print("Temperature: ");
    Serial.print(scd30.temperature);
    Serial.println(" degrees C");
    
    Serial.print("Relative Humidity: ");
    Serial.print(scd30.relative_humidity);
    Serial.println(" %");
    
    Serial.print("CO2: ");
    Serial.print(scd30.CO2, 3);
    Serial.println(" ppm");
    Serial.println("");
    displayCo2(scd30.CO2);
  } else {
    //Serial.println("No data");
  }
  delay(5000);
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

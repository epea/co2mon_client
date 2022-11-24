#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <MHZ19_uart.h>

#define RX_PIN 3
#define TX_PIN 1
#define INTERVAL 6

ESP8266WebServer server(80);

MHZ19_uart mhz19;

void setup() {
  doConnectOrWifiSetting();
  initializeServer();
  initializeMhz();
}

void doConnectOrWifiSetting() {
  WiFi.begin();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if ( 10000 < millis() ) {
      break;
    }
  }

  if ( WiFi.status() != WL_CONNECTED ) {
    WiFi.mode(WIFI_STA);
    WiFi.beginSmartConfig();
    while (!WiFi.smartConfigDone()) {
      delay(500);
      if ( 30000 < millis() ) {
        ESP.restart();
      }
    }

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      if ( 60000 < millis() ) {
        ESP.restart();
      }
    }
  }
}

void handleRoot() {
  server.send(200, "text/html", String(mhz19.getCO2PPM()));
}

void initializeServer() {
  server.on("/", HTTP_GET, handleRoot);
  server.begin();
}

void initializeMhz() {
  mhz19.begin(RX_PIN, TX_PIN);
  mhz19.setAutoCalibration(false);
  delay(1000);
}

void loop() {
  server.handleClient();
}

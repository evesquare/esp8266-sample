#include <Arduino.h>
#include "WifiManager.h"
#include "WebServerManager.h"

WifiManager wifiManager(WIFI_SSID, WIFI_PASSWORD);
WebServerManager webServer;

void setup()
{
  Serial.begin(115200);

  if (!wifiManager.connect())
  {
    Serial.println("Failed to connect to WiFi");
    return;
  }

  webServer.begin();
}

void loop()
{
  webServer.handle();
}
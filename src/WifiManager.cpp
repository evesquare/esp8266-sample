#include "WifiManager.h"

WifiManager::WifiManager(const char *ssid, const char *password)
    : ssid(ssid), password(password) {}

bool WifiManager::connect()
{
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting to WiFi");
    Serial.print("SSID: ");
    Serial.println(ssid);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20)
    {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("\nWiFi connection failed!");
        Serial.print("Status code: ");
        Serial.println(WiFi.status());
        return false;
    }

    Serial.println("\nConnected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    return true;
}

String WifiManager::getIP()
{
    return WiFi.localIP().toString();
}
#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <ESP8266WiFi.h>

class WifiManager
{
private:
    const char *ssid;
    const char *password;

public:
    WifiManager(const char *ssid, const char *password);
    bool connect();
    String getIP();
};

#endif
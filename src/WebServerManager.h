#ifndef WEB_SERVER_MANAGER_H
#define WEB_SERVER_MANAGER_H

#include <ESP8266WebServer.h>

class WebServerManager
{
private:
    ESP8266WebServer server;
    static const int BLUE_LED = 2; // GPIO2: 青LED
    static const int RED_LED = 16; // GPIO16: 赤LED
    bool blueLedState;
    bool redLedState;

    void handleRoot();
    void handleToggleBlue();
    void handleToggleRed();
    String getHtml();

public:
    WebServerManager(int port = 80);
    void begin();
    void handle();
};

#endif
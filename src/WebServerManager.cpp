#include "WebServerManager.h"

WebServerManager::WebServerManager(int port)
    : server(port), blueLedState(false), redLedState(false)
{

    pinMode(BLUE_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    digitalWrite(BLUE_LED, HIGH); // 初期状態はOFF
    digitalWrite(RED_LED, HIGH);  // 初期状態はOFF
}

void WebServerManager::begin()
{
    server.on("/", [this]()
              { handleRoot(); });
    server.on("/toggleBlue", [this]()
              { handleToggleBlue(); });
    server.on("/toggleRed", [this]()
              { handleToggleRed(); });
    server.begin();
    Serial.println("HTTP server started");
}

void WebServerManager::handle()
{
    server.handleClient();
}

void WebServerManager::handleRoot()
{
    server.send(200, "text/html", getHtml());
}

void WebServerManager::handleToggleBlue()
{
    blueLedState = !blueLedState;
    digitalWrite(BLUE_LED, !blueLedState);
    server.send(200, "text/plain", blueLedState ? "ON" : "OFF");
    Serial.println("Blue LED: " + String(blueLedState ? "ON" : "OFF"));
}

void WebServerManager::handleToggleRed()
{
    redLedState = !redLedState;
    digitalWrite(RED_LED, !redLedState);
    server.send(200, "text/plain", redLedState ? "ON" : "OFF");
    Serial.println("Red LED: " + String(redLedState ? "ON" : "OFF"));
}

String WebServerManager::getHtml()
{
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }";
    html += ".button { border: none; color: white; padding: 15px 32px; ";
    html += "text-align: center; text-decoration: none; display: inline-block; font-size: 16px; ";
    html += "margin: 4px 2px; cursor: pointer; border-radius: 4px; }";
    html += ".blue { background-color: #0275d8; }";
    html += ".red { background-color: #d9534f; }";
    html += "</style></head><body>";
    html += "<h1>ESP8266 LED Control</h1>";

    html += "<div style='margin: 20px;'>";
    html += "<h2>Blue LED</h2>";
    html += "<p>Status: " + String(blueLedState ? "ON" : "OFF") + "</p>";
    html += "<button class='button blue' onclick='toggleLED(\"Blue\")'>Toggle Blue LED</button>";
    html += "</div>";

    html += "<div style='margin: 20px;'>";
    html += "<h2>Red LED</h2>";
    html += "<p>Status: " + String(redLedState ? "ON" : "OFF") + "</p>";
    html += "<button class='button red' onclick='toggleLED(\"Red\")'>Toggle Red LED</button>";
    html += "</div>";

    html += "<script>";
    html += "function toggleLED(color) {";
    html += "  const endpoint = color === 'Blue' ? '/toggleBlue' : '/toggleRed';";
    html += "  fetch(endpoint).then(response => response.text()).then(data => {";
    html += "    location.reload();";
    html += "  });";
    html += "}";
    html += "</script>";
    html += "</body></html>";
    return html;
}
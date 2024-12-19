#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

// DNS サーバー用の設定
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

// WiFi 設定用の構造体
struct Settings
{
    char ssid[32];
    char password[64];
} settings;

// WiFi 設定ページのHTML
const char *settingsHTML = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset='utf-8'>
    <title>WiFi Settings</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 20px; }
        .container { max-width: 400px; margin: 0 auto; }
        input { width: 100%; padding: 5px; margin: 5px 0; }
        button { width: 100%; padding: 10px; margin: 10px 0; }
    </style>
</head>
<body>
    <div class='container'>
        <h1>WiFi Settings</h1>
        <form method='post' action='/save'>
            <label>SSID:</label><br>
            <input type='text' name='ssid' required><br>
            <label>Password:</label><br>
            <input type='password' name='password' required><br>
            <button type='submit'>Save</button>
        </form>
    </div>
</body>
</html>
)";

void handleRoot()
{
    webServer.send(200, "text/html", settingsHTML);
}

void handleSave()
{
    if (webServer.method() != HTTP_POST)
    {
        webServer.send(405, "text/plain", "Method Not Allowed");
        return;
    }

    // フォームデータの取得と保存
    strlcpy(settings.ssid, webServer.arg("ssid").c_str(), sizeof(settings.ssid));
    strlcpy(settings.password, webServer.arg("password").c_str(), sizeof(settings.password));

    EEPROM.put(0, settings);
    EEPROM.commit();

    // 設定完了ページの表示
    String html = "<html><body>";
    html += "<h1>Settings Saved</h1>";
    html += "<p>The device will now restart and attempt to connect to the WiFi network.</p>";
    html += "</body></html>";
    webServer.send(200, "text/html", html);

    // 少し待ってからリセット
    delay(2000);
    ESP.restart();
}

void setupAP()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("ESP8266-Setup");

    // DNS サーバーの開始
    dnsServer.start(DNS_PORT, "*", apIP);

    // Webサーバーのルート設定
    webServer.on("/", handleRoot);
    webServer.on("/save", handleSave);
    webServer.onNotFound(handleRoot);
    webServer.begin();

    Serial.println("AP Mode Started");
    Serial.println("SSID: ESP8266-Setup");
    Serial.println("IP: " + apIP.toString());
}

void setup()
{
    Serial.begin(115200);

    // EEPROM初期化
    EEPROM.begin(sizeof(Settings));
    EEPROM.get(0, settings);

    // 保存済みの設定で接続を試みる
    if (strlen(settings.ssid) > 0)
    {
        WiFi.begin(settings.ssid, settings.password);

        // 接続を待機
        int tries = 0;
        while (WiFi.status() != WL_CONNECTED && tries < 20)
        {
            delay(500);
            Serial.print(".");
            tries++;
        }
    }

    // 接続できない場合はAP モードを開始
    if (WiFi.status() != WL_CONNECTED)
    {
        setupAP();
    }
    else
    {
        Serial.println("\nConnected to WiFi");
        Serial.println("IP: " + WiFi.localIP().toString());
    }
}

void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        dnsServer.processNextRequest();
        webServer.handleClient();
    }
}
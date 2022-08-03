//Reference: https://iotdesignpro.com/articles/esp32-data-logging-to-google-sheets-with-google-scripts#:~:text=In%20the%20loop%20function%20if,data%20from%20the%20Google%20sheets

//(1)-Inlude Library
#include "ESP32_WiFiManager.h"
#include "WiFi.h"
#include <HTTPClient.h>

//(2)-Define Constant Value


//(3)-Object Mapping
//WiFiClientSecure client;
//HTTPSRedirect* client = nullptr;
ESP32_WiFiManager wm("ESP32Wifi-AP", 2, 15); //WiFi AP = "ESP32Wifi-AP", LED at Pin 2, Trigger at Pin 15

//(4)-I/O Mapping
const byte LED1 = 16;


//(5)-Global Variable Declaration
uint32_t send_http_request_tick = 0;
String payload = "";

//6.1 Function - read_gsheet(String cell)
//API:https://script.google.com/macros/s/AKfycbwb6gG1YvpO-yYDCC8355Gu-3hdqMf7Fcs8Pms-CDqVBEmF9I5NxWXWZDD1UB3HLgV0RQ/exec?read=B2
void read_gsheet(String cell) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://script.google.com/macros/s/AKfycbwb6gG1YvpO-yYDCC8355Gu-3hdqMf7Fcs8Pms-CDqVBEmF9I5NxWXWZDD1UB3HLgV0RQ/exec?read=" + cell;
    Serial.println("Making a request");
    http.begin(url.c_str()); //Specify the URL and certificate
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();
    if (httpCode > 0) { //Check for the returning code
      payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
    }
    else {
      Serial.println("Error on HTTP request");
    }
    http.end();
  }
}

//======================= SETUP ========================
void setup() {
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
  wm.begin();
  pinMode (LED1, OUTPUT);
  send_http_request_tick = millis();
}

//====================== LOOP ===========================
void loop() {
  // put your main code here, to run repeatedly:
  wm.running();
  if (send_http_request_tick < millis ()) {
    send_http_request_tick = millis() + 30000;
    read_gsheet("B2");
    //Applications
    if (payload == "on" || payload == "ON" || payload == "On" || payload == "1") {
      digitalWrite(LED1, HIGH);
    }else{
      digitalWrite (LED1, LOW);
    }
  }
}

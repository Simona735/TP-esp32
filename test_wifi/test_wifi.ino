
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "";  // SSID (meno) WiFi
const char* password = "";  // heslo WiFi

// VVV tu treba dat cielovu URL
String serverName = "https://httpbin.org/ip";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000; //5 sekund

void setup() {
  Serial.begin(9600); 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Pripajanie");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Pripojenie na WiFi uspesne. Lokalna IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  
  if ((millis() - lastTime) > timerDelay) {
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverName;
      
      http.begin(serverPath.c_str());
      
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

#include "WifiManager.h"

WifiManager::WifiManager(const char* ssid, const char* password) {
  this->ssid = ssid;
  this->password = password;
}

void WifiManager::connect() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a WiFi!");
}

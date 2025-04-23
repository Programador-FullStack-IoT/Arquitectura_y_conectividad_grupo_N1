#include "MQTTManager.h"

MQTTManager::MQTTManager(PubSubClient& client, const char* mqttServer, int mqttPort) 
  : client(client), mqttServer(mqttServer), mqttPort(mqttPort) {}

void MQTTManager::connect() {
  while (!client.connected()) {
    Serial.print("Intentando conectar a MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado a MQTT");
    } else {
      Serial.print("Falló, reintentando en 5 segundos...");
      delay(5000);
    }
  }
}

void MQTTManager::subscribe(const char* topic) {
  client.subscribe(topic);
}

void MQTTManager::publish(const char* topic, const char* message) {
  client.publish(topic, message);
}

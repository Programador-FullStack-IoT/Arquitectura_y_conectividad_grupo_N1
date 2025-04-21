#include <Arduino.h>
#include <Wire.h>
#include "MQ135Sensor.h"
#include "AHT25Sensor.h"
#include "Extractor.h"
#include "SensorDataMQTT.h"

// --------------------- Configuración de Pines ---------------------
#define MQ135_PIN 34       // GPIO36 (ADC1_CH0)
//#define BATERIA 0         // GPIO34 (ADC1_CH6)
#define EXTRACTOR 32       // 

// --------------------- WiFi y MQTT ---------------------
const char* ssid = "Fibertel-WiFi032";
const char* password = "vuelalto@67";
const char* broker = "192.168.0.3";  // Reemplazar por IP local del broker
const int mqttPort = 1883;

// --------------------- IP estática ---------------------
IPAddress ip(192, 168, 0, 100);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

// --------------------- Objetos globales ---------------------
MQ135Sensor mq135(MQ135_PIN);
AHT25Sensor aht25;
Extractor extractor(EXTRACTOR);

SensorDataMQTT sensorMQTT(ssid, password, broker, mqttPort, ip, gateway, subnet, extractor);

unsigned long lastPublish = 0;
const unsigned long publishInterval = 5000;  // 5 segundos

void setup() {
  Serial.begin(115200);
  Wire.begin();

  sensorMQTT.conectarWiFi();
  sensorMQTT.conectarMQTT();

  mq135.begin();
  aht25.begin();

  Serial.println("Sistema MQTT para control de CO₂ iniciado.");
}

void loop() {
  sensorMQTT.manejarMQTT();

  unsigned long currentMillis = millis();
  if (currentMillis - lastPublish >= publishInterval) {
    lastPublish = currentMillis;

    float co2 = mq135.readFilteredData();
    float temp, hum;
    aht25.readData(temp, hum);
    //float bat = (analogRead(BATERIA) / 4095.0) * 3.3 * 2;

    Serial.println("Publicando lecturas por MQTT...");
    sensorMQTT.publicarLecturas(co2, temp, hum/*, bat*/);
  }

  delay(100);
}

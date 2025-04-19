#include "SensorDHT.h"
#include <Arduino.h>  // Asegura que Serial esté disponible

SensorDHT::SensorDHT(uint8_t pin, uint8_t type) : dht(pin, type) {}

void SensorDHT::begin() {
  dht.begin();
}

float SensorDHT::leerTemperatura() {
  float temp = dht.readTemperature();
  if (isnan(temp)) {
    Serial.println("Error leyendo del DHT22");
    return -999.0;
  }
  return temp;
}

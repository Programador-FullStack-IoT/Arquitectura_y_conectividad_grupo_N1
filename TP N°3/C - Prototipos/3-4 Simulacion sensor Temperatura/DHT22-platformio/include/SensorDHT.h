#ifndef SENSORDHT_H
#define SENSORDHT_H

#include <Arduino.h>
#include <DHT.h>

class SensorDHT {
  private:
    DHT dht;  // Objeto de la clase DHT

  public:
    SensorDHT(uint8_t pin, uint8_t type);  // Constructor con pin y tipo de sensor
    void begin();                          // Inicializa el sensor
    float leerTemperatura();               // Devuelve la temperatura, o -999.0 si falla
};

#endif

#include "Potentiometer.h"

Potentiometer::Potentiometer(uint8_t pin) : pin(pin) {
  pinMode(pin, INPUT);
}

int Potentiometer::readValue() {
  return analogRead(pin);  // Devuelve el valor crudo del potenciómetro
}

int Potentiometer::getMappedValue() {
  return map(readValue(), 0, 4095, 0, 255);  // Devuelve el valor mapeado entre 0 y 255
}

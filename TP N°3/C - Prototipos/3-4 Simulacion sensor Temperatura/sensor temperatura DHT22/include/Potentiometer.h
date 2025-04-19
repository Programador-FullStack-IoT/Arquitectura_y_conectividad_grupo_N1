#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include <Arduino.h>

class Potentiometer {
  private:
    uint8_t pin;

  public:
    Potentiometer(uint8_t pin);
    int readValue();
    int getMappedValue();
};

#endif

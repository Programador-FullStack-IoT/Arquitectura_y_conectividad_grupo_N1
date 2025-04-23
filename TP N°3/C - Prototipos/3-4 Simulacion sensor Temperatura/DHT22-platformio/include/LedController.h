#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <Arduino.h>

class LedController {
  private:
    uint8_t pin;
    int channel;
  
  public:
    LedController(uint8_t ledPin, int ledChannel);
    void begin();
    void setBrightness(int brightness);
    void turnOff();
};

#endif

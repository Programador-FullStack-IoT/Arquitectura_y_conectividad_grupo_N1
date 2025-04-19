#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
  private:
    uint8_t pin;
    int lastState;
    unsigned long lastDebounceTime;
    unsigned long debounceDelay;

  public:
    Button(uint8_t pin);
    bool isPressed();
    void update();
};

#endif

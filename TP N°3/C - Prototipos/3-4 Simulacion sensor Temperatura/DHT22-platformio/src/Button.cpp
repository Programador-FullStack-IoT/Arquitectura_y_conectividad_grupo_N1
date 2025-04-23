#include "Button.h"

Button::Button(uint8_t pin) : pin(pin), lastState(HIGH), lastDebounceTime(0), debounceDelay(50) {
  pinMode(pin, INPUT_PULLUP);
}

bool Button::isPressed() {
  return digitalRead(pin) == LOW;
}

void Button::update() {
  int reading = digitalRead(pin);
  if (reading != lastState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW && lastState == HIGH) {
      // El botón fue presionado
    }
  }

  lastState = reading;
}

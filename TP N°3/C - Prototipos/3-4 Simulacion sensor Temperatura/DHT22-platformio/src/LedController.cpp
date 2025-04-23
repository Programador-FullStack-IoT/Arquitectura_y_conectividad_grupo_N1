#include "LedController.h"

LedController::LedController(uint8_t ledPin, int ledChannel) : pin(ledPin), channel(ledChannel) {}

void LedController::begin() {
  pinMode(pin, OUTPUT);
  ledcAttachPin(pin, channel);
  ledcSetup(channel, 5000, 8); // 5kHz frequency, 8-bit resolution
}

void LedController::setBrightness(int brightness) {
  ledcWrite(channel, brightness);
}

void LedController::turnOff() {
  ledcWrite(channel, 0); // Apaga el LED
}

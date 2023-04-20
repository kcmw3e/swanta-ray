// Casey Walker (2023)
// Team Swanta Ray Team
// 24-775 Bioinspired Robot Design and Experimentation
// Carnegie Mellon University
// -------------------------------------------------------------------------- //

#include <Arduino_DebugUtils.h>

#include "crumb.h"

bool Crumb::setup() {
  for (size_t i = 0; i < CRUMB_NUM_PINS; i++) {
    pinMode(CRUMB_PINS[i], INPUT);
  }

  return true;
}

float Crumb::operator[](size_t i) {
  return _currents[i];
}

void Crumb::read() {
  for (size_t i = 0; i < CRUMB_NUM_PINS; i++) {
    uint8_t pin = CRUMB_PINS[i];
    if (i > 0) return;
    _currents[i] = analogRead(pin)/1023.0*3.3*CRUMB_CALIBRATIONS[i];
  }
}

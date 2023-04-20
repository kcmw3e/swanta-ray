// Casey Walker (2023)
// Team Swanta Ray Team
// 24-775 Bioinspired Robot Design and Experimentation
// Carnegie Mellon University
// -------------------------------------------------------------------------- //

#include <Arduino_DebugUtils.h>

#include "crumb.h"

const float Crumb::_calibrations[] = { 1.005, 1.004, 1.002, 1.006, 0.996, 1.005, 1.004, 1.005, 1.002, 1.007, 1.002, 1.005, 1.005, 1.004, 1.002, 0.998 };
const uint32_t Crumb::_pins[] = { 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 38, 39 };
const size_t Crumb::_num_pins = sizeof(Crumb::_pins)/sizeof(*Crumb::_pins);

Crumb::Crumb() {
  _currents[Crumb::_num_pins] = {0};
}

bool Crumb::setup() {
  for (size_t i = 0; i < _num_pins; i++) {
    pinMode(_pins[i], INPUT);
  }

  return true;
}

float Crumb::operator[](size_t i) {
  return _currents[i];
}

void Crumb::read() {
  for (size_t i = 0; i < _num_pins; i++) {
    _currents[i] = analogRead(_pins[i]) / 1023.0 * 3.3 * _calibrations[i];
  }
}

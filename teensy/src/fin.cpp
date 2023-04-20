// Casey Walker (2023)
// Team Swanta Ray Team
// 24-775 Bioinspired Robot Design and Experimentation
// Carnegie Mellon University
// -------------------------------------------------------------------------- //
// Casey Walker (2023)
// Team Swanta Ray Team
// 24-775 Bioinspired Robot Design and Experimentation
// Carnegie Mellon University
// -------------------------------------------------------------------------- //

#include <Arduino_DebugUtils.h>

#include "fin.h"

const int Fin::_calibrations[] = { 0 };
const uint32_t Fin::_pins[] = { 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 38, 39 };
const size_t Fin::_num_pins = sizeof(Fin::_pins)/sizeof(*Fin::_pins);

Fin::Fin() {
  _pos[Fin::_num_pins] = {0};
}

bool Fin::setup() {
  for (size_t i = 0; i < _num_pins; i++) {
    pinMode(_pins[i], OUTPUT);
  }

  return true;
}

void Fin::set(int pos[]) {
  for (size_t i = 0; i < Fin::_num_pins; i++) _pos[i] = pos[i];
}

void Fin::write() {
  for (size_t i = 0; i < _num_pins; i++) {
    analogWrite(_pins[i], _pos[i]*_calibrations[i]);
  }
}

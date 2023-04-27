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

bool Fin::setup() {
  for (size_t i = 0; i < FIN_NUM_PINS; i++) {
    uint8_t pin = FIN_PINS[i];
    analogWriteFrequency(pin, FIN_PWM_FREQ);
    analogWriteResolution(FIN_PWM_BITS);
    pinMode(pin, OUTPUT);

    _pos[i] = 0;
  }

  write();

  return true;
}

void Fin::set(int pos[]) {
  for (size_t i = 0; i < FIN_NUM_PINS; i++) _pos[i] = pos[i];
}

int* Fin::get() {
  return _pos;
}

void Fin::write() {
  for (size_t i = 0; i < FIN_NUM_PINS; i++) {
    int pos = _pos[i] + FIN_OFFSETS[i];
    int pul = map(pos, FIN_POS_LO, FIN_POS_HI, FIN_PWM_LO, FIN_PWM_HI);

    int val = (pul*32757)/4167;

    analogWrite(FIN_PINS[i], val);
  }
}

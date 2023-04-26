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
    _current_buffer[i] = 0;
  }
  analogReadAveraging(1);
  analogReadResolution(8);

  return true;
}

float Crumb::operator[](size_t i) {
  return _currents[i];
}

void Crumb::read() {
  for (size_t i = 0; i < CRUMB_NUM_PINS; i++) {
    uint8_t pin = CRUMB_PINS[i];
    int V = analogRead(pin);
    _current_buffer[i] += V;
  }
  _current_buffer_count++;

  if(_current_buffer_count == CRUMB_NUM_SAMPLES_FOR_AVERAGE) {
    for (size_t i = 0; i < CRUMB_NUM_PINS; i++) {
      int V = _current_buffer[i] / _current_buffer_count;
      _voltages[i] = V;
      _currents[i] = V/255.0*3.3*CRUMB_CALIBRATIONS[i];
      _current_buffer[i] = 0;
    }
    _current_buffer_count = 0;
  }
}

int* Crumb::voltages() {
  return _voltages;
}

float* Crumb::currents() {
  return _currents;
}

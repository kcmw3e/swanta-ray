// Casey Walker (2023)
// Team Swanta Ray Team
// 24-775 Bioinspired Robot Design and Experimentation
// Carnegie Mellon University
// -------------------------------------------------------------------------- //
//
// crumb.h
// For reading current sensor data
//

#ifndef SWANTA_CRUMB_H
#define SWANTA_CRUMB_H

#include <Arduino.h>

const float CRUMB_CALIBRATIONS[] = { 1.005, 1.004, 1.002, 1.006, 0.996, 1.005, 1.004, 1.005, 1.002, 1.007, 1.002, 1.005, 1.005, 1.004, 1.002, 0.998 };
const uint8_t CRUMB_PINS[] = { 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 38, 39 };
const size_t CRUMB_NUM_PINS = sizeof(CRUMB_PINS)/sizeof(*CRUMB_PINS);

class Crumb {
  private:
    float _currents[CRUMB_NUM_PINS];
  public:
    bool setup();
    void read();
    float operator[](size_t i);
};

#endif // SWANTA_CRUMB_H

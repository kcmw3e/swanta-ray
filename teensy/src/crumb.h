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

class Crumb {
  static const float _calibrations[];
  static const uint32_t _pins[];
  static const size_t _num_pins;
  
  private:
    int tmp;
    float _currents[];
  public:
    Crumb();
    bool setup();
    void read();
    float operator[](size_t i);
};

#endif // SWANTA_CRUMB_H

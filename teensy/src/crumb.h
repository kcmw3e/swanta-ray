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
const size_t CRUMB_NUM_SAMPLES_FOR_AVERAGE = 333;

class Crumb {
  private:
    float _voltages[CRUMB_NUM_PINS];
    float _currents[CRUMB_NUM_PINS];

    long _current_buffer[CRUMB_NUM_PINS]; // We sample fast and add the measurements to this buffer.
    int _current_buffer_count; // We track how many values were added to the buffer, so we know what to divide by when taking the average.
  public:
    bool setup();
    void read();
    float operator[](size_t i);
    float* voltages();
    float* currents();
};

#endif // SWANTA_CRUMB_H

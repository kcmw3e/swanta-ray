// Casey Walker (2023)
// Team Swanta Ray Team
// 24-775 Bioinspired Robot Design and Experimentation
// Carnegie Mellon University
// -------------------------------------------------------------------------- //
//
// fin.h
// For moving fin servos
//

#ifndef SWANTA_FIN_H
#define SWANTA_FIN_H

const uint8_t FIN_PINS[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 28, 29, 36, 37 };
const size_t FIN_NUM_PINS = sizeof(FIN_PINS)/sizeof(*FIN_PINS);
const int FIN_OFFSETS[] = { -10, -6, -4, -2, -1, -10, -1, -2, // left side offsets
                             -0, -7, -6, -3, -3, -18, -6, -8 }; // right side offsets

#define FIN_PWM_FREQ 240.0 // Hz
#define FIN_PWM_BITS 15

#define FIN_POS_LO 0 // deg
#define FIN_POS_HI 180 // deg

#define FIN_PWM_LO 800 // microseconds
#define FIN_PWM_HI 2200 // microseconds
  
#define FIN_ANALOG_LO 0
#define FIN_ANALOG_HI 1 << FIN_PWM_BITS

class Fin {
  private:
    int _pos[FIN_NUM_PINS];
  public:
    bool setup();
    void set(int pos[]);
    int* get();
    void write();
};

#endif // SWANTA_FIN_H

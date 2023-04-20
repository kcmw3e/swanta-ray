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

class Fin {
  static const int _calibrations[];
  static const uint32_t _pins[];
  static const size_t _num_pins;
  
  private:
    int tmp;
    int _pos[];
  public:
    Fin();
    bool setup();
    void set(int pos[]);
    void write();
};

#endif // SWANTA_FIN_H

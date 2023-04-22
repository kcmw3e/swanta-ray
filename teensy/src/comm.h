// Casey Walker (2023)
// Team Swanta Ray Team
// 24-775 Bioinspired Robot Design and Experimentation
// Carnegie Mellon University
// -------------------------------------------------------------------------- //
//
// comm.h
// Communications between serial (typically ESP32) and Teensy
//

#ifndef SWANTA_COMM_H
#define SWANTA_COMM_H

#include <Arduino.h>

#include <string>

#include "fin.h"

using namespace std;

#define BAUD 115200

#define BUF_LEN 4096

const byte COMM_HEADER[] = {0xc0, 0xff};
const byte COMM_TAIL[] = {0xee};
#define COMM_MSG_LEN sizeof(COMM_HEADER) + FIN_NUM_PINS + sizeof(COMM_TAIL) // bytes

class Comm {
  private:
    char _buf[BUF_LEN]; // buffer for holding contents coming in from serial (circular)
    size_t _buf_head; // start of currently-unread buffer
    size_t _buf_tail; // end of currently-unread buffer
  public:
    Comm();
    bool setup();
    void read();
    string next();
    bool next_servos(int pos[]);
};

#endif // SWANTA_COMM_H

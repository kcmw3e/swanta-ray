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

using namespace std;

#define BAUD 115200

#define BUF_LEN 4096

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
};

#endif // SWANTA_COMM_H

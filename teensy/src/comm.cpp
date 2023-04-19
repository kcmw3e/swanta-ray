// Casey Walker (2023)
// Team Swanta Ray Team
// 24-775 Bioinspired Robot Design and Experimentation
// Carnegie Mellon University
// -------------------------------------------------------------------------- //
//
// comm.c
// Implementation of comm.h

#include <Arduino_DebugUtils.h>

#include "comm.h"

Comm::Comm() {
  _buf_head = 0;
  _buf_tail = 0;
}

bool Comm::setup() {
  Serial.begin(BAUD);
  //while (!Serial); // wait for serial to connect
  delay(1000);

  DEBUG_INFO("Serial connected successfully.");

  return true;
}

void Comm::read() {
  DEBUG_INFO("Reading serial.");
  size_t n = Serial.available();
  DEBUG_INFO("> Available to read: %zu", n);
  if (n > 0) n = Serial.readBytes(_buf, n); // fix to read circular, fix to check n < len(buf)
  DEBUG_INFO("> Read: %zu", n);
  _buf_tail = _buf_head + n;
}

string Comm::next() {
  size_t i = _buf_head;
  while (i < _buf_tail) { // fix to do curcular, check less than len of buf
    if (_buf[i] == '\n') break;
    i++;
  }

  DEBUG_INFO("Newline found at %zu", i);

  string s(_buf, _buf_head, i);
  //_buf_head = i + 1; // fix to be circular

  return s;
}

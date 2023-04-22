// Casey Walker (2023)
// Team Swanta Ray Team
// 24-775 Bioinspired Robot Design and Experimentation
// Carnegie Mellon University
// -------------------------------------------------------------------------- //

#include <Arduino_DebugUtils.h>

#include "comm.h"

Comm::Comm() {
  _buf_head = 0;
  _buf_tail = 0;
}

bool Comm::setup() {
  Serial1.begin(BAUD);
  //while (!Serial); // wait for serial to connect
  delay(1000);

  DEBUG_INFO("Serial connected successfully.");

  return true;
}

void Comm::read() {
  size_t n = Serial.available();
  if (n == 0) return;
  Serial.readBytes(_buf, n); // fix to read circular, fix to check n < len(buf)
  _buf_tail = _buf_head + n;
}

string Comm::next() {
  size_t i = _buf_head;
  while (i < _buf_tail) { // fix to do curcular, check less than len of buf
    if (_buf[i] == '\n') break;
    i++;
  }

  string s(_buf, _buf_head, i);
  _buf_tail = _buf_head;
  //_buf_head = i + 1; // fix to be circular

  return s;
}

bool Comm::next_servos(int pos[]) {
  int n = Serial1.available();
  if (n <= 0) return false;

  for (size_t i = 0; i < sizeof(COMM_HEADER); i++) if (Serial1.read() != COMM_HEADER[i]) return false;

  Serial1.readBytes(_buf, FIN_NUM_PINS);
  _buf[COMM_MSG_LEN] = '\0';

  for (size_t i = 0; i < FIN_NUM_PINS; i++) pos[i] = (int)_buf[i];
  for (size_t i = 0; i < sizeof(COMM_TAIL); i++) if (Serial1.read() != COMM_TAIL[i]) return false;
  return true;
}

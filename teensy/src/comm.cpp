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
  Serial.begin(BAUD);
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

void Comm::next_servos(int pos[]) {
  size_t n = Serial.available();
  if (n < COMM_MSG_LEN) return;

  Serial.readBytes(_buf, COMM_MSG_LEN);
  for (size_t i = 0; i < sizeof(COMM_HEADER); i++) if (_buf[i] != ((char*)&COMM_HEADER)[i]) return;
  for (size_t i = 0; i < sizeof(COMM_TAIL); i++) if (_buf[i] != ((char*)&COMM_TAIL)[i]) return;

  for (size_t i = 0; i < FIN_NUM_PINS; i++) pos[i] = (int)_buf[i];
}

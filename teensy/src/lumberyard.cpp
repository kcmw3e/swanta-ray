// Casey Walker (2023)
// Team Swanta Ray Team
// 24-775 Bioinspired Robot Design and Experimentation
// Carnegie Mellon University
// -------------------------------------------------------------------------- //

#include <Arduino_DebugUtils.h>

#include "lumberyard.h"
#include "fin.h"
#include "crumb.h"

Lumberyard::Lumberyard() {
  _root = File();
  _pwd = File();
}

bool Lumberyard::setup() {
  if (!SD.begin(SD_CARD)) return false;

  _root = SD.open("/");
  if (!_root) return false;
  
  _pwd = _root;

  return true;
}

void Lumberyard::cd() {

}

void Lumberyard::ls() {
  DEBUG_INFO("Listing file contents.");
  DEBUG_INFO("> Folder: '%s'", _pwd.name());
  for (File entry = _pwd.openNextFile(); entry; entry = _pwd.openNextFile()) {
      if (entry.isDirectory()) DEBUG_INFO("> |--> ['%s']", entry.name());
      else DEBUG_INFO("> |--> '%s'", entry.name());
  }
  _pwd.rewindDirectory();
}

bool Lumberyard::mkdir(string dir) {
  DEBUG_INFO("Making directory.");
  string path(_pwd.name());
  DEBUG_INFO("> Making '%s' directory in '%s'", dir.c_str(), path.c_str());
  path = path + "/" + dir;
  DEBUG_INFO("> Full path: %s", path.c_str());
  bool result = SD.mkdir(path.c_str());
  DEBUG_INFO("> Result: %s", result ? "success" : "failure");
  return result;
}

bool Lumberyard::cat(const char* filepath) {
  File file = SD.open(filepath);
  if (!file) return false;
  if (file.isDirectory()) {
    file.close();
    return false;
  }

  DEBUG_INFO("%s:\n", filepath);
  while (file.available()) {
    char buf[1024];
    file.readBytes(buf, 1023);
    buf[1023] = '\0';
    DEBUG_INFO("%s", buf);
  }
  return true;
}

bool Lumberyard::open_gait(const char* filepath) {
  _file_gait = SD.open(filepath, FILE_READ);
  if (!_file_gait) return false;
  if (_file_gait.isDirectory()) {
    _file_gait.close();
    return false;
  }
  return true;
}

bool Lumberyard::open_save(const char* filepath) {
  _file_save = SD.open(filepath, FILE_WRITE);
  if (!_file_save) return false;
  if (_file_save.isDirectory()) {
    _file_save.close();
    return false;
  }

  char sep = ',';
  _file_save.write(LUMBERYARD_HEADER_TIME);
  _file_save.write(sep);

# if LUMBERYARD_SAVE_POS
    for (size_t i = 0; i < FIN_NUM_PINS; i++) {
      _file_save.write(LUMBERYARD_HEADER_POS);
      _file_save.write(" ");
      _file_save.print(i);
      _file_save.write(sep);
    }
#endif // LUMBERYARD_SAVE_POS

# if LUMBERYARD_SAVE_VOLTAGES
    for (size_t i = 0; i < CRUMB_NUM_PINS; i++) {
      _file_save.write(LUMBERYARD_HEADER_VOLTAGES);
      _file_save.write(" ");
      _file_save.print(i);
      _file_save.write(sep);
    }
#endif // LUMBERYARD_SAVE_VOLTAGES

  for (size_t i = 0; i < CRUMB_NUM_PINS; i++) {
      _file_save.write(LUMBERYARD_HEADER_CURRENTS);
      _file_save.write(" ");
      _file_save.print(i);
      if (i == CRUMB_NUM_PINS - 1) sep = '\n';
      _file_save.write(sep);
    }
  
  _file_save.flush();

  return true;
}

bool Lumberyard::read_csv_line(int buf[], size_t len) {
  if (!_file_gait.available()) {
    _file_gait.seek(0);
    return false;
  }

  size_t n = _file_gait.readBytesUntil('\n', _buf, LUMBERYARD_BUF_LEN);
  if (n == 0) return false;
  
  char* s = _buf;
  size_t i = 0;
  while (s < _buf + n - 1) {
    for (char c = *s; c == ','; c = *s) s++;
    buf[i] = strtol(s, &s, 10);
    i++;
  }
  return true;
}

void Lumberyard::save_csv_line(float voltages[], float currents[], int pos[]) {
  _file_save.print(millis());
  _file_save.write(",");

# if LUMBERYARD_SAVE_POS
    save_csv_pos(pos);
# endif // LUMBERYARD_SAVE_VOLTAGES

# if LUMBERYARD_SAVE_VOLTAGES
    save_csv_voltages(voltages);
# endif // LUMBERYARD_SAVE_VOLTAGES
  save_csv_currents(currents);

  _file_save.flush();
}

void Lumberyard::save_csv_pos(int pos[]) {
  char buf[256];
  char sep = ',';
  for (size_t i = 0; i < FIN_NUM_PINS; i++) {
    int n = sprintf(buf, "%d", pos[i]);
    _file_save.write(buf, n);
    _file_save.write(sep);
  }
}

void Lumberyard::save_csv_voltages(float voltages[]) {
  char buf[256];
  char sep = ',';
  for (size_t i = 0; i < CRUMB_NUM_PINS; i++) {
    int n = sprintf(buf, "%f", voltages[i]);
    _file_save.write(buf, n);
    _file_save.write(sep);
  }
}

void Lumberyard::save_csv_currents(float currents[]) {
  char buf[256];
  char sep = ',';
  for (size_t i = 0; i < CRUMB_NUM_PINS; i++) {
    int n = sprintf(buf, "%f", currents[i]);
    _file_save.write(buf, n);
    if (i == CRUMB_NUM_PINS - 1) sep = '\n';
    _file_save.write(sep);
  }
}

// Casey Walker (2023)
// Team Swanta Ray Team
// 24-775 Bioinspired Robot Design and Experimentation
// Carnegie Mellon University
// -------------------------------------------------------------------------- //

#include <Arduino_DebugUtils.h>

#include "lumberyard.h"

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

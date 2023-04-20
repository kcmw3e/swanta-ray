// Casey Walker (2023)
// Team Swanta Ray Team
// 24-775 Bioinspired Robot Design and Experimentation
// Carnegie Mellon University
// -------------------------------------------------------------------------- //
//
// teensy.ino
// The main deal
//

#include <string>

#include <Arduino_DebugUtils.h>

#include "src/comm.h"
#include "src/lumberyard.h"
#include "src/crumb.h"
#include "src/cal.h"

using namespace std;

#define SD_CARD BUILTIN_SDCARD

Comm comm;
Lumberyard ly;
Crumb crumb;
Cal cal;

void setup() {
  if (!comm.setup()) while (true);
  DEBUG_INFO("Serial connected.");

  if (!ly.setup()) while (true);
  DEBUG_INFO("SD card connected.");

  if (!crumb.setup()) while (true);
  DEBUG_INFO("Current sensors ready.");
}

void loop() {
  cal.tick();
}

void serialEvent() {
  DEBUG_INFO("Serial event triggered.");
  comm.read();
  string str = comm.next();
  DEBUG_INFO("string read: %s", str.c_str());

  if (str == "ls") {
    ly.ls();
  }

  size_t pos = str.find("mkdir");
  if (pos != string::npos) {
    string dir = str.substr(pos + sizeof("mkdir"));
    dir.erase(0, dir.find_first_not_of(" "));
    ly.mkdir(dir);
  }
}

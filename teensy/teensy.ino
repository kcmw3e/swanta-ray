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

#define HEARTBEAT_DT 1000
#define CRUMB_DT 100

Comm comm;
Lumberyard ly;
Crumb crumb;
Cal cal;

bool setup_tasks();
void heartbeat();
void crumb_read();

void setup() {
  if (!comm.setup()) while (true);
  DEBUG_INFO("Serial connected.");

  if (!ly.setup()) while (true);
  DEBUG_INFO("SD card connected.");

  if (!crumb.setup()) while (true);
  DEBUG_INFO("Current sensors ready.");

  pinMode(LED_BUILTIN, OUTPUT);

  if (!setup_tasks()) while (true);
  DEBUG_INFO("Tasks setup.");

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

bool setup_tasks() {
  cal.add(heartbeat, HEARTBEAT_DT);
  cal.add(crumb_read, CRUMB_DT);

  return true;
}

void heartbeat() {
  static uint32_t beat = LOW;
  digitalWrite(LED_BUILTIN, beat);
  beat = !beat;
}

void crumb_read() {
  crumb.read();
}

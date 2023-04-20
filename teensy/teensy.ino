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
#include "src/fin.h"

using namespace std;

#define SD_CARD BUILTIN_SDCARD

#define HEARTBEAT_DT 1000
#define CRUMB_DT 1000
#define FIN_DT 1000
#define SERIAL_DT 10

Comm comm;
Lumberyard ly;
Crumb crumb;
Cal cal;
Fin fin;

bool setup_tasks();
void heartbeat();
void crumb_read();
void fin_write();

void setup() {
  if (!comm.setup()) while (true);
  DEBUG_INFO("Serial connected.");

  if (!ly.setup()) while (true);
  DEBUG_INFO("SD card connected.");
  
  if (!crumb.setup()) while (true);
  DEBUG_INFO("Current sensors ready.");

  if (!fin.setup()) while (true);
  DEBUG_INFO("Fin ready to sway.");

  //pinMode(LED_BUILTIN, OUTPUT);

  if (!setup_tasks()) while (true);
  DEBUG_INFO("Tasks setup.");
}

void loop() {
  cal.tick();
  delay(10);
}

void check_serial() {
  comm.read();
  string str = comm.next();
  if (str.size() == 0) return;
  if (str == "ls") {
    ly.ls();
    return;
  }

  size_t pos = str.find("mkdir");
  if (pos != string::npos) {
    string dir = str.substr(pos + sizeof("mkdir"));
    dir.erase(0, dir.find_first_not_of(" "));
    ly.mkdir(dir);
    return;
  }

  if (str == "go") {
    int pos[] = {0};
    fin.set(pos);
  }
}

bool setup_tasks() {
  //cal.add(heartbeat, HEARTBEAT_DT);
  cal.add(check_serial, SERIAL_DT);
  cal.add(crumb_read, CRUMB_DT);
  cal.add(fin_write, FIN_DT);
  cal.add(fin_set, 100);

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

void fin_write() {
  fin.write();
}

void fin_set() {
  static int p = 0;
  p = p + 1;
  if (p > 180) p = 0;

  int pos[16] = {0};
  for (size_t i = 0; i < 16; i++) {
    pos[i] = p;
  }

  fin.set(pos);
}

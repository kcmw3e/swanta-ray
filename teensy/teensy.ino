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
#define FIN_DT 2
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
  Serial.begin(BAUD);
  
  if (!comm.setup()) while (true);
  DEBUG_INFO("Serial connected.");

  if (!ly.setup()) while (true);
  DEBUG_INFO("SD card connected.");
  
  if (!crumb.setup()) while (true);
  DEBUG_INFO("Current sensors ready.");

  if (!fin.setup()) while (true);
  DEBUG_INFO("Fin ready to sway.");

  bool res = ly.open("gait_und.csv", FILE_READ);
  if (!res) while(true);
  DEBUG_INFO("Opened gait successfully.");
  
  //pinMode(LED_BUILTIN, OUTPUT);

  if (!setup_tasks()) while (true);
  DEBUG_INFO("Tasks setup.");
}

void loop() {
  cal.tick();
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

  if (str == "cat") {
    ly.cat("test.csv");
  }
}

bool setup_tasks() {
  //cal.add(heartbeat, HEARTBEAT_DT);
  cal.add(check_serial, SERIAL_DT);
  cal.add(crumb_read, CRUMB_DT);
  cal.add(fin_write, FIN_DT);
  cal.add(fin_set, 1);
  //cal.add(save, 1000);

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
  int pos[FIN_NUM_PINS];

  //bool res = ly.read_csv_line(pos, FIN_NUM_PINS);
  //if (!res) return;

  if (!comm.next_servos(pos)) return;


  // for (size_t i = 0; i < FIN_NUM_PINS; i++) {
  //   DEBUG_INFO("pos[%zu] = %d", i, pos[i]);
  // }
  
  for (size_t i = FIN_NUM_PINS/2; i < FIN_NUM_PINS; i++) {
    pos[i] = map(pos[i - FIN_NUM_PINS/2], FIN_POS_LO, FIN_POS_HI, FIN_POS_HI, FIN_POS_LO);
  }

  fin.set(pos);
}

void save() {
  ly.save_data(crumb.currents(), FIN_NUM_PINS);  
}

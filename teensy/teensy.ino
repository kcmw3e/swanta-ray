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

// configurations
#define FIN_CSV_CONTROL 0
#define BEAT_HEART 0

// all intervals in microseconds
#define HEARTBEAT_INTERVAL 1000000
#define CRUMB_INTERVAL 60 // Reading the sensors takes ~50us
#define SAVE_INTERVAL 20000
#define FIN_CSV_UPDATE_INTERVAL 10000
#define FIN_SERIAL_UPDATE_INTERVAL 100
#define SERIAL_INTERVAL 10000

#define GAIT_FILE_UND "gait_und.csv"
#define GAIT_FILE_OSC "gait_und.csv"
#define GAIT_FILE GAIT_FILE_UND

#define SAVE_FILE "data.csv"

Comm comm;
Lumberyard ly;
Crumb crumb;
Cal cal;
Fin fin;

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

# if FIN_CSV_CONTROL
    if (!ly.open_gait(GAIT_FILE)) while(true);
    DEBUG_INFO("Opened gait file successfully.");
# endif // FIN_CSV_CONTROL

  if (!ly.open_save(SAVE_FILE)) while(true);
  DEBUG_INFO("Opened save file successfully.");

# if BEAT_HEART
    pinMode(LED_BUILTIN, OUTPUT);
# endif // BEAT_HEART

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
#  if BEAT_HEART
    cal.add(heartbeat, HEARTBEAT_DT);
#  endif // BEAT_HEART

  cal.add(check_serial, SERIAL_INTERVAL);
  cal.add(crumb_read, CRUMB_INTERVAL);
  cal.add(save, SAVE_INTERVAL);

# if FIN_CSV_CONTROL
    cal.add(fin_csv_update, FIN_CSV_UPDATE_INTERVAL);
# else
    cal.add(fin_serial_update, FIN_SERIAL_UPDATE_INTERVAL);
# endif // FIN_CSV_CONTROL

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

void fin_serial_update() {
  int pos[FIN_NUM_PINS];

  if (!comm.next_servos(pos)) return;

  for (size_t i = FIN_NUM_PINS/2; i < FIN_NUM_PINS; i++) {
    pos[i] = map(pos[i - FIN_NUM_PINS/2], FIN_POS_LO, FIN_POS_HI, FIN_POS_HI, FIN_POS_LO);
  }

  fin.set(pos);
  fin.write();
}

void fin_csv_update() {
  int pos[FIN_NUM_PINS];

  bool res = ly.read_csv_line(pos, FIN_NUM_PINS);
  if (!res) return;

  for (size_t i = FIN_NUM_PINS/2; i < FIN_NUM_PINS; i++) {
    pos[i] = map(pos[i - FIN_NUM_PINS/2], FIN_POS_LO, FIN_POS_HI, FIN_POS_HI, FIN_POS_LO);
  }

  fin.set(pos);
  fin.write();
}

void save() {
  ly.save_csv_line(crumb.voltages(), crumb.currents());  
}

// Casey Walker (2023)
// Team Swanta Ray Team
// 24-775 Bioinspired Robot Design and Experimentation
// Carnegie Mellon University
// -------------------------------------------------------------------------- //
//
// lumberyard.h
// For data logging
//

#ifndef SWANTA_LUMBERYARD_H
#define SWANTA_LUMBERYARD_H

#include <string>

#include <Arduino.h>

#include <SPI.h>
#include <SD.h>

using namespace std;

// configuration
#define LUMBERYARD_SAVE_VOLTAGES 1
#define LUMBERYARD_SAVE_POS 1

#define SD_CARD BUILTIN_SDCARD
#define LUMBERYARD_BUF_LEN 512

#define LUMBERYARD_HEADER_TIME "Time [ms]"
#define LUMBERYARD_HEADER_CURRENTS "Current [A]"
#define LUMBERYARD_HEADER_VOLTAGES "Voltage [V]"
#define LUMBERYARD_HEADER_POS "Pos [deg*]"

class Lumberyard {
  private:
    File _root;
    File _pwd;
    File _file_gait;
    File _file_save;
    char _buf[LUMBERYARD_BUF_LEN];
  public:
    Lumberyard();
    bool setup();
    void cd();
    void ls();
    bool mkdir(string dir);
    bool cat(const char* filepath);
    bool open_gait(const char* filepath);
    bool open_save(const char* filepath);
    bool read_csv_line(int buf[], size_t len);
    void save_csv_line(float voltages[], float currents[], int pos[]);
    void save_csv_voltages(float voltages[]);
    void save_csv_pos(int pos[]);
    void save_csv_currents(float currents[]);
};

#endif // SWANTA_LUMBERYARD_H

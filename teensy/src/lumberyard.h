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

#define SD_CARD BUILTIN_SDCARD
#define LUMBERYARD_BUF_LEN 512

class Lumberyard {
  private:
    File _root;
    File _pwd;
    File _file;
    char _buf[LUMBERYARD_BUF_LEN];
    const char* _savepath = "test.csv";
  public:
    Lumberyard();
    bool setup();
    void cd();
    void ls();
    bool mkdir(string dir);
    bool cat(const char* filepath);
    bool open(const char* filepath, uint8_t mode);
    bool read_csv_line(int buf[], size_t len);
    void save_csv_line(float data[], size_t len);
};

#endif // SWANTA_LUMBERYARD_H

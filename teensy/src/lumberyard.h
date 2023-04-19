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

class Lumberyard {
  private:
    File _root;
    File _pwd;
  public:
    Lumberyard();
    bool setup();
    void cd();
    void ls();
    bool mkdir(string dir);
};

#endif // SWANTA_LUMBERYARD_H

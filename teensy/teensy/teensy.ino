// Casey Walker (2023)
// Team Swanta Ray Team
// 24-775 Bioinspired Robot Design and Experimentation
// Carnegie Mellon University
// -------------------------------------------------------------------------- //
//
// teensy.ino
// The main deal
//

#include <SPI.h>
#include <SD.h>

#define SD_CARD BUILTIN_SDCARD

#define BAUD 115200

File root; // root directory

void setup() {
  Serial.begin(BAUD);
  while (!Serial);

  if (!SD.begin(4)) {
    // failed initialization of sd card?
  }

  root = SD.open("/");

  printDirectory(root, 0);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}

void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      break;
    }

    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

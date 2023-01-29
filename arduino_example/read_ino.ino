/*
  SD card read/write

  This example shows how to read and write data to and from an SD card file
  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

#include <SPI.h>
#include <SD.h>

File myFile;

const int BUFFER_SIZE = 50;
char buf[BUFFER_SIZE];

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("205F97~1/RO_.RAW", O_READ);
}

void loop() {
  while (myFile.available() > 0) {
    int rlen = myFile.readBytes(buf, BUFFER_SIZE);
    // prints the received data
    Serial.print("I received: ");
    for(int i = 0; i < rlen; i++) {
      Serial.print((unsigned char)(buf[i]) + 0);
      Serial.print(' ');
    }
    Serial.println();
  }

  myFile.close();
}



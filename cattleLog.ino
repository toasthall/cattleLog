#include <TinyGPS.h>
#include <SoftwareSerial.h>
#define SD_CS_PIN SS
#include <SPI.h>
#include "SdFat.h"
SdFat SD;


SoftwareSerial ss(5, 6); //RX, TX for dev only
File myFile;
TinyGPS gps;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
    //Wait for USB Serial 
    while (!Serial) 
  {
    SysCall::yield();
  }
    ss.begin(9600);


  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);

  if (!SD.begin(SD_CS_PIN)) 
  {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}

void loop()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      if (gps.encode(c))
      newData = true;
    }
  }
   if (newData)
   {
    myFile = SD.open("log.txt", FILE_WRITE);

    // if the file opened okay, write to it:
    if (myFile) 
      {
        float flat, flon;
        unsigned long age;
        gps.f_get_position(&flat, &flon, &age);
        myFile.print("LAT=");
        myFile.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
        myFile.print(" LON=");
        myFile.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
        myFile.print(" SAT=");
        myFile.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
        myFile.print(" PREC=");
        myFile.println(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
        // flash write led
        myFile.close(); //close the file
      } 
      else 
      {
        // ERROR LED
      }
    }
  gps.stats(&chars, &sentences, &failed);
  Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");
} 

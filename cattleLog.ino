#define SD_CS_PIN SS
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include "SdFat.h"
SdFat SD;

File myFile;

static const int RXPin = 8, TXPin = 9;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(57600);
  ss.begin(GPSBaud);
  
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(10, OUTPUT);

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  Serial.println("Cold Reboot");
  ss.println("$PMTK104*37"); //clears bad almanac data
  //ss.flush();
  //ss.end();
  
  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
}

void loop()
{
 getGPS();
 if (gps.location.isValid() && (float(gps.hdop.value()/100) < 5.0))
 {
  displayInfo();
 }
 else
 {
    if(!gps.location.isValid())
    {
      Serial.println(F("INVALID"));
    }
    
    else
    {
      Serial.println(float(gps.hdop.value())/ 100); 
    }
 }
 delay(1000);
}

void getGPS()
{
  while (ss.available() > 0)
  //Serial.println(ss.read());
    gps.encode(ss.read());
    
    
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}



void displayInfo()
{
    myFile = SD.open("log.txt", FILE_WRITE);
    if(myFile)
    {
      Serial.println("Log file opened");
    }
    else
    {
      Serial.println("error opening test.txt");
    }
    
    Serial.print(gps.location.lat(), 6);
    myFile.print(gps.location.lat(), 6);
    Serial.print(F("\t"));
    myFile.print(F("\t"));
    Serial.print(gps.location.lng(), 6);
    myFile.print(gps.location.lng(), 6);
    Serial.print(F("\t"));
    myFile.print(F("\t"));
    Serial.print(gps.altitude.meters());
    myFile.print(gps.altitude.meters());
    Serial.print(F("\t"));
    myFile.print(F("\t"));
    Serial.print(float(gps.hdop.value())/100.00);
    myFile.print(float(gps.hdop.value())/100.00);
    Serial.print(F("\t"));
    myFile.print(F("\t"));
      
    Serial.print(gps.date.year());
    myFile.print(gps.date.year());
    Serial.print(F("-"));
    myFile.print(F("-"));
    Serial.print(gps.date.month());
    myFile.print(gps.date.month());
    Serial.print(F("-"));
    myFile.print(F("-"));
    Serial.print(gps.date.day());
    myFile.print(gps.date.day());
    Serial.print(F("\t"));
    myFile.print(F("\t"));
      

    if (gps.time.hour() < 10) myFile.print(F("0"));
    myFile.print(gps.time.hour());
    myFile.print(F(":"));
    if (gps.time.minute() < 10) myFile.print(F("0"));
    myFile.print(gps.time.minute());
    myFile.print(F(":"));
    if (gps.time.second() < 10) myFile.print(F("0"));
    myFile.print(gps.time.second());
    myFile.print(F("."));
    if (gps.time.centisecond() < 10) myFile.print(F("0"));
    myFile.println(gps.time.centisecond());

    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.println(gps.time.centisecond());
      
    delay(1000);
    
    myFile.close();

    //return 0;
    
  } 
/*  else  
    { 
      Serial.println(F("INVALID"));
      Serial.println(float(gps.hdop.value())/ 100);      
    }
}

/*  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();*/

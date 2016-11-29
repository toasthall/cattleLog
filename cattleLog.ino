#include <Time.h>
#include <TimeLib.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include <Average.h>


TinyGPS gps;
SoftwareSerial ss(10, 11);


void setup() 
{
  Serial.begin(115200);
  ss.begin(9600);
}

void loop() 
{
  char unitID[] = "TEST01"; //unique ID for animal eg ear tag
  bool newData = false;
  float flat, flong;
  float offset = 9.5;
  unsigned long age;
  unsigned long chars;
  unsigned short sentences, failed;
  Average <float> flatStore(29);
  Average <float> flongStore(29);
  int Year;
  byte Month, Day, Hour, Minute, Second;

  
  //for (unsigned long start = millis(); millis() - start < 1000;)
  for (int x = 1; x <29; x++) //30 readings
  {
    for (unsigned long start = millis(); millis() - start < 1000;)
    {
      while (ss.available())
      {
        char c = ss.read();
        //Serial.write(c);
        if (gps.encode(c))
        newData = true;
      }
    }
      
      if (newData)
      {  
        gps.f_get_position(&flat, &flong, &age);
        gps.crack_datetime(&Year, &Month, &Day, &Hour, &Minute, &Second, NULL, NULL);
        setTime(Hour, Minute, Second, Day, Month, Year);
        adjustTime(offset * SECS_PER_HOUR);
        digitalClockDisplay();
        Serial.print(unitID);
        Serial.print(", ");
        Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
        Serial.print(", ");
        Serial.print(flong == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flong, 6);
        Serial.print(", ");
        Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
        Serial.print(", ");
        Serial.println(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
      }

     else
     {
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
  }
  
  Serial.println("wait 10 seconds");
  delay(10000);
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(", ");
  Serial.print(day());
  Serial.print(month());
  Serial.print(year()); 
  Serial.print(", "); 
}

void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}



#include <TinyGPS++.h> // Include the TinyGPS++ library
TinyGPSPlus tinyGPS; // Create a TinyGPSPlus object

#define GPS_BAUD 9600 // GPS module baud rate. GP3906 defaults to 9600.


#include <SoftwareSerial.h>
#define ARDUINO_GPS_RX 0 // GPS TX, Arduino RX pin
#define ARDUINO_GPS_TX 1 // GPS RX, Arduino TX pin
SoftwareSerial ssGPS(ARDUINO_GPS_TX, ARDUINO_GPS_RX); // Create a SoftwareSerial


#define gpsPort ssGPS  

'
#define SerialMonitor Serial

void setup()
{
  SerialMonitor.begin(9600);
  gpsPort.begin(GPS_BAUD);
}

void loop()
{
 
  printGPSInfo();


  smartDelay(1000); 
}

void printGPSInfo()
{
  // Print latitude, longitude, altitude in feet, course, speed, date, time,
  // and the number of visible satellites.
  SerialMonitor.print("Lat: "); SerialMonitor.println(tinyGPS.location.lat(), 6);
  SerialMonitor.print("Long: "); SerialMonitor.println(tinyGPS.location.lng(), 6);
  SerialMonitor.print("Alt: "); SerialMonitor.println(tinyGPS.altitude.feet());
  SerialMonitor.print("Course: "); SerialMonitor.println(tinyGPS.course.deg());
  SerialMonitor.print("Speed: "); SerialMonitor.println(tinyGPS.speed.mph());
  SerialMonitor.print("Date: "); printDate();
  SerialMonitor.print("Time: "); printTime();
  SerialMonitor.print("Sats: "); SerialMonitor.println(tinyGPS.satellites.value());
  SerialMonitor.println();
}

// This custom version of delay() ensures that the tinyGPS object
// is being "fed". From the TinyGPS++ examples.
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
   
    while (gpsPort.available())
      tinyGPS.encode(gpsPort.read()); 
    
  } while (millis() - start < ms);
}

// printDate() formats the date into dd/mm/yy.
void printDate()
{
  SerialMonitor.print(tinyGPS.date.day());
  SerialMonitor.print("/");
  SerialMonitor.print(tinyGPS.date.month());
  SerialMonitor.print("/");
  SerialMonitor.println(tinyGPS.date.year());
}


void printTime()
{
  SerialMonitor.print(tinyGPS.time.hour());
  SerialMonitor.print(":");
  if (tinyGPS.time.minute() < 10) SerialMonitor.print('0');
  SerialMonitor.print(tinyGPS.time.minute());
  SerialMonitor.print(":");
  if (tinyGPS.time.second() < 10) SerialMonitor.print('0');
  SerialMonitor.println(tinyGPS.time.second());
}

#include <ADXL345.h>
#include <TinyGPS++.h> 
#include <Wire.h>
#include <SoftwareSerial.h>
TinyGPSPlus tinyGPS;
#define ARDUINO_GPS_RX 9 
#define ARDUINO_GPS_TX 8 
SoftwareSerial ssGPS(ARDUINO_GPS_TX, ARDUINO_GPS_RX); 
#define gpsPort ssGPS  

#define SerialMonitor Serial 

#define GPS_BAUD 9600 


#include <Wire.h>

int ledPin = 13;

int knockSensor = 0;               

byte val = 0;

int statePin = LOW;

int Threshhold = 100;

ADXL345 accelerometer;

void  setup(void) 
{
  Serial.begin(9600);
 pinMode(ledPin, OUTPUT); 

 // Serial.println("Initialize ADXL345");
  gpsPort.begin(GPS_BAUD);
  pinMode(2,INPUT);

  if (!accelerometer.begin())
  {
    Serial.println("Could not find a valid ADXL345 sensor, check wiring!");
    delay(500);
  }


  accelerometer.setActivityThreshold(2.0);    
  accelerometer.setInactivityThreshold(2.0);  
  accelerometer.setTimeInactivity(5);         


  accelerometer.setActivityXYZ(1);         
  accelerometer.setInactivityXYZ(1);

  
  accelerometer.setFreeFallThreshold(0.35); 
  accelerometer.setFreeFallDuration(0.1);
  
         

  accelerometer.setTapDetectionX(0);       
  accelerometer.setTapDetectionY(0);       
  accelerometer.setTapDetectionZ(1);

  accelerometer.setTapThreshold(2.5);      
  accelerometer.setTapDuration(0.02);      
  accelerometer.setDoubleTapLatency(0.10); 
  accelerometer.setDoubleTapWindow(0.30);
 
  accelerometer.useInterrupt(ADXL345_INT1);

  checkSetup();
  checkSetup1();
  checkSetup2();
 
}


void checkSetup()
{
  Serial.print("Activity Threshold = "); Serial.println(accelerometer.getActivityThreshold());
  Serial.print("Inactivity Threshold = "); Serial.println(accelerometer.getInactivityThreshold());
  Serial.print("Time Inactivity = "); Serial.println(accelerometer.getTimeInactivity());

  Serial.print("Look activity on axis = "); 
  if (accelerometer.getActivityX()) { Serial.print(" X "); }
  if (accelerometer.getActivityY()) { Serial.print(" Y "); }
  if (accelerometer.getActivityZ()) { Serial.print(" Z "); }
  Serial.println();

  Serial.print("Look inactivity on axis = "); 
  if (accelerometer.getInactivityX()) { Serial.print(" X "); }
  if (accelerometer.getInactivityY()) { Serial.print(" Y "); }
  if (accelerometer.getInactivityZ()) { Serial.print(" Z "); }
  Serial.println();  
}


void checkSetup1()
{
   Serial.print("Free Fall Threshold = "); Serial.println(accelerometer.getFreeFallThreshold());
   Serial.print("Free Fall Duration = "); Serial.println(accelerometer.getFreeFallDuration());
}



void checkSetup2()
{
    Serial.print("Look tap on axis = "); 
  if (accelerometer.getTapDetectionX()) { Serial.print(" X "); }
  if (accelerometer.getTapDetectionY()) { Serial.print(" Y "); }
  if (accelerometer.getTapDetectionZ()) { Serial.print(" Z "); }
  Serial.println();

  Serial.print("Tap Threshold = "); Serial.println(accelerometer.getTapThreshold());
  Serial.print("Tap Duration = "); Serial.println(accelerometer.getTapDuration());
  Serial.print("Double Tap Latency = "); Serial.println(accelerometer.getDoubleTapLatency());
  Serial.print("Double Tap Window = "); Serial.println(accelerometer.getDoubleTapWindow());
}

void loop(void) 
{



   val = analogRead(knockSensor);
  
  delay(50);
  Vector norm = accelerometer.readNormalize();

  
  Activites activ = accelerometer.readActivites();


  if (activ.isActivity and val> Threshhold )
  {
    Serial.println("Activity Detected");
    printGPSInfo();
    smartDelay(1000); 
  }
  

  if (activ.isInactivity )
  {
    
    Serial.println("Inactivity Detected");

    
  }

  if (activ.isFreeFall  and val >Threshhold)
  {
    Serial.println("Free fall Detected!");
      printGPSInfo();
    smartDelay(1000); 
    
  }

   if (activ.isDoubleTap and val>Threshhold)
  {
    Serial.println("Double Tap Detected");
      printGPSInfo();
    smartDelay(1000); 
  } 
  else
  if (activ.isTap and val>Threshhold)
  {
    Serial.println("Tap Detected");
      printGPSInfo();
    smartDelay(1000); 
  }
}
void printGPSInfo()
{
 
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

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
   
    while (gpsPort.available())
      tinyGPS.encode(gpsPort.read()); 
    
  } while (millis() - start < ms);
}

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


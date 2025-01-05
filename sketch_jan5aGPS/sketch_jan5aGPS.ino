#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup(){
  Serial.begin(9600);
  ss.begin(GPSBaud);
  Serial.println("GPS test started...");
}

void loop(){
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }

  delay(1000);  // Wait for a bit to accumulate GPS data

  if (gps.location.isUpdated()) {
    Serial.print("Latitude= ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(" Longitude= ");
    Serial.println(gps.location.lng(), 6);
  } else {
    Serial.println("Waiting for GPS fix...");
  }
}

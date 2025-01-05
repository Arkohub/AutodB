#include <SD.h>             // Include SD library
#include <TinyGPS++.h>      // Include TinyGPS++ library
#include <SoftwareSerial.h> // Include SoftwareSerial for GPS communication

// Pin definitions
#define SoundSensorPin A1  // Pin for the sound sensor
#define VREF  5.0          // Voltage reference for the analog input
#define SD_CS_PIN 4        // Chip select pin for the SD card (modify if necessary)
static const int RXPin = 3, TXPin = 5; // GPS module pins
static const uint32_t GPSBaud = 9600;  // GPS baud rate

// GPS setup
TinyGPSPlus gps;           // Create TinyGPS++ object
SoftwareSerial ss(RXPin, TXPin); // GPS serial communication

// SD file
File dataFile;

void setup() {
  Serial.begin(115200);   // Start serial communication for debugging
  ss.begin(GPSBaud);      // Start GPS communication
  Serial.println("Starting...");

  // Initialize SD card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");

  // Open file for writing the header
  dataFile = SD.open("data.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Timestamp (ms), Decibel Reading (dBA), Latitude, Longitude");  // CSV header
    dataFile.close();  // Close file after writing the header
  } else {
    Serial.println("Error opening data.csv for writing");
  }
}

void loop() {
  // Variables for sound sensor reading
  float voltageValue, dbValue;

  // Read decibel data from sound sensor
  voltageValue = analogRead(SoundSensorPin) / 1024.0 * VREF;
  dbValue = voltageValue * 50.0;  // Convert voltage to decibel value

  // Get the timestamp in milliseconds
  unsigned long timestamp = millis();

  // GPS processing: Check if a new GPS location is available
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }

  // Read the GPS data if updated
  double latitude = 0.0, longitude = 0.0;
  if (gps.location.isUpdated()) {
    latitude = gps.location.lat();
    longitude = gps.location.lng();
  } else {
    Serial.println("Waiting for GPS fix...");
  }

  // Output data to Serial Monitor
  Serial.print(timestamp);            // Print timestamp
  Serial.print(", ");                 // Separator
  Serial.print(dbValue, 1);           // Print decibel value
  Serial.print(", ");                 // Separator
  Serial.print(latitude, 6);          // Print latitude
  Serial.print(", ");                 // Separator
  Serial.println(longitude, 6);       // Print longitude

  // Open the file for appending data
  dataFile = SD.open("data.csv", FILE_WRITE);

  // If the file is available, write timestamp, dbValue, and GPS coordinates
  if (dataFile) {
    dataFile.print(timestamp);        // Write timestamp
    dataFile.print(", ");             // Separator
    dataFile.print(dbValue, 1);       // Write decibel value
    dataFile.print(", ");             // Separator
    dataFile.print(latitude, 6);      // Write latitude
    dataFile.print(", ");             // Separator
    dataFile.println(longitude, 6);   // Write longitude
    dataFile.close();  // Close the file after writing the data
  } else {
    Serial.println("Error opening data.csv for writing");
  }

  delay(500);  // Delay for 500 milliseconds
}

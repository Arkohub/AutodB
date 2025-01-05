#include <SD.h>  // Include SD library

#define SoundSensorPin A1  // Pin for the sound sensor
#define VREF  5.0  // Voltage reference for the analog input
#define SD_CS_PIN 4  // Chip select pin for the SD card (modify if necessary)

// Declare the file
File dataFile;

void setup() {
  Serial.begin(115200);
  Serial.println("Timestamp (ms), Decibel Reading (dBA)");  // Header

  // Initialize SD card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Initialization failed!");
    return;
  }
  Serial.println("Initialization done.");

  // Open the file for writing
  dataFile = SD.open("data.csv", FILE_WRITE);
  
  // If the file is available, write the header
  if (dataFile) {
    dataFile.println("Timestamp (ms), Decibel Reading (dBA)");  // Write header
    dataFile.close();  // Close the file after writing the header
  } else {
    Serial.println("Error opening data.csv");
  }
}

void loop() {
  float voltageValue, dbValue;
  
  voltageValue = analogRead(SoundSensorPin) / 1024.0 * VREF;
  dbValue = voltageValue * 50.0;  // Convert voltage to decibel value
  
  // Get the timestamp in milliseconds
  unsigned long timestamp = millis();

  // Output data to Serial Monitor
  Serial.print(timestamp);        // Print timestamp
  Serial.print(", ");             // Separator
  Serial.println(dbValue, 1);     // Print decibel value

  // Open the file for appending new data
  dataFile = SD.open("data.csv", FILE_WRITE);

  // If the file is available, write the timestamp and dbValue
  if (dataFile) {
    dataFile.print(timestamp);        // Write timestamp
    dataFile.print(", ");             // Separator
    dataFile.println(dbValue, 1);     // Write decibel value
    dataFile.close();  // Close the file after writing
  } else {
    Serial.println("Error opening data.csv for writing");
  }

  delay(500);  // Delay for 500 milliseconds
}

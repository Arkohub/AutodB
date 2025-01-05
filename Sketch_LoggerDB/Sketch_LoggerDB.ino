#define SoundSensorPin A1  // Pin for the sound sensor
#define VREF  5.0  // Voltage reference for the analog input

void setup() {
  Serial.begin(115200);
  Serial.println("Timestamp (ms), Decibel Reading (dBA)");  // Header
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

  delay(500);  // Delay for 500 milliseconds
}

/*
  Simple Weather Data Receiver & Parser
  Expected input format (terminated by newline):
  TIME:xxxxxxZ; CITY:yyyyy; SKY:zzzz; TEMP:aa.a; HUM:bb; WIND:cc.c
*/

// Parsed Variables
String timeVal = "";
String cityVal = "";
String skyVal  = "";
float  tempVal = 0.0;
int    humVal  = 0;
float  windVal = 0.0;

void setup() {
  Serial.begin(9600);
  while (!Serial) { ; } // Wait for USB connection on boards like Leonardo/Pico
  Serial.println("Ready to receive weather data...");
}

void loop() {
  if (Serial.available() > 0) {
    // Read the complete line sent from the Web Serial app
    String payload = Serial.readStringUntil('\n');
    payload.trim();

    if (payload.length() > 0) {
      parsePayload(payload);
      printVariables();
    }
  }
}

void parsePayload(String data) {
  int startPos = 0;

  while (startPos < data.length()) {
    // Find the delimiter ';'
    int endPos = data.indexOf(';', startPos);
    if (endPos == -1) {
      endPos = data.length(); // Last pair in the string
    }

    // Extract key-value pair (e.g., " TEMP:18.2")
    String token = data.substring(startPos, endPos);
    token.trim();

    // Split key and value by ':'
    int colonPos = token.indexOf(':');
    if (colonPos != -1) {
      String key   = token.substring(0, colonPos);
      String value = token.substring(colonPos + 1);
      key.trim();
      value.trim();

      // Assign to respective variables
      if (key == "TIME")  timeVal = value;
      if (key == "CITY")  cityVal = value;
      if (key == "SKY")   skyVal  = value;
      if (key == "TEMP")  tempVal = value.toFloat();
      if (key == "HUM")   humVal  = value.toInt();
      if (key == "WIND")  windVal = value.toFloat();
    }

    startPos = endPos + 1;
  }
}

void printVariables() {
  Serial.println("\n--- [RECEIVED & PARSED DATA] ---");
  Serial.print("City:        "); Serial.println(cityVal);
  Serial.print("Temperature: "); Serial.print(tempVal); Serial.println(" °C");
  Serial.print("Sky:         "); Serial.println(skyVal);
  Serial.print("Humidity:    "); Serial.print(humVal);  Serial.println(" %");
  Serial.print("Wind Speed:  "); Serial.print(windVal); Serial.println(" km/h");
  Serial.print("Timestamp:   "); Serial.println(timeVal);
  Serial.println("--------------------------------\n");
}


#define DEBUG true
#include <SoftwareSerial.h>



SoftwareSerial A9GSerial(3, 4); // RX, TX - Define the RX and TX pins for SoftwareSerial 87

String sendData(String command, const int timeout, boolean debug) {
  String response = "";
  A9GSerial.println(command);
  long int time = millis();
  while ( (time + timeout) > millis()) {
    while (A9GSerial.available()) {
      response = A9GSerial.readString();
    }
  }
  if (debug) {
    Serial.print(response);
  }
  return response;
}

void testSDCardDebug() {
  Serial.println("Testing SD card...");

  // Test SD card
  String response = sendData("AT+SDTEST", 2000, DEBUG);
  Serial.println("SDTEST Response: " + response);

  // Attempt to mount SD card
  response = sendData("AT+SDMOUNT", 2000, DEBUG);
  Serial.println("SDMOUNT Response: " + response);

  // Check free space
  response = sendData("AT+SDFREE", 2000, DEBUG);
  Serial.println("SDFREE Response: " + response);

  // List files
  response = sendData("AT+SDLIST", 2000, DEBUG);
  Serial.println("SDLIST Response: " + response);
}



void checkSDCard() {
  // Test if the SD card is initialized
  String response = sendData("AT+SDTEST", 1000, DEBUG);
  if (response.indexOf("OK") != -1) {
    Serial.println("SD card initialized successfully.");
  } else {
    Serial.println("SD card initialization failed. Check the SD card.");
    return; // Exit the function if SD card is not initialized
  }

  // List all files on the SD card
  Serial.println("Listing files on the SD card:");
  response = sendData("AT+SDLIST", 2000, DEBUG);
  if (response.indexOf("OK") != -1) {
    Serial.println("Files on SD card:");
    Serial.println(response); // Print the file list to Serial Monitor
  } else {
    Serial.println("Failed to list files. Check if the SD card is properly formatted.");
  }
}


void setup() {
  Serial.begin(9600);
  A9GSerial.begin(9600);

  // Initialize and verify the SD card
  checkSDCard();
  testSDCardDebug();
}

void loop() {
  // Add your code here
}

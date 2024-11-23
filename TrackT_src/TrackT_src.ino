#include <Arduino.h>

#include <SoftwareSerial.h>

#define DEBUG true
#define BUTTON_PIN 2
volatile bool buttonPresses = false;


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

void initialization(){
   sendData("AT+GPS=1", 1000, DEBUG); // Turn on GPS
    sendData("AT+CGATT=1", 1000, DEBUG);
    sendData("AT+CGATT=1", 1000, DEBUG);

    sendData("AT+CGDCONT=1,\"IP\",\"APN\"",1000, DEBUG); // Add APN
    sendData("AT+CGACT=1,1",1000, DEBUG);
    sendData("AT+GPS=1", 1000, DEBUG); // Get GPS address
    
}

// Function to send an SMS
void sendSMS(String phoneNumber, String messageContent, String messageType) {
  sendData("AT+CMGF=1", 1000, DEBUG); // Set SMS to Text Mode
  sendData("AT+CSCS=\"" + messageType + "\"", 1000, DEBUG); // Set character set (e.g., "GSM")
  A9GSerial.print("AT+CMGS=\"");
  A9GSerial.print(phoneNumber);
  A9GSerial.println("\"");
  delay(100);

  A9GSerial.print(messageContent); // Write the SMS content
  delay(100);

  A9GSerial.write(26); // CTRL+Z character to send the SMS
  delay(5000);

  Serial.println("Message Sent!");
}

void setup() {
  Serial.begin(115200);
  A9GSerial.begin(115200);
  pinMode(BUTTON_PIN,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN),handleButtonPress,FALLING);
  sendData("AT+GPS=1",1000,DEBUG);
  sendData("AT+CGATT=1",1000,DEBUG);
  sendData("AT+CGDCONT=1,\"IP\",\"vodafone\"",1000,DEBUG);
  sendData("AT+CGDCONT=2,\"IP\",\"vodafone\"",1000,DEBUG);
  sendData("AT+CGACT=1,1",1000,DEBUG);
  sendData("AT+CGACT=1,2",1000,DEBUG);
}

void loop(){
  if (buttonPresses){
    String coordinates = sendData("AT+LOCATION=2", 1000, DEBUG);
    sendSMS("+94769054603", "GPS Coordinates: " + coordinates, "GSM");
    delay(2000);
    buttonPresses = false;
  }
}

void handleButtonPress(){
  buttonPresses = true;
  Serial.println("Button Pressed");
}
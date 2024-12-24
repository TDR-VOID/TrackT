#include <Arduino.h>

#include <SoftwareSerial.h>

#define DEBUG true

unsigned long timeCount;


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
    timeCount = millis();
    Serial.println("Arduino Uno - A9G GPS Coordinates:");
}


void setup() {
  Serial.begin(115200);
  A9GSerial.begin(115200);

  sendData("AT+GPS=1",1000,DEBUG);
  sendData("AT+CGATT=1",1000,DEBUG);
  sendData("AT+CGDCONT=1,\"IP\",\"vodafone\"",1000,DEBUG);
  sendData("AT+CGDCONT=2,\"IP\",\"vodafone\"",1000,DEBUG);
  sendData("AT+CGACT=1,1",1000,DEBUG);
  sendData("AT+CGACT=1,2",1000,DEBUG);

}
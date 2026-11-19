#include <Wire.h>
#include <ZumoShield.h>
#include <SoftwareSerial.h>

ZumoMotors motors;
SoftwareSerial bluetooth(2, 13);    //Rx-Pin2 and Tx-Pin13 (unused)
const int SPEED = 200;    // Speed can vary between 0 up to 400

void drive(char cmd) {
  
  switch(cmd) {
    
    case 'F' :    // FORWARD command
      motors.setSpeeds(SPEED, SPEED);
      break;
  
    case 'B' :    // BACK/REVERSE command
      motors.setSpeeds(-SPEED, -SPEED);
      break;

    case 'L' :    // LEFT command
      motors.setSpeeds(-SPEED, SPEED);
      break;

    case 'R' :    // RIGHT command
      motors.setSpeeds(SPEED, -SPEED);
      break;

    case 'S' :    // STOP command
      default :
      motors.setSpeeds(0,0);
      break;

  }

}

  void handleChar(char c) {

          if(c == '\r' || c == '\n') return;
          Serial.println( "Got: ");
          Serial.print(c);
          drive(toupper(c));
  
  }


void setup() {
  
  Serial.begin(9600);   // set BAUD
  bluetooth.begin(9600);
  Serial.println("Tiny Z Ready");   // displays text

}

void loop() {

  if(Serial.available()) {

    handleChar(Serial.read());

  }
  if(bluetooth.available()) {

    handleChar(bluetooth.read());
  
  }
}




















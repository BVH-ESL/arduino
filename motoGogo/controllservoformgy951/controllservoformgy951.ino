#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial mySerial(9, 8); // RX, TX
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
float yaw;

Servo myservo; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  mySerial.begin(57600);
  myservo.attach(10); 
}

void loop() {
  serialEvent(); //call the function
  // print the string when a newline arrives:
  if (stringComplete) {
//    Serial.println(inputString);
//    Serial.print("index , as \t");Serial.println(inputString.indexOf(','));
    yaw = inputString.substring(inputString.indexOf('=')+1, inputString.indexOf(',')).toFloat();
    Serial.println(yaw);
    if(yaw > 0 && yaw < 45){
      myservo.write(0);
    }else if(yaw > 45 && yaw < 135){
      myservo.write(90);
    }else if(yaw > 135 && yaw < 180){
      myservo.write(180);
    }
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (mySerial.available()) {
    char inChar = (char)mySerial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

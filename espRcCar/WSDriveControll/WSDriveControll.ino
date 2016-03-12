/*
 * WebSocketServer.ino
 *
 *  Created on: 22.05.2015
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <Hash.h>

ESP8266WiFiMulti WiFiMulti;

WebSocketsServer webSocket = WebSocketsServer(81);

#define USE_SERIAL Serial
String data_in = "";

int speed_a;
int speed_b;

// connect motor controller pins to Arduino digital pins
// motor one
int enA = 5;    //D1
int in1 = 4;    //D2
int in2 = 2;    //D4

// motor two
int in3 = 13;   //D7
int in4 = 12;   //D6
int enB = 14;   //D5

void drive(int speed_a, int speed_b) {
  if (speed_a > 0 && speed_b > 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  } else if (speed_a < 0 && speed_b < 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    speed_a *= -1;
    speed_b *= -1;
  } else if (speed_a > 0 && speed_b < 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    speed_b *= -1;
  } else if (speed_a < 0 && speed_b > 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    speed_a *= -1;
  }

  if (speed_a == 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }

  if (speed_b == 0) {
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }

  float s_a = speed_a * 1023 / 100.0;
  float s_b = speed_b * 1023 / 100.0;
  analogWrite(enA, s_a);
  analogWrite(enB, s_b);
}

void checkInput(String input) {
  //  Serial.println(sizeof(input));
  String inString;
  for (int i = 1; i < input.length(); i++) {
    USE_SERIAL.printf("%c", input[i]);
    if (input[0] == 'm') {
      if (input[i] == ':' || input[i] == '\n') {
        drive(speed_a, speed_b);
        inString = "";
      } else {
        if (input[i] != ',' && input[i] != '(' && input[i] != ')') {
          inString += input[i];
        } else {
          if (input[i] == ',') {
            speed_a = inString.toInt();
            inString = "";
          } else if (input[i] == ')') {
            speed_b = inString.toInt();
            inString = "";
          }
        }
      }
    }
  }
  USE_SERIAL.printf("\n");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

  switch (type) {
    case WStype_DISCONNECTED:
      USE_SERIAL.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "Connected");
      }
      break;
    case WStype_TEXT:
//      USE_SERIAL.printf("[%u] get Text: %s\n", lenght, payload);
      data_in += (char *)payload;
//      USE_SERIAL.printf("[%u] get Text: %S\n", lenght, data_in);
//      USE_SERIAL.printf("%s", data_in);
      checkInput(data_in);
      data_in = "";
//      String stringData = (char*) payload;
//      char* input_cr=(char*) &payload;
//      String data_in = "";
//      for (int i = 0; i < sizeof(payload); i++) {
//       data_in += (char)payload[i];
//      }
//      checkInput(data_in);
//      data_in = "";
      // send message to client
      // webSocket.sendTXT(num, "message here");

      // send data to all connected clients
      // webSocket.broadcastTXT("message here");
      break;
    case WStype_BIN:
      USE_SERIAL.printf("[%u] get binary lenght: %u\n", num, lenght);
      hexdump(payload, lenght);

      // send message to client
      // webSocket.sendBIN(num, payload, lenght);
      break;
  }

}

void setup() {
  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  // USE_SERIAL.begin(921600);
  USE_SERIAL.begin(115200);

  //Serial.setDebugOutput(true);
  USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  WiFiMulti.addAP("SSID", "passpasspass");

  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
}

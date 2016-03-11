#include <ESP8266WiFi.h>

const char* ssid = "ESL_Lab1";
const char* password = "wifi@esl";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(8080);
char data_input[100];
char* data;
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

void drive(int speed_a, int speed_b){
  if (speed_a > 0 && speed_b > 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }else if (speed_a < 0 && speed_b < 0) {
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
  
  if (speed_a == 0){
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }

  if (speed_b == 0){
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
  
  float s_a = speed_a * 1023 / 100.0;
  float s_b = speed_b * 1023 / 100.0;
  analogWrite(enA, s_a);
  analogWrite(enB, s_b);
}

void check_input(String input) {
  //  Serial.println(sizeof(input));
  String inString;
  for (int i = 1; i < input.length(); i++) {
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
            //          Serial.print("a : ");
            //          Serial.println(speed_a);
            inString = "";
          } else if (input[i] == ')') {
            speed_b = inString.toInt();
            //          Serial.print("b : ");
            //          Serial.println(speed_b);
            inString = "";
          }
        }
      }
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  // Connect to WiFi network
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  check_input(req);
  client.flush();
}


#include <ESP8266WiFi.h>

const char* ssid = "ESL_Lab1";
const char* password = "wifi@esl";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(8080);
char data_input[100];
char* data;

void check_input(String input) {
  //  Serial.println(sizeof(input));
  String inString;
  for (int i = 1; i < input.length(); i++) {
    if (input[0] == 'm') {
      if (input[i] != ',' && input[i] != '(' && input[i] != ')') {
        inString += input[i];
      } else {
        if (input[i] == ',') {
          speed_a = inString.toInt();
          Serial.print("a : ");
          Serial.println(speed_a);
          inString = "";
        } else if (input[i] == ')') {
          speed_b = inString.toInt();
          Serial.print("b : ");
          Serial.println(speed_b);
          inString = "";
        }
      }
    }
  }
}

void setup() {
  // put your setup code here, to run once:
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
  //  req.toCharArray(data_input, req.length()+1);
  check_input(req);
  client.flush();
}


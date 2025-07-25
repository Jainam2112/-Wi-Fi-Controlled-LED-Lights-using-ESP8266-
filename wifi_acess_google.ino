#include <ESP8266WiFi.h>

const char* ssid = "INFOLABZ 407";
const char* password = "Infolabz@123";

int ledPin1 = D3; 
int ledPin2 = D8; 

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);

  // Connect to WiFi network
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
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
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
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request
  int value1 = LOW;
  int value2 = LOW;

  if (request.indexOf("/LED1=ON") != -1) {
    digitalWrite(ledPin1, HIGH);
    value1 = HIGH;
  }
  if (request.indexOf("/LED1=OFF") != -1) {
    digitalWrite(ledPin1, LOW);
    value1 = LOW;
  }
  
  if (request.indexOf("/LED2=ON") != -1) {
    digitalWrite(ledPin2, HIGH);
    value2 = HIGH;
  }
  if (request.indexOf("/LED2=OFF") != -1) {
    digitalWrite(ledPin2, LOW);
    value2 = LOW;
  }

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.print("LED 1 is now: ");
  if (value1 == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }
  
  client.print("<br>LED 2 is now: ");
  if (value2 == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }

  client.println("<br><br>");
  client.println("<a href=\"/LED1=ON\"\"><button>LED 1 On</button></a>");
  client.println("<a href=\"/LED1=OFF\"\"><button>LED 1 Off</button></a><br />");
  
  client.println("<a href=\"/LED2=ON\"\"><button>LED 2 On</button></a>");
  client.println("<a href=\"/LED2=OFF\"\"><button>LED 2 Off</button></a><br />");

  client.println("</html>");

  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
}

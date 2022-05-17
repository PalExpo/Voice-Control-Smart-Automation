#include <WiFi.h>
WiFiClient client;
WiFiServer server(80);
const char* ssid = "realme 7 pro";
const char* password = "hubham123";
String  command = ""; // Command received from Android device

// Set Relay Pins
int relay1 = 4;
int relay2 = 2;

void setup() {
  Serial.begin(115200);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  connectWiFi();
  server.begin();
}

void loop() {
  client = server.available();
  if (!client) return;
  command = checkClient ();
  if (command == "r1on"  || command == "turn on relay 1"    || command == "r1%20on" || command == "are%20one%20on") {
    digitalWrite(relay1, 0);
    Serial.println("Relay1 ON");
  } else if (command == "r1off" || command == "turn off relay 1" || command == "r1%20of" || command == "are%20one%20of") {
    digitalWrite(relay1, 1);
    Serial.println("Relay1 OFF");
  } else if (command == "r2on"  || command == "turn on relay 2"    || command == "r2%20on" || command == "are%20to%20on" || command == "are%20tu%20on") {
    digitalWrite(relay2, 0);
    Serial.println("Relay2 ON");
  } else if (command == "r2off" || command == "turn off relay 2" || command == "r2%20of" || command == "are%20to%20of" || command == "are%20tu%20of" || command == "are%20to%20off" || command == "are%20tu%20off") {
    digitalWrite(relay2, 1);
    Serial.println("Relay2 OFF");
  } else if (command == "allon" || command == "Turn on all devices" || command == "all%20on") {
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
    Serial.println("All ON");
  } else if (command == "alloff" || command == "Turn off all devices" || command == "all off" || command == "all%20of" || command == "all%20off" || command == "All%20Off") {
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    Serial.println("ALL OFF");
  } sendBackEcho(command); // send command echo back to android device
  command = "";
}

/* connecting WiFi */
void connectWiFi() {
  Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED))) {
    delay(300);
    Serial.print("..");
  } Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("ESP32 Local IP is : ");
  Serial.print((WiFi.localIP()));
}

/* check command received from Android Device */
String checkClient (void) {
  while (!client.available()) {
    delay(1);
  } Serial.print("Data---");
  Serial.println(client.available());
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length() - 9, 9);
  return request;
}

/* send command echo back to android device */
void sendBackEcho(String echo) {
  client.println("HTTP/1.1 200 OK ");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println(echo);
  client.println("</html>");
  client.stop();
  delay(1);
}

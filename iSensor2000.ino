#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
 
const char* ssid = "NETGEAR00";
const char* password = "yellowcartoon661";
MDNSResponder mdns;

ESP8266WebServer server(80);
 
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 5
 
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

void handleRoot() {
//  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
//  digitalWrite(led, 0);
}

void handleNotFound(){
//  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
//  digitalWrite(led, 0);
}

void handleTemp() {
//  String temperature = String(getTemp(), 1);
  String temperature = getTemp();
  Serial.println("Temperature: " + temperature);
  server.send(200, "text/html", "<h1>" + temperature + "</h1>");
}

void handleMoisture() {
  String moistureLevel = getMoisture();
  server.send(200, "text/html", "<h1>" + moistureLevel + "</h1>");
}

void handleStatus() {
  server.send(200, "text/html", "<h1>" + getMoisture() + "<br/>" + getTemp() + "</h1>");
}

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  // Start up the library
  sensors.begin();
  server.on("/", handleRoot);
  server.on("/temp", handleTemp);
  server.on("/moisture", handleMoisture);
  server.on("/status", handleStatus);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  
//  String moistureLevel = String(getMoisture(), 1);
//  Serial.println("Moisture: " + moistureLevel + "%");
//  
//  delay(1000);
//
//  String temperature = String(getTemp(), 1);
//  Serial.println("Temperatur: " + temperature + "F");
}

String getTemp() {
  Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");

  Serial.print("Temperature for Device 1 is: ");
//  return sensors.getTempFByIndex(0);
  
  String temperature = String(sensors.getTempFByIndex(0), 1);
  String msg = "Temperature: " + temperature + "F";
  return msg;
}

String getMoisture() {
//  digitalWrite(3, HIGH);
  int value = analogRead(A0);
//  digitalWrite(3, LOW);
  float percent = float(value)/1023.0*100.0;
  String strVal = String(percent, 1);
  String msg = "Moisture Level:" + strVal + "%";
  return msg;
}

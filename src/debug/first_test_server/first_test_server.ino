// https://robotzero.one/sending-data-esp8266-to-esp8266/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "poopssid";
const char *password = "pingu4prez";

ESP8266WebServer server(80);

void handleSentVar() {
  if (server.hasArg("sensor_reading")) { // this is the variable sent from the client
    int readingInt = server.arg("sensor_reading").toInt();
    char readingToPrint[5];
    itoa(readingInt, readingToPrint, 10); //integer to string conversion for OLED library
    Serial.println(readingToPrint);
    server.send(200, "text/html", "Data received");
  }
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  server.on("/data/", HTTP_GET, handleSentVar); // when the server receives a request with /data/ in the string then run the handleSentVar function
  server.begin();
}

void loop() {
  server.handleClient();
}

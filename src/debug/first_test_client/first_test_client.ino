// https://robotzero.one/sending-data-esp8266-to-esp8266/

#include <ESP8266WiFi.h>

const char *ssid = "poopssid";
const char *password = "pingu4prez";

const int analogInPin = 0;  // Analog input pin that the potentiometer is attached to
int sensorValue = 0;        // value read from the potentiometer
int outputValue = 0;        // value sent to server

void setup() {
  Serial.begin(115200);
  delay(10);

  // Explicitly set the ESP8266 to be a WiFi-client
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("OK");
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(A0);
  // map to range. The pot goes from about 3 to 1023. This makes the sent value be between 0 and 999 to fit on the OLED
  outputValue = map(sensorValue, 3, 1023, 0, 999);
  outputValue = 123;

  char intToPrint[5];
  itoa(outputValue, intToPrint, 10); //integer to string conversion for OLED library

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const char * host = "192.168.4.1";
  const int httpPort = 80;

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request. Something like /data/?sensor_reading=123
  String url = "/data/";
  url += "?sensor_reading=";
  url += intToPrint;
  Serial.println(intToPrint);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  
  while (client.available()) {
    String last_input_data = client.readStringUntil('\n') + "!!!\n";
    //String a = "";
    char a[200];
    //client.read(a,200);
    //a += "&\n";
    Serial.print(last_input_data);
    //for (int i = 0; i<200; i++) Serial.print(a);
    //Serial.println("&");
  }
  client.stop();

  delay(500);
}

// https://diytech.ru/projects/sozdanie-veb-servera-esp8266-kod-i-skhemy
// https://www.embedded-robotics.com/esp8266-wifi/

#include <ESP8266WiFi.h> 

const char* ap_ssid = "poopssid"; //Access Point SSID
const char* ap_password= "pingu4prez"; //Access Point Password
uint8_t max_connections=8;//Maximum Connection Limit for AP
int current_stations=0, new_stations=0;

WiFiServer server(80);
String header; // Variable to store the HTTP request
 
void setup()
{
  //Start the serial communication channel
  Serial.begin(115200);
  Serial.println();
   
  //Setting the AP Mode with SSID, Password, and Max Connection Limit
  if(WiFi.softAP(ap_ssid,ap_password,1,false,max_connections)) {
    Serial.print("Access Point is Creadted with SSID: ");
    Serial.println(ap_ssid);
    Serial.print("Max Connections Allowed: ");
    Serial.println(max_connections);
    Serial.print("Access Point IP: ");
    Serial.println(WiFi.softAPIP());
  }
  else
  {
    Serial.println("Unable to Create Access Point");
  }
  
  server.begin();
}

void test_connect_devices() {
  new_stations=WiFi.softAPgetStationNum();
   
  if(current_stations<new_stations)//Device is Connected
  {
    current_stations=new_stations;
    Serial.print("New Device Connected to SoftAP... Total Connections: ");
    Serial.println(current_stations);
  }
   
  if(current_stations>new_stations)//Device is Disconnected
  {
    current_stations=new_stations;
    Serial.print("Device disconnected from SoftAP... Total Connections: ");
    Serial.println(current_stations);
  }
}

void server_loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
//    String currentLine = "";                // make a String to hold incoming data from the client
//    while (client.connected()) {            // loop while the client's connected
//      if (client.available()) {             // if there's bytes to read from the client,
//        char c = client.read();             // read a byte, then
//        Serial.print(c);                    // print it out the serial monitor
//      }
//    }
    while (client.connected()) {
      String last_input_data = client.readStringUntil('\n') + "\n"; // лучше сделать без этого, а "в лоб"
      Serial.print(last_input_data);
    }
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  
  
}
 
void loop()
{
  server_loop();
  // test_connect_devices(); // WARNING: using only one fun 
}

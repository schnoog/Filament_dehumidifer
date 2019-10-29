#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266OTA.h>

const char* host = "esp8266--filahum-webupdate";
const char* ssid = "YourWIFISSID";
const char* password = "YourWIFIPassword";

ESP8266WebServer httpServer(80);
ESP8266OTA otaUpdater;

#include "DHT.h"





#define DHTTYPE DHT22

uint8_t DHTPin = D2;
DHT dht(DHTPin, DHTTYPE);

int PumpPin = D5;

float Temperature;
float Humidity;

float HumUpperLimit = 35.0;
float HumLowerLimit = 30.0;

int PumpStatus = 0;

unsigned long previousMillis = 0;
const long interval = 180000;

void setup() {
  Serial.begin(9600);
  delay(100);

  pinMode(DHTPin, INPUT);
  pinMode(PumpPin, OUTPUT);

  dht.begin();

  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  while(WiFi.waitForConnectResult() != WL_CONNECTED){
    WiFi.begin(ssid, password);
    Serial.println("WiFi failed, retrying.");
  }

  Serial.print("Connected as  ");
  Serial.println(WiFi.localIP());

  MDNS.begin(host);

  //set web UI
  otaUpdater.setUpdaterUi("Filament dehumidifier","Filament-Dehumidifier","Build : 0.01","Branch : master","Device info : NodeMCU","footer");

  //setup web UI , with chip ID auto generated
  //otaUpdater.setUpdaterUi("Title","Banner","Build : 0.01","Branch : master","footer");

  otaUpdater.setup(&httpServer);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);
  //Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", host);
  String MyIP = WiFi.localIP().toString();
  Serial.print("HTTPUpdateServer ready! Open http://");
  Serial.print(WiFi.localIP());
  Serial.println("/update in your browser\n");

}


////////////////////////

////////////////////////






void loop() {
  httpServer.handleClient();
  // put your main code here, to run repeatedly:
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
            previousMillis = currentMillis;
          Temperature = dht.readTemperature(); // Gets the values of the temperature
          Humidity = dht.readHumidity(); // Gets the values of the humidity
          if(Humidity > HumUpperLimit){
           digitalWrite(PumpPin, HIGH);
           PumpStatus = 1;
          }
          if(Humidity < HumLowerLimit){
            digitalWrite(PumpPin, LOW);
            PumpStatus = 0;
          }
          Serial.println("---------------------");
          Serial.println(Temperature);
          Serial.println(Humidity);
          Serial.print("pumpe: ");
          Serial.println(PumpStatus);
          if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
              HTTPClient http;  //Declare an object of class HTTPClient
              http.begin((String)"http://192.168.178.27/logdata.php?hum=" + (String)Humidity + (String)"&temp=" + (String)Temperature +  (String)"&pump=" + (String)PumpStatus);  //Specify request destination
              int httpCode = http.GET();                                                                  //Send the request
              if (httpCode > 0) { //Check the returning code
                String payload = http.getString();   //Get the request response payload
                Serial.println(payload);                     //Print the response payload
              }
              http.end();   //Close connection
          }
  }

}


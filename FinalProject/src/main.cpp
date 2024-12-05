#include <Arduino.h>
// #include <Arduino_OV767X.h>
#include <Wire.h>
#include "DHT20.h"
#include <HttpClient.h>
#include <WiFi.h>
DHT20 DHT;
int PROXIMITY_PIN = 37;
int BUZZER_PIN = 12;
int SOIL_SENSOR = 38;
int hasObstacle;
int moist_lvl=50;
boolean buzzState = false;
const char kHostname[] = "13.57.43.159";
// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30 * 1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

void setup() {
  hasObstacle = HIGH;
  pinMode(PROXIMITY_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(SOIL_SENSOR, INPUT);
  Serial.begin(9600);
  Wire.begin();

  char *ssid2 = "CALIBURN 0042";
  char *pass2 = "+1N8h876";
  WiFi.begin(ssid2, pass2);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("MAC address: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  int err = 0;
  WiFiClient c;
  HttpClient http(c);
  delay(1000);
  hasObstacle = digitalRead(PROXIMITY_PIN);
  // Serial.print(hasObstacle);
  moist_lvl = analogRead(SOIL_SENSOR);
  Serial.println(moist_lvl);
  DHT.read();
  Serial.println(DHT.getTemperature(), 1);
  float temp = DHT.getTemperature();
  std::string path ="/?temp="+ std::to_string(temp) + "&moist=" + std::to_string(moist_lvl);
  delay(1000);
  err = http.get(kHostname,5000,  path.c_str(), NULL);
  if (hasObstacle == LOW)
  {
    
    Serial.println("OBSTACLE!!, OBSTACLE!!");
    digitalWrite(BUZZER_PIN, HIGH);
    if(buzzState == false){
      tone(BUZZER_PIN, 5000);
      buzzState = true;
    }
  }
  else
  {
    Serial.println("clear");
    digitalWrite(BUZZER_PIN, LOW);
    if(buzzState == true){
      noTone(BUZZER_PIN);
      buzzState = false;
    }
  }
  http.stop();
}

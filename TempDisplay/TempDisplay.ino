#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266HTTPClient.h>
#include <Phant.h>
#include "creds.h"

const long day = 86400000; // 86400000 milliseconds in a day
const long hour = 3600000; // 3600000 milliseconds in an hour
const long minute = 60000; // 60000 milliseconds in a minute
const long second =  1000; // 1000 milliseconds in a second
const bool DEBUG = true;

String current_temp;

// pin 4, but GPIO 2
OneWire ds(2);
DallasTemperature sensors(&ds);

Phant phant("data.sparkfun.com", sparkfun_public_key, sparkfun_private_key);

HTTPClient http;

void log(char* msg) {
  Serial.println(msg);
}

void debug(String msg) {
  if (DEBUG) {
    Serial.println(msg);
  }
}

void boot() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(10);
  }
  sensors.begin();
  delay(10);
}

String get_temp() {
    sensors.requestTemperatures();
    return String(sensors.getTempCByIndex(0));
}

void setup() {
  Serial.begin(115200);
  Serial.println("back in business");
  Serial.println();
  boot();
  Serial.println("Booted");

  current_temp = get_temp();
  Serial.println(current_temp);
  publish_temp(current_temp);
  // 5min sleep
  Serial.println("going to sleep");
  ESP.deepSleep(5*60*1000000);
}

void publish_temp(String temp) {
  phant.add("temp", temp);
  http.begin(phant.url());
  Serial.println(phant.url());
  int httpCode = http.GET();
  if(httpCode > 0) {
      if(httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.println(payload);
      }
  } else {
    Serial.println("Failure on http payload");
  }
  http.end();

}

void loop() {
}

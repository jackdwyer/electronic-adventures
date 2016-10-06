#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266HTTPClient.h>
#include <Phant.h>

long day = 86400000; // 86400000 milliseconds in a day
long hour = 3600000; // 3600000 milliseconds in an hour
long minute = 60000; // 60000 milliseconds in a minute
long second =  1000; // 1000 milliseconds in a second


SSD1306  display(0x3c, D3, D5);

// pin 4, but GPIO 2
OneWire ds(2);
DallasTemperature sensors(&ds);

Phant phant("data.sparkfun.com", "", "");

const char* ssid     = "";
const char* password = "";


void log(char* msg) {
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 52, msg);
}

void debug(String msg) {
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 52, msg);
}

String uptime(){
    long timeNow = millis();
     
    String days = String((timeNow / day));                                //number of days
    String hours = String((timeNow % day) / hour);                       //the remainder from days division (in milliseconds) divided by hours, this gives the full hours
    String minutes = String((((timeNow % day) % hour) / minute));         //and so on...
    String seconds = String((((timeNow % day) % hour) % minute) / second);
    
    String msg = "";
    Serial.println(days.length());
    if (days.length() == 1) {
        msg += "0" + days + ":";
    } else {
        msg += days + ":";
    }
    if (hours.length() == 1) {
        msg += "0" + hours + ":";
    } else {
        msg += hours + ":";
    }
    if (minutes.length() == 1) {
        msg += "0" + minutes + ":";
    } else {
        msg += minutes + ":";
    }
    if (seconds.length() == 1) {
        msg += "0" + seconds;
    } else {
        msg += seconds;
    }

    // Serial.println(days + ":" + hours + ":" + minutes + ":" + seconds);
    // Serial.println(uptime());
    return msg;
     // digital clock display of current time
    // Serial.print(days,DEC);  
    // Serial.print(printDigits(hours));
    // printDigits(minutes);
    // printDigits(seconds);
    // Serial.println();  
}


void draw_boot_screen() {
    log("in draw_boot_screen");

    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "booting...");
    display.display();
    display.drawString(0, 10, "connecting...");
    display.display();
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        display.clear();
        display.drawString(0, 10, "trying..");
        delay(10);
    }
    display.clear();
    display.drawString(0, 20, "[01] connected!");
    display.display();
    delay(10);
    display.drawString(0, 20, "[02] [CALL] sensors.begin()");
    sensors.begin();
    delay(10);
    display.drawString(0, 20, "[02] [DONE] sensors.begin()");
}

String get_temp() {
    sensors.requestTemperatures();
    return String(sensors.getTempCByIndex(0));
}

void draw_info_screen() {
    // log("in draw_info_screen");
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    // display.drawString(0, 0, "IP:");
    display.drawString(0, 0, WiFi.localIP().toString());
    display.drawString(0, 12, "STATUS:");
    display.drawString(46, 12, String(WiFi.status()));
    // display.drawString(0, 24, String(millis(), DEC));

    display.setFont(ArialMT_Plain_16);
    // sensors.requestTemperatures();
    display.drawString(0, 30, get_temp()); // String(sensors.getTempCByIndex(0)));
    display.drawString(44, 30, ".C");
    Serial.println(WiFi.localIP());
}

 void draw_info() {
   display.clear();
   // log("in draw_info");
   draw_info_screen();
   display.display();
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  display.init();

  // display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  // WiFi.mode(WIFI_STA);
  draw_boot_screen();

}

void loop() {
    HTTPClient http;
    display.clear();
    draw_info();
    debug(uptime());
    display.display();
    phant.add("temp", get_temp());
    // Serial.println(phant.url());
    // Serial.println(phant.get());
    http.begin(phant.url());
    int httpCode = http.GET();
    if(httpCode > 0) {
        if(httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            Serial.println(payload);
        }
    }
    http.end();
    delay(10000);
}

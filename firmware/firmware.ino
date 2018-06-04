#define TIME_UPDATE_PERIOD_S 25
#define SERVER_PORT 80

#define LIGHT_PIN 12
#define PUMP_PIN 13
#define LED_PIN 13
#define BUTTON_PIN 16

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ArduinoJson.h>
#include "FS.h"
#include <Ticker.h>

Ticker handler;
Ticker wateringHandler;
Ticker buttonHandler;

int wateringPeriodM = 1000;
int wateringLengthS = 10;
int timeZone = 2;
int startLightH = 8;
int startLightM = 0;
int endLightH = 18;
int endLightM = 0;
const char *pointName = "Home";
const char *pointPass = "senyasonyakev";
int  pointEnc = 1;
bool isConnected = false;
int workingMode = 1;
int SubstratHumidity = 0;
int SubstratCriticalHumidity = 10;

int hours = 0;
int minutes = 0;
bool timeUpdated = false;
int currentWateringLength = 0;
bool isWateringNow = false;
int lastWateringHours = 0;
int lastWateringMinutes = 0;
bool isButtonPressed = false;

ESP8266WebServer server ( SERVER_PORT );

// By default 'time.nist.gov' is used with 60 seconds update interval and
// no offset
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 20000);

// You can specify the time server pool and the offset, (in seconds)
// additionaly you can specify the update interval (in milliseconds).
// NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

void setup ( void ) {

  pinMode(LIGHT_PIN, OUTPUT);
  digitalWrite(LIGHT_PIN, HIGH);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
  
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH);

  Serial.begin ( 115200 );
  Serial.println ( "mounting FS" );
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  Serial.println(wateringPeriodM);
  loadConfig();
  Serial.println(wateringPeriodM);

  switch (workingMode) {
    case 1: ConnectToWifi();
      break;
    case 2: StartAP();
      break;
    default: break;
  }

  timeClient.begin();

  handler.attach(TIME_UPDATE_PERIOD_S, handleHandler);

  ConfigAndStartServer();
}

void loop ( void ) {
  if (digitalRead(BUTTON_PIN) == 0) {
    if (!isButtonPressed) {
       Serial.println("button pressed");
      isButtonPressed = true;
      buttonHandler.once(10, checkButton);
    }
  } else {
    if (isButtonPressed) {
        Serial.println("button released");
    isButtonPressed = false;
    }
     
  }
  server.handleClient();
  updateTime();
}



void checkButton() {
  if (digitalRead(BUTTON_PIN) == 0) {
    if (!isButtonPressed) {
      workingMode++;
      Serial.println("Changing mode");
      if (workingMode > 2) {
        workingMode = 1;
      }
      saveConfig();
    }
  } else {
    isButtonPressed = false;
  }
}


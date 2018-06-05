#define TIME_UPDATE_PERIOD_S 30
#define SERVER_PORT 80

#define LIGHT_PIN 4
#define PUMP_PIN 5
#define LED_PIN 1
#define BUTTON_PIN 14

#define DEBUG_NTPClient

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include "FS.h"
#include <Ticker.h>
#include <TimeLib.h>
#include <NtpClientLib.h>

Ticker handler;
Ticker wateringHandler;
Ticker ntpHandler;

unsigned long lastWateringTime;

int wateringPeriodM = 1000;
int wateringLengthS = 10;
int timeZone = 2;
int startLightH = 8;
int startLightM = 0;
int endLightH = 18;
int endLightM = 0;
String ssid = "Home";
String pwd = "senyasonyakev";
int  pointEnc = 1;
bool isConnected = false;
int workingMode = 1;
int SubstratHumidity = 0;
int SubstratCriticalHumidity = 10;
String toPointConnected = "";

int currentWateringLength = 0;
bool isWateringNow = false;
bool isButtonPressed = false;

ESP8266WebServer server ( SERVER_PORT );

void setup ( void ) {
  pinMode(LIGHT_PIN, OUTPUT);
  digitalWrite(LIGHT_PIN, HIGH);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, HIGH);

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

  Serial.print("workingMode: ");
  Serial.println(workingMode);
  switch (workingMode) {
    case 1: ConnectToWifi();
      break;
    case 2: StartAP();
      break;
    default: break;
  }

  //timeClient.begin();
  ConfigNTP();
  handler.attach(TIME_UPDATE_PERIOD_S, handleHandler);

  ConfigAndStartServer();
}


int buttonTime = 0;
void loop ( void ) {
  server.handleClient();
  handleButton();
}

void handleButton() {
  if (digitalRead(BUTTON_PIN) == 0) {
    if (!isButtonPressed) {
      Serial.println("button pressed");
      isButtonPressed = true;
    } else {
      delay(1);
      buttonTime++;
      if (buttonTime % 10000 == 0) {
        workingMode++;
        if (workingMode > 2) {
          workingMode = 1;
        }
        Serial.print("Working mode changed to ");
        Serial.println(workingMode);
        saveConfig();
        //        Serial.print("Button Pressed for: ");
        //        Serial.println(buttonTime / 1000);
        //        Serial.println(digitalRead(BUTTON_PIN));
      }
    }
  } else {
    if (isButtonPressed) {
      Serial.println("button released");
      buttonTime = 0;
      isButtonPressed = false;
    }
  }
}






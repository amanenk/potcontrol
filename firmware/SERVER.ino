void getParameters() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["toPointConnected"] = WiFi.SSID();
  root["wateringPeriodM"] = wateringPeriodM;
  root["wateringLengthS"] = wateringLengthS;
  root["startLightH"] = startLightH;
  root["startLightM"] = startLightM;
  root["endLightH"] = endLightH;
  root["endLightM"] = endLightM;
  root["isConnected"] = isConnected;
  root["SubstratHudimity"] = SubstratHumidity;
  root["SubstratCriticalHumidity"] = SubstratCriticalHumidity;
  root["timeZone"] = timeZone;
  root.prettyPrintTo(Serial);
  char buffer1[1200];
  root.printTo(buffer1, sizeof(buffer1));
  server.send ( 200, "application/json",  String(buffer1));
}

void setParameters() {
  if (server.method() == HTTP_POST) {
    Serial.println("IT IS POST REQUEST");

    Serial.println(server.arg("plain"));
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(server.arg("plain"));


    wateringPeriodM = root["wateringPeriodM"];
    wateringLengthS = root["wateringLengthS"];
    startLightH = root["startLightH"];
    startLightM = root["startLightM"];
    endLightH = root["endLightH"];
    endLightM = root["endLightM"];
    timeZone  = root["timeZone"];
    SubstratCriticalHumidity = root["SubstratCriticalHumidity"];
    saveConfig();
    server.send ( 200, "application/json",  "OK");

  } else {
    Serial.println(server.arg("plain"));
    server.send ( 502, "application/json",  "it must be post reqest here");
  }
}

void setWifiParameters() {
  if (server.method() == HTTP_POST) {
    Serial.println("IT IS POST REQUEST");

    Serial.println(server.arg("plain"));
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(server.arg("plain"));


    pointName = root["pointName"];
    pointPass = root["pointPass"];
    pointEnc = root["pointEnc"];

    saveConfig();
    server.send ( 200, "application/json",  "OK");

  } else {
    Serial.println(server.arg("plain"));
    server.send ( 502, "application/json",  "it must be post reqest here");
  }
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
}

void ConfigAndStartServer(){
  server.on ( "/", getParameters);
  server.on ( "/getwifi", getVisiblePoints );
  server.on ( "/setwifi", setWifiParameters );
  server.on ( "/getparams", getParameters );
  server.on ( "/setparams", setParameters );
  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println ( "HTTP server started" );
}


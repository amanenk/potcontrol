void getVisiblePoints() {
  DynamicJsonBuffer jsonBuffer;
  JsonArray& root = jsonBuffer.createArray();
  Serial.print(F("Scanning..."));
  int n = WiFi.scanNetworks(); // WiFi.scanNetworks will return the number of networks found
  Serial.println(F("scan done"));
  if (n == 0)
    Serial.println(F("no networks found"));
  else
  {
    Serial.print(n); Serial.print(F(" network"));
    if (n > 1) Serial.print('s');
    Serial.println(F(" found:"));
    for (int i = 0; i < n; ++i)
    {
      JsonObject& point = root.createNestedObject();
      point["SSID"] = WiFi.SSID(i);
      point["RSSI"] = WiFi.RSSI(i);
      point["ENC"] = WiFi.encryptionType(i);
      Serial.println("point added to json");
    }
  }
  root.prettyPrintTo(Serial);
  char buffer1[1200];
  root.printTo(buffer1, sizeof(buffer1));
  server.send ( 200, "application/json",  String(buffer1));
}

void ConnectToWifi() {


  const char *ssid = "Home";
  const char *password = "senyasonyakev";

  WiFi.begin ( ssid, password );
  Serial.println ( "" );
  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }



  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );
}

void StartAP() {

  const char *APssid = "PLANT_SITTER_" + ESP.getFlashChipId();
  WiFi.softAP(APssid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}


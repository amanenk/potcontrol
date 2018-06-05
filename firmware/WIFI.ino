void getVisiblePoints() {
  DynamicJsonBuffer jsonBuffer;
  JsonArray& root = jsonBuffer.createArray();

  Serial.println("NEW REQUEST");

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
    }
  }
  root.prettyPrintTo(Serial);
  char buffer1[1200];
  root.printTo(buffer1, sizeof(buffer1));
  server.send ( 200, "application/json",  String(buffer1));
}

void ConnectToWifi() {

  if (workingMode == 1) {
    WiFi.mode(WIFI_STA);
  }

  Serial.println ( "connect" );
  if (pointEnc == 7) {
    WiFi.begin ( ssid.c_str());
  }  else {
    WiFi.begin ( ssid.c_str(), pwd.c_str() );
  }

  Serial.println ( "" );
  // Wait for connection
  int i = 0;
  while ( WiFi.status() != WL_CONNECTED && i < 30 ) {
    delay ( 500 );
    Serial.print ( "." );
    i++;
  }

  if ( WiFi.status() == WL_CONNECTED) {
    Serial.println ( "" );
    Serial.print ( "Connected to " );
    Serial.println ( WiFi.SSID() );
    toPointConnected = WiFi.SSID();
    Serial.print ( "IP address: " );
    Serial.println ( WiFi.localIP() );
  } else {
    Serial.println ( "" );
    Serial.print ( "Can't connetc to " );
    Serial.println ( WiFi.SSID());
    toPointConnected = "";
  }
  StartNTP();
}

void StartAP() {
  WiFi.mode(WIFI_AP_STA);
  Serial.println ( "AP" );
  const char *APssid = "PLANT@SITTER";
  WiFi.softAP(APssid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  StartNTP();
}

void StartNTP() {
  NTP.begin("europe.pool.ntp.org", 1, true);
  NTP.setInterval(63);
  NTP.setTimeZone(timeZone);
}

void ConfigNTP() {
  NTP.onNTPSyncEvent([](NTPSyncEvent_t ntpEvent) {
    if (ntpEvent) {
      Serial.print("Time Sync error: ");
      if (ntpEvent == noResponse)
        Serial.println("NTP server not reachable");
      else if (ntpEvent == invalidAddress)
        Serial.println("Invalid NTP server address");
    }
    else {
      Serial.print("Got NTP time: ");
      Serial.print(NTP.getTimeDateString()); Serial.print(" ");

    }
  });
}


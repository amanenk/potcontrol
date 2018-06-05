void handleHandler() {
  Serial.print(NTP.getTimeDateString()); Serial.print(" ");
  Serial.print(NTP.isSummerTime() ? "Summer Time. " : "Winter Time. ");
  Serial.print("WiFi is ");
  Serial.print(WiFi.isConnected() ? "connected" : "not connected"); Serial.print(". ");
  Serial.print("Uptime: ");
  Serial.print(NTP.getUptimeString()); Serial.print(" since ");
  Serial.println(NTP.getTimeDateString(NTP.getFirstSync()).c_str());
  // digital clock display of the time
  handleLight();
  updateWatering();
}

void handleWatering()  {
  isWateringNow = false;
  //turn off watering pin
  digitalWrite(PUMP_PIN, HIGH);
  Serial.print("Turning off Pump ");
}

void updateWatering() {
  bool isTimeToWater = false;
  if (!isWateringNow) {
    unsigned long delta = wateringPeriodM * 60;
    Serial.print("Last Watering time:");
    prettyPtintTime(lastWateringTime);
    Serial.print("Now:");
    prettyPtintTime(now());
    Serial.print("delta:");
    Serial.println(delta);
    Serial.print("Last Watering time mills:");
    Serial.print(lastWateringTime);
    Serial.print("Now in mills:");
    Serial.print(now());
    Serial.print("delta now :");
    Serial.println(now() - lastWateringTime);


    if (((now() - lastWateringTime) >= delta) && lastWateringTime != now()) {
      Serial.print("need to water");
      lastWateringTime = now();
      isTimeToWater = true;
      saveConfig();
      //trouble will bee if time is not sincronized
    }
  } else {

    Serial.println("It is watering now ");
    currentWateringLength += TIME_UPDATE_PERIOD_S;
    if (currentWateringLength >= wateringLengthS) {
      currentWateringLength = 0;
      if (digitalRead(PUMP_PIN) == 1) {
        handleWatering();
      }
    }
  }
  if (isTimeToWater) {
    //watering stuff
    isWateringNow = true;
    //turn on watering pin
    digitalWrite(PUMP_PIN, LOW);
    Serial.println("Turning on Pump");
    wateringHandler.once(wateringLengthS, handleWatering);
  }
  Serial.println("");
}

void   handleLight() {

  // digital clock display of the time


  int startM = startLightH * 60 + startLightM;
  int endM = endLightH * 60 + endLightM;
  int currM = hour() * 60 + minute();

  Serial.print("Start light: ");
  Serial.print(startM);
  Serial.println();

  Serial.print("Curr light: ");
  Serial.print(currM);
  Serial.println();

  Serial.print("End light: ");
  Serial.print(endM);
  Serial.println();

  if (currM >= startM && currM < endM) {
    Serial.println("Turn the lihgt ON");
    if (digitalRead(LIGHT_PIN) == 1) {
      digitalWrite(LIGHT_PIN, LOW);
    }
  } else {
    Serial.println("Turn the lihtt OFF");
    if (digitalRead(LIGHT_PIN) == 0) {
      digitalWrite(LIGHT_PIN, HIGH);
    }
  }
}

void prettyPtintTime(unsigned long t ) {
  // digital clock display of the time
  Serial.print(hour( t));
  Serial.print(":");
  Serial.print(minute( t));
  Serial.print(":");
  Serial.print(second( t));
  Serial.print(" ");
  Serial.print(day( t));
  Serial.print("/");
  Serial.print(month( t));
  Serial.print("/");
  Serial.print(year( t));
  Serial.println();
}


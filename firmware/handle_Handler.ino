void handleHandler() {
  timeUpdated = false;
}

void handleWatering() {
  isWateringNow = false;
  //turn off watering pin
  digitalWrite(PUMP_PIN, LOW);
  Serial.print("Turning off Pump ");
}

void updateTime() {
  if (!timeUpdated) {
    timeClient.update();
    hours = timeClient.getHours() + timeZone;
    if (hours > 23) {
      hours -= 24;
    }
    minutes = timeClient.getMinutes();
    Serial.print(hours);
    Serial.print(":");
    Serial.println(minutes);
    handleJobs();
    updateWatering();
    timeUpdated = true;
  }
}

void updateWatering() {
  bool isTimeToWater = false;
  if (!isWateringNow) {
    int wateringHourP = wateringPeriodM / 60;
    int WateringMinuteP = wateringPeriodM % 60;
    Serial.println(wateringHourP);
    Serial.println(WateringMinuteP);
    int wateringHour = 0;
    int WateringMinute = 0;
    Serial.print("Handling watering");
    int i = 0;
    while (wateringHour < 24) {
      i++;
      if (i > 150) {
        break;
      }
      Serial.print("|");
      Serial.print(wateringHour);
      Serial.print(":");
      Serial.print(WateringMinute);
      if (wateringHour == hours && WateringMinute == minutes && WateringMinute != lastWateringMinutes && lastWateringHours != wateringHour) {
        lastWateringMinutes = WateringMinute;
        lastWateringHours = wateringHour;
        isTimeToWater = true;
        break;
      } else {
        wateringHour += wateringHourP;
        WateringMinute += WateringMinuteP;
        if (WateringMinute >= 60) {
          WateringMinute = WateringMinute - 60;
          wateringHour++;
        }
      }
    }
  } else {
    currentWateringLength += TIME_UPDATE_PERIOD_S;
    if (currentWateringLength >= wateringLengthS) {
      wateringLengthS = 0;
      handleWatering();
    }
  }
  if (isTimeToWater) {
    //watering stuff
    isWateringNow = true;
    //turn on watering pin

    digitalWrite(PUMP_PIN, HIGH);
    Serial.println("Turning on Pump");
    wateringHandler.once(wateringLengthS, handleWatering);
  }
  Serial.println("");
}


void   handleJobs() {
  Serial.println((hours >= startLightH && minutes >= startLightM && hours <= endLightH && minutes < endLightM));
  if (hours >= startLightH && minutes >= startLightM && hours <= endLightH && minutes < endLightM) {
    Serial.println("Turn the lihtt ON");
    if (digitalRead(LIGHT_PIN) == 0) {
      digitalWrite(LIGHT_PIN, HIGH);
    }
  } else {
    Serial.println("Turn the lihtt OFF");
    if (digitalRead(LIGHT_PIN) == 1) {
      digitalWrite(LIGHT_PIN, LOW);
    }
  }

}


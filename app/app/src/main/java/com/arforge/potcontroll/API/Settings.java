package com.arforge.potcontroll.API;


import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class Settings {

    @SerializedName("toPointConnected")
    @Expose
    private String toPointConnected;
    @SerializedName("wateringPeriodM")
    @Expose
    private Integer wateringPeriodM;
    @SerializedName("wateringLengthS")
    @Expose
    private Integer wateringLengthS;
    @SerializedName("startLightH")
    @Expose
    private Integer startLightH;
    @SerializedName("startLightM")
    @Expose
    private Integer startLightM;
    @SerializedName("endLightH")
    @Expose
    private Integer endLightH;
    @SerializedName("endLightM")
    @Expose
    private Integer endLightM;
    @SerializedName("isConnected")
    @Expose
    private Boolean isConnected;
    @SerializedName("SubstratHudimity")
    @Expose
    private Integer substratHudimity;
    @SerializedName("SubstratCriticalHumidity")
    @Expose
    private Integer substratCriticalHumidity;
    @SerializedName("timeZone")
    @Expose
    private Integer timeZone;
    @SerializedName("lastWateringTime")
    @Expose
    private long lastWateringTime;

    public String getToPointConnected() {
        return toPointConnected;
    }

    public void setToPointConnected(String toPointConnected) {
        this.toPointConnected = toPointConnected;
    }

    public Integer getWateringPeriodM() {
        return wateringPeriodM;
    }

    public void setWateringPeriodM(Integer wateringPeriodM) {
        this.wateringPeriodM = wateringPeriodM;
    }

    public Integer getWateringLengthS() {
        return wateringLengthS;
    }

    public void setWateringLengthS(Integer wateringLengthS) {
        this.wateringLengthS = wateringLengthS;
    }

    public Integer getStartLightH() {
        return startLightH;
    }

    public void setStartLightH(Integer startLightH) {
        this.startLightH = startLightH;
    }

    public Integer getStartLightM() {
        return startLightM;
    }

    public void setStartLightM(Integer startLightM) {
        this.startLightM = startLightM;
    }

    public Integer getEndLightH() {
        return endLightH;
    }

    public void setEndLightH(Integer endLightH) {
        this.endLightH = endLightH;
    }

    public Integer getEndLightM() {
        return endLightM;
    }

    public void setEndLightM(Integer endLightM) {
        this.endLightM = endLightM;
    }

    public Boolean getIsConnected() {
        return isConnected;
    }

    public void setIsConnected(Boolean isConnected) {
        this.isConnected = isConnected;
    }

    public Integer getSubstratHudimity() {
        return substratHudimity;
    }

    public void setSubstratHudimity(Integer substratHudimity) {
        this.substratHudimity = substratHudimity;
    }

    public Integer getSubstratCriticalHumidity() {
        return substratCriticalHumidity;
    }

    public void setSubstratCriticalHumidity(Integer substratCriticalHumidity) {
        this.substratCriticalHumidity = substratCriticalHumidity;
    }

    public Integer getTimeZone() {
        return timeZone;
    }

    public void setTimeZone(Integer timeZone) {
        this.timeZone = timeZone;
    }

    public long getLastWateringTime() {
        return lastWateringTime;
    }

    public void setLastWateringTime(long lastWateringDay) {
        this.lastWateringTime = lastWateringDay;
    }

}
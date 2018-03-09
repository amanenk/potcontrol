package com.arforge.potcontroll.API;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class WiFiPoint {

    @SerializedName("SSID")
    @Expose
    private String sSID;
    @SerializedName("RSSI")
    @Expose
    private Integer rSSI;
    @SerializedName("ENC")
    @Expose
    private Integer eNC;

    public String getSSID() {
        return sSID;
    }

    public void setSSID(String sSID) {
        this.sSID = sSID;
    }

    public Integer getRSSI() {
        return rSSI;
    }

    public void setRSSI(Integer rSSI) {
        this.rSSI = rSSI;
    }

    public Integer getENC() {
        return eNC;
    }

    public void setENC(Integer eNC) {
        this.eNC = eNC;
    }

}

package com.arforge.potcontroll.API;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class WiFiSettings {

    @SerializedName("pointName")
    @Expose
    private String pointName;
    @SerializedName("pointPass")
    @Expose
    private String pointPass;
    @SerializedName("pointEnc")
    @Expose
    private Integer pointEnc;

    public String getPointName() {
        return pointName;
    }

    public void setPointName(String pointName) {
        this.pointName = pointName;
    }

    public String getPointPass() {
        return pointPass;
    }

    public void setPointPass(String pointPass) {
        this.pointPass = pointPass;
    }

    public Integer getPointEnc() {
        return pointEnc;
    }

    public void setPointEnc(Integer pointEnc) {
        this.pointEnc = pointEnc;
    }

}

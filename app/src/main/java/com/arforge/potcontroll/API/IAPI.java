package com.arforge.potcontroll.API;

import java.util.List;

import okhttp3.ResponseBody;
import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.GET;
import retrofit2.http.Headers;
import retrofit2.http.POST;


/**
 * Created by Owner on 2017-04-23 in PlantSitter
 */

public interface IAPI {
    // Requests method and URL specified in the annotation
    // Callback for the parsed response is the last parameter
    @Headers({"Content-Type: application/json"})
    @GET("/getparams")
    Call<Settings> getAllSettings();

    @Headers({"Content-Type: application/json"})
    @GET("/getwifi")
    Call<List<WiFiPoint>> getVisivleWifi();

    @Headers({"Content-Type: application/json"})
    @POST("/setwifi")
    Call<ResponseBody> setWifiSettings(@Body WiFiSettings body);

    @Headers({"Content-Type: application/json"})
    @POST("/setparams")
    Call<ResponseBody> setAllSettings(@Body Settings body);

}

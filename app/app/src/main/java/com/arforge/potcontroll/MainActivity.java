package com.arforge.potcontroll;

import android.app.TimePickerDialog;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.util.Patterns;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextClock;
import android.widget.TimePicker;
import android.widget.Toast;

import com.arforge.potcontroll.API.IAPI;
import com.arforge.potcontroll.API.Settings;
import com.arforge.potcontroll.API.WiFiPoint;
import com.arforge.potcontroll.API.WiFiSettings;

import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import okhttp3.OkHttpClient;
import okhttp3.logging.HttpLoggingInterceptor;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class MainActivity extends AppCompatActivity {

    public String API_BASE_URL = "http://192.168.31.42";
    static IAPI apiService;
    Settings setn;
    EditText ipText;

    Button butGet;
    Button butSet;
    TextClock lastwatering;
    TextClock beginLight;
    TextClock endLight;
    EditText period;
    EditText length;
    private String TAG = "MainActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        butGet = (Button) findViewById(R.id.buttonGet);
        butSet = (Button) findViewById(R.id.buttonSet);
        lastwatering = (TextClock) findViewById(R.id.textClock);
        beginLight = (TextClock) findViewById(R.id.beginClock);
        endLight = (TextClock) findViewById(R.id.endClock);
        period = (EditText) findViewById(R.id.period);

        period.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                if (setn != null) {
                    try {
                        setn.setWateringPeriodM(Integer.parseInt(s.toString()));
                    } catch (Exception ex) {
                        ex.printStackTrace();
                    }
                }
            }

            @Override
            public void afterTextChanged(Editable s) {

            }
        });
        length = (EditText) findViewById(R.id.length);

        length.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                if (setn != null) {
                    try {
                        setn.setWateringLengthS(Integer.parseInt(s.toString()));
                    } catch (Exception ex) {
                        ex.printStackTrace();
                    }
                }
            }

            @Override
            public void afterTextChanged(Editable s) {

            }
        });
        ipText = (EditText) findViewById(R.id.editText);

        ipText.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                API_BASE_URL = "http://" + s;
                savePerfs();
                createAPI();
            }

            @Override
            public void afterTextChanged(Editable s) {

            }
        });

        getPerfs();

        butGet.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getAllSettingss();
            }
        });

        butSet.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (setn != null) {
                    setAllSettings(setn);
                    Log.d(TAG, "sent");
                }
            }
        });

        beginLight.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                Calendar mcurrentTime = Calendar.getInstance();
                int hour = mcurrentTime.get(Calendar.HOUR_OF_DAY);
                int minute = mcurrentTime.get(Calendar.MINUTE);
                TimePickerDialog mTimePicker;
                mTimePicker = new TimePickerDialog(MainActivity.this, new TimePickerDialog.OnTimeSetListener() {
                    @Override
                    public void onTimeSet(TimePicker timePicker, int selectedHour, int selectedMinute) {
                        beginLight.setText(selectedHour + ":" + selectedMinute);
                        setn.setStartLightH(selectedHour);
                        setn.setStartLightM(selectedMinute);
                    }
                }, hour, minute, true);//Yes 24 hour time
                mTimePicker.setTitle("Select Time");
                mTimePicker.show();

            }
        });

        endLight.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                Calendar mcurrentTime = Calendar.getInstance();
                int hour = mcurrentTime.get(Calendar.HOUR_OF_DAY);
                int minute = mcurrentTime.get(Calendar.MINUTE);
                TimePickerDialog mTimePicker;
                mTimePicker = new TimePickerDialog(MainActivity.this, new TimePickerDialog.OnTimeSetListener() {
                    @Override
                    public void onTimeSet(TimePicker timePicker, int selectedHour, int selectedMinute) {
                        endLight.setText(selectedHour + ":" + selectedMinute);
                        setn.setEndLightH(selectedHour);
                        setn.setEndLightM(selectedMinute);
                    }
                }, hour, minute, true);//Yes 24 hour time
                mTimePicker.setTitle("Select Time");
                mTimePicker.show();

            }
        });


        createAPI();

    }

    void createAPI() {
        if (Patterns.WEB_URL.matcher(API_BASE_URL).matches()) {
            HttpLoggingInterceptor interceptor = new HttpLoggingInterceptor();
            interceptor.setLevel(HttpLoggingInterceptor.Level.BODY);

            final OkHttpClient okHttpClient = new OkHttpClient.Builder()
                    .addInterceptor(interceptor)
                    .build();

            Retrofit retrofit = new Retrofit.Builder()
                    .baseUrl(API_BASE_URL)
                    .client(okHttpClient)
                    .addConverterFactory(GsonConverterFactory.create())
                    .build();

            apiService = retrofit.create(IAPI.class);
        }
    }

    public void getAllSettingss() {
        getAPIService().getAllSettings().enqueue(new Callback<Settings>() {
            @Override
            public void onResponse(Call<Settings> call, Response<Settings> response) {
                setn = response.body();
                Date dt = new Date(setn.getLastWateringTime() * 1000);
                dt.setMinutes(dt.getMinutes() + (dt.getTimezoneOffset()));
                beginLight.setText(setn.getStartLightH() + ":" + setn.getStartLightM());
                endLight.setText(setn.getEndLightH() + ":" + setn.getEndLightM());
                period.setText(setn.getWateringPeriodM().toString());
                length.setText(setn.getWateringLengthS().toString());
                lastwatering.setText(android.text.format.DateFormat.format("yyyy-MM-dd HH:mm:ss", dt));
            }

            @Override
            public void onFailure(Call<Settings> call, Throwable t) {
                Toast.makeText(MainActivity.this, "Error", Toast.LENGTH_SHORT).show();
            }
        });
    }

    public void getVisivleWifi() {
        getAPIService().getVisivleWifi().enqueue(new Callback<List<WiFiPoint>>() {
            @Override
            public void onResponse(Call<List<WiFiPoint>> call, Response<List<WiFiPoint>> response) {
                Toast.makeText(MainActivity.this, response.body().toString(), Toast.LENGTH_SHORT).show();
            }

            @Override
            public void onFailure(Call<List<WiFiPoint>> call, Throwable t) {
                Toast.makeText(MainActivity.this, t.toString(), Toast.LENGTH_SHORT).show();

            }
        });
    }

    public void setWifiSettings(WiFiSettings settings) {
        getAPIService().setWifiSettings(settings).enqueue(new Callback() {
            @Override
            public void onResponse(Call call, Response response) {
                Toast.makeText(MainActivity.this, response.message(), Toast.LENGTH_SHORT).show();

            }

            @Override
            public void onFailure(Call call, Throwable t) {
                Toast.makeText(MainActivity.this, t.toString(), Toast.LENGTH_SHORT).show();
            }
        });
    }

    public void setAllSettings(Settings settings) {
        getAPIService().setAllSettings(settings).enqueue(new Callback() {
            @Override
            public void onResponse(Call call, Response response) {
                Toast.makeText(MainActivity.this, response.body().toString(), Toast.LENGTH_SHORT).show();
            }

            @Override
            public void onFailure(Call call, Throwable t) {
                Toast.makeText(MainActivity.this, t.toString(), Toast.LENGTH_SHORT).show();
            }
        });
    }


    public static IAPI getAPIService() {
        return apiService;
    }

    private static boolean testIP(String ip) {
        Pattern p = Pattern.compile("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}");
        Matcher m = p.matcher(ip);
        return m.matches();
    }

    private void getPerfs() {
        Log.d(TAG, "Loading perferences");
        try {
            SharedPreferences mySettings = getPreferences(MODE_PRIVATE);
            if (!mySettings.getString("API_BASE_URL", "").equals("")) {
                API_BASE_URL = mySettings.getString("API_BASE_URL", "");
                ipText.setText(API_BASE_URL.substring(7));
            }

            Log.i(TAG, "Settings loaded");
        } catch (Exception e) {
            Log.e(TAG, e.getMessage());
        }
    }

    private void savePerfs() {
        Log.d(TAG, "Loading perferences");
        try {
            SharedPreferences mySettings = getPreferences(MODE_PRIVATE);
            SharedPreferences.Editor ed = mySettings.edit();
            ed.putString("API_BASE_URL", API_BASE_URL);
            ed.apply();

            Log.i(TAG, "Settings saved");
        } catch (Exception e) {
            Log.e(TAG, e.getMessage());
        }
    }
}

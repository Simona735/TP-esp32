
// funkcie na komunikaciu cez wifi a s firebase-om

#include <WiFi.h>
#include <HTTPClient.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include "wifi.h"
#include "settings.h"



WifiVals WifiV;

FirebaseData FBDATA;
FirebaseAuth FBAUTH;
FirebaseConfig FBCFG;

int wifiConnect(){
  if(WiFi.status() != WL_CONNECTED){
    WiFi.mode(WIFI_STA);
    WiFi.begin(TPCFG.sWifiSSID.c_str(), TPCFG.sWifiPassword.c_str());
    int i;
    for(i=0; i<WIFI_MAX_CONNECT_ATTEMPTS; i++){
      if(WiFi.status() == WL_CONNECTED){
        return 1;
      }
      delay(WIFI_RETRY_CON_DELAY);
    }
    return 0;
  }
  return 1;
}


String wifiHTTPGet(String url){
  if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      String outstr;

      http.begin(url.c_str());
      
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        outstr = http.getString();
      }
      else {
        http.end();
        outstr = "HTTP Error";
        
      }
      http.end();
      return outstr;
    }
    return "wifi not connected";
}

int FBInit(){
  FBCFG.api_key = TPCFG.sFBKey;
  FBCFG.database_url = TPCFG.sFBURL;
  FBCFG.token_status_callback = tokenStatusCallback;
  FBAUTH.user.email = TPCFG.sFBMail;
  FBAUTH.user.password = TPCFG.sFBPassword;
  return 1;
}

int FBConnect(){
  Firebase.begin(&FBCFG, &FBAUTH);
  Firebase.setDoubleDigits(5);

  return 1;
}

int FBStatus(){
  if(FBCFG.database_url == "-1"){
    return 2;
  }
  else if(!Firebase.ready()){
    return 1;
  }
  else{
    return 0;
  }
}

String FBLastError(){
  return FBDATA.errorReason();
}


bool FBSetBool(const char* path, bool input){
  return Firebase.RTDB.setBool(&FBDATA, path, input);
}

bool FBSetInt(const char* path, int input){
  return Firebase.RTDB.setInt(&FBDATA, path, input);
}

bool FBSetFloat(const char* path, float input){
  return Firebase.RTDB.setFloat(&FBDATA, path, input);
}

bool FBSetDouble(const char* path, double input){
  return Firebase.RTDB.setDouble(&FBDATA, path, input);
}

bool FBSetString(const char* path, String str){
  return Firebase.RTDB.setString(&FBDATA, path, str.c_str());
}

bool FBSetString(const char* path, const char* str){
  return Firebase.RTDB.setString(&FBDATA, path, str);
}

bool FBSetJson(const char* path, FirebaseJson json){
  return Firebase.RTDB.setJSON(&FBDATA, path, &json);
}

bool FBSetTimestampNow(const char* path){
  return Firebase.RTDB.setTimestamp(&FBDATA, path);
}


bool FBGetBool(const char* path, bool* target){
  return Firebase.RTDB.getBool(&FBDATA, path, target);
}

bool FBGetInt(const char* path, int* target){
  return Firebase.RTDB.getInt(&FBDATA, path, target);
}

bool FBGetFloat(const char* path, float* target){
  return Firebase.RTDB.getFloat(&FBDATA, path, target);
}

bool FBGetDouble(const char* path, double* target){
  return Firebase.RTDB.getDouble(&FBDATA, path, target);
}

bool FBGetString(const char* path, String* target){
  return Firebase.RTDB.getString(&FBDATA, path, target);
}

bool FBGetJson(const char* path, FirebaseJson* target){
  return Firebase.RTDB.getJSON(&FBDATA, path, target);
}


int sendNewMailNotif(){
	// TODO
	
	return 0;
}

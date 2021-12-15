
// funkcie na komunikaciu cez wifi a s firebase-om

#include <WiFi.h>
#include <HTTPClient.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include "wifi.h"
#include "settings.h"


#define WIFI_RETRY_CON_DELAY 200
#define WIFI_MAX_CONNECT_ATTEMPTS 20
#define FB_API_KEY "AIzaSyBnvv9CnwggizzoOVf7Pj3-IO_Tv9sMKlo"
#define FB_DATABASE_URL "https://timovy-projekt-97069-default-rtdb.europe-west1.firebasedatabase.app/" 
#define FB_USER_EMAIL "acoak@hhh.ck"
#define FB_USER_PASSWORD "999999"


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
  FBCFG.api_key = FB_API_KEY;
  FBCFG.database_url = FB_DATABASE_URL;
  FBCFG.token_status_callback = tokenStatusCallback;
  FBAUTH.user.email = FB_USER_EMAIL;
  FBAUTH.user.password = FB_USER_PASSWORD;
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
  return Firebase.RTDB.set(&FBDATA, path, &json);
}

int sendNewMailNotif(){
	// TODO
	
	return 0;
}

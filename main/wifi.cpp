
// funkcie na komunikaciu cez wifi a s firebase-om

#include <WiFi.h>
#include <HTTPClient.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include "wifi.h"
#include "settings.h"
#include "compileswitches.h"
#include "saveload.h"



WifiVals WifiV;

FirebaseData FBDATA;
FirebaseAuth FBAUTH;
FirebaseConfig FBCFG;

int wifiConnect(){
  serialDBGOut("pripajanie k wifi");
  WiFi.disconnect();
  delay(2000);
    WiFi.mode(WIFI_STA);
    WiFi.begin(TPCFG.sWifiSSID.c_str(), TPCFG.sWifiPassword.c_str());
    delay(2000);
    int i;
    for(i=0; i<WIFI_MAX_CONNECT_ATTEMPTS; i++){
      if(WiFi.status() == WL_CONNECTED){
        serialDBGOut("pripojenie wifi uspesne");
        return 1;
      }
      delay(WIFI_RETRY_CON_DELAY);
    }
    serialDBGOut("pripojenie k wifi zlyhalo");
    return 0;

}

int wifiConnect2(){
  serialDBGOut("pripajanie k wifi");
  if(WiFi.status() != WL_CONNECTED){
    WiFi.mode(WIFI_STA);
    WiFi.begin(TPCFG.sWifiSSID.c_str(), TPCFG.sWifiPassword.c_str());
    int i;
    for(i=0; i<WIFI_MAX_CONNECT_ATTEMPTS; i++){
      if(WiFi.status() == WL_CONNECTED){
        serialDBGOut("pripojenie wifi uspesne");
        return 1;
      }
      delay(WIFI_RETRY_CON_DELAY);
    }
    serialDBGOut("pripojenie k wifi zlyhalo");
    return 0;
  }
  serialDBGOut("pripojenie k wifi uz je");
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

int wifiDisconnect(){
  WiFi.disconnect();
  return 1;
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

int FBDisconnect(){
  Firebase.RTDB.end(&FBDATA);
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

bool FBPathExists(const char* path){
  return Firebase.RTDB.pathExisted(&FBDATA, path);
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
  serialDBGOut("odosielanie eventu: nova posta");
  if(!wifiConnect()){return 0;}
  FBInit();
  FBConnect();
  if(FBStatus()){return 0;}
  char path[MAX_PATH_LENGTH] = "/";
  strncat(path, TPCFG.sFBUser.c_str(), 40);
  strncat(path, "/", 40);
  strncat(path, TPCFG.sFBID.c_str(), 40);
  strncat(path, "/events/LastMsgTime", 40);
  FBSetTimestampNow(path);
  serialDBGOut(path);
  serialDBGOut(FBDATA.errorReason().c_str());
  memset(path, 0, MAX_PATH_LENGTH);
  strncat(path, "/", 40);
  strncat(path, TPCFG.sFBUser.c_str(), 40);
  strncat(path, "/", 40);
  strncat(path, TPCFG.sFBID.c_str(), 40);
  strncat(path, "/events/NewMail",40);
  FBSetBool(path, true);
  serialDBGOut(path);
  serialDBGOut(FBDATA.errorReason().c_str());
  FBDisconnect();
  wifiDisconnect();
  return 1;
}

int sendEmptyMailNotif(){
  serialDBGOut("odosielanie eventu: prazdna posta");
  if(!wifiConnect()){return 0;}
  FBInit();
  FBConnect();
  if(FBStatus()){return 0;}
  char path[MAX_PATH_LENGTH] = "/";
  strncat(path, TPCFG.sFBUser.c_str(), 40);
  strncat(path, "/", 40);
  strncat(path, TPCFG.sFBID.c_str(), 40);
  strncat(path, "/events/LastMsgTime", 40);
  FBSetTimestampNow(path);
  serialDBGOut(path);
  serialDBGOut(FBDATA.errorReason().c_str());
  memset(path, 0, MAX_PATH_LENGTH);
  strncat(path, "/", 40);
  strncat(path, TPCFG.sFBUser.c_str(), 40);
  strncat(path, "/", 40);
  strncat(path, TPCFG.sFBID.c_str(), 40);
  strncat(path, "/events/EmptyBox",40);
  FBSetBool(path, true);
  serialDBGOut(path);
  serialDBGOut(FBDATA.errorReason().c_str());
  FBDisconnect();
  wifiDisconnect();
  return 1;
}

int sendDeviceFatalError(char* cause){
  serialDBGOut("odosielanie eventu: fatal error");
  if(!wifiConnect()){return 0;}
  FBInit();
  FBConnect();
  if(FBStatus()){return 0;}
  char path[MAX_PATH_LENGTH] = "/";
  strncat(path, TPCFG.sFBUser.c_str(), 40);
  strncat(path, "/", 40);
  strncat(path, TPCFG.sFBID.c_str(), 40);
  strncat(path, "/events/LastMsgTime", 40);
  FBSetTimestampNow(path);
  serialDBGOut(path);
  serialDBGOut(FBDATA.errorReason().c_str());
  memset(path, 0, MAX_PATH_LENGTH);
  strncat(path, "/", 40);
  strncat(path, TPCFG.sFBUser.c_str(), 40);
  strncat(path, "/", 40);
  strncat(path, TPCFG.sFBID.c_str(), 40);
  strncat(path, "/events/FatalError",40);
  FBSetBool(path, true);
  serialDBGOut(path);
  serialDBGOut(FBDATA.errorReason().c_str());
  FBDisconnect();
  wifiDisconnect();
  return 1;
}

int fetchSettings(){
  serialDBGOut("stahovanie nastaveni z DB");
  if(!wifiConnect()){return 0;}
  FBInit();
  FBConnect();
  if(FBStatus()){return 0;}
  //int rst = 0;
  char path[MAX_PATH_LENGTH] = "/";
  strncat(path, TPCFG.sFBUser.c_str(), 40);
  strncat(path, "/", 40);
  strncat(path, TPCFG.sFBID.c_str(), 40);
  strncat(path, "/settings", 40);
  /*
  if (!FBPathExists(path)){
    serialDBGOut("nenasiel sa priecinok v DB -> factory reset!");
    serialDBGOut(path);
    serialDBGOut(FBDATA.errorReason().c_str());
    eraseSavedConfig();
    ESP.restart();
  }
  */
  strncat(path, "/", 40);
  char* path2 = path+strlen(path);
  
  /*
  strncat(path, "reset", 40);
  FBGetInt(path, &rst);
  if(rst == 1){
    serialDBGOut("vymazanie nastaveni!");
    eraseSavedConfig();
    ESP.restart();
  }
  memset (path2, 0, 10);
  */
  strncat(path, "UCI", 40);
  FBGetInt(path, &(TPCFG.iUltraCheckInterval));
  serialDBGOut(path);
  serialDBGOut(FBDATA.errorReason().c_str());
  memset (path2, 0, 10);
  strncat(path, "UEC", 40);
  FBGetInt(path, &(TPCFG.iUltraExtraChecks));
  serialDBGOut(path);
  serialDBGOut(FBDATA.errorReason().c_str());
  //memset (path2, 0, 10);
  //strncat(path, "UECI", 40);
  //FBGetInt(path, &(TPCFG.iUltraExtraChecksIntervalMS));
  memset (path2, 0, 10);
  strncat(path, "UT", 40);
  FBGetFloat(path, &(TPCFG.fUltraTolerance));
  serialDBGOut(path);
  serialDBGOut(FBDATA.errorReason().c_str());
  //memset (path2, 0, 10);

  if (strcmp(FBDATA.errorReason().c_str(), "path not exist") == 0){
    serialDBGOut("nenasiel sa priecinok v DB -> factory reset!");
    serialDBGOut(path);
    serialDBGOut(FBDATA.errorReason().c_str());
    FBDisconnect();
    wifiDisconnect();
    eraseSavedConfig();
    ESP.restart();
  }
  FBDisconnect();
  wifiDisconnect();
  return 1;
}

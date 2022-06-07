#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include <rom/rtc.h>


#define MAX_PATH_LENGTH 100


FirebaseData FBDATA;
FirebaseAuth FBAUTH;
FirebaseConfig FBCFG;

void setup(){
  Serial.begin(115200);
  delay(3000);
  Serial.println("start");
  WiFi.mode(WIFI_STA);
  Serial.println("pripajanie");
  WiFi.begin("abc", "123"); // SSID, heslo
  int i;
  for(i=0; i<30; i++){
    if(WiFi.status() == WL_CONNECTED){
      break;
    }
    Serial.println(i);
    delay(1000);
  }
  if(WiFi.status() != WL_CONNECTED){
        Serial.println("WiFi zlyhalo");
        return;
  }
  FBCFG.api_key = "AIzaSyBnvv9CnwggizzoOVf7Pj3-IO_Tv9sMKlo";
  FBCFG.database_url = "https://timovy-projekt-97069-default-rtdb.europe-west1.firebasedatabase.app/";
  FBCFG.token_status_callback = tokenStatusCallback;
  FBAUTH.user.email = "a@test.123";  // email FB
  FBAUTH.user.password = "heslo";  // heslo FB

  char mailboxID[] = "m123";   // ID schranky
  char userID[] = "xyz";   // ID usera
  
  Serial.println("odosielanie");
  Firebase.begin(&FBCFG, &FBAUTH);
  Serial.println(FBDATA.errorReason());
  Firebase.setDoubleDigits(5);

  char path[MAX_PATH_LENGTH] = "/";
  strncat(path, userID, 40);
  strncat(path, "/", 40);
  strncat(path, mailboxID, 40);
  strncat(path, "/events/LastMsgTime", 40);
  Serial.println(path);
  //FBSetTimestampNow(path);
  memset(path, 0, MAX_PATH_LENGTH);
  strncat(path, "/", 40);
  strncat(path, userID, 40);
  strncat(path, "/", 40);
  strncat(path, mailboxID, 40);
  strncat(path, "/events/NewMail",40);
  Serial.println(path);
  //FBSetBool(path, true);
  //Serial.println(FBDATA.errorReason());
  Serial.printf("Set bool... %s\n", Firebase.RTDB.setBool(&FBDATA, path, true) ? "ok" : FBDATA.errorReason().c_str());

  char path2[MAX_PATH_LENGTH] = "/";
  strncat(path2, userID, 40);
  strncat(path2, "/", 40);
  strncat(path2, mailboxID, 40);
  strncat(path2, "/settings", 40);
  Serial.println(path2);
  if (!Firebase.RTDB.pathExisted(&FBDATA, path2)){
    Serial.println("cesta neexistuje");
  }
  strncat(path2, "/", 40);
  char* path3 = path2+strlen(path2);
  strncat(path2, "UCI", 40);
  Serial.println(path2);
  //FBGetInt(path2, &(TPCFG.iUltraCheckInterval));
  Serial.printf("Get int... %s\n", Firebase.RTDB.getInt(&FBDATA, path2) ? String(FBDATA.to<int>()).c_str() : FBDATA.errorReason().c_str());
  memset (path3, 0, 10);
  strncat(path2, "UEC", 40);
  Serial.println(path2);
  //FBGetInt(path2, &(TPCFG.iUltraExtraChecks));
  Serial.printf("Get int... %s\n", Firebase.RTDB.getInt(&FBDATA, path2) ? String(FBDATA.to<int>()).c_str() : FBDATA.errorReason().c_str());
  memset (path3, 0, 10);
  strncat(path2, "UT", 40);
  Serial.println(path2);
  //FBGetFloat(path2, &(TPCFG.fUltraTolerance));
  Serial.printf("Get float... %s\n", Firebase.RTDB.getFloat(&FBDATA, path2) ? String(FBDATA.to<float>()).c_str() : FBDATA.errorReason().c_str());

  Serial.println("koniec");
  esp_deep_sleep(10000000);
}

void loop(){
  
}

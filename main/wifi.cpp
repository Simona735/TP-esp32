
// funkcie na komunikaciu cez wifi a s firebase-om

#include <WiFi.h>
#include <HTTPClient.h>
#include "wifi.h"
#include "settings.h"


#define WIFI_RETRY_CON_DELAY 200
#define WIFI_MAX_CONNECT_ATTEMPTS 20


WifiVals WifiV;

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





int sendNewMailNotif(){
	// TODO
	
	return 0;
}

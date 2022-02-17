
// nastavenia

#include "settings.h"



DeviceConfigs TPCFG;

   //predvolene hodnoty

void setDefaults(){
  TPCFG.iUltraCheckInterval = 7000000;          //doba spanku medzi kontrolou posty
  TPCFG.iUltraExtraChecks = 4;            //kolko krat sa pri detekcii posty kontroluje navyse na zabranenie falosnych poplachov
  TPCFG.iUltraExtraChecksIntervalMS = 500;      //cas medzi extra kontrolami
  TPCFG.fUltraTolerance = 0.1;              //tolerancia/citlivost ultrazvukoveho senzora, mensie znamena citlivejsie, minimum/default = 0
  
}

void setDefaultsWifi(){
  TPCFG.sWifiSSID = "-1";  // SSID (meno) WiFi
  TPCFG.sWifiPassword = "-1";  // heslo WiFi
}

void setDefaultsFB(){
  TPCFG.sFBKey = "-1";   //"AIzaSyBnvv9CnwggizzoOVf7Pj3-IO_Tv9sMKlo";
  TPCFG.sFBURL = "https://timovy-projekt-97069-default-rtdb.europe-west1.firebasedatabase.app/";
  TPCFG.sFBMail = "-1";   //"acoak@hhh.ck";
  TPCFG.sFBPassword = "-1";   //"999999";
  TPCFG.sFBID = "-1";
  TPCFG.sFBUser = "-1";
}

void setDummyCfg(String str){
  TPCFG.sWifiSSID = str;
  TPCFG.sWifiPassword = str;
  TPCFG.sFBKey = str;
  TPCFG.sFBMail = str;
  TPCFG.sFBPassword = str;
  TPCFG.sFBID = str;
  TPCFG.sFBUser = str;
}

int pinInit(){
  pinMode(TRIGPIN1, OUTPUT);
  pinMode(ECHOPIN1, INPUT);
  pinMode(TRIGPIN2, OUTPUT);
  pinMode(ECHOPIN2, INPUT);
  pinMode(TRIGPIN3, OUTPUT);
  pinMode(ECHOPIN3, INPUT);
  return 1;
}

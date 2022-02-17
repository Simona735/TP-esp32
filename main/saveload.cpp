
// ukladanie a nacitavanie dat, inicializacia po starte

#include <Arduino.h>
#include "saveload.h"
#include "settings.h"
#include <Preferences.h>
#include <nvs_flash.h>



Preferences NVS;

// nazvy hodnot (kluce) sa asi ukladaju do pamate tiez, tak su pouzite len kratke nazvy
// nazvy a hodnoty by bolo dobre dat ako #define v separatnom hlavickovom subore

int checkLoadedConfig(){
  if(
    TPCFG.iUltraCheckInterval == -1 ||
    TPCFG.iUltraExtraChecks == -1 ||
    TPCFG.iUltraExtraChecksIntervalMS == -1 ||
    TPCFG.fUltraTolerance == -1 ||
    TPCFG.sWifiSSID == "-1" ||
    TPCFG.sWifiPassword == "-1" ||
    TPCFG.sFBKey == "-1" ||
    TPCFG.sFBURL == "-1" ||
    TPCFG.sFBMail == "-1" ||
    TPCFG.sFBPassword == "-1" ||
    TPCFG.sFBID == "-1" ||
    TPCFG.sFBUser == "-1"
  ){
    return 0;
  }
  return 1;
}

int checkLoadedConfigCritical(){
  if(
    TPCFG.sWifiSSID == "-1" ||
    TPCFG.sWifiPassword == "-1" ||
    TPCFG.sFBKey == "-1" ||
    TPCFG.sFBURL == "-1" ||
    TPCFG.sFBMail == "-1" ||
    TPCFG.sFBPassword == "-1" ||
    TPCFG.sFBID == "-1" ||
    TPCFG.sFBUser == "-1"
  ){
    return 0;
  }
  return 1;
}

int checkLoadedConfigDiag(){
  int i=1;
  if(TPCFG.iUltraCheckInterval == -1){return i;} i++;
  if(TPCFG.iUltraExtraChecks == -1){return i;} i++;
  if(TPCFG.iUltraExtraChecksIntervalMS == -1){return i;} i++;
  if(TPCFG.fUltraTolerance == -1){return i;} i++;
  if(TPCFG.sWifiSSID == "-1"){return i;} i++;
  if(TPCFG.sWifiPassword == "-1"){return i;} i++;
  if(TPCFG.sFBKey == "-1"){return i;} i++;
  if(TPCFG.sFBURL == "-1"){return i;} i++;
  if(TPCFG.sFBMail == "-1"){return i;} i++;
  if(TPCFG.sFBPassword == "-1"){return i;} i++;
  if(TPCFG.sFBID == "-1"){return i;} i++;
  if(TPCFG.sFBUser == "-1"){return i;} i++;
  
  return 1000;
}

int loadConfig(){
	NVS.begin(NVSNAME, true);       // komentare su uuid pre prisluchajuce charakteristiky BLE
 
  TPCFG.iUltraCheckInterval = NVS.getInt("UCI", -1);                   // b81b8cac-7dce-4de0-a568-31b4a0b35816
  TPCFG.iUltraExtraChecks = NVS.getInt("UEC", -1);                       // 70510db2-616d-4598-82e8-4efe1f5ad71a
  TPCFG.iUltraExtraChecksIntervalMS = NVS.getInt("UECI", -1);            // 47865e5e-35f4-4119-a9dd-7cc0d3cd7c7e
  TPCFG.fUltraTolerance = NVS.getFloat("UT", -1);                       // c5fa8d3e-8216-4e1e-b75e-dcfbd7b01455
  TPCFG.sWifiSSID = NVS.getString("WS", "-1");                          // 5f08db8e-16da-4f74-8084-4fe93be581dc
  TPCFG.sWifiPassword = NVS.getString("WP", "-1");                       // eff0c5bd-426a-46de-904c-a3acbb847ba0
  TPCFG.sFBKey = NVS.getString("FBK", "-1");                            // 8ef7a713-b363-4831-86cb-b4ae3ab69498
  TPCFG.sFBURL = NVS.getString("FBU", "-1");                           // 9c8657d5-7343-4a1d-a5b8-f5acd689c763
  TPCFG.sFBMail = NVS.getString("FBM", "-1");                          // 88558ac3-0138-424c-9b4f-460757bcb6ec
  TPCFG.sFBPassword = NVS.getString("FBP", "-1");                     // 820aeda2-1d70-42b9-8831-3b515f88d9a2
  TPCFG.sFBID = NVS.getString("FBI", "-1");
  TPCFG.sFBUser = NVS.getString("FBU", "-1");
  
  NVS.end();
	return checkLoadedConfig();
}

int saveConfig(){
	NVS.begin(NVSNAME, false);
  
  NVS.putInt("UCI", TPCFG.iUltraCheckInterval);
  NVS.putInt("UEC", TPCFG.iUltraExtraChecks);
  NVS.putInt("UECI", TPCFG.iUltraExtraChecksIntervalMS);
  NVS.putFloat("UT", TPCFG.fUltraTolerance);
  NVS.putString("WS", TPCFG.sWifiSSID);
  NVS.putString("WP", TPCFG.sWifiPassword);
  NVS.putString("FBK", TPCFG.sFBKey);
  NVS.putString("FBU", TPCFG.sFBURL);
  NVS.putString("FBM", TPCFG.sFBMail);
  NVS.putString("FBP", TPCFG.sFBPassword);
  NVS.putString("FBI", TPCFG.sFBID);
  NVS.putString("FBU", TPCFG.sFBUser);
  
  NVS.end();
	return 1;
}

int eraseSavedConfig(){
  NVS.begin(NVSNAME, false);
  NVS.clear();
  NVS.end();
  return 1;
}

int wipeFlash(){
  nvs_flash_erase();
  nvs_flash_init();
  return 1;
}

void saveSingleInt(char* tag, char* val){
  NVS.begin(NVSNAME, false);
  NVS.putInt(tag, atoi(val));
  NVS.end();
}

void saveSingleFloat(char* tag, char* val){
  NVS.begin(NVSNAME, false);
  NVS.putFloat(tag, atof(val));
  NVS.end();
}

void saveSingleString(char* tag, char* val){
  NVS.begin(NVSNAME, false);
  NVS.putString(tag, String(val));
  NVS.end();
}

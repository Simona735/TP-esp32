
// ukladanie a nacitavanie dat, inicializacia po starte

#include <Arduino.h>
#include "saveload.h"
#include "settings.h"
#include <Preferences.h>
#include <nvs_flash.h>


#define NVSNAME "MAILBOX"

Preferences NVS;

// nazvy hodnot (kluce) sa asi ukladaju do pamate tiez, tak su pouzite len kratke nazvy
// nazvy a hodnoty by bolo dobre dat ako #define v separatnom hlavickovom subore

int checkLoadedConfig(){
  if(
    TPCFG.iUltraCheckIntervalMS == -1 ||
    TPCFG.iUltraExtraChecks == -1 ||
    TPCFG.iUltraExtraChecksIntervalMS == -1 ||
    TPCFG.iUltraTrigPin == -1 ||
    TPCFG.iUltraEchoPin == -1 ||
    TPCFG.fUltraTolerance == -1 ||
    TPCFG.sWifiSSID == "-1" ||
    TPCFG.sWifiPassword == "-1" ||
    TPCFG.sServerName == "-1"
  ){
    return 0;
  }
  return 1;
}

int loadConfig(){
	NVS.begin(NVSNAME, true);
 
  TPCFG.iUltraCheckIntervalMS = NVS.getInt("UCI", -1);
  TPCFG.iUltraExtraChecks = NVS.getInt("UEC", -1);
  TPCFG.iUltraExtraChecksIntervalMS = NVS.getInt("UECI", -1);
  TPCFG.iUltraTrigPin = NVS.getInt("UTP", -1);
  TPCFG.iUltraEchoPin = NVS.getInt("UEP", -1);
  TPCFG.fUltraTolerance = NVS.getFloat("UT", -1);
  TPCFG.sWifiSSID = NVS.getString("WS", "-1");
  TPCFG.sWifiPassword = NVS.getString("WP", "-1");
  TPCFG.sServerName = NVS.getString("SN", "-1");
  
  NVS.end();
	return checkLoadedConfig();
}

int saveConfig(){
	NVS.begin(NVSNAME, false);
  
  NVS.putInt("UCI", TPCFG.iUltraCheckIntervalMS);
  NVS.putInt("UEC", TPCFG.iUltraExtraChecks);
  NVS.putInt("UECI", TPCFG.iUltraExtraChecksIntervalMS);
  NVS.putInt("UTP", TPCFG.iUltraTrigPin);
  NVS.putInt("UEP", TPCFG.iUltraEchoPin);
  NVS.putFloat("UT", TPCFG.fUltraTolerance);
  NVS.putString("WS", TPCFG.sWifiSSID);
  NVS.putString("WP", TPCFG.sWifiPassword);
  NVS.putString("SN", TPCFG.sServerName);
  
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

int pinInit(){
  pinMode(TPCFG.iUltraTrigPin, OUTPUT);
  pinMode(TPCFG.iUltraEchoPin, INPUT);
  return 1;
}

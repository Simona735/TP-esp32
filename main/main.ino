
// jadro programu, obsahuje hlavny cyklus

#include <Arduino.h>
#include "bluetooth.h"
#include "saveload.h"
#include "settings.h"
#include "wifi.h"
#include "ultra.h"
#include <rom/rtc.h>
#include "compileswitches.h"





RTC_DATA_ATTR int iCycle;


void setup() {

  //-------- STARTUP
  
  

  DBG_SERIAL_BEGIN;

  
  
  if(rtc_get_reset_reason(0)==5)    // PREBUDENIE
  {
    serialDBGOut("PREBUDENIE");
    // ked sa zariadenie da do spankoveho rezimu, vymaze sa RAM. Premenne sa daju ulozit aj do specialnej pamate ktora toto prezije pomocou RTC_DATA_ATTR, ale tej je malo (asi 8kB) a nedaju sa pouzit String a podobne typy ktore dynamicky alokuju pamat pre hodnoty
    
    if(iCycle >= ADDITIONAL_TASKS_FREQUENCY){
      serialDBGOut("stahovanie nastaveni...");
      fetchSettings();
      reloadUltraConfigs();
      saveConfig();
      iCycle = 0;
    }
    else{
      serialDBGOut("ziadne stahovanie");
      iCycle++;
    }
    CHECK_MAIL();
    
  }
  else if(rtc_get_reset_reason(0)==1 || rtc_get_reset_reason(0)==12)     // STANDARDNY RESET (odpojenie napajania alebo softverovy reset)
  {
    serialDBGOut("STANDARDNY RESET");
    if(loadConfig()){  // nacita nastavenia
      serialDBGOut("kratke BLE");
      blueConfig(RESET_BLE_CONFIG_SECONDS);  // prijme nove nastavenia cez BLE
      fetchSettings();
      reloadUltraConfigs();
      saveConfig();
      iCycle = 0;
    }
    else{
      serialDBGOut("dlhe BLE");
      blueConfig(0);  // neexistuju ulozene nastavenia, nutne prijme nove nastavenia cez BLE, inak po case permanentne zaspi (do manualneho restartu)
      fetchSettings();
      reloadUltraConfigs();
      saveConfig();
      iCycle = 0;
    }
    serialDBGOut("Wifi:");
    serialDBGOut(TPCFG.sWifiSSID);
    serialDBGOut("Wifi heslo:");
    serialDBGOut(TPCFG.sWifiPassword);
    serialDBGOut("FB mail");
    serialDBGOut(TPCFG.sFBMail);
    serialDBGOut("FB heslo");
    serialDBGOut(TPCFG.sFBPassword);
    serialDBGOut("FB id schranky");
    serialDBGOut(TPCFG.sFBID);
    serialDBGOut("FB id usera");
    serialDBGOut(TPCFG.sFBUser);
    
    ultraSetEmpty();
    
  
  }
  else     // INY RESET
  {
    serialDBGOut("INY RESET");
    if(!loadConfig()){
      serialDBGOut("L1B : Chyba pri nacitani nastaveni");
      esp_deep_sleep(FATAL_ERROR_RESET_TIME);
    }
  
    
  }
  
  
  
  


  //loadConfig();  // TEST
  //reloadUltraConfigs();  // TEST
  serialDBGOut("ID:");
  serialDBGOut(TPCFG.sFBID);
  serialDBGOut("cyklus");
  serialDBGOut(iCycle);
  serialDBGOut("SSID:");
  serialDBGOut(TPCFG.sWifiSSID);
  serialDBGOut("zariadenie zaspalo");
  esp_deep_sleep(UltraV.iUltraCheckInterval);
  
}

void loop() {
  // ked sa zariadenie zobudi zo spanku, ide zase do setup(), takze loop() je prazdny
}

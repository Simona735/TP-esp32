
// jadro programu, obsahuje hlavny cyklus

#include <Arduino.h>
#include "bluetooth.h"
#include "saveload.h"
#include "settings.h"
#include "wifi.h"
#include "ultra.h"
#include <rom/rtc.h>


// zakomentovanie nasledujuceho riadku zkompiluje program bez debugovacich vypisov na seriovom vystupe pomocou makra serialDBOut("")
#define TPCOMPILEDEBUG

#ifdef TPCOMPILEDEBUG
#define serialDBOut(a); Serial.println(a);
#else
#define serialDBOut(a);
#endif


RTC_DATA_ATTR int iCycle;


void setup() {

  //-------- STARTUP
  
  

  #ifdef TPCOMPILEDEBUG
  Serial.begin(115200);
  #endif

  
  
  if(rtc_get_reset_reason(0)==5)    // PREBUDENIE
  {
    // ked sa zariadenie da do spankoveho rezimu, vymaze sa RAM. Premenne sa daju ulozit aj do specialnej pamate ktora toto prezije pomocou RTC_DATA_ATTR, ale tej je malo (asi 8kB) a nedaju sa pouzit String a podobne typy ktore dynamicky alokuju pamat pre hodnoty
    
    //checkU();   TODO !!!
    if(iCycle >= ADDITIONAL_TASKS_FREQUENCY){
      fetchSettings();
      iCycle = 0;
    }
    else{
      iCycle++;
    }
    
  }
  else if(rtc_get_reset_reason(0)==1 || rtc_get_reset_reason(0)==12)     // STANDARDNY RESET (odpojenie napajania alebo softverovy reset)
  {
    if(loadConfig()){  // nacita nastavenia
      blueConfig(RESET_BLE_CONFIG_SECONDS);  // prijme nove nastavenia cez BLE
    }
    else{
      blueConfig(0);  // neexistuju ulozene nastavenia, nutne prijme nove nastavenia cez BLE, inak po case permanentne zaspi (do manualneho restartu)
      iCycle = 0;
    }
    ultraSetEmpty();  // kalibracia
    
  
  }
  else     // INY RESET
  {
    if(!loadConfig()){
      serialDBOut("L1B : Chyba pri nacitani nastaveni");
      esp_deep_sleep(FATAL_ERROR_RESET_TIME);
    }
  
    
  }
  
  
  
  


  //FBSetFloat("/database/id_0/UI_sound/senzor_0", ultraMeasure());   // TEST

  
  serialDBOut("zariadenie zaspalo");
  esp_deep_sleep(TPCFG.iUltraCheckInterval); // trvanie spanku sa udava v mikrosekundach, premenna je milisekundy
  
}

void loop() {
  // ked sa zariadenie zobudi zo spanku, ide zase do setup(), takze loop() je prazdny
}


int checkU(){
  pinInit();
  if(ultraCheckAll()){
    int i;
    for(i=TPCFG.iUltraExtraChecks; i!=0; i--){
      delay(TPCFG.iUltraExtraChecksIntervalMS);
      if(!(ultraCheckAll())){
        i=2;
        break;
      }
    }
    if(i==0){
      if(!wifiConnect()){
        serialDBOut("pripojenie wifi zlyhalo");
        
      }

      if(!loadConfig()){
        serialDBOut("C1 : Chyba pri nacitani nastaveni");
        esp_deep_sleep(FATAL_ERROR_RESET_TIME);
      }

      FBInit();
      FBConnect();
      sendNewMailNotif();
    }
  }
}


// jadro programu, obsahuje hlavny cyklus

#include <Arduino.h>
#include "bluetooth.h"
#include "saveload.h"
#include "settings.h"
#include "wifi.h"
#include "ultra.h"
#include <rom/rtc.h>

#define TPCOMPILEDEBUG



void setup() {

  //-------- STARTUP
  
  

  #ifdef TPCOMPILEDEBUG
  Serial.begin(9600);
  #endif

  
  
  if(rtc_get_reset_reason(0)==5)    // PREBUDENIE
  {
    // ked sa zariadenie da do spankoveho rezimu, vymaze sa RAM. Premenne sa daju ulozit aj do specialnej pamate ktora toto prezije pomocou RTC_DATA_ATTR, ale tej je malo (asi 8kB) a nedaju sa pouzit String a podobne typy ktore dynamicky alokuju pamat pre hodnoty
    if(!loadConfig()){ // nacita nastavenia, ak nastane chyba, zastavi sa
      sendDeviceFatalError("L1A : Chyba pri nacitani nastaveni");
      esp_deep_sleep(FATAL_ERROR_RESET_TIME * 1000);
      
    }
  
    pinInit();     // mozno netreba ?
  
  }
  else if(rtc_get_reset_reason(0)==1)     // STANDARDNY RESET (odpojenie napajania)
  {
    loadConfig();  // nacita nastavenia
    blueConfig();  // prijme nove nastavenia cez BLE
    ultraSetEmpty();  // kalibracia
    
  
    pinInit();
  
    
  }
  else     // INY RESET
  {
    if(!loadConfig()){
      sendDeviceFatalError("L1B : Chyba pri nacitani nastaveni");
      esp_deep_sleep(FATAL_ERROR_RESET_TIME * 1000);
    }
  
    pinInit();
  
    
  }
  
  
  
  //---------- JADRO

  if(ultraCheck1() || ultraCheck2() || ultraCheck3()){
    int i;
    for(i=TPCFG.iUltraExtraChecks; i!=0; i--){
      delay(TPCFG.iUltraExtraChecksIntervalMS);
      if(!(ultraCheck1() || ultraCheck2() || ultraCheck3())){
        i=2;
        break;
      }
    }
    if(i==0){
      if(!wifiConnect()){
        //pripojenie wifi zlyhalo
        #ifdef TPCOMPILEDEBUG
        Serial.println("wifi connection error");
        #endif

    
      }

      FBInit();
      FBConnect();
      sendNewMailNotif();
    }
  }



  //FBSetFloat("/database/id_0/UI_sound/senzor_0", ultraMeasure());   // TEST

  
  
  esp_deep_sleep(TPCFG.iUltraCheckIntervalMS * 1000); // trvanie spanku sa udava v mikrosekundach, premenna je milisekundy
  
}

void loop() {
  // ked sa zariadenie zobudi zo spanku, ide zase do setup(), takze loop() je prazdny
}

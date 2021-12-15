
// jadro programu, obsahuje hlavny cyklus

#include <Arduino.h>
#include "bluetooth.h"
#include "saveload.h"
#include "settings.h"
#include "wifi.h"
#include "ultra.h"

#define COMPILEDEBUG 1



void setup() {

  setDefaults();

  #if COMPILEDEBUG
  Serial.begin(9600);
  #endif

  
  if(!loadConfig()){
    firstConfig();
    ultraSetEmpty();
  }
  
  pinInit();
  
  ultraCheck();
  if(!wifiConnect()){
    //pripojenie wifi zlyhalo
    Serial.println("wifi connection error");
    
  }
  


  
  

}

void loop() {
  if(ultraCheck()){
    int i;
    for(i=TPCFG.iUltraExtraChecks; i!=0; i--){
      delay(TPCFG.iUltraExtraChecksIntervalMS);
      if(!ultraCheck()){
        i=2;
        break;
      }
    }
    if(i==0){
      sendNewMailNotif();
    }
  }


  //  ----   TEST     -----------------------------
  FBSetFloat("/database/id_0/UI_sound/senzor_0", ultraMeasure());

  
  delay(TPCFG.iUltraCheckIntervalMS);  // vymenit za low power mod
}

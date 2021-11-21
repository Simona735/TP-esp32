
// jadro programu, obsahuje hlavny cyklus


#include "bluetooth.h"
#include "saveload.h"
#include "settings.h"
#include "wifi.h"
#include "ultra.h"


DeviceConfigs TPCFG;


void setup() {
  if(!loadConfig()){
    firstConfig();
  }
  ultraInit();



  
  

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
    
  delay(TPCFG.iUltraCheckIntervalMS);  // vymenit za low power mod
}

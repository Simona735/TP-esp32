
// funkcie pre ultrazvukovy senzor

#include <Arduino.h>
#include "settings.h"
#include "ultra.h"

RTC_DATA_ATTR UltraVals UltraV;    // prezije spanok

void ultraSetEmpty(){
  pinInit();
  UltraV.fDistEmptyCM1 = ultraMeasure1();
  delay(ULTRA_SEQUENCE_DELAY);
  UltraV.fDistEmptyCM2 = ultraMeasure2();
  delay(ULTRA_SEQUENCE_DELAY);
  UltraV.fDistEmptyCM3 = ultraMeasure3();
  return;
}

float ultraMeasure1(){
  digitalWrite(TRIGPIN1, LOW);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN1, LOW);
  return SOUND_SPEED * pulseIn(ECHOPIN1, HIGH);
  
}

float ultraMeasure2(){
  digitalWrite(TRIGPIN2, LOW);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN2, LOW);
  return SOUND_SPEED * pulseIn(ECHOPIN2, HIGH);
  
}

float ultraMeasure3(){
  digitalWrite(TRIGPIN3, LOW);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN3, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN3, LOW);
  return SOUND_SPEED * pulseIn(ECHOPIN3, HIGH);
  
}

bool ultraCheck1(){
	if((ultraMeasure1() + UltraV.fUltraTolerance) < UltraV.fDistEmptyCM1){
    return true;
	}
	else{
    return false;
	}
}

bool ultraCheck2(){
  if((ultraMeasure2() + UltraV.fUltraTolerance) < UltraV.fDistEmptyCM2){
    return true;
  }
  else{
    return false;
  }
}

bool ultraCheck3(){
  if((ultraMeasure3() + UltraV.fUltraTolerance) < UltraV.fDistEmptyCM3){
    return true;
  }
  else{
    return false;
  }
}

bool ultraCheckAll(){
  if(ultraMeasure1()){
    return true;
  }
  delay(ULTRA_SEQUENCE_DELAY);
  if(ultraMeasure2()){
    return true;
  }
  delay(ULTRA_SEQUENCE_DELAY);
  if(ultraMeasure3()){
    return true;
  }
  return false;
}


void reloadUltraConfigs(){
  UltraV.iUltraCheckInterval = TPCFG.iUltraCheckInterval;
  UltraV.iUltraExtraChecks = TPCFG.iUltraExtraChecks;
  UltraV.iUltraExtraChecksIntervalMS = TPCFG.iUltraExtraChecksIntervalMS;
  UltraV.fUltraTolerance = TPCFG.fUltraTolerance;
}

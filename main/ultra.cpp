
// funkcie pre ultrazvukovy senzor

#include <Arduino.h>
#include "settings.h"
#include "ultra.h"

RTC_DATA_ATTR UltraVals UltraV;    // prezije spanok

void ultraSetEmpty(){
  UltraV.fDistEmptyCM1 = ultraMeasure1();
  UltraV.fDistEmptyCM2 = ultraMeasure2();
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
	if((ultraMeasure1() + TPCFG.fUltraTolerance) < UltraV.fDistEmptyCM1){
    return true;
	}
	else{
    return false;
	}
}

bool ultraCheck2(){
  if((ultraMeasure2() + TPCFG.fUltraTolerance) < UltraV.fDistEmptyCM2){
    return true;
  }
  else{
    return false;
  }
}

bool ultraCheck3(){
  if((ultraMeasure3() + TPCFG.fUltraTolerance) < UltraV.fDistEmptyCM3){
    return true;
  }
  else{
    return false;
  }
}

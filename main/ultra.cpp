
// funkcie pre ultrazvukovy senzor

#include <Arduino.h>
#include "settings.h"
#include "ultra.h"

UltraVals UltraV;

void ultraSetEmpty(){
  UltraV.fDistEmptyCM = ultraMeasure();
  return;
}

float ultraMeasure(){
  digitalWrite(TRIGPIN1, LOW);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN1, LOW);
  return SOUND_SPEED * pulseIn(ECHOPIN1, HIGH);
  
}

int ultraCheck(){
	if((ultraMeasure() + TPCFG.fUltraTolerance) < UltraV.fDistEmptyCM){
    return 1;
	}
	else{
    return 0;
	}
}

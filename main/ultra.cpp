
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
  digitalWrite(TPCFG.iUltraTrigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(TPCFG.iUltraTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TPCFG.iUltraTrigPin, LOW);
  return SOUND_SPEED * pulseIn(TPCFG.iUltraEchoPin, HIGH);
  
}

int ultraCheck(){
	if(ultraMeasure() < UltraV.fDistEmptyCM){
    return 1;
	}
	else{
    return 0;
	}
}

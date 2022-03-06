
// funkcie pre ultrazvukovy a infracerveny senzor

// funkcie na kontroly vracaju True ked je v schranke zaznamenana posta
// funkcie "Empty" su pre stav, ked je postova schranka prazdna, "NotEmpty" sa pouzivaju ked uz bola odoslana notifikacia a kontroluje sa pritomnost dalsej posty

#include <Arduino.h>
#include "settings.h"
#include "saveload.h"
#include "ultra.h"
#include "diags.h"
#include "wifi.h"

RTC_DATA_ATTR UltraVals UltraV;    // prezije spanok

void ultraSetEmpty(){
  pinInit();
  int i;
  float x = 0;
  for(i = ULTRA_SET_EMPTY_SAMPLES; i!=0; i--){
    x += ultraMeasure1();
  }
  UltraV.fDistEmptyCM1 = x/ULTRA_SET_EMPTY_SAMPLES;
  x = 0;
  delay(ULTRA_SEQUENCE_DELAY);
  for(i = ULTRA_SET_EMPTY_SAMPLES; i!=0; i--){
    x += ultraMeasure2();
  }
  UltraV.fDistEmptyCM2 = x/ULTRA_SET_EMPTY_SAMPLES;
  x = 0;
  delay(ULTRA_SEQUENCE_DELAY);
  for(i = ULTRA_SET_EMPTY_SAMPLES; i!=0; i--){
    x += ultraMeasure3();
  }
  UltraV.fDistEmptyCM3 = x/ULTRA_SET_EMPTY_SAMPLES;
  UltraV.bMailEmpty = true;
  return;
}

void ultraSetLast(){
  pinInit();
  int i;
  float x = 0;
  for(i = ULTRA_SET_EMPTY_SAMPLES; i!=0; i--){
    x += ultraMeasure1();
  }
  UltraV.fDistLastCM1 = x/ULTRA_SET_EMPTY_SAMPLES;
  x = 0;
  delay(ULTRA_SEQUENCE_DELAY);
  for(i = ULTRA_SET_EMPTY_SAMPLES; i!=0; i--){
    x += ultraMeasure2();
  }
  UltraV.fDistLastCM2 = x/ULTRA_SET_EMPTY_SAMPLES;
  x = 0;
  delay(ULTRA_SEQUENCE_DELAY);
  for(i = ULTRA_SET_EMPTY_SAMPLES; i!=0; i--){
    x += ultraMeasure3();
  }
  UltraV.fDistLastCM3 = x/ULTRA_SET_EMPTY_SAMPLES;
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

bool ultraCheck1Empty(){
	if((ultraMeasure1() + UltraV.fUltraTolerance) < UltraV.fDistEmptyCM1){
    return true;
	}
	else{
    return false;
	}
}

bool ultraCheck2Empty(){
  if((ultraMeasure2() + UltraV.fUltraTolerance) < UltraV.fDistEmptyCM2){
    return true;
  }
  else{
    return false;
  }
}

bool ultraCheck3Empty(){
  if((ultraMeasure3() + UltraV.fUltraTolerance) < UltraV.fDistEmptyCM3){
    return true;
  }
  else{
    return false;
  }
}

bool ultraCheckAllEmpty(){
  if(ultraCheck1Empty()){
    return true;
  }
  delay(ULTRA_SEQUENCE_DELAY);
  if(ultraCheck2Empty()){
    return true;
  }
  delay(ULTRA_SEQUENCE_DELAY);
  if(ultraCheck3Empty()){
    return true;
  }
  return false;
}

bool ultraCheck1NotEmpty(){
  if((ultraMeasure1() + UltraV.fUltraTolerance) < UltraV.fDistLastCM1){
    return true;
  }
  else{
    return false;
  }
}

bool ultraCheck2NotEmpty(){
  if((ultraMeasure2() + UltraV.fUltraTolerance) < UltraV.fDistLastCM2){
    return true;
  }
  else{
    return false;
  }
}

bool ultraCheck3NotEmpty(){
  if((ultraMeasure3() + UltraV.fUltraTolerance) < UltraV.fDistLastCM3){
    return true;
  }
  else{
    return false;
  }
}

bool ultraCheckAllNotEmpty(){
  if(ultraCheck1NotEmpty()){
    return true;
  }
  delay(ULTRA_SEQUENCE_DELAY);
  if(ultraCheck2NotEmpty()){
    return true;
  }
  delay(ULTRA_SEQUENCE_DELAY);
  if(ultraCheck3NotEmpty()){
    return true;
  }
  return false;
}

bool IRCheck4(){
  digitalWrite(IRTRIGPIN, HIGH);
  delay(IR_WAIT_TRIG);
  if(digitalRead(IRPIN1) || digitalRead(IRPIN2) || digitalRead(IRPIN3) || digitalRead(IRPIN4)){
    digitalWrite(IRTRIGPIN, LOW);
    return true;
  }
  else{
    digitalWrite(IRTRIGPIN, LOW);
    return false;
  }
  
}


int checkMail(){
  pinInit();
  if(UltraV.bMailEmpty){
    if(ultraCheckAllEmpty() || IRCheck4()){
      int i;
      for(i=UltraV.iUltraExtraChecks; i!=0; i--){
        delay(UltraV.iUltraExtraChecksIntervalMS);
        if(!(ultraCheckAllEmpty() || IRCheck4())){
          return 0;
        }
      }
      if(!loadConfig()){
        serialDBGOut("C1 : Chyba pri nacitani nastaveni");
        esp_deep_sleep(FATAL_ERROR_RESET_TIME);
      }
      if(!wifiConnect()){
        serialDBGOut("pripojenie wifi zlyhalo");
        return 0;
        
      }
      sendNewMailNotif();
      ultraSetLast();
      UltraV.bMailEmpty = false;
      return 1;
    }
  }
  else{
    if(! IRCheck4()){
      if(! ultraCheckAllEmpty()){
        if(!loadConfig()){
        serialDBGOut("C1 : Chyba pri nacitani nastaveni");
        esp_deep_sleep(FATAL_ERROR_RESET_TIME);
      }
      if(!wifiConnect()){
        serialDBGOut("pripojenie wifi zlyhalo");
        return 0;
        
      }
      sendEmptyMailNotif();
      UltraV.bMailEmpty = true;
      return 1;
      }
    }
    if(ultraCheckAllNotEmpty()){
      int i;
      for(i=UltraV.iUltraExtraChecks; i!=0; i--){
        delay(UltraV.iUltraExtraChecksIntervalMS);
        if(! ultraCheckAllNotEmpty()){
          return 0;
        }
      }
      if(!loadConfig()){
        serialDBGOut("C1 : Chyba pri nacitani nastaveni");
        esp_deep_sleep(FATAL_ERROR_RESET_TIME);
      }
      if(!wifiConnect()){
        serialDBGOut("pripojenie wifi zlyhalo");
        return 0;
        
      }
      sendNewMailNotif();
      ultraSetLast();
      return 1;
    }
  }
  return 0;
}


void setMailEmpty(bool in){
  UltraV.bMailEmpty = in;
}


void reloadUltraConfigs(){
  UltraV.iUltraCheckInterval = TPCFG.iUltraCheckInterval;
  UltraV.iUltraExtraChecks = TPCFG.iUltraExtraChecks;
  UltraV.iUltraExtraChecksIntervalMS = TPCFG.iUltraExtraChecksIntervalMS;
  UltraV.fUltraTolerance = TPCFG.fUltraTolerance;
}

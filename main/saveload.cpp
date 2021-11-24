
// ukladanie a nacitavanie dat, inicializacia po starte

#include <Arduino.h>
#include "saveload.h"
#include "settings.h"


int loadConfig(){
	// TODO
	
	return 0;
}

int saveConfig(){
	// TODO
	
	return 0;
}

int pinInit(){
  pinMode(TPCFG.iUltraTrigPin, OUTPUT);
  pinMode(TPCFG.iUltraEchoPin, INPUT);
  return 1;
}

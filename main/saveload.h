
// ukladanie a nacitavanie z a do SD karty

#include <Preferences.h>


extern Preferences NVS;


int loadConfig();
int saveConfig();
int eraseSavedConfig();
int wipeFlash();
int pinInit();

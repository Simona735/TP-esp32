
// ukladanie a nacitavanie z a do SD karty

#include <Preferences.h>


extern Preferences NVS;

int checkLoadedConfig();
int checkLoadedConfigDiag();
int loadConfig();
int saveConfig();
int eraseSavedConfig();
int wipeFlash();

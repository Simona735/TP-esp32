
// ukladanie a nacitavanie z a do SD karty

#include <Preferences.h>


extern Preferences NVS;

int checkLoadedConfig();
int checkLoadedConfigCritical();
int checkLoadedConfigDiag();
int loadConfig();
int saveConfig();
int eraseSavedConfig();
int wipeFlash();
void saveSingleInt(char* tag, char* val);
void saveSingleFloat(char* tag, char* val);
void saveSingleString(char* tag, char* val);

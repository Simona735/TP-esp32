
// nastavenia

#include "settings.h"



DeviceConfigs TPCFG;

   //predvolene hodnoty

void setDefaults(){
  TPCFG.iUltraCheckIntervalMS = 10000;          //doba spanku medzi kontrolou posty
  TPCFG.iUltraExtraChecks = 4;            //kolko krat sa pri detekcii posty kontroluje navyse na zabranenie falosnych poplachov
  TPCFG.iUltraExtraChecksIntervalMS = 500;      //cas medzi extra kontrolami
  TPCFG.iUltraTrigPin = 13;
  TPCFG.iUltraEchoPin = 15;
  TPCFG.fUltraTolerance = 0;              //tolerancia/citlivost ultrazvukoveho senzora, mensie znamena citlivejsie, minimum/default = 0
  TPCFG.sWifiSSID = "0";  // SSID (meno) WiFi
  TPCFG.sWifiPassword = "0";  // heslo WiFi
  TPCFG.sServerName = "0";
}

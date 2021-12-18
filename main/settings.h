
// nastavenia

#include <Arduino.h>



// natvrdo nastavene hodnoty
#define TRIGPIN1 13
#define ECHOPIN1 15
#define WIFI_CRED_LENGTH 30



struct DeviceConfigs{
	int iUltraCheckIntervalMS;					//doba spanku medzi kontrolou posty
	int iUltraExtraChecks;						//kolko krat sa pri detekcii posty kontroluje navyse na zabranenie falosnych poplachov
	int iUltraExtraChecksIntervalMS;			//cas medzi extra kontrolami
  float fUltraTolerance;              //tolerancia/citlivost ultrazvukoveho senzora, mensie znamena citlivejsie, minimum/default = 0
  //char sWifiSSID[WIFI_CRED_LENGTH];  // SSID (meno) WiFi
  //char sWifiPassword[WIFI_CRED_LENGTH];  // heslo WiFi
  String sWifiSSID;  // SSID (meno) WiFi
  String sWifiPassword;  // heslo WiFi
	String sFBKey;
  String sFBURL;
  String sFBMail;
  String sFBPassword;
  

	
	
	
	
};

extern DeviceConfigs TPCFG;
//extern String TPCFGNames;
//extern String TPCFGVals;

void setDefaults();
void setDefaultsFB();
int pinInit();

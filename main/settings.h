
// nastavenia

#include <Arduino.h>


#define WIFI_CRED_LENGTH 30

struct DeviceConfigs{
	int iUltraCheckIntervalMS;					//doba spanku medzi kontrolou posty
	int iUltraExtraChecks;						//kolko krat sa pri detekcii posty kontroluje navyse na zabranenie falosnych poplachov
	int iUltraExtraChecksIntervalMS;			//cas medzi extra kontrolami
	int iUltraTrigPin = 13;
  int iUltraEchoPin = 15;
  //char sWifiSSID[WIFI_CRED_LENGTH];  // SSID (meno) WiFi
  //char sWifiPassword[WIFI_CRED_LENGTH];  // heslo WiFi
  String sWifiSSID;  // SSID (meno) WiFi
  String sWifiPassword;  // heslo WiFi
	String sServerName;
  

	
	
	
	
};

extern DeviceConfigs TPCFG;

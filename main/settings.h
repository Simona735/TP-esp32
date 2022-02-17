
// nastavenia

#include <Arduino.h>


// natvrdo nastavene hodnoty
#define TRIGPIN1 12
#define ECHOPIN1 13
#define TRIGPIN2 12
#define ECHOPIN2 13
#define TRIGPIN3 12
#define ECHOPIN3 13
#define WIFI_CRED_LENGTH 30
#define NVSNAME "MAILBOX"
#define SOUND_SPEED 0.017
#define WIFI_RETRY_CON_DELAY 200
#define WIFI_MAX_CONNECT_ATTEMPTS 20
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define FATAL_ERROR_RESET_TIME 20000000
#define ULTRA_SEQUENCE_DELAY 10
#define RESET_BLE_CONFIG_SECONDS 30
#define RESET_BLE_CONFIG_SECONDS_REQUIRED 600
#define ADDITIONAL_TASKS_FREQUENCY 100
#define MAX_PATH_LENGTH 100



struct DeviceConfigs{
	int iUltraCheckInterval;					//doba spanku medzi kontrolou posty (mikrosekundy)
	int iUltraExtraChecks;						//kolko krat sa pri detekcii posty kontroluje navyse na zabranenie falosnych poplachov
	int iUltraExtraChecksIntervalMS;			//cas medzi extra kontrolami (milisekundy)
  float fUltraTolerance;              //tolerancia/citlivost ultrazvukoveho senzora v CM, mensie znamena citlivejsie, minimum/default = 0
  //char sWifiSSID[WIFI_CRED_LENGTH];  // SSID (meno) WiFi
  //char sWifiPassword[WIFI_CRED_LENGTH];  // heslo WiFi
  String sWifiSSID;  // SSID (meno) WiFi
  String sWifiPassword;  // heslo WiFi
	String sFBKey;
  String sFBURL;
  String sFBMail;
  String sFBPassword;
  String sFBID;
  String sFBUser;

	
	
	
	
};

extern DeviceConfigs TPCFG;
//extern String TPCFGNames;
//extern String TPCFGVals;

void setDefaults();
void setDefaultsWifi();
void setDefaultsFB();
void setDummyCfg(String str);
int pinInit();

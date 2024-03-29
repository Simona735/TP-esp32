
// funkcie na komunikaciu cez wifi a s firebase-om

#include <Firebase_ESP_Client.h>


struct WifiVals{
  
  
};

extern WifiVals WifiV;

extern FirebaseData FBDATA;
extern FirebaseAuth FBAUTH;
extern FirebaseConfig FBCFG;

int wifiConnect();
String wifiHTTPGet(String url);
int wifiDisconnect();
int FBInit();
int FBConnect();
int FBDisconnect();
int FBStatus();         //vrati stav pripojenia s FB, 0 == OK
String FBLastError();
bool FBPathExists(const char* path);
bool FBSetBool(const char* path, bool input);
bool FBSetInt(const char* path, int input);
bool FBSetFloat(const char* path, float input);
bool FBSetDouble(const char* path, double input);
bool FBSetString(const char* path, String str);
bool FBSetString(const char* path, const char* str);
bool FBSetJson(const char* path, FirebaseJson json);
int sendNewMailNotif();
int sendEmptyMailNotif();
int sendDeviceFatalError(char* cause);
int fetchSettings();

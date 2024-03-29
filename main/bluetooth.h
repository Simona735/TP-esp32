
// funkcie na komunikaciu cez bluetooth a s mobilom

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>


#define MAXBLECHARACTERISTICS 5



struct TPBLEVals{
  BLEServer *pServer;
  BLEService *pService;
  BLECharacteristic *pCharacteristics[MAXBLECHARACTERISTICS];
  int iNumCharacteristics;
  BLEAdvertising *pAdvertising;
  BLESecurity *pSecurity;
};

int initBLE(char* sServUUID);
BLECharacteristic* addCharBLE(bool bRW, char* sCharUUID, char* sVal, BLECharacteristicCallbacks* oCallback);
BLECharacteristic* addCharBLE(bool bRW, char* sCharUUID, String sVal, BLECharacteristicCallbacks* oCallback);
BLECharacteristic* addCharBLE(bool bRW, char* sCharUUID, int iVal, BLECharacteristicCallbacks* oCallback);
BLECharacteristic* addCharBLE(bool bRW, char* sCharUUID, float fVal, BLECharacteristicCallbacks* oCallback);
int startBLE();
int stopBLE();
int showBLE();
int hideBLE();
int blueConfig(int iTimeSecs);

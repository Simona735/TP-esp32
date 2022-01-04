
// funkcie na komunikaciu cez bluetooth a s mobilom

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>


#define MAXBLECHARACTERISTICS 30


struct TPBLEVals{
  BLEServer *pServer;
  BLEService *pService;
  BLECharacteristic *pCharacteristics[MAXBLECHARACTERISTICS];
  int iNumCharacteristics;
  BLEAdvertising *pAdvertising;
};

int initBLE(char* sServUUID);
int addCharBLE(bool bRW, char* sCharUUID, char* sVal, BLECharacteristicCallbacks* oCallback);
int startBLE();
int firstConfig();

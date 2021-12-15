
// funkcie na komunikaciu cez bluetooth a s mobilom

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

extern BLEServer *pServer;
extern BLEService *pService;
extern BLECharacteristic *pCharacteristic;
extern BLEAdvertising *pAdvertising;

int firstConfig();


// funkcie na komunikaciu cez bluetooth a s mobilom

#include <Arduino.h>
#include "bluetooth.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "saveload.h"
#include "settings.h"

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

TPBLEVals TPBLEV;


// BLE callbacks for wifi connection
class CallbackConfigSaverInt: public BLECharacteristicCallbacks {
  int* pTarget;
  public:
  CallbackConfigSaverInt(int* pTar){
    pTarget = pTar;
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    *pTarget = *(pCharacteristic->getData());
  }
};



int initBLE(char* sServUUID){
  BLEDevice::init("MailBox");
  TPBLEV.pServer = BLEDevice::createServer();
  TPBLEV.pService = TPBLEV.pServer->createService(sServUUID);
  TPBLEV.iNumCharacteristics = 0;
  return 1;
}

int addCharBLE(bool bRW, char* sCharUUID, char* sVal, BLECharacteristicCallbacks* oCallback){
  if(bRW){
    TPBLEV.pCharacteristics[TPBLEV.iNumCharacteristics] = TPBLEV.pService->createCharacteristic(sCharUUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    TPBLEV.iNumCharacteristics++;
  }
  else{
    TPBLEV.pCharacteristics[TPBLEV.iNumCharacteristics] = TPBLEV.pService->createCharacteristic(sCharUUID, BLECharacteristic::PROPERTY_READ);
    TPBLEV.iNumCharacteristics++;
  }
  if(oCallback != nullptr){
    TPBLEV.pCharacteristics[TPBLEV.iNumCharacteristics - 1]->setCallbacks(oCallback);
  }
  TPBLEV.pCharacteristics[TPBLEV.iNumCharacteristics - 1]->setValue(sVal);
}

int startBLE(){
  TPBLEV.pService->start();
  TPBLEV.pAdvertising = TPBLEV.pServer->getAdvertising();
  TPBLEV.pAdvertising->start();
  return 1;
}


int firstConfig(){
  // TODO
  initBLE(SERVICE_UUID);
  addCharBLE(true, CHARACTERISTIC_UUID, "test", new CallbackConfigSaverInt(&TPCFG.iUltraCheckIntervalMS));
  startBLE();
	
	
	return 0;
}

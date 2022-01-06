
// funkcie na komunikaciu cez bluetooth a s mobilom

#include <Arduino.h>
#include "bluetooth.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "saveload.h"
#include "settings.h"



TPBLEVals TPBLEV;


// BLE callbacks for wifi connection
class CallbackConfigSaverIntRaw: public BLECharacteristicCallbacks {
  int* pTarget;
  public:
  CallbackConfigSaverIntRaw(int* pTar){
    pTarget = pTar;
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    *pTarget = *(pCharacteristic->getData());
  }
};

class CallbackConfigSaverFloatRaw: public BLECharacteristicCallbacks {
  float* pTarget;
  public:
  CallbackConfigSaverFloatRaw(float* pTar){
    pTarget = pTar;
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    *pTarget = *(pCharacteristic->getData());
  }
};

class CallbackConfigSaverString: public BLECharacteristicCallbacks {
  String* pTarget;
  public:
  CallbackConfigSaverString(String* pTar){
    pTarget = pTar;
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    *pTarget = pCharacteristic->getValue().c_str();
  }
};

class CallbackConfigSaverInt: public BLECharacteristicCallbacks {
  int* pTarget;
  public:
  CallbackConfigSaverInt(int* pTar){
    pTarget = pTar;
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    *pTarget = atoi(pCharacteristic->getValue().c_str());
  }
};

class CallbackConfigSaverFloat: public BLECharacteristicCallbacks {
  float* pTarget;
  public:
  CallbackConfigSaverFloat(float* pTar){
    pTarget = pTar;
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    *pTarget = strtof(pCharacteristic->getValue().c_str(), nullptr);
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

int addCharBLE(bool bRW, char* sCharUUID, String sVal, BLECharacteristicCallbacks* oCallback){
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
  TPBLEV.pCharacteristics[TPBLEV.iNumCharacteristics - 1]->setValue(sVal.c_str());
}

int addCharBLE(bool bRW, char* sCharUUID, int iVal, BLECharacteristicCallbacks* oCallback){
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
  char sVal[20];
  snprintf(sVal, 20, "%d", iVal);
  TPBLEV.pCharacteristics[TPBLEV.iNumCharacteristics - 1]->setValue(sVal);
}

int addCharBLE(bool bRW, char* sCharUUID, float fVal, BLECharacteristicCallbacks* oCallback){
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
  char sVal[20];
  snprintf(sVal, 20, "%f", fVal);
  TPBLEV.pCharacteristics[TPBLEV.iNumCharacteristics - 1]->setValue(sVal);
}



int startBLE(){
  TPBLEV.pService->start();
  TPBLEV.pAdvertising = TPBLEV.pServer->getAdvertising();
  TPBLEV.pAdvertising->start();
  return 1;
}


int blueConfig(){
  initBLE(SERVICE_UUID);
  addCharBLE(true, "b81b8cac-7dce-4de0-a568-31b4a0b35816", TPCFG.iUltraCheckIntervalMS, new CallbackConfigSaverInt(&TPCFG.iUltraCheckIntervalMS));
  addCharBLE(true, "70510db2-616d-4598-82e8-4efe1f5ad71a", TPCFG.iUltraExtraChecks, new CallbackConfigSaverInt(&TPCFG.iUltraExtraChecks));
  addCharBLE(true, "47865e5e-35f4-4119-a9dd-7cc0d3cd7c7e", TPCFG.iUltraExtraChecksIntervalMS, new CallbackConfigSaverInt(&TPCFG.iUltraExtraChecksIntervalMS));
  addCharBLE(true, "c5fa8d3e-8216-4e1e-b75e-dcfbd7b01455", TPCFG.fUltraTolerance, new CallbackConfigSaverFloat(&TPCFG.fUltraTolerance));
  addCharBLE(true, "5f08db8e-16da-4f74-8084-4fe93be581dc", TPCFG.sWifiSSID, new CallbackConfigSaverString(&TPCFG.sWifiSSID));
  addCharBLE(true, "eff0c5bd-426a-46de-904c-a3acbb847ba0", TPCFG.sWifiPassword, new CallbackConfigSaverString(&TPCFG.sWifiPassword));
  addCharBLE(true, "8ef7a713-b363-4831-86cb-b4ae3ab69498", TPCFG.sFBKey, new CallbackConfigSaverString(&TPCFG.sFBKey));
  addCharBLE(true, "9c8657d5-7343-4a1d-a5b8-f5acd689c763", TPCFG.sFBURL, new CallbackConfigSaverString(&TPCFG.sFBURL));
  addCharBLE(true, "88558ac3-0138-424c-9b4f-460757bcb6ec", TPCFG.sFBMail, new CallbackConfigSaverString(&TPCFG.sFBMail));
  addCharBLE(true, "820aeda2-1d70-42b9-8831-3b515f88d9a2", TPCFG.sFBPassword, new CallbackConfigSaverString(&TPCFG.sFBPassword));
  startBLE();
  for(int i = 0; i < 10; i++){
    delay(1000);
  }
  saveConfig();
	return 1;
}

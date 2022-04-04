
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
class CallbackUniversal: public BLECharacteristicCallbacks {
  int* piState;
  public:
  CallbackUniversal(int* out){
    piState = out;
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    if(pCharacteristic->getValue().compare("0") == 0){
      return;
    }
    
    char str[pCharacteristic->getValue().length()+2];
    strcpy(str, pCharacteristic->getValue().c_str());
    char* sep = strpbrk(str, ";");
    *sep = '\0';
    sep += 1;
    //Serial.println(str);
    //Serial.println(sep);
    
    if(strcmp(str, "WS") == 0){
      TPCFG.sWifiSSID = String(sep);
      
    }
    else if(strcmp(str, "WP") == 0){
      TPCFG.sWifiPassword = String(sep);
      
    }
    else if(strcmp(str, "FBK") == 0){
      TPCFG.sFBKey = String(sep);
      
    }
    else if(strcmp(str, "FBM") == 0){
      TPCFG.sFBMail = String(sep);
      
    }
    else if(strcmp(str, "FBP") == 0){
      TPCFG.sFBPassword = String(sep);
      
    }
    else if(strcmp(str, "FBI") == 0){
      TPCFG.sFBID = String(sep);
      
    }
    else if(strcmp(str, "FBU") == 0){
      TPCFG.sFBUser = String(sep);
      
    }
    else if(strcmp(str, "+FRST") == 0){
      eraseSavedConfig();
      ESP.restart();
      
    }
    else if(strcmp(str, "+CONF") == 0){
      *piState = 2;
      
    }
    else if(strcmp(str, "+TEST") == 0){
      setDummyCfg(String(sep));
      
    }

    pCharacteristic->setValue("0");
    return;
  }
};

class CallbackStateDescriptor: public BLECharacteristicCallbacks {
  int* pTarget;
  public:
  CallbackStateDescriptor(int* pTar){
    pTarget = pTar;
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    *pTarget = atoi(pCharacteristic->getValue().c_str());
  }
};

class CallbackCommand: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    if(pCharacteristic->getValue()=="ERASESETTINGS"){
      eraseSavedConfig();
      ESP.restart();
    }
  }
};

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
  TPBLEV.pAdvertising = TPBLEV.pServer->getAdvertising();
  TPBLEV.pSecurity = new BLESecurity();
  TPBLEV.pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);
  return 1;
}



BLECharacteristic* addCharBLE(bool bRW, char* sCharUUID, char* sVal, BLECharacteristicCallbacks* oCallback){
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
  return TPBLEV.pCharacteristics[TPBLEV.iNumCharacteristics - 1];
}

BLECharacteristic* addCharBLE(bool bRW, char* sCharUUID, String sVal, BLECharacteristicCallbacks* oCallback){
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
  return TPBLEV.pCharacteristics[TPBLEV.iNumCharacteristics - 1];
}

BLECharacteristic* addCharBLE(bool bRW, char* sCharUUID, int iVal, BLECharacteristicCallbacks* oCallback){
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
  return TPBLEV.pCharacteristics[TPBLEV.iNumCharacteristics - 1];
}

BLECharacteristic* addCharBLE(bool bRW, char* sCharUUID, float fVal, BLECharacteristicCallbacks* oCallback){
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
  return TPBLEV.pCharacteristics[TPBLEV.iNumCharacteristics - 1];
}



int startBLE(){
  TPBLEV.pService->start();
  return 1;
}

int stopBLE(){
  TPBLEV.pService->stop();
  return 1;
}

int showBLE(){
  TPBLEV.pAdvertising->start();
  return 1;
}

int hideBLE(){
  TPBLEV.pAdvertising->stop();
  return 1;
}

int blueConfig(int iTimeSecs){
  initBLE(SERVICE_UUID);
  
  int iState = 0;
  
  BLECharacteristic* opChar = addCharBLE(true, "11aa358f-9224-46d9-b0f5-3a7ba1ac651e", "0", new CallbackUniversal(&iState));
  opChar->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
  
  //addCharBLE(true, "0b62f0bc-37b2-4345-973e-3138c37ff4ca", "0", new CallbackCommand());
  //addCharBLE(true, "b81b8cac-7dce-4de0-a568-31b4a0b35816", TPCFG.iUltraCheckIntervalMS, new CallbackConfigSaverInt(&TPCFG.iUltraCheckIntervalMS));
  //addCharBLE(true, "70510db2-616d-4598-82e8-4efe1f5ad71a", TPCFG.iUltraExtraChecks, new CallbackConfigSaverInt(&TPCFG.iUltraExtraChecks));
  //addCharBLE(true, "47865e5e-35f4-4119-a9dd-7cc0d3cd7c7e", TPCFG.iUltraExtraChecksIntervalMS, new CallbackConfigSaverInt(&TPCFG.iUltraExtraChecksIntervalMS));
  //addCharBLE(true, "c5fa8d3e-8216-4e1e-b75e-dcfbd7b01455", TPCFG.fUltraTolerance, new CallbackConfigSaverFloat(&TPCFG.fUltraTolerance));
  //addCharBLE(true, "5f08db8e-16da-4f74-8084-4fe93be581dc", TPCFG.sWifiSSID, new CallbackConfigSaverString(&TPCFG.sWifiSSID));
  //addCharBLE(true, "eff0c5bd-426a-46de-904c-a3acbb847ba0", TPCFG.sWifiPassword, new CallbackConfigSaverString(&TPCFG.sWifiPassword));
  //addCharBLE(true, "8ef7a713-b363-4831-86cb-b4ae3ab69498", TPCFG.sFBKey, new CallbackConfigSaverString(&TPCFG.sFBKey));
  //addCharBLE(true, "9c8657d5-7343-4a1d-a5b8-f5acd689c763", TPCFG.sFBURL, new CallbackConfigSaverString(&TPCFG.sFBURL));
  //addCharBLE(true, "88558ac3-0138-424c-9b4f-460757bcb6ec", TPCFG.sFBMail, new CallbackConfigSaverString(&TPCFG.sFBMail));
  //addCharBLE(true, "820aeda2-1d70-42b9-8831-3b515f88d9a2", TPCFG.sFBPassword, new CallbackConfigSaverString(&TPCFG.sFBPassword));
  
  
  if(iTimeSecs > 0){
    iTimeSecs = iTimeSecs * 20;
    startBLE();
    showBLE();
    for(int i = 0; i < iTimeSecs; i++){
      delay(50);
      if(iState == 2){
        opChar->setValue("3");
        delay(3000);
        hideBLE();
        stopBLE();
        saveConfig();
        return 1;
      }
    }
    opChar->setValue("2");
    delay(3000);
    hideBLE();
    stopBLE();
    saveConfig();
    return 1;
  }
  else{
    iTimeSecs = RESET_BLE_CONFIG_SECONDS_REQUIRED * 20;
    setDefaults();
    setDefaultsWifi();
    setDefaultsFB();
    startBLE();
    showBLE();
    for(int i = 0; i < iTimeSecs; i++){
      delay(50);
      if(iState == 2){
        opChar->setValue("3");
        delay(3000);
        hideBLE();
        stopBLE();
        if(!checkLoadedConfigCritical()){
          esp_deep_sleep_start();
        }
        saveConfig();
        return 1;
      }
    }
    opChar->setValue("2");
    delay(3000);
    hideBLE();
    stopBLE();
    if(!checkLoadedConfigCritical()){
      esp_deep_sleep_start();
    }
    saveConfig();
    return 1;
  }
  
}

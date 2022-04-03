
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include "mbedtls/rsa.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include <string.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID1 "11aa358f-9224-46d9-b0f5-3a7ba1ac651e"


BLECharacteristic *pCharacteristic1;




std::string rsaEnc(std::string){
  std::string out;

  mbedtls_rsa_context rsa;
  mbedtls_entropy_context entropy;
  mbedtls_ctr_drbg_context ctr_drbg;






  //mbedtls_mpi_free( &N ); mbedtls_mpi_free( &E );
  mbedtls_ctr_drbg_free( &ctr_drbg );
  mbedtls_entropy_free( &entropy );
  mbedtls_rsa_free( &rsa );
  
  return out;
}







class CallbackEncrypt: public BLECharacteristicCallbacks {
  //String* pTarget;
  public:
  /*
  CallbackConfigSaverString(String* pTar){
    pTarget = pTar;
  }
  */
  void onWrite(BLECharacteristic *pCharacteristic) {
    //*pTarget = pCharacteristic->getValue().c_str();
  }
};








void setup() {
  Serial.begin(115200);
  
  Serial.println("Start");
  delay(3000);
  BLEDevice::init("BLE RSA test");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic1 = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID1,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic1->setValue("0");
  pCharacteristic1->setCallbacks(new CallbackEncrypt());
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("BLE Aktivne");
  Serial.println("Staticky test RSA:");
  Serial.println("OT:");
  std::string testOT = std::string("TestRSA123..........TestRSA..........TestRSA..........TestRSA..........TestRSA..........TestRSA..........TestRSA123");
  Serial.println(testOT.c_str());
  std::string testZT = rsaEnc(testOT);
  Serial.println("ZT:");
  Serial.println(testZT.c_str());
  

  
}

void loop() {
  Serial.println("cakam...");
  //Serial.println(pCharacteristic1->getValue().c_str());
  delay(10000);
}

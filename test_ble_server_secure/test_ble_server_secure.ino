/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID1 "11aa358f-9224-46d9-b0f5-3a7ba1ac651e"
#define CHARACTERISTIC_UUID2 "aea5483e-36e1-4688-b7f5-ea07361b26a9"
#define CHARACTERISTIC_UUID3 "25ed4f18-d8f7-4ba6-a7c1-8edfc852ae8e"

BLECharacteristic *pCharacteristic1;
BLECharacteristic *pCharacteristic2;
BLECharacteristic *pCharacteristic3;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("BLE test sec");
  BLEDevice::setMTU(512);

  BLESecurity *pSecurity = new BLESecurity();
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);
  
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic1 = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID1,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  pCharacteristic1->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
  pCharacteristic2 = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID2,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  pCharacteristic2->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
  pCharacteristic3 = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID3,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  pCharacteristic3->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
  
  pCharacteristic1->setValue("ch1");
  pCharacteristic2->setValue("ch2");
  pCharacteristic3->setValue("ch3");
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  //pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  //pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
  Serial.println("");
  Serial.println("char1:");
  Serial.println(pCharacteristic1->getValue().c_str());
  Serial.println("char2:");
  Serial.println(pCharacteristic2->getValue().c_str());
  Serial.println("char3:");
  Serial.println(pCharacteristic3->getValue().c_str());
  //Serial.println("mtu:");
  //Serial.println(BLEDevice::getMTU());
  delay(10000);
}

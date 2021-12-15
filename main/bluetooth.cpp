
// funkcie na komunikaciu cez bluetooth a s mobilom

#include "bluetooth.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"



BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pCharacteristic;
BLEAdvertising *pAdvertising;
bool waiting_ble_callback = false;
char write_data[256];
int write_data_counter = 0;
char parsing_data[2][256];

// BLE callbacks for wifi connection
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        //Serial.println("*********");
        //Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++){
          //Serial.print(value[i]);
          write_data[write_data_counter++] = value[i];
        }
        
        //Serial.println();
        //Serial.println("*********");
        waiting_ble_callback = true;
      }
    }
};



int startBLE(){
  BLEDevice::init("MailBox");
  pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
                                       
  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->setValue("Wait for set wifi");
  pService->start();
  pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
  return 1;
}


int firstConfig(){
	// TODO
	
	return 0;
}

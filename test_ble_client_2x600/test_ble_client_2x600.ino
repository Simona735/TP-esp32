/**
 * A BLE client example that is rich in capabilities.
 * There is a lot new capabilities implemented.
 * author unknown
 * updated by chegewara
 */

#include "BLEDevice.h"
//#include "BLEScan.h"

// The remote service we wish to connect to.
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID1("11aa358f-9224-46d9-b0f5-3a7ba1ac651e");
static BLEUUID    charUUID2("aea5483e-36e1-4688-b7f5-ea07361b26a9");

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic1;
static BLERemoteCharacteristic* pRemoteCharacteristic2;
static BLEAdvertisedDevice* myDevice;

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
    Serial.print("data: ");
    Serial.println((char*)pData);
}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic1 = pRemoteService->getCharacteristic(charUUID1);
    if (pRemoteCharacteristic1 == nullptr) {
      Serial.print("Failed to find our characteristic 1 UUID: ");
      Serial.println(charUUID1.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic 1");
    pRemoteCharacteristic2 = pRemoteService->getCharacteristic(charUUID2);
    if (pRemoteCharacteristic2 == nullptr) {
      Serial.print("Failed to find our characteristic 2 UUID: ");
      Serial.println(charUUID2.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic 2");

    // Read the value of the characteristic.
    if(pRemoteCharacteristic1->canRead()) {
      std::string value1 = pRemoteCharacteristic1->readValue();
      Serial.print("The characteristic 1 value was: ");
      Serial.println(value1.c_str());
    }
    if(pRemoteCharacteristic2->canRead()) {
      std::string value2 = pRemoteCharacteristic2->readValue();
      Serial.print("The characteristic 2 value was: ");
      Serial.println(value2.c_str());
    }

    if(pRemoteCharacteristic1->canNotify())
      pRemoteCharacteristic1->registerForNotify(notifyCallback);

    connected = true;
    return true;
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks


void setup() {
  Serial.begin(115200);
  
  BLEDevice::init("");
  Serial.println("Waiting 10 seconds...");
  for(int i = 0; i<10; i++){
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Starting Arduino BLE Client application...");
  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
} // End of setup.


// This is the Arduino main loop function.
void loop() {



  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("Connected to the BLE Server!");
    } else {
      Serial.println("Failed to connect to the server.");
    }
    doConnect = false;
  }

  
  if (connected) {
    char newValue[600];
    for(int i = 0; i < 600; i++){
      newValue[i] = '.';
      newValue[i]='.';
    }
    newValue[0] = 'A';
    newValue[99] = '1';
    newValue[199] = '2';
    newValue[299] = '3';
    newValue[399] = '4';
    newValue[499] = '5';
    newValue[598] = 'Z';
    newValue[599] = 0; //'\0';
    Serial.println("Setting new characteristic 1 value:");
    pRemoteCharacteristic1->writeValue(newValue, 600);
    newValue[0] = 'B';
    pRemoteCharacteristic2->writeValue(newValue, 600);
    delay(1000);
    esp_deep_sleep_start();  // turn off ESP32
  }else if(doScan){
    BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
  }
  
  delay(3000); // Delay 3 seconds between loops.
} // End of loop

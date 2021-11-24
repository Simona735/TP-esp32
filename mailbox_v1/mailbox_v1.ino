// include libraries
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <EEPROM.h>
// include BLE libraries
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// maco definitions 
#define LENGTH(x) (strlen(x) + 1)

//define sound speed in cm/uS
#define SOUND_SPEED 0.034

// EEPROM bytes access
#define EEPROM_SIZE 128
#define EEPROM_RESET_PIN 2

// BLE definitions
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

bool waiting_ble_callback = false;
char write_data[256];
int write_data_counter = 0;
char parsing_data[2][256];

// BLE callbacks for wifi connection
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++){
          Serial.print(value[i]);
          write_data[write_data_counter++] = value[i];
        }
        
        Serial.println();
        Serial.println("*********");
        waiting_ble_callback = true;
      }
    }
};


// includes fot firebase
//Provide the token generation process info.
#include <addons/TokenHelper.h>
//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

// definitions for firebase
#define API_KEY "AIzaSyBnvv9CnwggizzoOVf7Pj3-IO_Tv9sMKlo"

#define DATABASE_URL "https://timovy-projekt-97069-default-rtdb.europe-west1.firebasedatabase.app/" 

#define USER_EMAIL "acoak@hhh.ck"
#define USER_PASSWORD "999999"

// data sending counter
unsigned long sendDataPrevMillis = 0;

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// measurements
const int TrigPin = 13;
const int EchoFront = 15;
const int EchoMiddle = 0;
const int EchoBack = 4;
const int InFront = 14;
const int InMiddle = 12;
const int InBack = 16;


void setup() {
  Serial.begin(115200);

  // init EEPROM reset pin
  pinMode(EEPROM_RESET_PIN, INPUT);

  // set pines for distance and infrared sensors
  pinMode(TrigPin, OUTPUT); 
  pinMode(EchoFront, INPUT);
  pinMode(EchoMiddle, INPUT); 
  pinMode(EchoBack, INPUT);  
  pinMode(InFront, INPUT);
  pinMode(InMiddle, INPUT);
  pinMode(InBack, INPUT);

  String WIFI_SSID, WIFI_PASSWORD;
  // initialize EEPROM with predefined size
  EEPROM.begin(EEPROM_SIZE);
  if (EEPROM.read(0) != 1){
    // BLE initialize
    BLEDevice::init("MailBox");
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);
    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                           CHARACTERISTIC_UUID,
                                           BLECharacteristic::PROPERTY_READ |
                                           BLECharacteristic::PROPERTY_WRITE
                                         );
                                         
    pCharacteristic->setCallbacks(new MyCallbacks());
    pCharacteristic->setValue("Wait for set wifi");
    pService->start();

    Serial.println("Waiting for set wifi ");
  
    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->start();

    // set wifi from bluetooth
    while(!waiting_ble_callback){
      delay(300);
      Serial.print(".");
    }
    delay(1000);

    Serial.println(write_data);
    int position_in_write_data = 0, parsing_array = 0;
    for(int i = 0; i < strlen(write_data); i++){
      if(write_data[i] == ';'){
        parsing_array++;
        position_in_write_data = 0;
        continue;
      }
      parsing_data[parsing_array][position_in_write_data++] = write_data[i];
    }
    Serial.println( parsing_data[0]);
    Serial.println( parsing_data[1]);
    
    writeString(parsing_data[0], 1);
    Serial.println(readStringFromFlash(1));
    int addres = readStringFromFlash(1).length();
    writeString(parsing_data[1], 2+addres);
    Serial.println(readStringFromFlash(2+addres));
    EEPROM.write(0, 1);
    EEPROM.commit();
    
    ESP.restart();
  }
  else {
    Serial.println("Data after EEPROM set->");
    WIFI_SSID = readStringFromFlash(1);
    Serial.println("SSID->     "+WIFI_SSID);
    int addres = WIFI_SSID.length();
    WIFI_PASSWORD = readStringFromFlash(2+addres);
    Serial.println("PASSWORD-> "+WIFI_PASSWORD);
  }

  // Wifi connesction
  WiFi.begin(WIFI_SSID.c_str(), WIFI_PASSWORD.c_str());
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Connect to Firebase
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);

  if(Firebase.ready()){
    Serial.println("Firebase is ready for use");
  } else {
    Serial.println("Firebase RIP");
  }
}

void loop() {
  // hard reset EEPROM DATA
  if (!digitalRead(EEPROM_RESET_PIN)){
    Serial.println("Reset EEPROM");
    EEPROM.write(0, 255);
    EEPROM.commit();
  }

  if (Firebase.ready() && (millis() - sendDataPrevMillis > 2000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    
    float distance_measure = distance(TrigPin, EchoFront);
    Serial.println("Distance value-> " + String(distance_measure));
    Serial.printf("Set distance... %s\n", Firebase.RTDB.setFloat(&fbdo, "/database/id_0/UI_sound/senzor_0", distance_measure) ? "ok" : fbdo.errorReason().c_str());

    distance_measure = distance(TrigPin, EchoMiddle);
    Serial.println("Distance value-> " + String(distance_measure));
    Serial.printf("Set distance... %s\n", Firebase.RTDB.setFloat(&fbdo, "/database/id_0/UI_sound/senzor_1", distance_measure) ? "ok" : fbdo.errorReason().c_str());

    distance_measure = distance(TrigPin, EchoBack);
    Serial.println("Distance value-> " + String(distance_measure));
    Serial.printf("Set distance... %s\n", Firebase.RTDB.setFloat(&fbdo, "/database/id_0/UI_sound/senzor_2", distance_measure) ? "ok" : fbdo.errorReason().c_str());

    bool isdetected = !digitalRead(InFront);
    Serial.println("InfraRed Front sensor detected val-> " + String(isdetected));
    Serial.printf("Set isdetected... %s\n", Firebase.RTDB.setBool(&fbdo, "/database/id_0/Inf_Red/sensor_0", isdetected) ? "ok" : fbdo.errorReason().c_str());
  
    isdetected = !digitalRead(InMiddle);
    Serial.println("InfraRed Middle sensor detected val-> " + String(isdetected));
    Serial.printf("Set isdetected... %s\n", Firebase.RTDB.setBool(&fbdo, "/database/id_0/Inf_Red/sensor_1", isdetected) ? "ok" : fbdo.errorReason().c_str());

    isdetected = !digitalRead(InBack);
    Serial.println("InfraRed Back sensor detected val-> " + String(isdetected));
    Serial.printf("Set isdetected... %s\n", Firebase.RTDB.setBool(&fbdo, "/database/id_0/Inf_Red/sensor_2", isdetected) ? "ok" : fbdo.errorReason().c_str());

    FirebaseJson json;
    json.set("/.sv", "timestamp");
    Serial.printf("Set timestamp... %s\n", Firebase.RTDB.set(&fbdo, "/database/id_0/timestamp", &json) ? "ok" : fbdo.errorReason().c_str());

    Serial.println();
  }
  
}

// EEPROM write & read string functions
void writeString(const char* toStore, int startAddr) {
  int i = 0;
  for (; i < LENGTH(toStore); i++) {
    EEPROM.write(startAddr + i, toStore[i]);
  }
  EEPROM.write(startAddr + i, '\0');
  EEPROM.commit();
}

String readStringFromFlash(int startAddr) {
  char in[EEPROM_SIZE];
  char curIn;
  int i = 0;
  for (; i < EEPROM_SIZE; i++) {
    curIn = EEPROM.read(startAddr + i);
    in[i] = curIn;
    if (curIn == '\0')
      break; 
  }
  return String(in);
}


// Distance measurement
float distance(const int trigerPin,const int echoPin) {
  // Clears the trigPin
  digitalWrite(trigerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigerPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  float duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  return duration * SOUND_SPEED/2;
  
}

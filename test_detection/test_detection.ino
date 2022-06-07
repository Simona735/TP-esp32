
// TEST DETEKCIE POSTY
// kopia funkcii na detekciu posty z hlavnej (main) casti projektu
// ostatne funkcie (loadConfig(), wifiConnect(), atd...) nic nerobia, iba sa tvaria ze funguju

// funkcie pre ultrazvukovy a infracerveny senzor

// funkcie na kontroly vracaju True ked je v schranke zaznamenana posta
// funkcie "Empty" su pre stav, ked je postova schranka prazdna, "NotEmpty" sa pouzivaju ked uz bola odoslana notifikacia a kontroluje sa pritomnost dalsej posty

#include <Arduino.h>

#define serialDBGOut(a); Serial.println(a);
#define DBG_SERIAL_BEGIN; Serial.begin(115200);

#define TRIGPIN1 13
#define ECHOPIN1 15
#define TRIGPIN2 13
#define ECHOPIN2 0
#define TRIGPIN3 13
#define ECHOPIN3 4
#define IRPIN1 12
#define IRPIN2 14
#define IRPIN3 16
#define WIFI_CRED_LENGTH 30
#define NVSNAME "MAILBOX"
#define SOUND_SPEED 0.017
#define WIFI_RETRY_CON_DELAY 200
#define WIFI_MAX_CONNECT_ATTEMPTS 20
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define FATAL_ERROR_RESET_TIME 20000000
#define ULTRA_SEQUENCE_DELAY 10
#define RESET_BLE_CONFIG_SECONDS 30
#define RESET_BLE_CONFIG_SECONDS_REQUIRED 600
#define ADDITIONAL_TASKS_FREQUENCY 100
#define MAX_PATH_LENGTH 100
#define IR_WAIT_TRIG 10
#define ULTRA_SET_EMPTY_SAMPLES 3

struct UltraVals{
  float fDistEmptyCM1;       // merana vzdialenost, ked je schranka prazdna
  float fDistEmptyCM2;
  float fDistEmptyCM3;
  float fDistLastCM1;
  float fDistLastCM2;
  float fDistLastCM3;
  int iUltraCheckInterval;
  int iUltraExtraChecks;
  int iUltraExtraChecksIntervalMS;
  float fUltraTolerance;
  bool bMailEmpty;
};

extern UltraVals UltraV;

void ultraSetEmpty();
void ultraSetLast();
float ultraMeasure1();
float ultraMeasure2();
float ultraMeasure3();
bool ultraCheck1Empty();
bool ultraCheck2Empty();
bool ultraCheck3Empty();
bool ultraCheckAllEmpty();
bool ultraCheck1NotEmpty();
bool ultraCheck2NotEmpty();
bool ultraCheck3NotEmpty();
bool ultraCheckAllNotEmpty();
bool IRCheck4();
int checkMail();
void setMailEmpty(bool in);

RTC_DATA_ATTR UltraVals UltraV;    // prezije spanok








int loadConfig(){
  //Serial.println("configs loaded");
  return 1;
}

int wifiConnect(){
  //Serial.println("wifi connected");
  return 1;
}

int sendEmptyMailNotif(){
  Serial.println("poslany empty mail event");
  return 1;
}

int sendNewMailNotif(){
  Serial.println("poslany new mail event");
  return 1;
}







int pinInit(){
  //serialDBGOut("nastavenie pinov");
  pinMode(TRIGPIN1, OUTPUT);
  pinMode(ECHOPIN1, INPUT);
  pinMode(TRIGPIN2, OUTPUT);
  pinMode(ECHOPIN2, INPUT);
  pinMode(TRIGPIN3, OUTPUT);
  pinMode(ECHOPIN3, INPUT);

  pinMode(IRPIN1, INPUT);
  pinMode(IRPIN2, INPUT);
  pinMode(IRPIN3, INPUT);
  return 1;
}

void setDefaults(){
  serialDBGOut("nastavenie defaultnych senzorovych nastaveni");
  UltraV.iUltraCheckInterval = 7000000;          //doba spanku medzi kontrolou posty
  UltraV.iUltraExtraChecks = 4;            //kolko krat sa pri detekcii posty kontroluje navyse na zabranenie falosnych poplachov
  UltraV.iUltraExtraChecksIntervalMS = 500;      //cas medzi extra kontrolami
  UltraV.fUltraTolerance = 0.1;              //tolerancia/citlivost ultrazvukoveho senzora, mensie znamena citlivejsie, minimum/default = 0
  
}








void ultraSetEmpty(){
  serialDBGOut("nastavenie velkosti prazdnej schranky:");
  pinInit();
  int i;
  float x = 0;
  for(i = ULTRA_SET_EMPTY_SAMPLES; i!=0; i--){
    x += ultraMeasure1();
  }
  UltraV.fDistEmptyCM1 = x/ULTRA_SET_EMPTY_SAMPLES;
  serialDBGOut(UltraV.fDistEmptyCM1);
  x = 0;
  delay(ULTRA_SEQUENCE_DELAY);
  for(i = ULTRA_SET_EMPTY_SAMPLES; i!=0; i--){
    x += ultraMeasure2();
  }
  UltraV.fDistEmptyCM2 = x/ULTRA_SET_EMPTY_SAMPLES;
  serialDBGOut(UltraV.fDistEmptyCM2);
  x = 0;
  delay(ULTRA_SEQUENCE_DELAY);
  for(i = ULTRA_SET_EMPTY_SAMPLES; i!=0; i--){
    x += ultraMeasure3();
  }
  UltraV.fDistEmptyCM3 = x/ULTRA_SET_EMPTY_SAMPLES;
  serialDBGOut(UltraV.fDistEmptyCM3);
  UltraV.bMailEmpty = true;
  return;
}

void ultraSetLast(){
  serialDBGOut("nastavenie momentalnej velkosti schranky:");
  pinInit();
  int i;
  float x = 0;
  for(i = ULTRA_SET_EMPTY_SAMPLES; i!=0; i--){
    x += ultraMeasure1();
  }
  UltraV.fDistLastCM1 = x/ULTRA_SET_EMPTY_SAMPLES;
  serialDBGOut(UltraV.fDistLastCM1);
  x = 0;
  delay(ULTRA_SEQUENCE_DELAY);
  for(i = ULTRA_SET_EMPTY_SAMPLES; i!=0; i--){
    x += ultraMeasure2();
  }
  UltraV.fDistLastCM2 = x/ULTRA_SET_EMPTY_SAMPLES;
  serialDBGOut(UltraV.fDistLastCM2);
  x = 0;
  delay(ULTRA_SEQUENCE_DELAY);
  for(i = ULTRA_SET_EMPTY_SAMPLES; i!=0; i--){
    x += ultraMeasure3();
  }
  UltraV.fDistLastCM3 = x/ULTRA_SET_EMPTY_SAMPLES;
  serialDBGOut(UltraV.fDistLastCM3);
  return;
}

float ultraMeasure1(){
  digitalWrite(TRIGPIN1, LOW);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN1, LOW);
  return SOUND_SPEED * pulseIn(ECHOPIN1, HIGH);
  
}

float ultraMeasure2(){
  digitalWrite(TRIGPIN2, LOW);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN2, LOW);
  return SOUND_SPEED * pulseIn(ECHOPIN2, HIGH);
  
}

float ultraMeasure3(){
  digitalWrite(TRIGPIN3, LOW);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN3, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN3, LOW);
  return SOUND_SPEED * pulseIn(ECHOPIN3, HIGH);
  
}

bool ultraCheck1Empty(){
  if((ultraMeasure1() + UltraV.fUltraTolerance) < UltraV.fDistEmptyCM1){
    return true;
  }
  else{
    return false;
  }
}

bool ultraCheck2Empty(){
  if((ultraMeasure2() + UltraV.fUltraTolerance) < UltraV.fDistEmptyCM2){
    return true;
  }
  else{
    return false;
  }
}

bool ultraCheck3Empty(){
  if((ultraMeasure3() + UltraV.fUltraTolerance) < UltraV.fDistEmptyCM3){
    return true;
  }
  else{
    return false;
  }
}

bool ultraCheckAllEmpty(){
  if(ultraCheck1Empty()){
    return true;
  }
  delay(ULTRA_SEQUENCE_DELAY);
  if(ultraCheck2Empty()){
    return true;
  }
  delay(ULTRA_SEQUENCE_DELAY);
  if(ultraCheck3Empty()){
    return true;
  }
  return false;
}

bool ultraCheck1NotEmpty(){
  if((ultraMeasure1() + UltraV.fUltraTolerance) < UltraV.fDistLastCM1){
    return true;
  }
  else{
    return false;
  }
}

bool ultraCheck2NotEmpty(){
  if((ultraMeasure2() + UltraV.fUltraTolerance) < UltraV.fDistLastCM2){
    return true;
  }
  else{
    return false;
  }
}

bool ultraCheck3NotEmpty(){
  if((ultraMeasure3() + UltraV.fUltraTolerance) < UltraV.fDistLastCM3){
    return true;
  }
  else{
    return false;
  }
}

bool ultraCheckAllNotEmpty(){
  if(ultraCheck1NotEmpty()){
    return true;
  }
  delay(ULTRA_SEQUENCE_DELAY);
  if(ultraCheck2NotEmpty()){
    return true;
  }
  delay(ULTRA_SEQUENCE_DELAY);
  if(ultraCheck3NotEmpty()){
    return true;
  }
  return false;
}

bool IRCheck3(){
  //digitalWrite(IRTRIGPIN, HIGH);
  //delay(IR_WAIT_TRIG);
  if(!digitalRead(IRPIN1) || !digitalRead(IRPIN2) || !digitalRead(IRPIN3)){
    //digitalWrite(IRTRIGPIN, LOW);
    return true;
  }
  else{
    //digitalWrite(IRTRIGPIN, LOW);
    return false;
  }
  
}


int checkMail(){
  serialDBGOut("kontrola posty");
  pinInit();
  if(UltraV.bMailEmpty){
    if(ultraCheckAllEmpty() || IRCheck3()){
      int i;
      for(i=UltraV.iUltraExtraChecks; i!=0; i--){
        delay(UltraV.iUltraExtraChecksIntervalMS);
        if(!(ultraCheckAllEmpty() || IRCheck3())){
          return 0;
        }
      }
      if(!loadConfig()){
        serialDBGOut("C1 : Chyba pri nacitani nastaveni");
        esp_deep_sleep(FATAL_ERROR_RESET_TIME);
      }
      if(!wifiConnect()){
        serialDBGOut("pripojenie wifi zlyhalo");
        return 0;
        
      }
      sendNewMailNotif();
      ultraSetLast();
      UltraV.bMailEmpty = false;
      return 1;
    }
  }
  else{
    if(! IRCheck3()){
      if(! ultraCheckAllEmpty()){
        if(!loadConfig()){
          serialDBGOut("C1 : Chyba pri nacitani nastaveni");
          esp_deep_sleep(FATAL_ERROR_RESET_TIME);
        }
        if(!wifiConnect()){
          serialDBGOut("pripojenie wifi zlyhalo");
          return 0;
        
        }
        sendEmptyMailNotif();
        UltraV.bMailEmpty = true;
        return 1;
      }
    }
    if(ultraCheckAllNotEmpty()){
      int i;
      for(i=UltraV.iUltraExtraChecks; i!=0; i--){
        delay(UltraV.iUltraExtraChecksIntervalMS);
        if(! ultraCheckAllNotEmpty()){
          return 0;
        }
      }
      if(!loadConfig()){
        serialDBGOut("C1 : Chyba pri nacitani nastaveni");
        esp_deep_sleep(FATAL_ERROR_RESET_TIME);
      }
      if(!wifiConnect()){
        serialDBGOut("pripojenie wifi zlyhalo");
        return 0;
        
      }
      sendNewMailNotif();
      ultraSetLast();
      return 1;
    }
  }
  return 0;
}

int checkMailNoIR(){
  serialDBGOut("kontrola posty - bez IR");
  pinInit();
  if(UltraV.bMailEmpty){
    if(ultraCheckAllEmpty()){
      int i;
      for(i=UltraV.iUltraExtraChecks; i!=0; i--){
        delay(UltraV.iUltraExtraChecksIntervalMS);
        if(!(ultraCheckAllEmpty())){
          return 0;
        }
      }
      if(!loadConfig()){
        serialDBGOut("C1 : Chyba pri nacitani nastaveni");
        esp_deep_sleep(FATAL_ERROR_RESET_TIME);
      }
      if(!wifiConnect()){
        serialDBGOut("pripojenie wifi zlyhalo");
        return 0;
        
      }
      sendNewMailNotif();
      ultraSetLast();
      UltraV.bMailEmpty = false;
      return 1;
    }
  }
  else{
      if(! ultraCheckAllEmpty()){
        if(!loadConfig()){
          serialDBGOut("C1 : Chyba pri nacitani nastaveni");
          esp_deep_sleep(FATAL_ERROR_RESET_TIME);
        }
        if(!wifiConnect()){
          serialDBGOut("pripojenie wifi zlyhalo");
          return 0;
        
        }
        sendEmptyMailNotif();
        UltraV.bMailEmpty = true;
        return 1;
      }
    if(ultraCheckAllNotEmpty()){
      int i;
      for(i=UltraV.iUltraExtraChecks; i!=0; i--){
        delay(UltraV.iUltraExtraChecksIntervalMS);
        if(! ultraCheckAllNotEmpty()){
          return 0;
        }
      }
      if(!loadConfig()){
        serialDBGOut("C1 : Chyba pri nacitani nastaveni");
        esp_deep_sleep(FATAL_ERROR_RESET_TIME);
      }
      if(!wifiConnect()){
        serialDBGOut("pripojenie wifi zlyhalo");
        return 0;
        
      }
      sendNewMailNotif();
      ultraSetLast();
      return 1;
    }
  }
  return 0;
}


void setMailEmpty(bool in){
  UltraV.bMailEmpty = in;
}











void setup(){
  Serial.begin(115200);
  Serial.println("start");
  setDefaults();
  pinInit();
  delay(5000);
  ultraSetEmpty();
}

void loop(){
  delay(5000);
  //checkMail();
  checkMailNoIR();
}

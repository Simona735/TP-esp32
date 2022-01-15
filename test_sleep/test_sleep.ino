
#include <Arduino.h>
#include <rom/rtc.h>


struct teststruct{
  int incr;
  int fix;
  char str[20];
};

RTC_DATA_ATTR teststruct prenosna;


void setup() {
  
  Serial.begin(9600);
  Serial.println("start");
  for(int i = 0; i < 10; i++){
    Serial.println(rtc_get_reset_reason(0));
    delay(1000);
  }
  if(rtc_get_reset_reason(0)==1){
    Serial.println("restarted, values set");
    prenosna.incr = 0;
    prenosna.fix = 158;
    //prenosna.str = "AHOJ";
    strcpy(prenosna.str, "AHOJ");
  }
  if(rtc_get_reset_reason(0)==5){
    Serial.println("woke");
    Serial.println("incr:");
    Serial.println(prenosna.incr);
    Serial.println("fix:");
    Serial.println(prenosna.fix);
    Serial.println("str:");
    Serial.println(prenosna.str);
    prenosna.incr++;
  }
  Serial.println("sleep.");









  esp_deep_sleep(5000000);
}

void loop() {
  

}

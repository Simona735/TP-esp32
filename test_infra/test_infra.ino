
#include <Arduino.h>

#define IRPIN1 12
#define IRPIN2 14
#define IRPIN3 16


void setup() {
  Serial.begin(115200); // Starts the serial communication

  pinMode(IRPIN1, INPUT);
  pinMode(IRPIN2, INPUT);
  pinMode(IRPIN3, INPUT);

}

void loop() {
  delay(2000);
  
  Serial.println("IRPIN1");
  Serial.println(digitalRead(IRPIN1));
  Serial.println("IRPIN2");
  Serial.println(digitalRead(IRPIN2));
  Serial.println("IRPIN3");
  Serial.println(digitalRead(IRPIN3));



}

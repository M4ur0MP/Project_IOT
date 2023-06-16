#include <Arduino.h>
#define rotacionIn A0
float lec;
void setup() {
  Serial.begin(115200);
  pinMode(rotacionIn, INPUT);
  // put your setup code here, to run once:
}

void loop() {
  lec = analogRead(rotacionIn);
 
  Serial.println(lec);
   delay(1000);
  // put your main code here, to run repeatedly:
}
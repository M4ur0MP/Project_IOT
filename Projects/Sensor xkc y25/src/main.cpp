#include <Arduino.h>

int li = 0;
void setup() {
  pinMode (5,INPUT);
  pinMode (8, OUTPUT);
}

void loop() {
  
  li = digitalRead(5);
  if (li==HIGH){
  digitalWrite (8, HIGH);
  }else
  {
    digitalWrite (8,LOW);
  }
  
}
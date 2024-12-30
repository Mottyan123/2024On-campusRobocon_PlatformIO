#include <Arduino.h>
const int moter1_pin1 = 32;
const int moter1_pin2 = 33;

void setup(){ //おまじない
  Serial.begin(115200);

  pinMode(moter1_pin1, OUTPUT);
  analogWrite(moter1_pin1, 0);

  pinMode(moter1_pin2, OUTPUT);
  analogWrite(moter1_pin2, 0);
  //pinMode(moter1_pin1, OUTPUT);
}

void loop(){

  // analogWrite(moter1_pin1, 0);
  // analogWrite(moter1_pin2, 128); //最大値128 //
  // delay(1000);

  analogWrite(moter1_pin1, 128); //最小値-127 //
  analogWrite(moter1_pin2, 0);
  delay(1000);
}
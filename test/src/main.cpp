#include <Arduino.h>
#include <ESP32Servo.h>

Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;

const int SV_PIN1 = 33;
const int SV_PIN2 = 25;
const int SV_PIN3 = 26;
const int SV_PIN4 = 27;

int servo_angle1 = 180;
int servo_angle2 = 0;

void setup(){
  myservo1.attach(SV_PIN1,500,2400);
  myservo1.write(servo_angle1);
  myservo2.attach(SV_PIN2,500,2400);
  myservo2.write(servo_angle2);
  myservo3.attach(SV_PIN3,500,2400);
  myservo3.write(servo_angle1);
  myservo4.attach(SV_PIN4,500,2400);
  myservo4.write(servo_angle1);
  
  delay(100);
}

void loop(){
  // myservo1.write(round(180)); //open
  // // myservo2.write(round(180));
  // // myservo3.write(round(180));
  // // myservo4.write(round(180));

  // delay(3000);

  // myservo1.write(round(135)); //close

  // delay(2000);

  // myservo1.write(round(180)); //open

  // delay(3000);

  // myservo1.write(round(145)); //close

  // delay(2000);

  // myservo2.write(round(90)); //turn

  // delay(2000);

  // myservo2.write(round(0)); //turn

  // delay(2000);

  // myservo1.write(0);
  // delay(2000);
  // myservo1.write(180);
  // delay(2000);

  // digitalWrite(SV_PIN1,HIGH);
  // delay(2000);
  // digitalWrite(SV_PIN1,LOW);
  // delay(2000);
}
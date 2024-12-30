#include <Arduino.h>
#include <PS4Controller.h>
#include <IcsHardSerialClass.h>
#include "CytronMotorDriver.h"

const byte EN_PIN = 2;
long BAUDRATE = 115200;
const int TIMEOUT = 10; //通信できていないか確認用にわざと遅めに設定

CytronMD motor1(PWM_DIR, 18, 19);//右前
CytronMD motor2(PWM_DIR, 33, 32);//右後
CytronMD motor3(PWM_DIR, 27, 26);//左前
CytronMD motor4(PWM_DIR, 12, 14);//左後

IcsHardSerialClass krs(&Serial2,EN_PIN,BAUDRATE,TIMEOUT);

void setup(){
  Serial.begin(115200);
  PS4.begin("08:b6:1f:ef:86:5a");

  krs.begin();
  krs.setPos(12,11500);
  krs.setPos(12,3500);
  delay(1000);
}
    
// int LY = 225*(PS4.LStickY()/128);
// int LR = 225*(PS4.LStickX()/128);
int speed = 180;

void loop(){
  //Serial.println(int(PS4.RStickY())); //PS4.LStickX()　で　傾きを指定　（初期０度）
  if (PS4.isConnected()) {
    if (PS4.L1()== 1){
      krs.setPos(11,11500);
      delay(100);
    }
    if (PS4.R1()== 1){
      krs.setPos(11,3500);
      delay(100);
    }
    if (PS4.L1()== 0 || PS4.R1()== 0){
      krs.setPos(11,7500);
      delay(100);
    }
    if (PS4.L2() > 0.3){
      motor1.setSpeed(speed);
      motor2.setSpeed(speed);
      motor3.setSpeed(speed);
      motor4.setSpeed(speed);
      delay(650);
    }
    if (PS4.R2() > 0.3){
      motor1.setSpeed(-speed);
      motor2.setSpeed(-speed);
      motor3.setSpeed(-speed);
      motor4.setSpeed(-speed);
      delay(650);
    }
    if (PS4.Up()== 1){
      Serial.println("1");
      motor1.setSpeed(speed);
      motor2.setSpeed(speed);
      motor3.setSpeed(-speed);
      motor4.setSpeed(-speed);
    }
    if (PS4.Down()== 1){
      motor1.setSpeed(-speed);
      motor2.setSpeed(-speed);
      motor3.setSpeed(speed);
      motor4.setSpeed(speed);
    }
    if (PS4.Right() == 1){
      motor1.setSpeed(speed);
      motor2.setSpeed(-speed);
      motor3.setSpeed(-speed);
      motor4.setSpeed(speed);
    }
    if (PS4.Left() == 1){
      motor1.setSpeed(-speed);
      motor2.setSpeed(speed);
      motor3.setSpeed(speed);
      motor4.setSpeed(-speed);
    }
    if(PS4.LStickY() > 30){
      Serial.println("2");
      motor1.setSpeed(speed);
      motor2.setSpeed(speed);
      motor3.setSpeed(-speed);
      motor4.setSpeed(-speed);
    }
    if(PS4.LStickY() < -30){
      motor1.setSpeed(-speed);
      motor2.setSpeed(-speed);
      motor3.setSpeed(speed);
      motor4.setSpeed(speed);
    }
    if(PS4.LStickX() > 30){
      motor1.setSpeed(speed);
      motor2.setSpeed(-speed);
      motor3.setSpeed(-speed);
      motor4.setSpeed(speed);
    }
    if(PS4.LStickX() < -30){
      motor1.setSpeed(-speed);
      motor2.setSpeed(speed);
      motor3.setSpeed(speed);
      motor4.setSpeed(-speed);
    }
    // if((PS4.LStickY() > 30 && PS4.LStickX() > 30) || (PS4.LStickY() < 30 && PS4.LStickX() < 30)){
    //   analogWrite(moter1_pin2, 0);
    //   analogWrite(moter2_pin2, abs(PS4.LStickY()));
    //   analogWrite(moter3_pin2, abs(PS4.LStickY()));
    //   analogWrite(moter4_pin2, 0);
    // }
    // if((PS4.LStickY() > 30 && PS4.LStickX() < 30) || (PS4.LStickY() < 30 && PS4.LStickX() > 30)){
    //   analogWrite(moter1_pin2, abs(PS4.LStickY()));
    //   analogWrite(moter2_pin2, 0);
    //   analogWrite(moter3_pin2, 0);
    //   analogWrite(moter1_pin2, abs(PS4.LStickY()));
    // }
    if (PS4.RStickY() > 30 && PS4.RStickY() < -30){
      int ARM15 = 4700*(PS4.RStickY()/128);
      int ARM10 = 2670*(PS4.RStickY()/128);
      krs.setPos(10,8400+ARM10);
      krs.setPos(15,8830-ARM15);
    }
    if (PS4.Touchpad() == 1) {
      Serial.println("0");
      motor1.setSpeed(0);
      motor2.setSpeed(0);
      motor3.setSpeed(0);
      motor4.setSpeed(0);
    }
  }

  delay(10);

}

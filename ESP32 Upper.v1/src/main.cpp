#include <Arduino.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <math.h>
#include "SSD1306.h"
#include "CytronMotorDriver.h"

Servo myservo1;          // Servoオブジェクトの宣言
Servo myservo2;

const int SV_PIN1 = 12;   // サーボモーターをデジタルピン設定
const int SV_PIN2 = 13;

int servo_angle1 = 90; //サーボ角度用変数（電源ON時に初期化）
int servo_angle2 = 0;

const int SY_PIN1 = 27; //シリンダーのピン設定

const int motorPin5 = 25; //モーターのピン設定
const int motorPin6 = 26;

//モータ設定
CytronMD motor1(PWM_DIR, 25, 26);  // PWM 1 = Pin , DIR 1 = Pin .

bool flag = false; //ディスプレイ用停止フラッグ
bool flag1 = false;
String data1 = ""; //ディスプレイ用変数
String data2 = "";
SSD1306 display(0x3c,21,22); //ディスプレイの規格

void setup() {
  Serial.begin(115200); //ポート開放

  display.init(); //初期状態のディスプレイ表示設定
  display.setFont(ArialMT_Plain_16);
  display.drawString(0,0,"program reset...");
  display.display();
  delay(1000);
  display.clear();
  display.display();
  display.drawString(0,0,"ESP32 Upper");
  display.drawString(0,20,"select button");
  display.display();

  myservo1.attach(SV_PIN1, 500, 2400);  // サーボの割当(パルス幅500~2400msに指定)
  myservo1.write(servo_angle1); //サーボ角度をリセット
  myservo2.attach(SV_PIN2, 500, 2400);
  myservo2.write(servo_angle2);

  digitalWrite(SY_PIN1, 1); // シリンダーの初期位置
  pinMode(SY_PIN1, OUTPUT); //シリンダーのピンをアウトプットに設定

  delay(100);
}

void loop() {
  if (Serial.available()) { //シリアル読みとり
    String data = Serial.readStringUntil('\n');
    float firstDelimiterIndex = data.indexOf(':');

    if (firstDelimiterIndex > 0){ //コマンド判定
      String command = data.substring(0, firstDelimiterIndex);
      float secondDelimiterIndex = data.indexOf(':', firstDelimiterIndex + 1);
      
      if (secondDelimiterIndex > 0) { //数値個数判定
        float thirdDelimiterIndex = data.indexOf(':',secondDelimiterIndex + 1);

        if (thirdDelimiterIndex > 0){ //数値3つ
          float firstValue = data.substring(firstDelimiterIndex + 1, secondDelimiterIndex).toInt();
          float secondValue = data.substring(secondDelimiterIndex + 1, thirdDelimiterIndex).toInt();
          float thirdValue = data.substring(thirdDelimiterIndex + 1).toInt();

          //ここに数値３つの制御

        } else { //数値2つ
          float firstValue = data.substring(firstDelimiterIndex + 1, secondDelimiterIndex).toInt();
          float secondValue = data.substring(secondDelimiterIndex + 1).toInt();

          //ここに数値2つの制御

        } 
      } else { //数値1つ
        float firstValue = data.substring(firstDelimiterIndex + 1).toInt();
        if (command == "KH"){ //シリンダー上昇
          if (firstValue == 1){
            digitalWrite(SY_PIN1, 0);
            myservo2.write(round(180));
          } 
        }
        if (command == "KL"){ //シリンダー下降
          if (firstValue == 1){
            digitalWrite(SY_PIN1, 1);
            myservo2.write(round(0));
          } 
        }
        if (command == "KD"){ //排出開始
          if (firstValue == 1){
            motor1.setSpeed(50);
          } 
        }
        if (command == "KS"){ //排出停止
          if (firstValue == 1){
            motor1.setSpeed(0);
          } 
        }
      }
      //ディスプレイ反映
      if (!flag){
        if (command == "redpro" || "bluepro" || "conpro"){
          data1 = data.substring(firstDelimiterIndex + 1);
          display.clear();
          display.display();
          display.drawString(0,0,"ESP32 Upper");
          display.drawString(0,20,data1);
          display.drawString(0,40,"prs space to start");
          display.display();
          flag = true;
        }
      }
      if (!flag1){
        if (command == "prostart"){
          data2 = data.substring(firstDelimiterIndex + 1);
          display.clear();
          display.display();
          display.drawString(0,0,"ESP32 Upper");
          display.drawString(0,20,data2);
          display.drawString(0,40,"proglam start...");
          display.display();
          flag1 = true;
        }
      // } else if (flag){ //ここのディスプレイの出力を行うと遅延が発生する
      //   display.clear();
      //   display.display();
      //   display.drawString(0,0,"ESP32 motion");
      //   display.drawString(0,20,data1);
      //   display.drawString(0,40,data);
      //   display.drawString(110,20,data2);
      //   display.display();
      }
    }
  }
}


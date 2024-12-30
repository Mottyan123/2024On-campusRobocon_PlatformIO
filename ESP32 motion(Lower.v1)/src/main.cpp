#include <Arduino.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include "SSD1306.h"

Servo myservo1;          // Servoオブジェクトの宣言
Servo myservo2;

const int SV_PIN1 = 12;   // サーボモーターをデジタルピン設定
const int SV_PIN2 = 13;

int servo_angle1 = 90; //サーボ角度用変数（電源ON時に初期化）
int servo_angle2 = 0;

const int motorPin1 = 16; //RR
const int motorPin2 = 17;
const int motorPin3 = 18; //FR
const int motorPin4 = 19;
const int motorPin5 = 25; //RL
const int motorPin6 = 26;
const int motorPin7 = 32; //FL
const int motorPin8 = 33;

bool flag = false;
String data1 = "";
String data2 = "";
SSD1306 display(0x3c,21,22);

void setup() {
  Serial.begin(115200);

  display.init();
  display.setFont(ArialMT_Plain_16);
  display.drawString(0,0,"program reset...");
  display.display();
  delay(1000);
  display.clear();
  display.display();
  display.drawString(0,0,"ESP32 motion");
  display.drawString(0,20,"select button");
  display.display();

  myservo1.attach(SV_PIN1, 500, 2400);  // サーボの割当(パルス幅500~2400msに指定)
  myservo1.write(servo_angle1);//サーボ角度を0度にリセット
  myservo2.attach(SV_PIN2, 500, 2400);
  myservo2.write(servo_angle2);

  // モーターピンを出力として設定
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(motorPin5, OUTPUT);
  pinMode(motorPin6, OUTPUT);
  pinMode(motorPin7, OUTPUT);
  pinMode(motorPin8, OUTPUT);
  //pinMode(enablePin, OUTPUT);

  // モーターを停止状態に設定
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 0);
  analogWrite(motorPin5, 0);
  analogWrite(motorPin6, 0);
  analogWrite(motorPin7, 0);
  analogWrite(motorPin8, 0);
  //analogWrite(enablePin, 0);  // モーターのスピードを0に設定
  delay(100);
}

void loop() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    int firstDelimiterIndex = data.indexOf(':');

    if (firstDelimiterIndex > 0){
      String command = data.substring(0, firstDelimiterIndex);
      int secondDelimiterIndex = data.indexOf(':', firstDelimiterIndex + 1);
      
      if (secondDelimiterIndex > 0) {
        int thirdDelimiterIndex = data.indexOf(':',secondDelimiterIndex + 1);

        if (thirdDelimiterIndex > 0){ //数値3つ
          int firstValue = data.substring(firstDelimiterIndex + 1, secondDelimiterIndex).toInt();
          int secondValue = data.substring(secondDelimiterIndex + 1, thirdDelimiterIndex).toInt();
          int thirdValue = data.substring(thirdDelimiterIndex + 1).toInt();

          //オムニホイール制御
          if (command == "LYLXRX") {
            int LSY_V = firstValue;
            int LSX_V = secondValue;
            int RSX_V = thirdValue;
            if (abs(LSY_V) > 10 || abs(LSX_V) > 10){
              if (RSX_V > 10){ //右旋回を含む
                if (LSY_V < -10 && abs(LSX_V) < 10){ //前
                  analogWrite(motorPin1,0 + RSX_V);
                  analogWrite(motorPin2,abs(LSY_V));
                  analogWrite(motorPin3,0 + RSX_V);
                  analogWrite(motorPin4,abs(LSY_V));
                  analogWrite(motorPin5,abs(LSY_V) + RSX_V);
                  analogWrite(motorPin6,0);
                  analogWrite(motorPin7,abs(LSY_V) + RSX_V);
                  analogWrite(motorPin8,0);
                  data2 = 12;
                }
                if (LSY_V > 10 && abs(LSX_V) < 10){ //後
                  analogWrite(motorPin1,abs(LSY_V) + RSX_V);
                  analogWrite(motorPin2,0);
                  analogWrite(motorPin3,abs(LSY_V) + RSX_V);
                  analogWrite(motorPin4,0);
                  analogWrite(motorPin5,0 + RSX_V);
                  analogWrite(motorPin6,abs(LSY_V));
                  analogWrite(motorPin7,0 + RSX_V);
                  analogWrite(motorPin8,abs(LSY_V));
                  data2 = 13;
                }
                if (abs(LSY_V) < 10 && LSX_V > 10){ //右
                  analogWrite(motorPin1,0 + RSX_V);
                  analogWrite(motorPin2,abs(LSX_V));
                  analogWrite(motorPin3,abs(LSX_V) + RSX_V);
                  analogWrite(motorPin4,0);
                  analogWrite(motorPin5,0 + RSX_V);
                  analogWrite(motorPin6,abs(LSX_V));
                  analogWrite(motorPin7,abs(LSX_V) + RSX_V);
                  analogWrite(motorPin8,0);
                  data2 = 14;
                }
                if (abs(LSY_V) < 10 && LSX_V < -10){ //左
                  analogWrite(motorPin1,abs(LSX_V) + RSX_V);
                  analogWrite(motorPin2,0);
                  analogWrite(motorPin3,0 + RSX_V);
                  analogWrite(motorPin4,abs(LSX_V));
                  analogWrite(motorPin5,abs(LSX_V) + RSX_V);
                  analogWrite(motorPin6,0);
                  analogWrite(motorPin7,0 + RSX_V);
                  analogWrite(motorPin8,abs(LSX_V));
                  data2 = 15;
                }
                if (LSY_V < -10 && LSX_V > 10){ //第1象限
                  analogWrite(motorPin1,0 + RSX_V);
                  analogWrite(motorPin2,abs(LSY_V + LSX_V));
                  analogWrite(motorPin3,abs(LSX_V) + RSX_V);
                  analogWrite(motorPin4,abs(LSY_V));
                  analogWrite(motorPin5,abs(LSY_V) + RSX_V);
                  analogWrite(motorPin6,abs(LSX_V));
                  analogWrite(motorPin7,abs(LSY_V + LSX_V) + RSX_V);
                  analogWrite(motorPin8,0);
                  data2 = 16;
                }
                if (LSY_V < -10 && LSX_V < -10){ //第2象限
                  analogWrite(motorPin1,abs(LSX_V) + RSX_V);
                  analogWrite(motorPin2,abs(LSY_V));
                  analogWrite(motorPin3,0 + RSX_V);
                  analogWrite(motorPin4,abs(LSY_V - LSX_V));
                  analogWrite(motorPin5,abs(LSY_V - LSX_V) + RSX_V);
                  analogWrite(motorPin6,0);
                  analogWrite(motorPin7,abs(LSY_V) + RSX_V);
                  analogWrite(motorPin8,abs(LSX_V));
                  data2 = 17;
                }
                if (LSY_V > 10 && LSX_V < -10){ //第3象限
                  analogWrite(motorPin1,abs(LSY_V + LSX_V) + RSX_V);
                  analogWrite(motorPin2,0);
                  analogWrite(motorPin3,abs(LSY_V) + RSX_V);
                  analogWrite(motorPin4,abs(LSX_V));
                  analogWrite(motorPin5,abs(LSX_V) + RSX_V);
                  analogWrite(motorPin6,abs(LSY_V));
                  analogWrite(motorPin7,0 + RSX_V);
                  analogWrite(motorPin8,abs(LSY_V + LSX_V));
                  data2 = 18;
                }
                if (LSY_V > 10 && LSX_V > 10){ //第4象限
                  analogWrite(motorPin1,abs(LSY_V) + RSX_V);
                  analogWrite(motorPin2,abs(LSX_V));
                  analogWrite(motorPin3,abs(LSY_V - LSX_V) + RSX_V);
                  analogWrite(motorPin4,0);
                  analogWrite(motorPin5,0 + RSX_V);
                  analogWrite(motorPin6,abs(LSY_V - LSX_V));
                  analogWrite(motorPin7,abs(LSX_V) + RSX_V);
                  analogWrite(motorPin8,abs(LSY_V));
                  data2 = 19;
                }
              }
              if (RSX_V < -10){ //左旋回を含む
                if (LSY_V < -10 && abs(LSX_V) < 10){ //前
                  analogWrite(motorPin1,0);
                  analogWrite(motorPin2,abs(LSY_V) - RSX_V);
                  analogWrite(motorPin3,0);
                  analogWrite(motorPin4,abs(LSY_V) - RSX_V);
                  analogWrite(motorPin5,abs(LSY_V));
                  analogWrite(motorPin6,0 - RSX_V);
                  analogWrite(motorPin7,abs(LSY_V));
                  analogWrite(motorPin8,0 - RSX_V);
                  data2 = 20;
                }
                if (LSY_V > 10 && abs(LSX_V) < 10){ //後
                  analogWrite(motorPin1,abs(LSY_V));
                  analogWrite(motorPin2,0 - RSX_V);
                  analogWrite(motorPin3,abs(LSY_V));
                  analogWrite(motorPin4,0 - RSX_V);
                  analogWrite(motorPin5,0);
                  analogWrite(motorPin6,abs(LSY_V) - RSX_V);
                  analogWrite(motorPin7,0);
                  analogWrite(motorPin8,abs(LSY_V) - RSX_V);
                  data2 = 21;
                }
                if (abs(LSY_V) < 10 && LSX_V > 10){ //右
                  analogWrite(motorPin1,0);
                  analogWrite(motorPin2,abs(LSX_V) - RSX_V);
                  analogWrite(motorPin3,abs(LSX_V));
                  analogWrite(motorPin4,0 - RSX_V);
                  analogWrite(motorPin5,0);
                  analogWrite(motorPin6,abs(LSX_V) - RSX_V);
                  analogWrite(motorPin7,abs(LSX_V));
                  analogWrite(motorPin8,0 - RSX_V);
                  data2 = 22;
                }
                if (abs(LSY_V) < 10 && LSX_V < -10){ //左
                  analogWrite(motorPin1,abs(LSX_V));
                  analogWrite(motorPin2,0 - RSX_V);
                  analogWrite(motorPin3,0);
                  analogWrite(motorPin4,abs(LSX_V) - RSX_V);
                  analogWrite(motorPin5,abs(LSX_V));
                  analogWrite(motorPin6,0 - RSX_V);
                  analogWrite(motorPin7,0);
                  analogWrite(motorPin8,abs(LSX_V) - RSX_V);
                  data2 = 23;
                }
                if (LSY_V < -10 && LSX_V > 10){ //第1象限
                  analogWrite(motorPin1,0);
                  analogWrite(motorPin2,abs(LSY_V + LSX_V) - RSX_V);
                  analogWrite(motorPin3,abs(LSX_V));
                  analogWrite(motorPin4,abs(LSY_V) - RSX_V);
                  analogWrite(motorPin5,abs(LSY_V));
                  analogWrite(motorPin6,abs(LSX_V) - RSX_V);
                  analogWrite(motorPin7,abs(LSY_V + LSX_V));
                  analogWrite(motorPin8,0 - RSX_V);
                  data2 = 24;
                }
                if (LSY_V < -10 && LSX_V < -10){ //第2象限
                  analogWrite(motorPin1,abs(LSX_V));
                  analogWrite(motorPin2,abs(LSY_V) - RSX_V);
                  analogWrite(motorPin3,0);
                  analogWrite(motorPin4,abs(LSY_V - LSX_V) - RSX_V);
                  analogWrite(motorPin5,abs(LSY_V - LSX_V));
                  analogWrite(motorPin6,0 - RSX_V);
                  analogWrite(motorPin7,abs(LSY_V));
                  analogWrite(motorPin8,abs(LSX_V) - RSX_V);
                  data2 = 25;
                }
                if (LSY_V > 10 && LSX_V < -10){ //第3象限
                  analogWrite(motorPin1,abs(LSY_V + LSX_V));
                  analogWrite(motorPin2,0 - RSX_V);
                  analogWrite(motorPin3,abs(LSY_V));
                  analogWrite(motorPin4,abs(LSX_V) - RSX_V);
                  analogWrite(motorPin5,abs(LSX_V));
                  analogWrite(motorPin6,abs(LSY_V) - RSX_V);
                  analogWrite(motorPin7,0);
                  analogWrite(motorPin8,abs(LSY_V + LSX_V) - RSX_V);
                  data2 = 26;
                }
                if (LSY_V > 10 && LSX_V > 10){ //第4象限
                  analogWrite(motorPin1,abs(LSY_V));
                  analogWrite(motorPin2,abs(LSX_V) - RSX_V);
                  analogWrite(motorPin3,abs(LSY_V - LSX_V));
                  analogWrite(motorPin4,0 - RSX_V);
                  analogWrite(motorPin5,0);
                  analogWrite(motorPin6,abs(LSY_V - LSX_V) - RSX_V);
                  analogWrite(motorPin7,abs(LSX_V));
                  analogWrite(motorPin8,abs(LSY_V) - RSX_V);
                  data2 = 27;
                }
              } 
              if (abs(RSX_V) < 10) { //旋回を含まない
                if (LSY_V < -10 && abs(LSX_V) < 10){ //前
                  analogWrite(motorPin1,0);
                  analogWrite(motorPin2,abs(LSY_V));
                  analogWrite(motorPin3,0);
                  analogWrite(motorPin4,abs(LSY_V));
                  analogWrite(motorPin5,abs(LSY_V));
                  analogWrite(motorPin6,0);
                  analogWrite(motorPin7,abs(LSY_V));
                  analogWrite(motorPin8,0);
                  data2 = 1;
                }
                if (LSY_V > 10 && abs(LSX_V) < 10){ //後
                  analogWrite(motorPin1,abs(LSY_V));
                  analogWrite(motorPin2,0);
                  analogWrite(motorPin3,abs(LSY_V));
                  analogWrite(motorPin4,0);
                  analogWrite(motorPin5,0);
                  analogWrite(motorPin6,abs(LSY_V));
                  analogWrite(motorPin7,0);
                  analogWrite(motorPin8,abs(LSY_V));
                  data2 = 2;
                }
                if (abs(LSY_V) < 10 && LSX_V > 10){ //右
                  analogWrite(motorPin1,0);
                  analogWrite(motorPin2,abs(LSX_V));
                  analogWrite(motorPin3,abs(LSX_V));
                  analogWrite(motorPin4,0);
                  analogWrite(motorPin5,0);
                  analogWrite(motorPin6,abs(LSX_V));
                  analogWrite(motorPin7,abs(LSX_V));
                  analogWrite(motorPin8,0);
                  data2 = 3;
                }
                if (abs(LSY_V) < 10 && LSX_V < -10){ //左
                  analogWrite(motorPin1,abs(LSX_V));
                  analogWrite(motorPin2,0);
                  analogWrite(motorPin3,0);
                  analogWrite(motorPin4,abs(LSX_V));
                  analogWrite(motorPin5,abs(LSX_V));
                  analogWrite(motorPin6,0);
                  analogWrite(motorPin7,0);
                  analogWrite(motorPin8,abs(LSX_V));
                  data2 = 4;
                }
                if (LSY_V < -10 && LSX_V > 10){ //第1象限
                  analogWrite(motorPin1,0);
                  analogWrite(motorPin2,abs(LSY_V + LSX_V));
                  analogWrite(motorPin3,abs(LSX_V));
                  analogWrite(motorPin4,abs(LSY_V));
                  analogWrite(motorPin5,abs(LSY_V));
                  analogWrite(motorPin6,abs(LSX_V));
                  analogWrite(motorPin7,abs(LSY_V + LSX_V));
                  analogWrite(motorPin8,0);
                  data2 = 5;
                }
                if (LSY_V < -10 && LSX_V < -10){ //第2象限
                  analogWrite(motorPin1,abs(LSX_V));
                  analogWrite(motorPin2,abs(LSY_V));
                  analogWrite(motorPin3,0);
                  analogWrite(motorPin4,abs(LSY_V - LSX_V));
                  analogWrite(motorPin5,abs(LSY_V - LSX_V));
                  analogWrite(motorPin6,0);
                  analogWrite(motorPin7,abs(LSY_V));
                  analogWrite(motorPin8,abs(LSX_V));
                  data2 = 6;
                }
                if (LSY_V > 10 && LSX_V < -10){ //第3象限
                  analogWrite(motorPin1,abs(LSY_V + LSX_V));
                  analogWrite(motorPin2,0);
                  analogWrite(motorPin3,abs(LSY_V));
                  analogWrite(motorPin4,abs(LSX_V));
                  analogWrite(motorPin5,abs(LSX_V));
                  analogWrite(motorPin6,abs(LSY_V));
                  analogWrite(motorPin7,0);
                  analogWrite(motorPin8,abs(LSY_V + LSX_V));
                  data2 = 7;
                }
                if (LSY_V > 10 && LSX_V > 10){ //第4象限
                  analogWrite(motorPin1,abs(LSY_V));
                  analogWrite(motorPin2,abs(LSX_V));
                  analogWrite(motorPin3,abs(LSY_V - LSX_V));
                  analogWrite(motorPin4,0);
                  analogWrite(motorPin5,0);
                  analogWrite(motorPin6,abs(LSY_V - LSX_V));
                  analogWrite(motorPin7,abs(LSX_V));
                  analogWrite(motorPin8,abs(LSY_V));
                  data2 = 8;
                }
              }
            } else if (abs(LSY_V) < 10 && abs(LSX_V) < 10 && abs(RSX_V) > 10){ //旋回のみ
              if (RSX_V > 10){ //右旋回
                analogWrite(motorPin1,abs(RSX_V)); 
                analogWrite(motorPin2,0);
                analogWrite(motorPin3,abs(RSX_V));
                analogWrite(motorPin4,0);
                analogWrite(motorPin5,abs(RSX_V));
                analogWrite(motorPin6,0);
                analogWrite(motorPin7,abs(RSX_V));
                analogWrite(motorPin8,0);
                data2 = 9;
              }
              if (RSX_V < -10){ //左旋回
                analogWrite(motorPin1,0);
                analogWrite(motorPin2,abs(RSX_V));
                analogWrite(motorPin3,0);
                analogWrite(motorPin4,abs(RSX_V));
                analogWrite(motorPin5,0);
                analogWrite(motorPin6,abs(RSX_V));
                analogWrite(motorPin7,0);
                analogWrite(motorPin8,abs(RSX_V));
                data2 = 10;
              }
            } else { //停止
              analogWrite(motorPin1,0);
              analogWrite(motorPin2,0);
              analogWrite(motorPin3,0);
              analogWrite(motorPin4,0);
              analogWrite(motorPin5,0);
              analogWrite(motorPin6,0);
              analogWrite(motorPin7,0);
              analogWrite(motorPin8,0);
              data2 = 11;
            }
          }
        } else { //数値2つ
          int firstValue = data.substring(firstDelimiterIndex + 1, secondDelimiterIndex).toInt();
          int secondValue = data.substring(secondDelimiterIndex + 1).toInt();

          if (command == "redcap"){
            if (firstValue >290 && firstValue <350 && secondValue >210 && secondValue < 270){
              myservo1.write(round(0));
            } else {
              myservo1.write(round(90));
            }  
          }
        } 
      } else { //数値1つ
        int firstValue = data.substring(firstDelimiterIndex + 1).toInt();
      }
      //ディスプレイ反映
      if (!flag){
        if (command == "redpro" || "bluepro" || "conpro"){
          data1 = data.substring(firstDelimiterIndex + 1);
          display.clear();
          display.display();
          display.drawString(0,0,"ESP32 motion");
          display.drawString(0,20,data1);
          display.drawString(0,40,"prs space to start");
          display.display();
          flag = true;
        }
      } else if (flag){
        display.clear();
        display.display();
        display.drawString(0,0,"ESP32 motion");
        display.drawString(0,20,data1);
        display.drawString(0,40,data);
        display.drawString(110,20,data2);
        display.display();
      }
    }
  }
}


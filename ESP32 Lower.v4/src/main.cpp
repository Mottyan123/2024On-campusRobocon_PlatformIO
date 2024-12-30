#include <Arduino.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <math.h>
#include "SSD1306.h"
#include "CytronMotorDriver.h"

//モータ設定
CytronMD motor1(PWM_DIR, 2, 4); //FR （R…後、R…右）
CytronMD motor2(PWM_DIR, 16, 17); //FL
CytronMD motor3(PWM_DIR, 5, 18); //RL
CytronMD motor4(PWM_DIR, 19, 23); //RR

const float wheelRadius = 0.03;  // タイヤの半径(m)
const float robotRadius = 0.1;   // マシンの中央からタイヤまでの距離(m)
const int settingspeed = 135; //モーターの最大スピード

//ディスプレイ設定
bool flag = false; //ディスプレイ用停止フラッグ
bool flag1 = false;
String data1 = ""; //ディスプレイ用変数
String data2 = "";
SSD1306 display(0x3c,21,22); //ディスプレイの規格

void setup() {
  Serial.begin(2500000); //ポート開放

  display.init(); //初期状態のディスプレイ表示設定
  display.setFont(ArialMT_Plain_16); //フォント設定
  display.drawString(0,0,"program reset...");
  display.display(); //ディスプレイ反映
  delay(1000);
  display.clear(); //ディスプレイ削除
  display.display();
  display.drawString(0,0,"ESP32 Lower.v4");
  display.drawString(0,20,"select button");
  display.display();

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

          //オムニホイール制御
          if (command == "LYLXRX") {
            float LSY_V = firstValue;//y軸
            float LSX_V = secondValue;//x軸
            float RSX_V = thirdValue;//旋回軸

            if(abs(LSX_V) > 0.5 || abs(LSY_V) > 0.5 || abs(RSX_V) > 3.5){ 

              float v1 = (LSX_V*cos(PI/4) + LSY_V*sin(PI/4) + robotRadius * RSX_V) / wheelRadius;  //各タイヤの速度制御 
              float v2 = ((-LSX_V*cos(3*PI/4)) + (-LSY_V*sin(3*PI/4)) + robotRadius * RSX_V) / wheelRadius;
              float v3 = (LSX_V*cos(5*PI/4) + LSY_V*sin(5*PI/4) + robotRadius * RSX_V) / wheelRadius;
              float v4 = ((-LSX_V*cos(7*PI/4)) + (-LSY_V*sin(7*PI/4)) + robotRadius * RSX_V) / wheelRadius;

              float maxspeed = max(max(abs(v1),abs(v2)),max(abs(v3),abs(v4))); //変数に速度制御での最大値を入れる

              if (maxspeed > settingspeed){ //　PWM値に変換　max255　
                v1 = v1 * settingspeed / maxspeed;
                v2 = v2 * settingspeed / maxspeed;
                v3 = v3 * settingspeed / maxspeed;
                v4 = v4 * settingspeed / maxspeed;
              }

              motor1.setSpeed(v1); // FR
              motor2.setSpeed(v2); // FL
              motor3.setSpeed(v3); // RL
              motor4.setSpeed(v4); // RR
            }else{
              motor1.setSpeed(0); // FR
              motor2.setSpeed(0); // FL
              motor3.setSpeed(0); // RL
              motor4.setSpeed(0); // RR
            }
          } 
        } else { //数値2つ
          float firstValue = data.substring(firstDelimiterIndex + 1, secondDelimiterIndex).toInt();
          float secondValue = data.substring(secondDelimiterIndex + 1).toInt();

          //ここに数値２つの制御

        } 
      } else { //数値1つ
        float firstValue = data.substring(firstDelimiterIndex + 1).toInt();
        
        //ここに数値１つの制御

      }
      //ディスプレイ反映
      if (!flag){
        if (command == "redpro" || command == "bluepro" || command == "conpro"){
          data1 = data.substring(firstDelimiterIndex + 1);
          display.clear();
          display.display();
          display.drawString(0,0,"ESP32 Lower.v4");
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
          display.drawString(0,0,"ESP32 Lower.v4");
          display.drawString(0,20,data2);
          display.drawString(0,40,"proglam start...");
          display.display();
          flag1 = true;
        }
      }
    }
  }
}

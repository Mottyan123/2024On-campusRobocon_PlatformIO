#include <Arduino.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <math.h>
#include "SSD1306.h"
#include "CytronMotorDriver.h"
#include <AutoPID.h>

Servo myservo1;          // Servoオブジェクトの宣言
Servo myservo2;

const int SV_PIN1 = 12;   // サーボモーターをデジタルピン設定
const int SV_PIN2 = 13;

int servo_angle1 = 90; //サーボ角度用変数（電源ON時に初期化）
int servo_angle2 = 0;

const float wheelRadius = 0.03;  // タイヤの半径
const float robotRadius = 0.1;   // マシンの中央からタイヤまでの距離

//モータ設定
CytronMD motor1(PWM_DIR, 18, 19);  // PWM 1 = Pin , DIR 1 = Pin . FR
CytronMD motor2(PWM_DIR, 32, 33); // PWM 2 = Pin , DIR 2 = Pin . FL
CytronMD motor3(PWM_DIR, 25, 26);  // PWM 3 = Pin , DIR 3 = Pin . RL
CytronMD motor4(PWM_DIR, 16, 17); // PWM 4 = Pin , DIR 4 = Pin . RR

const int settingspeed = 175; //モーターの最大スピード

//エンコーダー設定
const int encoderPinsA[4] = {2, 4, 6, 8}; // エンコーダA相のピン
const int encoderPinsB[4] = {3, 5, 7, 9}; // エンコーダB相のピン
volatile long encoderCounts[4] = {0, 0, 0, 0}; // エンコーダのカウント
double setpoints[4] = {0, 0, 0, 0}; // 目標値
double outputs[4] = {0, 0, 0, 0}; // PID制御出力
double inputs[4] = {0, 0, 0, 0};  // 現在の位置

double Kp = 2.0, Ki = 5.0, Kd = 0.0; // PIパラメータ（Kdは0に設定）

AutoPID myPIDs[4] = {
  AutoPID(&inputs[0], &setpoints[0], &outputs[0], -255, 255, Kp, Ki, Kd),
  AutoPID(&inputs[1], &setpoints[1], &outputs[1], -255, 255, Kp, Ki, Kd),
  AutoPID(&inputs[2], &setpoints[2], &outputs[2], -255, 255, Kp, Ki, Kd),
  AutoPID(&inputs[3], &setpoints[3], &outputs[3], -255, 255, Kp, Ki, Kd)
};

//ディスプレイ設定
bool flag = false; //ディスプレイ用停止フラッグ
String data1 = ""; //ディスプレイ用変数
String data2 = "";
SSD1306 display(0x3c,21,22); //ディスプレイの規格

void updateEncoderA() { //エンコーダA相のカウント
  for (int i = 0; i < 4; i++) {
    if (digitalRead(encoderPinsA[i])) {
      encoderCounts[i]++;
    } else {
      encoderCounts[i]--;
    }
  }
}

void updateEncoderB() { //エンコーダB相のカウント
  for (int i = 0; i < 4; i++) {
    if (digitalRead(encoderPinsB[i])) {
      encoderCounts[i]++;
    } else {
      encoderCounts[i]--;
    }
  }
}

void setup() {
  Serial.begin(115200); //ポート開放

  display.init(); //初期状態のディスプレイ表示設定
  display.setFont(ArialMT_Plain_16);
  display.drawString(0,0,"program reset...");
  display.display();
  delay(1000);
  display.clear();
  display.display();
  display.drawString(0,0,"ESP32 Lower");
  display.drawString(0,20,"select button");
  display.display();

  myservo1.attach(SV_PIN1, 500, 2400);  // サーボの割当(パルス幅500~2400msに指定)
  myservo1.write(servo_angle1); //サーボ角度をリセット
  myservo2.attach(SV_PIN2, 500, 2400);
  myservo2.write(servo_angle2);

  for (int i = 0; i < 4; i++) { //エンコーダー出力ピン設定
    pinMode(encoderPinsA[i], INPUT);
    pinMode(encoderPinsB[i], INPUT);

    attachInterrupt(digitalPinToInterrupt(encoderPinsA[i]), updateEncoderA, CHANGE);
    attachInterrupt(digitalPinToInterrupt(encoderPinsB[i]), updateEncoderB, CHANGE);

    myPIDs[i].setBangBang(50); // バンバン制御の範囲を設定（オプション）
    myPIDs[i].setTimeStep(100); // 更新間隔を設定（ミリ秒）
  }

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

              setpoints[0] = v1; // 目標値の更新
              setpoints[1] = v2;
              setpoints[2] = v3;
              setpoints[3] = v4;

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

          if (command == "redcap"){
            if (firstValue >290 && firstValue <350 && secondValue >210 && secondValue < 270){
              myservo1.write(round(0));
            } else {
              myservo1.write(round(90));
            }  
          }
        } 
      } else { //数値1つ
        float firstValue = data.substring(firstDelimiterIndex + 1).toInt();
        if (command == "CB"){
          if (firstValue == 1){
            myservo1.write(round(0));
          } else {
            myservo1.write(round(90));
          }
        }
      }
      //ディスプレイ反映
      if (!flag){
        if (command == "redpro" || "bluepro" || "conpro"){
          data1 = data.substring(firstDelimiterIndex + 1);
          display.clear();
          display.display();
          display.drawString(0,0,"ESP32 Lower");
          display.drawString(0,20,data1);
          display.drawString(0,40,"prs space to start");
          display.display();
          flag = true;
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

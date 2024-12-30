#include <Arduino.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <math.h>
#include "SSD1306.h"
#include "CytronMotorDriver.h"

//サーボ設定
Servo myservo1;   // Servoオブジェクトの宣言
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;
Servo myservo6;
Servo myservo7;
Servo myservo8;
Servo myservo9;
Servo myservo10;

const int SV_PIN1 = 19;   // サーボモーターをデジタルピン設定
const int SV_PIN2 = 18;
const int SV_PIN3 = 5;
const int SV_PIN4 = 17;
const int SV_PIN5 = 16;
const int SV_PIN6 = 32;
const int SV_PIN7 = 33;
const int SV_PIN8 = 25;
const int SV_PIN9 = 26;
const int SV_PIN10 = 27;

const int servo_angle1 = 180; //サーボ初期角度（電源ON時に初期化）
const int servo_angle2 = 180;
const int servo_angle3 = 180;
const int servo_angle4 = 0;
const int servo_angle5 = 110;
const int servo_angle6 = 180;
const int servo_angle7 = 0;
const int servo_angle8 = 180;
const int servo_angle9 = 0;
const int servo_angle10 = 110;

const int stepDelay = 15;    //角度変更速度(㎳)

//モーター設定
CytronMD motor(PWM_DIR,12,13);

//シリンダー設定
const int SL_PIN = 4;

//ディスプレイ設定
bool flag = false; //ディスプレイ用停止フラッグ
bool flag1 = false;
String data1 = ""; //ディスプレイ用変数
String data2 = "";
SSD1306 display(0x3c,21,22); //ディスプレイの規格

//マルチコア処理設定
TaskHandle_t Task1;
TaskHandle_t Task2;

//制御用変数
String command;
float firstValue;
float secondValue;
float thirdValue;

//サーボ複数同時制御
//目標角度1～5、角度変更速度
//右アーム用
void moveServosToTarget(int targetAngle1, int targetAngle2, int targetAngle3, int targetAngle4, int targetAngle5, int delayTime) {
  int currentAngle1 = myservo1.read(); //サーボの現在角度を取得
  int currentAngle2 = myservo2.read();
  int currentAngle3 = myservo3.read();
  int currentAngle4 = myservo4.read();
  int currentAngle5 = myservo5.read();

  bool servosMoving = false; //ループ停止用フラッグ

  while (!servosMoving) {
    servosMoving = true;

    // サーボ1の制御
    if (currentAngle1 < targetAngle1) {  //目標角度の方が大きいとき
      currentAngle1++; //現在角度に＋１
      myservo1.write(currentAngle1); //サーボ駆動
      servosMoving = false; //フラッグリセット
    } else if (currentAngle1 > targetAngle1) {  //目標角度の方が小さいとき
      currentAngle1--; //現在角度にー１
      myservo1.write(currentAngle1);
      servosMoving = false;
    }

    // サーボ2の制御
    if (currentAngle2 < targetAngle2) {
      currentAngle2++;
      myservo2.write(currentAngle2);
      servosMoving = false;
    } else if (currentAngle2 > targetAngle2) {
      currentAngle2--;
      myservo2.write(currentAngle2);
      servosMoving = false;
    }

    // サーボ3の制御
    if (currentAngle3 < targetAngle3) {
      currentAngle3++;
      myservo3.write(currentAngle3);
      servosMoving = false;
    } else if (currentAngle3 > targetAngle3) {
      currentAngle3--;
      myservo3.write(currentAngle3);
      servosMoving = false;
    }

    // サーボ4の制御
    if (currentAngle4 < targetAngle4) {
      currentAngle4++;
      myservo4.write(currentAngle4);
      servosMoving = false;
    } else if (currentAngle4 > targetAngle4) {
      currentAngle4--;
      myservo4.write(currentAngle4);
      servosMoving = false;
    }

    // サーボ5の制御
    if (currentAngle5 < targetAngle5) {
      currentAngle5++;
      myservo5.write(currentAngle5);
      servosMoving = false;
    } else if (currentAngle5 > targetAngle5) {
      currentAngle5--;
      myservo5.write(currentAngle5);
      servosMoving = false;
    }

    delay(delayTime);  // すべてのサーボを動かす間隔を待機
  }
}

//サーボ複数同時制御
//目標角度6～10、角度変更速度
//左アーム用
void moveServosToTarget1(int targetAngle6, int targetAngle7, int targetAngle8, int targetAngle9, int targetAngle10, int delayTime) {
  int currentAngle6 = myservo6.read(); //サーボの現在角度を取得
  int currentAngle7 = myservo7.read();
  int currentAngle8 = myservo8.read();
  int currentAngle9 = myservo9.read();
  int currentAngle10 = myservo10.read();

  bool servosMoving = false; //ループ停止用フラッグ

  while (!servosMoving) {
    servosMoving = true;

    // サーボ6の制御
    if (currentAngle6 < targetAngle6) {  //目標角度の方が大きいとき
      currentAngle6++; //現在角度に＋１
      myservo6.write(currentAngle6); //サーボ駆動
      servosMoving = false; //フラッグリセット
    } else if (currentAngle6 > targetAngle6) {  //目標角度の方が小さいとき
      currentAngle6--; //現在角度にー１
      myservo6.write(currentAngle6);
      servosMoving = false;
    }

    // サーボ7の制御
    if (currentAngle7 < targetAngle7) {
      currentAngle7++;
      myservo7.write(currentAngle7);
      servosMoving = false;
    } else if (currentAngle7 > targetAngle7) {
      currentAngle7--;
      myservo7.write(currentAngle7);
      servosMoving = false;
    }

    // サーボ8の制御
    if (currentAngle8 < targetAngle8) {
      currentAngle8++;
      myservo8.write(currentAngle8);
      servosMoving = false;
    } else if (currentAngle8 > targetAngle8) {
      currentAngle8--;
      myservo8.write(currentAngle8);
      servosMoving = false;
    }

    // サーボ9の制御
    if (currentAngle9 < targetAngle9) {
      currentAngle9++;
      myservo9.write(currentAngle9);
      servosMoving = false;
    } else if (currentAngle9 > targetAngle9) {
      currentAngle9--;
      myservo9.write(currentAngle9);
      servosMoving = false;
    }

    // サーボ10の制御
    if (currentAngle10 < targetAngle10) {
      currentAngle10++;
      myservo10.write(currentAngle10);
      servosMoving = false;
    } else if (currentAngle10 > targetAngle10) {
      currentAngle10--;
      myservo10.write(currentAngle10);
      servosMoving = false;
    }

    delay(delayTime);  // すべてのサーボを動かす間隔を待機
  }
}

//関数の前方宣言
void task1code(void * parameter);
void task2code(void * parameter);

//シリアルの読み込み＋右アーム
void task1code(void * parameter) {
  while (true){
    if (Serial.available()) { //シリアル読みとり
      String data = Serial.readStringUntil('\n');
      float firstDelimiterIndex = data.indexOf(':');

      if (firstDelimiterIndex > 0){ //コマンド判定
        command = data.substring(0, firstDelimiterIndex);
        float secondDelimiterIndex = data.indexOf(':', firstDelimiterIndex + 1);
        
        if (secondDelimiterIndex > 0) { //数値個数判定
          float thirdDelimiterIndex = data.indexOf(':',secondDelimiterIndex + 1);

          if (thirdDelimiterIndex > 0){ //数値3つ
            firstValue = data.substring(firstDelimiterIndex + 1, secondDelimiterIndex).toInt();
            secondValue = data.substring(secondDelimiterIndex + 1, thirdDelimiterIndex).toInt();
            thirdValue = data.substring(thirdDelimiterIndex + 1).toInt();

            //ここに数値３つの制御

          } else { //数値2つ
            firstValue = data.substring(firstDelimiterIndex + 1, secondDelimiterIndex).toInt();
            secondValue = data.substring(secondDelimiterIndex + 1).toInt();

            //ここに数値2つの制御

          } 
        } else { //数値1つ
          firstValue = data.substring(firstDelimiterIndex + 1).toInt();
          
          if (command == "CB"){ //A
            if (firstValue == 1){
              moveServosToTarget(180,90,180,100,90,stepDelay); //サーボ角度制御
            }
          }
          if (command == "TB"){ //X
            if (firstValue == 1){
              moveServosToTarget(180,90,160,55,30,stepDelay);
              moveServosToTarget(135,90,160,55,30,stepDelay);
              moveServosToTarget(135,90,180,100,90,stepDelay);
            }
          }
          if (command == "XB"){ //B
            if (firstValue == 1){
              moveServosToTarget(180,180,180,0,110,stepDelay);
            }
          }
          if (command == "SB"){ //Y
            if (firstValue == 1){
              moveServosToTarget(135,180,10,120,70,stepDelay);
              moveServosToTarget(180,180,10,120,70,stepDelay);
            }
          }
          if (command == "R2A"){ 
            if (firstValue == 1){
              moveServosToTarget(180,90,170,58,38,stepDelay);
            }
          }
          if (command == "R1B"){ 
            if (firstValue == 1){
              moveServosToTarget(180,180,180,0,110,stepDelay);
            }
          }
          if (command == "PB"){ 
            if (firstValue == 1){
              moveServosToTarget(180,90,180,90,38,stepDelay);
            }
          }       
        }
        //ディスプレイ反映
        if (!flag){
          if (command == "redpro" || command == "bluepro" || command == "conpro"){
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
}

//左アームプログラム＋その他のプログラム
void task2code(void * parameter) {
  while (true){
    if (command == "CB"){
      if (firstValue == 1){
        moveServosToTarget1(90,90,90,90,90,stepDelay); //サーボ角度制御
      }
    }
    if (command == "TB"){
      if (firstValue == 1){
        moveServosToTarget1(0,0,0,0,0,stepDelay);
      }
    }
    if (command == "XB"){
      if (firstValue == 1){
        moveServosToTarget1(180,180,180,180,180,stepDelay);
      }
    } 
    if (command == "KS"){
      if(firstValue == 1){
        motor.setSpeed(0);
      }
    }
    if (command == "KD"){
      if(firstValue == 1){
        motor.setSpeed(50);
      }
    }
    if (command == "KT"){
      if(firstValue == 1){
        motor.setSpeed(255);
      }
    }
    if (command == "KH"){
      if(firstValue == 1){
        digitalWrite(SL_PIN, 0);
      }
    } 
    if (command == "KL"){
      if(firstValue == 1){
        digitalWrite(SL_PIN, 1);
      }
    } 
  }
}

void setup() {
  Serial.begin(115200); //ポート開放

  display.init(); //初期状態のディスプレイ表示設定
  display.setFont(ArialMT_Plain_16); //フォント設定
  display.drawString(0,0,"program reset...");
  display.display(); //ディスプレイ反映
  delay(1000);
  display.clear(); //ディスプレイ削除
  display.display();
  display.drawString(0,0,"ESP32 Upper");
  display.drawString(0,20,"select button");
  display.display();

  myservo1.attach(SV_PIN1, 500, 2400);  // サーボの割当(パルス幅500~2400msに指定)
  myservo2.attach(SV_PIN2, 500, 2400);
  myservo3.attach(SV_PIN3, 500, 2400);
  myservo4.attach(SV_PIN4, 500, 2400);
  myservo5.attach(SV_PIN5, 500, 2400);
  myservo6.attach(SV_PIN6, 500, 2400);
  myservo7.attach(SV_PIN7, 500, 2400);
  myservo8.attach(SV_PIN8, 500, 2400);
  myservo9.attach(SV_PIN9, 500, 2400);
  myservo10.attach(SV_PIN10, 500, 2400);
  myservo1.write(servo_angle1); //サーボ角度をリセット
  myservo2.write(servo_angle2);
  myservo3.write(servo_angle3);
  myservo4.write(servo_angle4);
  myservo5.write(servo_angle5);
  myservo6.write(servo_angle6);
  myservo7.write(servo_angle7);
  myservo8.write(servo_angle8);
  myservo9.write(servo_angle9);
  myservo10.write(servo_angle10);

  pinMode(SL_PIN, OUTPUT); //シリンダーポートをOUTPUTに設定
  digitalWrite(SL_PIN, 1); //シリンダーの初期位置

  // Task1の作成 (Core 0で実行)
  xTaskCreatePinnedToCore(
    task1code,  // 実行する関数
    "Task1",    // タスク名
    10000,      // スタックサイズ
    NULL,       // パラメータ
    1,          // 優先度
    &Task1,     // タスクハンドル
    0           // コア番号 (0)
  );

  // Task2の作成 (Core 1で実行)
  xTaskCreatePinnedToCore(
    task2code,  // 実行する関数
    "Task2",    // タスク名
    10000,      // スタックサイズ
    NULL,       // パラメータ
    1,          // 優先度
    &Task2,     // タスクハンドル
    1           // コア番号 (1)
  );

  delay(100);
}

void loop(){
  //記述なし
}

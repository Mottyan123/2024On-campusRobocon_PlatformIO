#include <Arduino.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <math.h>
#include "SSD1306.h"
#include "CytronMotorDriver.h"
#include <AutoPID.h>

//モータ設定
CytronMD motor1(PWM_DIR, 5, 18); //FR （R…後、R…右）
CytronMD motor2(PWM_DIR, 19, 23); //FL
CytronMD motor3(PWM_DIR, 16, 17); //RL
CytronMD motor4(PWM_DIR, 2, 4); //RR

const float wheelRadius = 0.03;  // タイヤの半径
const float robotRadius = 0.1;   // マシンの中央からタイヤまでの距離
const int settingspeed = 125; //モーターの最大スピード

//エンコーダー設定
const int encoderPinsA[4] = {2, 4, 6, 8}; // エンコーダA相のピン
const int encoderPinsB[4] = {3, 5, 7, 9}; // エンコーダB相のピン
volatile long encoderCounts[4] = {0, 0, 0, 0}; // エンコーダのカウント
double setpoints[4] = {0, 0, 0, 0}; // 目標値
double outputs[4] = {0, 0, 0, 0}; // PID制御出力
double inputs[4] = {0, 0, 0, 0};  // 現在の位置

double Kp0 = 1.5, Ki0 = 0.0, Kd0 = 0.0; // PIDパラメータ0
double Kp1 = 1.5, Ki1 = 0.0, Kd1 = 0.0; // PIDパラメータ1
double Kp2 = 1.5, Ki2 = 0.0, Kd2 = 0.0; // PIDパラメータ2
double Kp3 = 1.5, Ki3 = 0.0, Kd3 = 0.0; // PIDパラメータ3

AutoPID myPIDs[4] = {
  AutoPID(&inputs[0], &setpoints[0], &outputs[0], -255, 255, Kp0, Ki0, Kd0),
  AutoPID(&inputs[1], &setpoints[1], &outputs[1], -255, 255, Kp1, Ki1, Kd1),
  AutoPID(&inputs[2], &setpoints[2], &outputs[2], -255, 255, Kp2, Ki2, Kd2),
  AutoPID(&inputs[3], &setpoints[3], &outputs[3], -255, 255, Kp3, Ki3, Kd3)
};

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
char senddata[32];
char recivedata[32];

//変数同期処理設定
// キューの定義
QueueHandle_t xQueue;

void updateEncoder0() { //エンコーダ0のカウント
  if (digitalRead(encoderPinsA[0]) == digitalRead(encoderPinsB[0])) {
    encoderCounts[0]--; // 同じなら逆方向
  } else {
    encoderCounts[0]++; // 異なれば正方向
  }
}

void updateEncoder1() { //エンコーダ1のカウント
  if (digitalRead(encoderPinsA[1]) == digitalRead(encoderPinsB[1])) {
    encoderCounts[1]--; // 同じなら逆方向
  } else {
    encoderCounts[1]++; // 異なれば正方向
  }
}

void updateEncoder2() { //エンコーダ2のカウント
  if (digitalRead(encoderPinsA[2]) == digitalRead(encoderPinsB[2])) {
    encoderCounts[2]--; // 同じなら逆方向
  } else {
    encoderCounts[2]++; // 異なれば正方向
  }
}

void updateEncoder3() { //エンコーダ3のカウント
  if (digitalRead(encoderPinsA[3]) == digitalRead(encoderPinsB[3])) {
    encoderCounts[3]--; // 同じなら逆方向
  } else {
    encoderCounts[3]++; // 異なれば正方向
  }
}

//関数の前方宣言
void task1code(void * parameter);
void task2code(void * parameter);

//シリアルの読み込み＋PID制御＋ディスプレイ
void task1code(void * parameter) {
  while (true){
    String data = Serial.readStringUntil('\n');
    data.toCharArray(senddata, sizeof(senddata)); //char型に変換
    xQueueOverwrite(xQueue, &senddata); //キューにデータを送信(上書きモード)

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

              for (int i = 0; i < 4; i++) {
                inputs[i] = encoderCounts[i]; // 現在の位置を入力として設定
                myPIDs[i].run(); // AutoPIDによる制御計算
              }

              motor1.setSpeed(outputs[0]); // FR
              motor2.setSpeed(outputs[1]); // FL
              motor3.setSpeed(outputs[2]); // RL
              motor4.setSpeed(outputs[3]); // RR
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
          display.drawString(0,0,"ESP32 Lower.v5");
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
          display.drawString(0,0,"ESP32 Lower.v5");
          display.drawString(0,20,data2);
          display.drawString(0,40,"proglam start...");
          display.display();
          flag1 = true;
        }
      }
    }
  } 
}

//RPM計算
void task2code(void * parameter) {
  while (true){
    xQueueReceive(xQueue, &recivedata, portMAX_DELAY); //キューからデータを受信
    String data(recivedata); // char型配列をString型に変換

    
  }
}

void setup() {
  Serial.begin(2500000); //ポート開放

  display.init(); //初期状態のディスプレイ表示設定
  display.setFont(ArialMT_Plain_16);
  display.drawString(0,0,"program reset...");
  display.display();
  delay(1000);
  display.clear();
  display.display();
  display.drawString(0,0,"ESP32 Lower.v5");
  display.drawString(0,20,"select button");
  display.display();

  for (int i = 0; i < 4; i++) { //エンコーダー出力ピン設定
    pinMode(encoderPinsA[i], INPUT);
    pinMode(encoderPinsB[i], INPUT);

    myPIDs[i].setBangBang(50); // バンバン制御の範囲を設定（オプション）
    myPIDs[i].setTimeStep(100); // 更新間隔を設定（ミリ秒）
  }

  attachInterrupt(digitalPinToInterrupt(encoderPinsA[0]), updateEncoder0, CHANGE); //割り込み設定
  attachInterrupt(digitalPinToInterrupt(encoderPinsB[0]), updateEncoder0, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinsA[1]), updateEncoder1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinsB[1]), updateEncoder1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinsA[2]), updateEncoder2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinsB[2]), updateEncoder2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinsA[3]), updateEncoder3, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinsB[3]), updateEncoder3, CHANGE);

  // キューの作成（1つの32batesデータを保持するキュー）
  xQueue = xQueueCreate(1, sizeof(char)*32);

  // Task1の作成 (Core 1で実行)
  xTaskCreatePinnedToCore(
    task1code,  // 実行する関数
    "Task1",    // タスク名
    8192,      // スタックサイズ
    NULL,       // パラメータ
    1,          // 優先度
    &Task1,     // タスクハンドル
    1           // コア番号 (1)
  );

  // Task2の作成 (Core 0で実行)
  xTaskCreatePinnedToCore(
    task2code,  // 実行する関数
    "Task2",    // タスク名
    8192,      // スタックサイズ
    NULL,       // パラメータ
    1,          // 優先度
    &Task2,     // タスクハンドル
    0           // コア番号 (0)
  );

  delay(100);
}

void loop() {
  //記述なし
}


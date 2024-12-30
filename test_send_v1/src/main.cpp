#include <Arduino.h>
#include <ESP32Servo.h>

//以下はサーボの初期設定の例
Servo myservo1;          // Servoオブジェクトの宣言
Servo myservo2;

const int SV_PIN1 = 12;   // サーボモーターをデジタルピン設定
const int SV_PIN2 = 13;

int servo_angle1 = 0; //サーボ角度用変数（電源ON時に初期化）
int servo_angle2 = 0;

void setup() {
  Serial.begin(115200); //ポート開放

  myservo1.attach(SV_PIN1, 500, 2400);  // サーボの割当(パルス幅500~2400msに指定)
  myservo1.write(servo_angle1); //サーボ角度をリセット
  myservo2.attach(SV_PIN2, 500, 2400);
  myservo2.write(servo_angle2);

  delay(100);
}

void loop() {
  if (Serial.available()) { //シリアル読みとり
    String data = Serial.readStringUntil('\n');
    int firstDelimiterIndex = data.indexOf(':');

    if (firstDelimiterIndex > 0){ //コマンド判定
      String command = data.substring(0, firstDelimiterIndex);
      int secondDelimiterIndex = data.indexOf(':', firstDelimiterIndex + 1);
      
      if (secondDelimiterIndex > 0) { //数値個数判定
        int thirdDelimiterIndex = data.indexOf(':',secondDelimiterIndex + 1);

        if (thirdDelimiterIndex > 0){ //アナログデータ3つ
          int firstValue = data.substring(firstDelimiterIndex + 1, secondDelimiterIndex).toInt();
          int secondValue = data.substring(secondDelimiterIndex + 1, thirdDelimiterIndex).toInt();
          int thirdValue = data.substring(thirdDelimiterIndex + 1).toInt();

        } else { //アナログデータ2つ
          int firstValue = data.substring(firstDelimiterIndex + 1, secondDelimiterIndex).toInt();
          int secondValue = data.substring(secondDelimiterIndex + 1).toInt();

        } 
      } else { //アナログデータ1つ
        int firstValue = data.substring(firstDelimiterIndex + 1).toInt();

        //以下は例
        if (command == "CB"){ //〇ボタンが押されたら…
          if (firstValue == 1){
            myservo1.write(round(180));
          } else {
            myservo1.write(round(0));
          }
        }
      }
    }
  }
}

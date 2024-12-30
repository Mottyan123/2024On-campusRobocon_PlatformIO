#include <Arduino.h>
#include <ESP32Servo.h>

Servo servo1;  // サーボ1
Servo servo2;  
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;
Servo servo7;
Servo servo8;
Servo servo9;
Servo servo10;

int servoPin1 = 19;  // サーボ1を接続するピン
int servoPin2 = 18;  
int servoPin3 = 5;
int servoPin4 = 17;
int servoPin5 = 16;
int servoPin6 = 32;
int servoPin7 = 33;
int servoPin8 = 25;
int servoPin9 = 26;
int servoPin10 = 27;

int currentAngle1 = 180;  // サーボ1の初期角度
int currentAngle2 = 180;  
int currentAngle3 = 180;
int currentAngle4 = 0;
int currentAngle5 = 90;
int currentAngle6 = 180;
int currentAngle7 = 0;
int currentAngle8 = 180;
int currentAngle9 = 0;
int currentAngle10 = 90;

void setup() {
  Serial.begin(115200);    // シリアルモニタの開始
  servo1.attach(servoPin1); // サーボ1をピンに接続
  servo2.attach(servoPin2); 
  servo3.attach(servoPin3);
  servo4.attach(servoPin4);
  servo5.attach(servoPin5);
  servo6.attach(servoPin6);
  servo7.attach(servoPin7);
  servo8.attach(servoPin8);
  servo9.attach(servoPin9);
  servo10.attach(servoPin10);

  // 初期角度を設定
  servo1.write(currentAngle1);
  servo2.write(currentAngle2);
  servo3.write(currentAngle3);
  servo4.write(currentAngle4);
  servo5.write(currentAngle5);
  servo6.write(currentAngle6);
  servo7.write(currentAngle7);
  servo8.write(currentAngle8);
  servo9.write(currentAngle9);
  servo10.write(currentAngle10);

  Serial.println("Enter 's1' to 's10' followed by an angle (0-180) to control the servos.");
  Serial.println("Example: 's1 90' sets Servo 1 to 90 degrees.");
}

void loop() {
  if (Serial.available() > 0) {
    // 入力を文字列として読み込む
    String inputString = Serial.readStringUntil('\n');
    inputString.trim();  // 改行や空白を削除

    // サーボ1からサーボ10を制御するか判断
    if (inputString.startsWith("s1")) {
      String angleString = inputString.substring(2); // 's1'の後の部分を取得
      int newAngle = angleString.toInt();  // 文字列を整数に変換
      if (newAngle >= 0 && newAngle <= 180) {
        currentAngle1 = newAngle;  // 新しい角度を設定
        servo1.write(currentAngle1); // サーボ1を新しい角度に移動
        Serial.print("Servo 1 - Current Angle: ");
        Serial.println(currentAngle1);
      } 
      else {
        Serial.println("Please enter a valid angle between 0 and 180 for Servo 1.");
      }
    }

    if (inputString.startsWith("s2")) {
      String angleString = inputString.substring(2); 
      int newAngle = angleString.toInt();  
      if (newAngle >= 0 && newAngle <= 180) {
        currentAngle2 = newAngle;  
        servo2.write(currentAngle2); 
        Serial.print("Servo 2 - Current Angle: ");
        Serial.println(currentAngle2);
      } 
      else {
        Serial.println("Please enter a valid angle between 0 and 180 for Servo 2.");
      }
    }

    if (inputString.startsWith("s3")) {
      String angleString = inputString.substring(2); 
      int newAngle = angleString.toInt();  
      if (newAngle >= 0 && newAngle <= 180) {
        currentAngle3 = newAngle;  
        servo3.write(currentAngle3); 
        Serial.print("Servo 3 - Current Angle: ");
        Serial.println(currentAngle3);
      } 
      else {
        Serial.println("Please enter a valid angle between 0 and 180 for Servo 3.");
      }
    }

    if (inputString.startsWith("s4")) {
      String angleString = inputString.substring(2); 
      int newAngle = angleString.toInt();  
      if (newAngle >= 0 && newAngle <= 180) {
        currentAngle4 = newAngle;  
        servo4.write(currentAngle4); 
        Serial.print("Servo 4 - Current Angle: ");
        Serial.println(currentAngle4);
      } 
      else {
        Serial.println("Please enter a valid angle between 0 and 180 for Servo 4.");
      }
    }

    if (inputString.startsWith("s5")) {
      String angleString = inputString.substring(2); 
      int newAngle = angleString.toInt();  
      if (newAngle >= 0 && newAngle <= 180) {
        currentAngle5 = newAngle;  
        servo5.write(currentAngle5); 
        Serial.print("Servo 5 - Current Angle: ");
        Serial.println(currentAngle5);
      } 
      else {
        Serial.println("Please enter a valid angle between 0 and 180 for Servo 5.");
      }
    }

    if (inputString.startsWith("s6")) {
      String angleString = inputString.substring(2); 
      int newAngle = angleString.toInt();  
      if (newAngle >= 0 && newAngle <= 180) {
        currentAngle6 = newAngle;  
        servo6.write(currentAngle6); 
        Serial.print("Servo 6 - Current Angle: ");
        Serial.println(currentAngle6);
      } 
      else {
        Serial.println("Please enter a valid angle between 0 and 180 for Servo 6.");
      }
    }

    if (inputString.startsWith("s7")) {
      String angleString = inputString.substring(2); 
      int newAngle = angleString.toInt();  
      if (newAngle >= 0 && newAngle <= 180) {
        currentAngle7 = newAngle;  
        servo7.write(currentAngle7); 
        Serial.print("Servo 7 - Current Angle: ");
        Serial.println(currentAngle7);
      } 
      else {
        Serial.println("Please enter a valid angle between 0 and 180 for Servo 7.");
      }
    }

    if (inputString.startsWith("s8")) {
      String angleString = inputString.substring(2); 
      int newAngle = angleString.toInt();  
      if (newAngle >= 0 && newAngle <= 180) {
        currentAngle8 = newAngle;  
        servo8.write(currentAngle8); 
        Serial.print("Servo 8 - Current Angle: ");
        Serial.println(currentAngle8);
      } 
      else {
        Serial.println("Please enter a valid angle between 0 and 180 for Servo 8.");
      }
    }

    if (inputString.startsWith("s9")) {
      String angleString = inputString.substring(2); 
      int newAngle = angleString.toInt();  
      if (newAngle >= 0 && newAngle <= 180) {
        currentAngle9 = newAngle;  
        servo9.write(currentAngle9); 
        Serial.print("Servo 9 - Current Angle: ");
        Serial.println(currentAngle9);
      } 
      else {
        Serial.println("Please enter a valid angle between 0 and 180 for Servo 9.");
      }
    }

    if (inputString.startsWith("s10")) {
      String angleString = inputString.substring(2); 
      int newAngle = angleString.toInt();  
      if (newAngle >= 0 && newAngle <= 180) {
        currentAngle10 = newAngle;  
        servo10.write(currentAngle10); 
        Serial.print("Servo 10 - Current Angle: ");
        Serial.println(currentAngle10);
      } 
      else {
        Serial.println("Please enter a valid angle between 0 and 180 for Servo 10.");
      }
    }
  }
}

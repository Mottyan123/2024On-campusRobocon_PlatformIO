#include <Arduino.h>
#include <PS4Controller.h>

void setup() {
  Serial.begin(115200);
  PS4.begin("08:b6:1f:ef:86:5a");
  Serial.println("Ready.");
}

void loop() {
  // Below has all accessible outputs from the controller
  if (PS4.isConnected()) {
    if (PS4.Right()) Serial.println("Right Button");
    if (PS4.Down()) Serial.println("Down Button");
    if (PS4.Up()) Serial.println("Up Button");
    if (PS4.Left()) Serial.println("Left Button");

    if (PS4.Square()) Serial.println("Square Button");
    if (PS4.Cross()) Serial.println("Cross Button");
    if (PS4.Circle()) Serial.println("Circle Button");
    if (PS4.Triangle()) Serial.println("Triangle Button");

    if (PS4.UpRight()) Serial.println("Up Right");
    if (PS4.DownRight()) Serial.println("Down Right");
    if (PS4.UpLeft()) Serial.println("Up Left");
    if (PS4.DownLeft()) Serial.println("Down Left");

    if (PS4.L1()) Serial.println("L1 Button");
    if (PS4.R1()) Serial.println("R1 Button");

    if (PS4.Share()) Serial.println("Share Button");
    if (PS4.Options()) Serial.println("Options Button");
    if (PS4.L3()) Serial.println("L3 Button");
    if (PS4.R3()) Serial.println("R3 Button");

    if (PS4.PSButton()) Serial.println("PS Button");
    if (PS4.Touchpad()) Serial.println("Touch Pad Button");

    if (PS4.L2()) {
      Serial.printf("L2 button at %d\n", PS4.L2Value());
    }
    if (PS4.R2()) {
      Serial.printf("R2 button at %d\n", PS4.R2Value());
    }

    if (PS4.LStickX()) {
      Serial.printf("Left Stick x at %d\n", PS4.LStickX());
    }
    if (PS4.LStickY()) {
      Serial.printf("Left Stick y at %d\n", PS4.LStickY());
    }
    if (PS4.RStickX()) {
      Serial.printf("Right Stick x at %d\n", PS4.RStickX());
    }
    if (PS4.RStickY()) {
      Serial.printf("Right Stick y at %d\n", PS4.RStickY());
    }

    if (PS4.Charging()) Serial.println("The controller is charging");
    if (PS4.Audio()) Serial.println("The controller has headphones attached");
    if (PS4.Mic()) Serial.println("The controller has a mic attached");

    Serial.printf("Battery Level : %d\n", PS4.Battery());

    Serial.println();
    // This delay is to make the output more human readable
    // Remove it when you're not trying to see the output
    delay(1000);
  }
}


#if 0

#include <PS4Controller.h>
#include "CytronMotorDriver.h"

// Configure the motor driver.
CytronMD motor1(PWM_DIR, 32, 33);  //FR
CytronMD motor2(PWM_DIR, 12, 13);  //FL
CytronMD motor3(PWM_DIR, 2, 4);  //RL
CytronMD motor4(PWM_DIR, 18, 19);  //RR

const int settingspeed = 100; //最大スピード設定

void setup() {
  Serial.begin(115200);

  // Replace the "1a:2b:3c:01:01:01" with the MAC address
  // the controller wants to pair to
  // Note: MAC address must be unicast
  PS4.begin("5c:96:56:6d:c0:0c");
  Serial.println("Ready.");
}

void loop() {
  if (PS4.isConnected()) {
    // Serial.printf("Left Stick x at %d\n", PS4.LStickX());
    // Serial.printf("Left Stick y at %d\n", PS4.LStickY());
    // Serial.printf("Right Stick x at %d\n", PS4.RStickX());

    float LY = (PS4.LStickY())*2; //制御値読み込み
    float LX = (PS4.LStickX())*2;
    float RX = (PS4.RStickX())*2;
    if (abs(PS4.LStickY()) > 20 || abs(PS4.LStickX()) > 20 || abs(PS4.RStickX()) > 20){ //メカナムホイール制御
      float v1 = LY-LX-RX; //角速度計算
      float v2 = -LY-LX-RX;
      float v3 = -LY+LX-RX;
      float v4 = LY+LX-RX;

      float maxspeed = max(max(abs(v1),abs(v2)),max(abs(v3),abs(v4))); //変数に速度制御での最大値を入れる

      if (maxspeed > settingspeed){ //　PWM値に変換　max255　
        v1 = v1 * settingspeed / maxspeed;
        v2 = v2 * settingspeed / maxspeed;
        v3 = v3 * settingspeed / maxspeed;
        v4 = v4 * settingspeed / maxspeed;
      }

      motor1.setSpeed(v1);
      motor2.setSpeed(v2);
      motor3.setSpeed(v3);
      motor4.setSpeed(v4);
    }
    else {
      motor1.setSpeed((0));
      motor2.setSpeed((0));
      motor3.setSpeed((0));
      motor4.setSpeed((0));
    }
  }

  delay(100);

}

#endif
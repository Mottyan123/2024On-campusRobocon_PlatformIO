#include <Arduino.h>
#include <Bluepad32.h>

ControllerPtr myControllers[BP32_MAX_CONTROLLERS];

void setup() {
  // Initialize serial
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  String fv = BP32.firmwareVersion();
  Serial.print("Firmware version installed: ");
  Serial.println(fv);

  const uint8_t* addr = BP32.localBdAddress();
  Serial.print("BD Address: ");
  for (int i = 0; i < 6; i++) {
    Serial.print(addr[i], HEX);
    if (i < 5)
      Serial.print(":");
    else
      Serial.println();
  }

  BP32.setup(&onConnectedController, &onDisconnectedController);

  BP32.forgetBluetoothKeys();
}

void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.print("CALLBACK: Controller is connected, index=");
      Serial.println(i);
      myControllers[i] = ctl;
      foundEmptySlot = true;

      ControllerProperties properties = ctl->getProperties();
      char buf[80];
      sprintf(buf,
              "BTAddr: %02x:%02x:%02x:%02x:%02x:%02x, VID/PID: %04x:%04x, "
              "flags: 0x%02x",
              properties.btaddr[0], properties.btaddr[1], properties.btaddr[2],
              properties.btaddr[3], properties.btaddr[4], properties.btaddr[5],
              properties.vendor_id, properties.product_id, properties.flags);
      Serial.println(buf);
      break;
    }
  }
  if (!foundEmptySlot) {
    Serial.println(
        "CALLBACK: Controller connected, but could not found empty slot");
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  bool foundGamepad = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.print("CALLBACK: Controller is disconnected from index=");
      Serial.println(i);
      myControllers[i] = nullptr;
      foundGamepad = true;
      break;
    }
  }

  if (!foundGamepad) {
    Serial.println(
        "CALLBACK: Controller disconnected, but not found in myControllers");
  }
}

void processGamepad(ControllerPtr gamepad) {

  if (gamepad->a()) {
    static int colorIdx = 0;
 
    switch (colorIdx % 3) {
      case 0:
        // Red
        gamepad->setColorLED(255, 0, 0);
        break;
      case 1:
        // Green
        gamepad->setColorLED(0, 255, 0);
        break;
      case 2:
        // Blue
        gamepad->setColorLED(0, 0, 255);
        break;
    }
    colorIdx++;
  }

  if (gamepad->b()) {
 
    static int led = 0;
    led++;

    gamepad->setPlayerLEDs(led & 0x0f);
  }

  if (gamepad->x()) {

    gamepad->setRumble(0xc0 /* force */, 0xc0 /* duration */);
  }

  char buf[256];
  snprintf(buf, sizeof(buf) - 1,
           "idx=%d, dpad: 0x%02x, buttons: 0x%04x, "
           "axis L: %4li, %4li, axis R: %4li, %4li, "
           "brake: %4ld, throttle: %4li, misc: 0x%02x, "
           "gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d, "
           "battery: %d",
           gamepad->index(),      
           gamepad->dpad(),        
           gamepad->buttons(),     
           gamepad->axisX(),       
           gamepad->axisY(),       
           gamepad->axisRX(),       
           gamepad->axisRY(),     
           gamepad->brake(),       
           gamepad->throttle(),     
           gamepad->miscButtons(),  
           gamepad->gyroX(),       
           gamepad->gyroY(),        
           gamepad->gyroZ(),        
           gamepad->accelX(),       
           gamepad->accelY(),       
           gamepad->accelZ(),       
           gamepad->battery()       
  );
  Serial.println(buf);

}

void processMouse(ControllerPtr mouse) {
  char buf[160];
  sprintf(buf,
          "idx=%d, deltaX:%4li, deltaY:%4li, buttons: 0x%04x, misc: 0x%02x, "
          "scrollWheel: %d, battery=%d",
          mouse->index(),       
          mouse->deltaX(),       
          mouse->deltaY(),       
          mouse->buttons(),      
          mouse->miscButtons(),  
          mouse->scrollWheel(),  
          mouse->battery()       
  );
  Serial.println(buf);
}

void processBalanceBoard(ControllerPtr balance) {
  char buf[160];
  sprintf(buf,
          "idx=%d, tl:%4i, tr:%4i, bl: %4i, br: %4i, temperature=%d, "
          "battery=%d",
          balance->index(),  
          balance->topLeft(), balance->topRight(), balance->bottomLeft(),
          balance->bottomRight(), balance->temperature(),
          balance->battery()  
  );
  Serial.println(buf);
}

void loop() {

  BP32.update();

  for (int i = 0; i < BP32_MAX_CONTROLLERS; i++) {
    ControllerPtr myController = myControllers[i];

    if (myController && myController->isConnected()) {
      if (myController->isGamepad())
        processGamepad(myController);
      else if (myController->isMouse())
        processMouse(myController);
      else if (myController->isBalanceBoard())
        processBalanceBoard(myController);
    }
  }
  delay(150);
}

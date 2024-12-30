// Copyright 2021 - 2022, Ricardo Quesada, http://retro.moe
// SPDX-License-Identifier: Apache-2.0 or LGPL-2.1-or-later

#ifndef BP32_GAMEPAD_PROPERTIES_H
#define BP32_GAMEPAD_PROPERTIES_H

//
// Deprecated. Use ControllerProperties.h instead
//

#include "ControllerProperties.h"

enum {
  GAMEPAD_PROPERTY_FLAG_RUMBLE = CONTROLLER_PROPERTY_FLAG_RUMBLE,
  GAMEPAD_PROPERTY_FLAG_PLAYER_LEDS = CONTROLLER_PROPERTY_FLAG_PLAYER_LEDS,
  GAMEPAD_PROPERTY_FLAG_PLAYER_LIGHTBAR =
      CONTROLLER_PROPERTY_FLAG_PLAYER_LIGHTBAR,

  GAMEPAD_PROPERTY_FLAG_GAMEPAD = CONTROLLER_PROPERTY_FLAG_GAMEPAD,
  GAMEPAD_PROPERTY_FLAG_MOUSE = CONTROLLER_PROPERTY_FLAG_MOUSE,
  GAMEPAD_PROPERTY_FLAG_KEYBOARD = CONTROLLER_PROPERTY_FLAG_KEYBOARD,
};

using GamepadProperties = ControllerProperties;
using GamepadPropertiesPtr = ControllerPropertiesPtr;

#endif  // BP32_GAMEPAD_PROPERTIES_H

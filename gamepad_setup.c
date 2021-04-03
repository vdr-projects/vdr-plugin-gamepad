/*
 * gamepad.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */
#include <vdr/plugin.h>

#include "gamepad_setup.h"
#include "gamepad_config.h"

cGamepadSetup::cGamepadSetup()
{
   data = gamepadConfig;

   Add(new cMenuEditStrItem(tr("Joystick Device"), data.device, sizeof(data.device)));
   Add(new cMenuEditBoolItem(tr("Test Mode (shows generic event IDs on OSD)"), &data.testMode, tr("off"), tr("on")));
}

void cGamepadSetup::Store(void)
{
   gamepadConfig = data;
   SetupStore("JoystickDevice", data.device);
   SetupStore("TestMode", data.testMode);
}

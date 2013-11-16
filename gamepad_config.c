/*
 * gamepad.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */
#include "gamepad_config.h"
#include <string.h>

cGamepadConfig::cGamepadConfig()
{
    strncpy(device, "/dev/input/js0", sizeof(device));
    testMode = 0;
}

cGamepadConfig gamepadConfig;

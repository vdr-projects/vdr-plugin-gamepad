/*
 * gamepad.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */
#ifndef GAMEPAD_CONFIG_H
#define GAMEPAD_CONFIG_H

class cGamepadConfig
{
public:
    cGamepadConfig(void);
    char device[200];
    int testMode;
};

extern cGamepadConfig gamepadConfig;

#endif
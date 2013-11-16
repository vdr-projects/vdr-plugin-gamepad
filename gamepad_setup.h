/*
 * gamepad.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */
#ifndef GAMEPAD_SETUP_H
#define GAMEPAD_SETUP_H

#include <vdr/plugin.h>
#include "gamepad_config.h"

class cGamepadSetup : public cMenuSetupPage {
private:
    cGamepadConfig data;

protected:
    virtual void Store(void);
public:
    cGamepadSetup();
};

#endif
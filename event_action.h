/*
 * gamepad.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */
#ifndef EVENT_ACTION_H
#define EVENT_ACTION_H

#include <vdr/remote.h>
#include "gamepad_handler.h"

class cEventAction
{
private:
    cGamepadHandler::event_type state;
    const eKeys key;
    const bool autorepeat;
    uint64_t lastEventTime;
    const int repeatTime;
    const int repeatHold;

    inline void fire(void);

public:
    cEventAction(eKeys _key, bool _autorepeat = false, int repeatTimeMs = 100, int repeatStartHoldMs = 300);
    void event(cGamepadHandler::generic_event *event);
    void repeat();
};

#endif
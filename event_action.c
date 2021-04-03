/*
 * gamepad.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */
#include "event_action.h"
#include <vdr/tools.h>

cEventAction::cEventAction(eKeys _key, bool _autorepeat, int repeatTimeMs, int repeatStartHoldMs) :  state(cGamepadHandler::RELEASE), key(_key), autorepeat(_autorepeat), lastEventTime(0), repeatTime(repeatTimeMs), repeatHold(repeatStartHoldMs)
{

}

void cEventAction::event(cGamepadHandler::generic_event *event)
{
    state = event->type;
    if (state == cGamepadHandler::PRESS)
    {
        fire();
    }
}

void cEventAction::fire()
{
    cRemote::Put(key);
    lastEventTime = cTimeMs::Now();
}

void cEventAction::repeat()
{
    if (autorepeat)
    {
        switch (state)
        {
            case cGamepadHandler::REPEAT:
                if ((cTimeMs::Now() - lastEventTime) > repeatTime)
                {
                    fire();
                }
                break;

            case cGamepadHandler::PRESS:
                if ((cTimeMs::Now() - lastEventTime) > repeatHold)
                {
                    fire();
                    state = cGamepadHandler::REPEAT;
                }
                break;
            default:
                break;
        }
    }
}

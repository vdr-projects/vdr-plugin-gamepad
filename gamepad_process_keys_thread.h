/*
 * gamepad.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */
#ifndef GAMEPAD_PROCESS_KEYS_H
#define GAMEPAD_PROCESS_KEYS_H

#include <vdr/thread.h>
#include "gamepad_handler_thread.h"
#include "event_action.h"

class cGamepadProcessKeysThread: public cThread {
    private:
        cGamepadHandlerThread *handler;
        cEventFifo *event_queue;
        cEventAction *keyMap[cGamepadHandler::MAX_KEYS];

    protected:
        virtual void Action(void);

    public:
        cGamepadProcessKeysThread();
        void Cancel(int WaitSeconds = 0);
        virtual ~cGamepadProcessKeysThread();
        void loadKeyMap(void);
};

#endif

/*
 * gamepad.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */
#ifndef GAMEPAD_HANDLER_THREAD_H
#define GAMEPAD_HANDLER_THREAD_H

#include <vdr/thread.h>
#include "gamepad_handler.h"
#include "fifo.h"

typedef cFifo<cGamepadHandler::generic_event> cEventFifo;

class cGamepadHandlerThread : public cThread
{
    private:
        cEventFifo *fifo;

    protected:
        virtual void Action(void);

    public:
        void Cancel(int WaitSeconds = 0);
        cGamepadHandlerThread(cEventFifo *_fifo);
        virtual ~cGamepadHandlerThread();
};

#endif

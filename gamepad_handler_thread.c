/*
 * gamepad.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */
#include "gamepad_handler_thread.h"
#include <vdr/tools.h>
#include <vdr/skins.h>
#include "gamepad_config.h"

cGamepadHandlerThread::cGamepadHandlerThread(cEventFifo *_fifo) : cThread("gamepad_handler")
{
    fifo = _fifo;
}

void cGamepadHandlerThread::Action()
{
    cGamepadHandler gamepad;
    gamepad.connect(gamepadConfig.device);

    if (gamepad.ready())
    {
        isyslog("[gamepad] Use %s", gamepad.name());
        isyslog("[gamepad] Number of Axes: %d", gamepad.axes());
        isyslog("[gamepad] Number of Buttons: %d", gamepad.buttons());

        while (Running())
        {
            cGamepadHandler::generic_event *event;

            event = gamepad.event();
            if (event != NULL)
            {
                dsyslog("[gamepad] got %d (%d)", event->key, event->type);

                fifo->put(event);
            }
        }
    }
    else
    {
        isyslog("[gamepad] connection failed");
    }

    gamepad.disconnect();
}

void cGamepadHandlerThread::Cancel(int waitSeconds)
{
    cThread::Cancel(waitSeconds);
}

cGamepadHandlerThread::~cGamepadHandlerThread()
{

}

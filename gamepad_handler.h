/*
 * gamepad.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */
#ifndef GAMEPAD_HANDLER_H
#define GAMEPAD_HANDLER_H

#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>

class cGamepadHandler
{
    public:
        typedef enum
        {
            PRESS,
            RELEASE,
            REPEAT,
        } event_type;

        typedef struct
        {
            int key;
            event_type type;
        } generic_event;

        cGamepadHandler();
        virtual ~cGamepadHandler();

        bool connect(const char* dev);
        void disconnect();
        bool ready();

        int buttons();
        int axes();
        char* name();

        generic_event* event(void);

        static const int MAX_NUM_BUTTONS = 16;
        static const int MAX_NUM_AXIS = 8;
        static const int AXIS_DEAD_ZONE = 15000;
        static const int MAX_KEYS = MAX_NUM_BUTTONS + 2*MAX_NUM_AXIS;

    private:
        int m_fd;
        generic_event m_event;
        int m_axis_state[MAX_NUM_AXIS];
};

#endif

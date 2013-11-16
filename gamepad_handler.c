/*
 * gamepad.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */
#include "gamepad_handler.h"
#include <linux/joystick.h>
#include <vdr/tools.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

cGamepadHandler::cGamepadHandler() : m_fd(0)
{

}

cGamepadHandler::~cGamepadHandler()
{
    disconnect();
}

bool cGamepadHandler::connect(const char* device)
{
    this->m_fd = open(device, O_RDONLY);
    return this->m_fd;
}

void cGamepadHandler::disconnect()
{
    if (m_fd)
    {
        close(this->m_fd);
        this->m_fd = 0;
    }
}

bool cGamepadHandler::ready()
{
    return (this->m_fd > 0);
}

int cGamepadHandler::buttons()
{
    char buttons;

    ioctl(this->m_fd, JSIOCGBUTTONS, &buttons);
    return (int)buttons;
}

int cGamepadHandler::axes()
{
    char axes;

    ioctl(this->m_fd, JSIOCGAXES, &axes);
    return (int)axes;
}

char* cGamepadHandler::name()
{
    char* name = new char[128];

    ioctl(this->m_fd, JSIOCGNAME(128), name);
    return name;
}

cGamepadHandler::generic_event* cGamepadHandler::event()
{
    js_event raw_event;
    int read_bytes;
    fd_set set;
    struct timeval timeout;

    FD_ZERO(&set);
    FD_SET(this->m_fd, &set);

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    if (select(FD_SETSIZE, &set, NULL, NULL, &timeout) > 0)
    {
        read_bytes = read(this->m_fd, &raw_event, sizeof(raw_event));

        if (read_bytes == sizeof(raw_event))
        {
            if (raw_event.type == JS_EVENT_BUTTON)
            {
                if (raw_event.value < MAX_NUM_BUTTONS)
                {
                    if (raw_event.value == 1)           // button pressed
                    {
                        dsyslog("[gamepad] Pressed %d", raw_event.number);
                        this->m_event.type = PRESS;
                    }
                    else                                // button released
                    {
                        dsyslog("[gamepad] Released %d", raw_event.number);
                        this->m_event.type = RELEASE;
                    }
                    this->m_event.key = raw_event.number;

                    return &(this->m_event);
                }
            }
            else if (raw_event.type == JS_EVENT_AXIS)
            {
                if (raw_event.number < MAX_NUM_AXIS)
                {
                    int last_axis_state;

                    last_axis_state = m_axis_state[raw_event.number];
                    m_axis_state[raw_event.number] = raw_event.value;

                    if (last_axis_state < AXIS_DEAD_ZONE && last_axis_state > -AXIS_DEAD_ZONE)
                    {
                        this->m_event.key = MAX_NUM_BUTTONS + raw_event.number*2 + (raw_event.value >= 0);
                        this->m_event.type = PRESS;

                        if (raw_event.value >= AXIS_DEAD_ZONE)
                        {
                            dsyslog("[gamepad] Axis+ %d", raw_event.number);
                            return &(this->m_event);
                        }
                        else if (raw_event.value <= -AXIS_DEAD_ZONE)
                        {
                            dsyslog("[gamepad] Axis- %d", raw_event.number);
                            return &(this->m_event);
                        }
                    }
                    else if (raw_event.value < AXIS_DEAD_ZONE && raw_event.value > -AXIS_DEAD_ZONE)
                    {
                        dsyslog("[gamepad] Axis stop %d", raw_event.number);

                        this->m_event.key = MAX_NUM_BUTTONS + raw_event.number*2 + (last_axis_state >= 0);
                        this->m_event.type = RELEASE;

                        return &(this->m_event);
                    }
                }
            }
        }
    }

    return NULL;
}

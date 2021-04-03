/*
 * gamepad.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */
#include "gamepad_process_keys_thread.h"
#include <vdr/remote.h>
#include <vdr/tools.h>
#include <vdr/plugin.h>
#include "gamepad_config.h"
#include <ctype.h>

cGamepadProcessKeysThread::cGamepadProcessKeysThread() : handler(NULL), event_queue(NULL)
{
    for (int i=0; i<cGamepadHandler::MAX_KEYS; i++) {
        keyMap[i] = NULL;
    }
}

void cGamepadProcessKeysThread::loadKeyMap()
{
    char path[300];
    char *line;
    FILE *file;

    line = (char*)malloc(300);
    if (line)
    {
        strncpy(path, cPlugin::ConfigDirectory("gamepad"), 300);
        strncat(path, "/keymap.txt", 300);

        file = fopen(path, "r");
        if (file)
        {
            int lineNr = 0;

            while (fgets(line, 300, file) != NULL)
            {
                lineNr++;
                if (strlen(line) > 0)
                {
                    if (line[0] != '#')
                    {
                        int key = -1;
                        eKeys vdrKey = kNone;
                        char *keyname = NULL;
                        int repeat = -1;
                        int hold = -1;

                        bool ok = true;

                        char *end = line + strlen(line) - 1;
                        while(end > line && isspace(*end)) end--;
                        *(end+1) = 0;

                        char *token = strtok (line, " ");
                        for (int i=0; i<4; i++)
                        {
                            if (token != NULL)
                            {
                                switch (i)
                                {
                                    case 0:
                                        key = atoi(token);
                                        break;
                                    case 1:
                                        if      (strcmp(token, "Down"       ) == 0) vdrKey = kDown      ;
                                        else if (strcmp(token, "Up"         ) == 0) vdrKey = kUp        ;
                                        else if (strcmp(token, "Menu"       ) == 0) vdrKey = kMenu      ;
                                        else if (strcmp(token, "Ok"         ) == 0) vdrKey = kOk        ;
                                        else if (strcmp(token, "Back"       ) == 0) vdrKey = kBack      ;
                                        else if (strcmp(token, "Left"       ) == 0) vdrKey = kLeft      ;
                                        else if (strcmp(token, "Right"      ) == 0) vdrKey = kRight     ;
                                        else if (strcmp(token, "Red"        ) == 0) vdrKey = kRed       ;
                                        else if (strcmp(token, "Green"      ) == 0) vdrKey = kGreen     ;
                                        else if (strcmp(token, "Yellow"     ) == 0) vdrKey = kYellow    ;
                                        else if (strcmp(token, "Blue"       ) == 0) vdrKey = kBlue      ;
                                        else if (strcmp(token, "Info"       ) == 0) vdrKey = kInfo      ;
                                        else if (strcmp(token, "0"          ) == 0) vdrKey = k0         ;
                                        else if (strcmp(token, "1"          ) == 0) vdrKey = k1         ;
                                        else if (strcmp(token, "2"          ) == 0) vdrKey = k2         ;
                                        else if (strcmp(token, "3"          ) == 0) vdrKey = k3         ;
                                        else if (strcmp(token, "4"          ) == 0) vdrKey = k4         ;
                                        else if (strcmp(token, "5"          ) == 0) vdrKey = k5         ;
                                        else if (strcmp(token, "6"          ) == 0) vdrKey = k6         ;
                                        else if (strcmp(token, "7"          ) == 0) vdrKey = k7         ;
                                        else if (strcmp(token, "8"          ) == 0) vdrKey = k8         ;
                                        else if (strcmp(token, "9"          ) == 0) vdrKey = k9         ;
                                        else if (strcmp(token, "PlayPause"  ) == 0) vdrKey = kPlayPause ;
                                        else if (strcmp(token, "Play"       ) == 0) vdrKey = kPlay      ;
                                        else if (strcmp(token, "Pause"      ) == 0) vdrKey = kPause     ;
                                        else if (strcmp(token, "Stop"       ) == 0) vdrKey = kStop      ;
                                        else if (strcmp(token, "Record"     ) == 0) vdrKey = kRecord    ;
                                        else if (strcmp(token, "FastFwd"    ) == 0) vdrKey = kFastFwd   ;
                                        else if (strcmp(token, "FastRew"    ) == 0) vdrKey = kFastRew   ;
                                        else if (strcmp(token, "Next"       ) == 0) vdrKey = kNext      ;
                                        else if (strcmp(token, "Prev"       ) == 0) vdrKey = kPrev      ;
                                        else if (strcmp(token, "Power"      ) == 0) vdrKey = kPower     ;
                                        else if (strcmp(token, "ChanUp"     ) == 0) vdrKey = kChanUp    ;
                                        else if (strcmp(token, "ChanDn"     ) == 0) vdrKey = kChanDn    ;
                                        else if (strcmp(token, "ChanPrev"   ) == 0) vdrKey = kChanPrev  ;
                                        else if (strcmp(token, "VolUp"      ) == 0) vdrKey = kVolUp     ;
                                        else if (strcmp(token, "VolDn"      ) == 0) vdrKey = kVolDn     ;
                                        else if (strcmp(token, "Mute"       ) == 0) vdrKey = kMute      ;
                                        else if (strcmp(token, "Audio"      ) == 0) vdrKey = kAudio     ;
                                        else if (strcmp(token, "Subtitles"  ) == 0) vdrKey = kSubtitles ;
                                        else if (strcmp(token, "Schedule"   ) == 0) vdrKey = kSchedule  ;
                                        else if (strcmp(token, "Channels"   ) == 0) vdrKey = kChannels  ;
                                        else if (strcmp(token, "Timers"     ) == 0) vdrKey = kTimers    ;
                                        else if (strcmp(token, "Recordings" ) == 0) vdrKey = kRecordings;
                                        else if (strcmp(token, "Setup"      ) == 0) vdrKey = kSetup     ;
                                        else if (strcmp(token, "Commands"   ) == 0) vdrKey = kCommands  ;
                                        else if (strcmp(token, "User0"      ) == 0) vdrKey = kUser0     ;
                                        else if (strcmp(token, "User1"      ) == 0) vdrKey = kUser1     ;
                                        else if (strcmp(token, "User2"      ) == 0) vdrKey = kUser2     ;
                                        else if (strcmp(token, "User3"      ) == 0) vdrKey = kUser3     ;
                                        else if (strcmp(token, "User4"      ) == 0) vdrKey = kUser4     ;
                                        else if (strcmp(token, "User5"      ) == 0) vdrKey = kUser5     ;
                                        else if (strcmp(token, "User6"      ) == 0) vdrKey = kUser6     ;
                                        else if (strcmp(token, "User7"      ) == 0) vdrKey = kUser7     ;
                                        else if (strcmp(token, "User8"      ) == 0) vdrKey = kUser8     ;
                                        else if (strcmp(token, "User9"      ) == 0) vdrKey = kUser9     ;

                                        keyname = token;
                                        break;
                                    case 2:
                                        repeat = atoi(token);
                                        break;
                                    case 3:
                                        hold = atoi(token);
                                        break;
                                }
                            }

                            token = strtok (NULL, " ");
                        }

                        if (key < 0 || key >= cGamepadHandler::MAX_KEYS) ok = false;
                        if (vdrKey == kNone) ok = false;
                        if (repeat < 0) ok = false;
                        if (hold < 0) ok = false;

                        if (ok)
                        {
                            keyMap[key] = new cEventAction(vdrKey, repeat!=0, repeat, hold);
                            dsyslog("[gamepad] map generic key %d to %s (autorepeat=%d, hold=%d)", key, keyname, repeat, hold);
                        }
                        else
                        {
                            esyslog("[gamepad] %s mapping error in line %d", path, lineNr);
                        }
                    }
                }
            }

            fclose(file);
        }
        else
        {
            esyslog("[gamepad] no keymap found");
        }

        free(line);
    }
    else {
        esyslog("ERROR: out of memory - abort!");
        abort();
    }
}

void cGamepadProcessKeysThread::Action()
{
    event_queue = new cEventFifo(16);
    handler = new cGamepadHandlerThread(event_queue);

    handler->Start();

    while (Running())
    {
        cGamepadHandler::generic_event *event;

        event = event_queue->read(50);
        if (event != NULL)
        {
            dsyslog("[gamepad] read %d (%d)", event->key, event->type);

            if (gamepadConfig.testMode)
            {
                if (event->type == cGamepadHandler::PRESS)
                {
                    char str[20];
                    sprintf(str, "Generic Event %d", event->key);
                    Skins.QueueMessage(mtInfo, str, 1, -1);
                }
            }
            else
            {
                if (keyMap[event->key])
                {
                    keyMap[event->key]->event(event);
                }
            }
            event_queue->remove();
        }
        for (int i=0; i<cGamepadHandler::MAX_KEYS; i++) {
            if (keyMap[i])
            {
                keyMap[i]->repeat();
            }
        }
    }
}

void cGamepadProcessKeysThread::Cancel(int waitSeconds)
{
    handler->Cancel(waitSeconds);
    cThread::Cancel(waitSeconds);
}

cGamepadProcessKeysThread::~cGamepadProcessKeysThread()
{
    if (handler) {
        delete handler;
    }

    if (event_queue) {
        delete event_queue;
    }
}

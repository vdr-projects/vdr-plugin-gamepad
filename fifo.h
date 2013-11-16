/*
 * gamepad.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */
#ifndef FIFO_H
#define FIFO_H

#include <vdr/tools.h>

template<class T> class cFifo {
private:
    cMutex mutex;
    cCondWait wake;
    T *buffer;
    int readp;
    int writep;
    const int length;
    int size;

public:
    cFifo(int _length) : readp(0), writep(0), length(_length), size(0)
    {
        buffer = (T*)malloc(length * sizeof(T));
        if (!buffer)
        {
            esyslog("ERROR: out of memory - abort!");
            abort();
        }
    }

    // non blocking write, returns true on success
    bool put(T *item)
    {
        bool success = false;
        mutex.Lock();
        if (size < length)
        {
            buffer[writep] = *item;
            size++;
            writep = (writep + 1) % length;
            success = true;
            wake.Signal();
        }
        mutex.Unlock();
        return success;
    }

    // returns a pointer to the head element
    // to delete it from the fifo use remove
    T* read(int timeoutMs)
    {
        bool wait = false;
        T* ret = NULL;
        mutex.Lock();
        if (size == 0) {
            wait = true;
        }
        mutex.Unlock();
        if (wait)
        {
            wake.Wait(timeoutMs);
        }
        mutex.Lock();
        if (size != 0)
        {
            ret = &(buffer[readp]);
        }
        mutex.Unlock();
        return ret;
    }

    void remove() {
        mutex.Lock();
        if (size >= 0)
        {
            readp = (readp + 1) % length;
            size--;
        }
        mutex.Unlock();
    }
};

#endif
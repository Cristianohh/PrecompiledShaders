/* Copyright (c) 2014, Intel Corporation
*
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met:
*
* - Redistributions of source code must retain the above copyright notice, 
*   this list of conditions and the following disclaimer.
* - Redistributions in binary form must reproduce the above copyright notice, 
*   this list of conditions and the following disclaimer in the documentation 
*   and/or other materials provided with the distribution.
* - Neither the name of Intel Corporation nor the names of its contributors 
*   may be used to endorse or promote products derived from this software 
*   without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
* POSSIBILITY OF SUCH DAMAGE.
*
*/

#include "timer.h"
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <time.h>

static struct timespec _time_difference(struct timespec a, struct timespec b)
{
    struct timespec temp;
    temp.tv_sec = a.tv_sec-b.tv_sec;
    temp.tv_nsec = a.tv_nsec-b.tv_nsec;
    return temp;
}

struct Timer
{
    struct timespec start_time;
    struct timespec prev_time;
};

Timer* create_timer(void)
{
    Timer* timer = (Timer*)calloc(1, sizeof(*timer));
    reset_timer(timer);
    return timer;
}
void destroy_timer(Timer* timer)
{
    free(timer);
}
void reset_timer(Timer* timer)
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    timer->prev_time = timer->start_time = time;
}
double get_delta_time(Timer* timer)
{
    struct timespec time;
    struct timespec diff;
    clock_gettime(CLOCK_MONOTONIC, &time);
    diff = _time_difference(time, timer->prev_time);
    timer->prev_time = time;
    return diff.tv_sec + diff.tv_nsec*1.0/1000000000;
}
double get_running_time(Timer* timer)
{
    struct timespec time;
    struct timespec diff;
    clock_gettime(CLOCK_MONOTONIC, &time);
    diff = _time_difference(time, timer->start_time);
    return diff.tv_sec + diff.tv_nsec*1.0/1000000000;
}
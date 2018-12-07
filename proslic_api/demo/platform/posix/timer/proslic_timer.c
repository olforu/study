/*
** Copyright 2015-2017, Silicon Labs
** $Id: proslic_timer.c 6801 2017-09-14 22:40:11Z nizajerk $
**
** System specific functions implementation file
**
** Distributed by:
** Silicon Laboratories, Inc
**
** File Description:
** This is the implementation file for the system specific timer functions.
**
**
*/

#include "si_voice_datatypes.h"
#include "proslic.h"
#include "si_voice_timer_intf.h"
#include "proslic_timer.h"

#ifdef __MINGW32__
#define timersub(NOW, THEN, RESULT) \
  (RESULT)->tv_sec = (NOW)->tv_sec - (THEN)->tv_sec;\
  (RESULT)->tv_usec = (NOW)->tv_usec - (THEN)->tv_usec;\
  if( (RESULT)->tv_usec < 0) /* check for NOW < THEN */\
  {\
    --(RESULT)->tv_usec;\
    (RESULT)->tv_usec += 1000000;\
  }
#endif


#ifdef WIN32_GET_TIME_NEEDED
/* This implements the minimum gettimeofday needed for our purposes, for a "real" windows system
   running for days, this will not work for us without a fuller implementation since we don't
   calculate the seconds structure completely, so we would have a wrap issue on the month boundaries.

   It does not return back the same values as found in other OS's such as Linux. Linux returns back
   uSec instead of mSec values (which is what is implemented here).
*/
int gettimeofday(struct timeval *tv, void *tz)
{
  SYSTEMTIME sys_time;
  SILABS_UNREFERENCED_PARAMETER(tz);

  if(tv != NULL)
  {
    GetSystemTime(&sys_time);
    tv->tv_sec = sys_time.wSecond;
    tv->tv_sec += (sys_time.wMinute*60);
    tv->tv_sec += (sys_time.wHour*60*60);
    tv->tv_sec += (sys_time.wDay*24*60*60);
    tv->tv_usec= sys_time.wMilliseconds;
    return 0;
  }
  return -1;
}
#endif

/*
** These are the global functions
*/

/*
** Function: SYSTEM_TimerInit
*/
void TimerInit (systemTimer_S *pTimerObj)
{
  SILABS_UNREFERENCED_PARAMETER(pTimerObj);

}
/*
** Function: SYSTEM_Delay
*/
int time_DelayWrapper (void *hTimer, int timeInMs)
{
  SILABS_UNREFERENCED_PARAMETER(hTimer);

#ifdef SILABS_USE_USLEEP
  usleep(timeInMs*1000);
#else
  struct timespec myDelay;
  if(timeInMs >= 1000)
  {
    myDelay.tv_sec = timeInMs/1000;
  }
  else
  {
    myDelay.tv_sec = 0;
  }

  myDelay.tv_nsec = (timeInMs - (myDelay.tv_sec*1000))*1000000UL;

  nanosleep(&myDelay, NULL);
#endif
  return RC_NONE;
}

/*
** Function: SYSTEM_TimeElapsed
*/
int time_TimeElapsedWrapper (void *hTimer, void *startTime, int *timeInMs)
{
  SILABS_UNREFERENCED_PARAMETER(hTimer);

  if((startTime != NULL) && (timeInMs != NULL))
  {
#ifdef SILABS_USE_TIMEVAL
    struct timeval timeNow;
    struct timeval result;
    gettimeofday(&timeNow, NULL);

    timersub(&(timeNow),&(((timeStamp *)startTime)->timerObj),&result);

    *timeInMs =(result.tv_usec/1000) + (result.tv_sec *1000);
#else
    struct timespec timeNow;
    struct timespec result;
    clock_gettime(SILABS_CLOCK, &timeNow);

    if( (timeNow.tv_nsec - ((timeStamp *)(startTime))->timerObj.tv_nsec) < 0)
    {
      result.tv_sec = timeNow.tv_sec -1 
        -((timeStamp *)(startTime))->timerObj.tv_sec;
      result.tv_nsec = 1000000000UL + timeNow.tv_nsec 
        -((timeStamp *)(startTime))->timerObj.tv_nsec;
    }
    else
    {
      result.tv_sec = timeNow.tv_sec - 
        ((timeStamp *)(startTime))->timerObj.tv_sec;
      result.tv_nsec = timeNow.tv_nsec - 
        ((timeStamp *)(startTime))->timerObj.tv_nsec;
    }
    *timeInMs = (result.tv_nsec /1000000L) + (result.tv_sec *1000);
#endif
  }

  return RC_NONE;
}

/*
** Function: SYSTEM_GetTime
*/
int time_GetTimeWrapper (void *hTimer, void *time)
{
  SILABS_UNREFERENCED_PARAMETER(hTimer);
  if(time != NULL)
  {
#ifdef SILABS_USE_TIMEVAL
    gettimeofday(&(( (timeStamp *)time)->timerObj), NULL);
#else
    clock_gettime(SILABS_CLOCK, &(((timeStamp *)time)->timerObj));
#endif
  }
  return 0;
}


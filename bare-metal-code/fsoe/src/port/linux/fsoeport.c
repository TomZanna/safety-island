/*********************************************************************
 *        _       _         _
 *  _ __ | |_  _ | |  __ _ | |__   ___
 * | '__|| __|(_)| | / _` || '_ \ / __|
 * | |   | |_  _ | || (_| || |_) |\__ \
 * |_|    \__|(_)|_| \__,_||_.__/ |___/
 *
 * www.rt-labs.com
 * Copyright 2019 rt-labs AB, Sweden.
 * See LICENSE file in the project root for full license information.
 ********************************************************************/

#include "fsoeport.h"
#include "fsoelog.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void fsoeport_log (uint8_t type, const char * fmt, ...)
{
   va_list list;

   switch(FSOELOG_LEVEL_GET (type))
   {
   case FSOELOG_LEVEL_DEBUG:   printf ("[DEBUG] "); break;
   case FSOELOG_LEVEL_INFO:    printf ("[INFO ] "); break;
   case FSOELOG_LEVEL_WARNING: printf ("[WARN ] "); break;
   case FSOELOG_LEVEL_ERROR:   printf ("[ERROR] "); break;
   default: break;
   }

   va_start (list, fmt);
   vprintf (fmt, list);
   va_end (list);
   fflush (stdout);
}

uint32_t fsoeport_current_time_us (void)
{
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return  (uint32_t)(((uint64_t)now.tv_nsec + (uint64_t)now.tv_sec * 1000000000) / 1000);
}

void fsoeport_memcpy (void * dest, const void * src, size_t num)
{
   (void)memcpy (dest, src, num);
}

int32_t fsoeport_memcmp (const void * ptr1, const void * ptr2, size_t num)
{
   return memcmp (ptr1, ptr2, num);
}

void fsoeport_memset (void * ptr, uint8_t value, size_t num)
{
   (void)memset(ptr, value, num);
}

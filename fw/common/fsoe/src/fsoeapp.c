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

/**
 * \file
 * \brief Utility functions to be called from application callback functions
 *
 */

#include <fsoeapp.h>

/*
 * \brief Get description of user error as a string literal
 *
 * See public header file for description
 */
const char * fsoeapp_user_error_description (
   fsoeapp_usererror_t user_error)
{
   const char * description;

   switch (user_error)
   {
   case FSOEAPP_USERERROR_NULL_INSTANCE:
      description = "null instance";
      break;
   case FSOEAPP_USERERROR_UNINITIALISED_INSTANCE:
      description = "uninitialised instance";
      break;
   case FSOEAPP_USERERROR_WRONG_INSTANCE_STATE:
      description = "wrong instance state";
      break;
   case FSOEAPP_USERERROR_NULL_ARGUMENT:
      description = "null argument";
      break;
   case FSOEAPP_USERERROR_BAD_CONFIGURATION:
      description = "bad configuration";
      break;
   default:
      description = "invalid error code";
      break;
   }

   return description;
}

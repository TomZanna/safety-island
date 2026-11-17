#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include <string.h>
#include <cc.h>
//#include <kern.h>
//#include <xmc4.h>
//#include <bsp.h>
//#include "esc_hw.h"
#include "esc.h"
//#include "ecat_slv.h"
//#include "config.h"
#include "utypes.h"
#include "fsoeslave.h"
#include "fsoeapp.h"

/* Min & Max watchdog */
#define FSOE_MIN_WATCHDOG   20
#define FSOE_MAX_WATCHDOG   1000
/* Running lockstep =1 , running 2 CPUs =0 */
#define FSOE_REDUNDANT_SCL_IN_HW   1
//#define FSOE_REDUNDANT_SCL_IN_HW   0

//#define SAFE_APP_LOG
#ifdef SAFE_APP_LOG
#define rprintf printf
#define rprintp printf
#else
#define rprintf
#define rprintp
#endif


/* Local Safety Data Inputs/Outputs */
typedef struct safety_in
{
   uint16_t SafeInput1[2];
} __attribute__((packed)) safety_in_t;
static safety_in_t safeInput1;

typedef struct safety_out
{
   uint16_t SafeOutput1[2];
} __attribute__((packed)) safety_out_t;
static safety_out_t safeOutput1;

/* Parameters to be received from master */
typedef struct parameters
{
   uint16_t watchdog_size;
   uint16_t watchdog_timeout_ms;
   uint16_t application_size;
   uint16_t app_param1;
   uint16_t app_param2;
} __attribute__((packed)) parameters_t;

/* Parameters to be received from master */
static const parameters_t parameters =
{
   2,       /* watchdog_size */
   0x0506,  /* watchdog_timeout_ms */
   4,       /* application_size */
   1,
   2, /* application */
};

/* FSoE utility variables */
static fsoeslave_t fsoe_slave;
static fsoeslave_syncstatus_t safetdata_status;


extern void updateFSout(uint16_t outWord);
extern uint16_t updateFSin(void);
extern uint8_t FSinBuf[];		//FailSafe Inpout buffer
extern uint8_t FSoutBuf[];		//FailSafe Output buffer


uint16_t fsoeapp_generate_session_id (void * app_ref)
{
#if FSOE_REDUNDANT_SCL_IN_HW
   (void)app_ref;
//   return (uint16_t) rand ();
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return (uint16_t)now.tv_nsec;
#else
   /* share session ID between instances */
   static uint16_t  last_rand = 0;
   fsoeslave_t * fsoe_slave_temp = (fsoeslave_t *)app_ref;
   if(fsoe_slave_temp == &fsoe_slave2)
   {
      last_rand = (uint16_t) rand ();
   }

   return last_rand;
#endif
}

/* FSoE stack send data to  black channel */
void fsoeapp_send (void * app_ref, const void * buffer, size_t size)
{
   (void)app_ref;
#if FSOE_REDUNDANT_SCL_IN_HW
   memcpy (FSinBuf, buffer, size);
#else
  /* Verify encoded safety data with second instance before sending */
   /* Compare buffer to send */
   if(memcmp(buffer, slave2_send_buf, size) != 0)
   {

      rprintp ("Mismatch in send safety data!\n");
      fsoeslave_set_reset_request_flag(&fsoe_slave);
      /*
       * Assume the other instance also reset to become synchronized
       * fsoeslave_set_reset_request_flag(&fsoe_slave2);
       */
      return;
   }
#endif
}

/* FSoE stack receive data from black channel */
size_t fsoeapp_recv (void * app_ref, void * buffer, size_t length)
{
   (void)app_ref;
   /* Ignore data sent in states < OP */
//?????   if((CC_ATOMIC_GET(ESCvar.App.state) & APPSTATE_OUTPUT) <= 1)
//   {
//      return 0;
//   }

   /*  Copy processdata buffers to the FSoE stack buffer */
	memcpy (buffer, FSoutBuf, length);
   return length;
}

uint8_t fsoeapp_verify_parameters (
      void * app_ref,
      uint16_t timeout_ms,
      const void * app_parameters,
      size_t size)
{
   (void)app_ref;
   /* Verify watchdog */
   if (timeout_ms < FSOE_MIN_WATCHDOG || timeout_ms > FSOE_MAX_WATCHDOG)
   {
      /* ERRORCODE_INVALID_COMPARA */
      return FSOEAPP_STATUS_BAD_TIMOUT;
   }
   /* Verify application parameters */
   if(size > 0)
   {
      uint16_t * param1 = (uint16_t *)app_parameters;
      uint16_t * param2 = (uint16_t *)app_parameters + 1;

      if(*param1 != parameters.app_param1 ||
         *param2 != parameters.app_param2)
      {
         /* ERRORCODE_INVALID_USERPARA */
         return FSOEAPP_STATUS_BAD_APP_PARAMETER;
      }
   }
   return FSOEAPP_STATUS_OK;
}

void fsoeapp_handle_user_error (
void * app_ref, fsoeapp_usererror_t user_error)
{
   rprintf ("We called an API function incorrectly: %s\n",
         fsoeapp_user_error_description (user_error));
}

void safety_app_run(void)
{
	safeInput1.SafeInput1[0] = updateFSin();
#if FSOE_REDUNDANT_SCL_IN_HW
   /* Run stack to decode data */
   if(fsoeslave_sync_with_master(&fsoe_slave,
         &safeInput1,
         &safeOutput1,
         &safetdata_status) != FSOESLAVE_STATUS_OK)
   {
	  return; /* let fsoeapp_handle_user_error print info and exit here */
   }
   if (safetdata_status.reset_event != FSOESLAVE_RESETEVENT_NONE)
   {
      rprintp ("Connection was reset by %s. Cause: %s\n",
                     safetdata_status.reset_event == FSOESLAVE_RESETEVENT_BY_MASTER ?
                     "master" : "slave",
                     fsoeslave_reset_reason_description (safetdata_status.reset_reason));
   }
   /* Enable data in parameter state */
   if(safetdata_status.current_state == FSOESLAVE_STATE_PARAMETER)
   {
      if(fsoeslave_set_process_data_sending_enable_flag(&fsoe_slave) != FSOESLAVE_STATUS_OK)
      {
         return; /* let fsoeapp_handle_user_error print info and exit here */
      }
   }
   if(safetdata_status.is_process_data_received)
   {
      /* Use data received */
	   updateFSout(safeOutput1.SafeOutput1[0]);
   }
#else
  /* Verify decoded safety data with second instance before passing/using */
   /* Cross check data */
   if(safeInput2.SafeInput1 != safeInput1.SafeInput1 ||
      safeOutput2.SafeOutput1 != safeOutput1.SafeOutput1 )
   {
      rprintp ("Mismatch in received safety data!\n");
      fsoeslave_set_reset_request_flag(&fsoe_slave);
      /*
       * Assume the other instance also reset to become synchronized
       * fsoeslave_set_reset_request_flag(&fsoe_slave2);
       */
      return;
   }
#endif
}

/* Call from EtherCAT slave main function */
int32_t safty_main(uint16_t address)
{
   /* Configuration of slave instance */
   /* Configuration of slave instance */
   const fsoeslave_cfg_t cfg =
   {
	  address,							/* slave_address */
      4,                               /* application_parameters_size */
      sizeof (safety_in_t),            /* inputs_size */
      sizeof (safety_out_t),           /* outputs_size */
   };

   /* Init a slave instance */
   if(fsoeslave_init (&fsoe_slave, &cfg, &fsoe_slave) != FSOESLAVE_STATUS_OK)
   {
      return 1;
   }
   return 0;
}

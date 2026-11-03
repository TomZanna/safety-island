
#include <stdint.h>
#include <string.h>
#include <cc.h>
//#include <kern/kern.h>
//#include <xmc4.h>
//#include <bsp.h>
//#include "esc_hw.h"
#include "esc.h"
//#include "ecat_slv.h"
//#include "config.h"
#include "utypes.h"
#include "fsoemaster.h"
#include "fsoeapp.h"
#include "fsoetypes.h"

/* Min & Max watchdog */
#define FSOE_MIN_WATCHDOG   20
#define FSOE_MAX_WATCHDOG   65535
/* Running lockstep =1 , running 2 CPUs =0 */
#define FSOE_REDUNDANT_SCL_IN_HW   1

#if 0
/* Inputs */
CC_PACKED_BEGIN
typedef struct
{
   CC_PACKED_BEGIN
   struct
   {
      uint8_t Command;
      uint16_t SafeInput1;
      uint16_t CRC0;
      uint16_t SafeInput2;
      uint16_t CRC1;
      uint16_t Connection_ID;
   } CC_PACKED FSOE_Slave_Frame_Elements;
   CC_PACKED_END
} CC_PACKED _Rbuffer;
CC_PACKED_END

/* Outputs */
CC_PACKED_BEGIN
typedef struct
{
   CC_PACKED_BEGIN
   struct
   {
      uint8_t Command;
      uint16_t SafeOutput1;
      uint16_t CRC0;
      uint16_t SafeOutput2;
      uint16_t CRC1;
      uint16_t Connection_ID;
   } CC_PACKED FSOE_Master_Frame_Elements;
   CC_PACKED_END
} CC_PACKED _Wbuffer;
CC_PACKED_END
#endif

/* Local Safety Data Inputs/Outputs */
typedef struct safety_in
{
   uint16_t SafeInput1;
   uint16_t SafeInput2;
} __attribute__((packed)) safety_in_t;
static safety_in_t safeInput1;

typedef struct safety_out
{
   uint16_t SafeOutput1;
   uint16_t SafeOutput2;
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
   uint16_t app_param3;
   uint16_t app_param4;
} __attribute__((packed)) parameters_t;

/* FSoE utility variables */
static fsoemaster_t fsoe_master;
static fsoemaster_syncstatus_t safetdata_status;

uint16_t fsoeapp_generate_session_id (void * app_ref)
{
   (void)app_ref;
   return (uint16_t) rand ();
}

/* FSoE stack send data to  black channel */

extern void putFrame(const void * buffer, size_t length );
extern void getFrame(void * buffer, size_t length);

void fsoeapp_send (void * app_ref, const void * buffer, size_t length)
{
   (void)app_ref;
#if FSOE_REDUNDANT_SCL_IN_HW
   _Rbuffer * fsoe_frame = (_Rbuffer *)buffer;
#if 0
   Obj.FSOE_Slave_Frame_Elements.Command = fsoe_frame->FSOE_Slave_Frame_Elements.Command;
   Obj.FSOE_Slave_Frame_Elements.CRC0 = fsoe_frame->FSOE_Slave_Frame_Elements.CRC0;
   Obj.FSOE_Slave_Frame_Elements.CRC1 = fsoe_frame->FSOE_Slave_Frame_Elements.CRC1;
   Obj.FSOE_Slave_Frame_Elements.Connection_ID = fsoe_frame->FSOE_Slave_Frame_Elements.Connection_ID;
   Obj.FSOE_Slave_Safety_Inputs.SafeInput1 = fsoe_frame->FSOE_Slave_Frame_Elements.SafeInput1;
   Obj.FSOE_Slave_Safety_Inputs.SafeInput2 = fsoe_frame->FSOE_Slave_Frame_Elements.SafeInput2;
#else
	putFrame(buffer, length);
#endif
#else
  /* Verify encoded safety data with second instance before sending */
#endif
}

/* FSoE stack receive data from black channel */
size_t fsoeapp_recv (void * app_ref, void * buffer, size_t length)
{
   (void)app_ref;
   /* Ignore data sent in states < OP */
#if 0
   if((CC_ATOMIC_GET(ESCvar.App.state) & APPSTATE_OUTPUT) <= 1)
   {
      return 0;
   }
#endif
   /*  Copy processdata buffers to the FSoE stack buffer */
   _Wbuffer * fsoe_frame = (_Wbuffer *)buffer;
#if 0
   fsoe_frame->FSOE_Master_Frame_Elements.Command = Obj.FSOE_Master_Frame_Elements.Command;
   fsoe_frame->FSOE_Master_Frame_Elements.CRC0 = Obj.FSOE_Master_Frame_Elements.CRC0;
   fsoe_frame->FSOE_Master_Frame_Elements.CRC1 = Obj.FSOE_Master_Frame_Elements.CRC1;
   fsoe_frame->FSOE_Master_Frame_Elements.Connection_ID = Obj.FSOE_Master_Frame_Elements.Connection_ID;
   fsoe_frame->FSOE_Master_Frame_Elements.SafeOutput1 = Obj.FSOE_Master_Safety_Outputs.SafeOutput1;
   fsoe_frame->FSOE_Master_Frame_Elements.SafeOutput2 = Obj.FSOE_Master_Safety_Outputs.SafeOutput2;
#else
	getFrame(buffer, length);
#endif
   return length;
}

void fsoeapp_handle_user_error (
      void * app_ref,
      fsoeapp_usererror_t user_error)
{
   printf ("We called an API function incorrectly: %s\n",
         fsoeapp_user_error_description (user_error));
}

/* FSoE Application function to be called cyclicly from EtherCAT slave application */
void safety_app_run(void *cmdPtr, void *statPtr)
{
#if FSOE_REDUNDANT_SCL_IN_HW
   /* Run stack to decode data */
   if(fsoemaster_sync_with_slave(&fsoe_master,
         &safeInput1,
         &safeOutput1,
         &safetdata_status) != FSOEMASTER_STATUS_OK)
   {
      return; /* let fsoeapp_handle_user_error print info and exit here */
   }

   if (safetdata_status.reset_event != FSOEMASTER_RESETEVENT_NONE)
   {
	  printf ("Connection was reset by %s. Cause: %s\n",
                     safetdata_status.reset_event == FSOEMASTER_RESETEVENT_NONE ?
                     "master" : "slave",
                     fsoemaster_reset_reason_description (safetdata_status.reset_reason));
   }
   /* Enable data in parameter state */
   if(safetdata_status.current_state == FSOEMASTER_STATE_PARAMETER)
   {
      if(fsoemaster_set_process_data_sending_enable_flag(&fsoe_master) != FSOEMASTER_STATUS_OK)
      {
         return; /* let fsoeapp_handle_user_error print info and exit here */
      }
   }
   if (safetdata_status.is_process_data_received)
   {
//      safeInput1.SafeInput1 = gpio_get(GPIO_BUTTON1);


	   //HMI inputs are available in OUT[16] and OUT[18]
	   memcpy(&safeInput1, &cmdPtr[16], sizeof(safeInput1));
      /* Use data received */
//      gpio_set(GPIO_LED2, safeOutput1.SafeOutput1);
	   //HMI output are available in IN[16] and IN[18]
	   memcpy(&statPtr[16], &safeOutput1.SafeOutput1, 2);
	   memcpy(&statPtr[18], &safeOutput1.SafeOutput2, 2);
   }
   else
	   memset(&statPtr[16], 0, 4);

#else
  /* Verify decoded safety data with second instance before passing/using */
#endif
}
uint16_t application_parameters[4] = { 0,0,0,0 };
/* Call from EtherCAT slave main function */
int32_t safety_main(void)
{
   /* Configuration of slave instance */
   const fsoemaster_cfg_t cfg =
   {
      0xfffe,                          /* slave_address */
      0x0003,                          /* connection_id */
	  0x01f4,                          /* watchdog_timeout_ms */
      application_parameters,          /* application_parameters */
      sizeof(application_parameters),  /* application_parameters_size */
      sizeof (safety_in_t),            /* inputs_size */
      sizeof (safety_out_t),           /* outputs_size */
   };

   /* Init a master instance */
   if(fsoemaster_init (&fsoe_master, &cfg, NULL) != FSOEMASTER_STATUS_OK)
   {
      return 1;
   }
   return 0;
}

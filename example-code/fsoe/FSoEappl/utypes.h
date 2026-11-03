#ifndef __UTYPES_H__
#define __UTYPES_H__

#include "cc.h"

/* Object dictionary storage */

#pragma pack(1)
typedef struct
{
   /* Identity */

   uint32_t serial;

   /* Inputs */

   uint16_t FSOE_Slave_Frame_Elements_NrofIndexes;		//Obj 6000
   struct
   {
      uint8_t Command;
      uint16_t Connection_ID;
      uint16_t CRC0;
      uint16_t CRC1;
   } FSOE_Slave_Frame_Elements;

   uint16_t FSOE_Slave_Safety_Inputs_NrofIndexes;		//Obj 6001
   struct
   {
	  uint16_t SafeInput1;
      uint16_t SafeInput2;
   } FSOE_Slave_Safety_Inputs;

   struct
   {
      uint32_t Button1;
   } Buttons;

   /* Outputs */

   uint16_t FSOE_Master_Frame_Elements_NrofIndexes;	//Obj 7000
   struct
   {
	  uint8_t Command;
	  uint16_t Connection_ID;
      uint16_t CRC0;
      uint16_t CRC1;
   } FSOE_Master_Frame_Elements;

   uint16_t FSOE_Master_Safety_Outputs_NrofIndexes;	//Obj 7001
   struct
   {
	  uint16_t SafeOutput1;
      uint16_t SafeOutput2;
   } FSOE_Master_Safety_Outputs;
   struct
   {
      uint32_t LED1;
   } LEDs;

   /* Parameters */

   uint16_t Safe_Params_NrofIndexes;					//Obj 8000
   struct
   {
	  uint16_t SafeParameters[4];
//      uint16_t SafeParameter2;
//	  uint16_t SafeParameter3;
//	  uint16_t SafeParameter4;
   } SafetyParams;

   uint16_t FSOE_Communication_Parameter_NrofIndexes;	//Obj 9001
   struct
   {
	  uint16_t Version;
      uint16_t Safe_Address;
      uint16_t Connection_ID;
      uint16_t Watchdog_Time;
      uint8_t Unique_ID[6];
      uint16_t Connection_Type;
      uint16_t Com_Parameter_Length;
      uint16_t Appl_Parameter_Length;
   } FSOE_Communication_Parameter;

   /* Manufacturer specific data */

   uint16_t Safe_Address_NrofIndexes;					//Obj F980
   struct
   {
	  uint16_t FSoE_Address;
      uint32_t Serial_Number;
   } Safe_Address;

   uint16_t SafeIOerror_NrofIndexes;
   uint16_t SafeIOerror;

} _Objects;
#pragma pack()

extern _Objects Obj;

/* Inputs FSOE frame  */
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

/* Outputs FSOE frame*/
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

#endif /* __UTYPES_H__ */

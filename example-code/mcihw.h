/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
*/

/**
 * \addtogroup MCI_HW Parallel ESC Access
 * @{
 */

/**
\file mcihw.h
\author EthercatSSC@beckhoff.com
\brief Defines and Macros to access the ESC via a parallel interface

\version 5.10

<br>Changes to version V5.01:<br>
V5.10 HW4: Add volatile directive for direct ESC DWORD/WORD/BYTE access<br>
           Add missing swapping in mcihw.c<br>
           Add "volatile" directive vor dummy vairables in enable and disable SyncManger functions<br>
           Add missing swapping in EL9800hw files<br>
<br>Changes to version - :<br>
V5.01 : Start file change log
 */


#ifndef _MCIHW_H_
#define _MCIHW_H_


/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#include <string.h>
#include <malloc.h>
#include <time.h>
#include "esc.h"
#include <stdint.h>	// for uint32_t and similar types
#include <stdbool.h>
#include <pthread.h>
#include <signal.h>


/*-----------------------------------------------------------------------------------------
------
------    Defines and Types
------
-----------------------------------------------------------------------------------------*/

//#define FPGA_LOCAL  1 // true if FPGA is connected "directly" to CPU
#define FPGA_PCIE   1   // true if FPGA is connected to CPU over PCIe
extern int i_dw_idx;
//#define FPGA_DEBUG_REGISTERS 1

#define ESC_BASE	0xFF2A0000  // FPGA/Avalon base address + FPGA/CPU bridge translation
//#define ESC_BASE	0xFF250000  // FPGA/Avalon base address + FPGA/CPU bridge translation
#define ESC_MAP_SIZE    (128*1024) // slave address space is 128kB
#define ESC_MAP_MASK    (ESC_MAP_SIZE-1)

#define ESC_BASE_SYNCS  0xFF2B0000

#define ESC_HW_CLOCK	CLOCK_MONOTONIC_RAW
//#define ESC_HW_CLOCK	CLOCK_MONOTONIC

#define INIT_ESC_INT    // initialize interrupts
#define ENABLE_ESC_INT()    // enable interrupts
#define DISABLE_ESC_INT()    // enable interrupts

#define ESC_MEM_ADDR    UINT8 /**< \brief ESC address type (32Bit)*/
#define ESC_MEM_SHIFT   0 /**< \brief shift to convert Byte address to ESC address type*/

/**
 * Define the hardware timer ticks per millisecond.
 */
#define ECAT_TIMER_INC_P_MS HW_GetTimerResolution()

#if defined(FPGA_PCIE)

#if defined(FPGA_DEBUG_REGISTERS) // with debug printouts
#define     HW_GetALEventRegister()             ((UINT16)(((volatile UINT32 ESCMEM *)pEsc)[((ESC_AL_EVENT_OFFSET)>>2)])) /**< \brief Returns the first 16Bit of the AL Event register (0x220)*/
#define     HW_GetALEventRegister_Isr           HW_GetALEventRegister /**< \brief Returns the first 16Bit of the AL Event register (0x220).<br>Called from ISRs.*/

#define     HW_EscRead(pData,Address,Len)       printf(DBGFMT "HW_EscRead %p(%d)=", DBGARGS, Address, Len);   \
                                                for (i_dw_idx = 0; i_dw_idx < (Len)>>2; ++i_dw_idx) \
                                                {   \
                                                    ((UINT32*)pData)[i_dw_idx] = ((volatile UINT32*)pEsc)[((Address)>>2) + i_dw_idx];    \
                                                    printf("%x ", ((UINT32*)pData)[i_dw_idx]);    \
                                                }   \
                                                printf("\n"); /**< \brief Generic ESC (register and DPRAM) read access.*/
#define     HW_EscReadIsr                       HW_EscRead /**< \brief Generic ESC (register and DPRAM) read access.<br>Called for ISRs.*/
#define     HW_EscReadByte(ByteValue, Address)  do { printf(DBGFMT "HW_EscReadByte %p=%x\n", DBGARGS, Address, ByteValue);   \
                                                ((ByteValue) = (UINT8)(((volatile UINT32 *)pEsc)[(Address)>>2]));    \
                                                } while(0) /**< \brief 8Bit specific ESC (register and DPRAM) read access.*/
#define     HW_EscReadWord(WordValue, Address)  do { printf(DBGFMT "HW_EscReadWord %p=%x\n", DBGARGS, Address, WordValue);  \
                                                ((WordValue) = (UINT16)(((volatile UINT32 *)pEsc)[(Address)>>2])); \
                                                } while(0) /**< \brief 16Bit specific ESC (register and DPRAM) read access.*/
#define     HW_EscReadDWord(DWordValue, Address) do { printf(DBGFMT "HW_EscReadDWord %p=%x\n", DBGARGS, Address, DWordValue); \
                                                ((DWordValue) = (UINT32)(((volatile UINT32 *)pEsc)[(Address)>>2]));   \
                                                } while(0) /**< \brief 32Bit specific ESC (register and DPRAM) read access.*/
#define     HW_EscReadDWordIsr(DWordValue, Address) HW_EscReadDWord(DWordValue, Address) /**< \brief 32Bit specific ESC (register and DPRAM) read access.<br>Called from ISRs.*/
#define     HW_EscReadMbxMem(pData,Address,Len) HW_EscRead(pData,Address,Len) /**< \brief Macro to copy data from the application mailbox memory(not the ESC memory, this access is handled by HW_EscRead).*/

#define     HW_EscWrite(pData,Address,Len)      printf(DBGFMT "HW_EscWrite %p(%d)=", DBGARGS, Address, Len);    \
                                                for (i_dw_idx = 0; i_dw_idx < (Len)>>2; ++i_dw_idx) \
                                                {   \
                                                    ((volatile UINT32*)pEsc)[((Address)>>2) + i_dw_idx] = ((UINT32*)pData)[i_dw_idx];    \
                                                    printf("%x ", ((UINT32*)pData)[i_dw_idx]);    \
                                                }   \
                                                printf("\n"); /**< \brief Generic ESC (register and DPRAM) write access.*/
#define     HW_EscWriteIsr                      HW_EscWrite /**< \brief Generic ESC (register and DPRAM) write access.<br>Called for ISRs.*/
#define     HW_EscWriteByte(ByteValue, Address) do { printf(DBGFMT "HW_EscWriteByte %p=%x\n", DBGARGS, Address, ByteValue);  \
                                                ((((volatile UINT32 *)pEsc)[(Address)>>2]) = (ByteValue)); \
                                                } while(0) /**< \brief 8Bit specific ESC (register and DPRAM) write access.*/
#define     HW_EscWriteWord(WordValue, Address) do { printf(DBGFMT "HW_EscWriteWord %p=%x\n", DBGARGS, Address, WordValue);  \
                                                ((((volatile UINT32 *)pEsc)[(Address)>>2]) = (WordValue)); \
                                                } while(0) /**< \brief 16Bit specific ESC (register and DPRAM) write access.*/
#define     HW_EscWriteDWord(DWordValue, Address)   do { printf(DBGFMT "HW_EscWriteDWord %p=%x\n", DBGARGS, Address, DWordValue);    \
                                                    ((((volatile UINT32 *)pEsc)[(Address)>>2]) = (DWordValue));    \
                                                    } while(0) /**< \brief 32Bit specific ESC (register and DPRAM) write access.*/
#define     HW_EscWriteDWordIsr(DWordValue, Address) HW_EscWriteDWord(DWordValue, Address) /**< \brief 32Bit specific ESC (register and DPRAM) write access.<br>Called from ISRs.*/
#define     HW_EscWriteMbxMem(pData,Address,Len)    HW_EscWrite(pData,Address,Len) /**< \brief Macro to copy data from the application mailbox memory (not the ESC memory, this access is handled by HW_EscWrite).*/

#else

#define     HW_GetALEventRegister()             ((UINT16)(((volatile UINT32 ESCMEM *)pEsc)[((ESC_AL_EVENT_OFFSET)>>2)])) /**< \brief Returns the first 16Bit of the AL Event register (0x220)*/
#define     HW_GetALEventRegister_Isr           HW_GetALEventRegister /**< \brief Returns the first 16Bit of the AL Event register (0x220).<br>Called from ISRs.*/

#define     HW_EscRead(pData,Address,Len)  		for (i_dw_idx = 0; i_dw_idx < (Len+3)>>2; ++i_dw_idx) \
												{   \
													((UINT32*)(pData))[i_dw_idx] = ((volatile UINT32*)pEsc)[((Address)>>2) + i_dw_idx];     \
												} 	/**< \brief Generic ESC (register and DPRAM) read access.*/
#define     HW_EscReadIsr                       HW_EscRead /**< \brief Generic ESC (register and DPRAM) read access.<br>Called for ISRs.*/
#define     HW_EscReadByte(ByteValue, Address)    ((ByteValue) = (UINT8) ((((volatile UINT32 *)pEsc)[(Address)>>2]) >> ((Address & 3) << 3))) /**< \brief 8Bit specific ESC (register and DPRAM) read access.*/
#define     HW_EscReadByteIsr(ByteValue, Address) HW_EscReadByte(ByteValue, Address)
#define     HW_EscReadWord(WordValue, Address)    ((WordValue) = (UINT16)((((volatile UINT32 *)pEsc)[(Address)>>2]) >> ((Address & 2) << 3))) /**< \brief 16Bit specific ESC (register and DPRAM) read access.*/
#define     HW_EscReadDWord(DWordValue, Address)    ((DWordValue) = (UINT32)(((volatile UINT32 *)pEsc)[(Address)>>2])) /**< \brief 32Bit specific ESC (register and DPRAM) read access.*/
#define     HW_EscReadDWordIsr(DWordValue, Address) ((DWordValue) = (UINT32)(((volatile UINT32 *)pEsc)[(Address)>>2])) /**< \brief 32Bit specific ESC (register and DPRAM) read access.<br>Called from ISRs.*/
#define     HW_EscReadMbxMem(pData,Address,Len) HW_EscRead(pData,Address,Len) /**< \brief Macro to copy data from the application mailbox memory(not the ESC memory, this access is handled by HW_EscRead).*/

#define     HW_EscWrite(pData,Address,Len)      for (i_dw_idx = 0; i_dw_idx < (Len+3)>>2; ++i_dw_idx) \
                                                {   \
													((volatile UINT32*)pEsc)[((Address)>>2) + i_dw_idx] = ((UINT32*)(pData))[i_dw_idx];    \
                                                } /**< \brief Generic ESC (register and DPRAM) write access.*/
#define     HW_EscWriteIsr                      HW_EscWrite /**< \brief Generic ESC (register and DPRAM) write access.<br>Called for ISRs.*/
#define     HW_EscWriteByte(ByteValue, Address)   ((((volatile UINT32 *)pEsc)[(Address)>>2]) = (((((volatile UINT32 *)pEsc)[(Address)>>2]) & (~(0xff << ((Address&3)<<3)))) | ((ByteValue)<<((Address&3)<<3)))) /**< \brief 8Bit specific ESC (register and DPRAM) write access.*/
#define     HW_EscWriteWord(WordValue, Address)   ((((volatile UINT32 *)pEsc)[(Address)>>2]) = (((((volatile UINT32 *)pEsc)[(Address)>>2]) & (~(0xffff << ((Address&2)<<3)))) | ((WordValue)<<((Address&2)<<3)))) /**< \brief 16Bit specific ESC (register and DPRAM) write access.*/
#define     HW_EscWriteDWord(DWordValue, Address)   ((((volatile UINT32 *)pEsc)[(Address)>>2]) = (DWordValue)) /**< \brief 32Bit specific ESC (register and DPRAM) write access.*/
#define     HW_EscWriteDWordIsr(DWordValue, Address)((((volatile UINT32 *)pEsc)[(Address)>>2]) = (DWordValue)) /**< \brief 32Bit specific ESC (register and DPRAM) write access.<br>Called from ISRs.*/
#define     HW_EscWriteMbxMem(pData,Address,Len)    HW_EscWrite(pData,Address,Len) /**< \brief Macro to copy data from the application mailbox memory (not the ESC memory, this access is handled by HW_EscWrite).*/
#endif

#elif defined(FPGA_LOCAL)
#define     HW_GetALEventRegister()             ((((volatile UINT16 ESCMEM *)pEsc)[((ESC_AL_EVENT_OFFSET)>>1)])) /**< \brief Returns the first 16Bit of the AL Event register (0x220)*/
#define     HW_GetALEventRegister_Isr           HW_GetALEventRegister /**< \brief Returns the first 16Bit of the AL Event register (0x220).<br>Called from ISRs.*/

//#define     HW_EscRead(pData,Address,Len)       ESCMEMCPY((MEM_ADDR *)(pData), &((ESC_MEM_ADDR ESCMEM *) pEsc)[((Address))], (Len)) /**< \brief Generic ESC (register and DPRAM) read access.*/
#define     HW_EscRead(pData,Address,Len)       ESCMEMCPY((MEM_ADDR *)(pData), &((ESC_MEM_ADDR ESCMEM *) pEsc)[((Address))], (Len)) /**< \brief Generic ESC (register and DPRAM) read access.*/
#define     HW_EscReadIsr                       HW_EscRead /**< \brief Generic ESC (register and DPRAM) read access.<br>Called for ISRs.*/
#define     HW_EscReadByte(ByteValue, Address)    ((ByteValue) = (UINT8)(((volatile UINT8 *)pEsc)[(Address)])) /**< \brief 8Bit specific ESC (register and DPRAM) read access.*/
#define     HW_EscReadWord(WordValue, Address)    ((WordValue) = (UINT16)(((volatile UINT16 *)pEsc)[(Address)>>1])) /**< \brief 16Bit specific ESC (register and DPRAM) read access.*/
#define     HW_EscReadDWord(DWordValue, Address)    ((DWordValue) = (UINT32)(((volatile UINT32 *)pEsc)[(Address)>>2])) /**< \brief 32Bit specific ESC (register and DPRAM) read access.*/
#define     HW_EscReadDWordIsr(DWordValue, Address) ((DWordValue) = (UINT32)(((volatile UINT32 *)pEsc)[(Address)>>2])) /**< \brief 32Bit specific ESC (register and DPRAM) read access.<br>Called from ISRs.*/
#define     HW_EscReadMbxMem(pData,Address,Len) ESCMBXMEMCPY((MEM_ADDR *)(pData), &((ESC_MEM_ADDR ESCMEM *) pEsc)[((Address))], (Len)) /**< \brief Macro to copy data from the application mailbox memory(not the ESC memory, this access is handled by HW_EscRead).*/

#if 1
#define     HW_EscWrite(pData,Address,Len)      ESCMEMCPY(&((ESC_MEM_ADDR ESCMEM *) pEsc)[((Address))], (MEM_ADDR *)(pData), (Len)) /**< \brief Generic ESC (register and DPRAM) write access.*/
#define     HW_EscWriteIsr                      HW_EscWrite /**< \brief Generic ESC (register and DPRAM) write access.<br>Called for ISRs.*/
#define     HW_EscWriteByte(ByteValue, Address)   ((((volatile UINT8 *)pEsc)[(Address)]) = (ByteValue)) /**< \brief 8Bit specific ESC (register and DPRAM) write access.*/
#define     HW_EscWriteWord(WordValue, Address)   ((((volatile UINT16 *)pEsc)[(Address)>>1]) = (WordValue)) /**< \brief 16Bit specific ESC (register and DPRAM) write access.*/
#define     HW_EscWriteDWord(DWordValue, Address)   ((((volatile UINT32 *)pEsc)[(Address)>>2]) = (DWordValue)) /**< \brief 32Bit specific ESC (register and DPRAM) write access.*/
#define     HW_EscWriteDWordIsr(DWordValue, Address)((((volatile UINT32 *)pEsc)[(Address)>>2]) = (DWordValue)) /**< \brief 32Bit specific ESC (register and DPRAM) write access.<br>Called from ISRs.*/
#define     HW_EscWriteMbxMem(pData,Address,Len)    ESCMBXMEMCPY(&((ESC_MEM_ADDR ESCMEM *) pEsc)[((Address))],(MEM_ADDR *)(pData), (Len)) /**< \brief Macro to copy data from the application mailbox memory (not the ESC memory, this access is handled by HW_EscWrite).*/
#else
#define     HW_EscWrite(pData,Address,Len)      do{printf(DBGFMT "HW_EscWrite:%04X %d\n", DBGARGS, Address, Len);ESCMEMCPY(&((ESC_MEM_ADDR ESCMEM *) pEsc)[((Address))], (MEM_ADDR *)(pData), (Len));}while(0);
#define     HW_EscWriteIsr                      HW_EscWrite
#define     HW_EscWriteByte(ByteValue, Address)   do{printf(DBGFMT "HW_EscWriteByte:%04X %02X\n", DBGARGS, Address, ByteValue);((((volatile UINT8 *)pEsc)[(Address)]) = (ByteValue));}while(0);
#define     HW_EscWriteWord(WordValue, Address)   do{printf(DBGFMT "HW_EscWriteWord:%04X %04X\n", DBGARGS, Address, WordValue);((((volatile UINT16 *)pEsc)[(Address)>>1]) = (WordValue));}while(0);
#define     HW_EscWriteDWord(DWordValue, Address)   do{printf(DBGFMT "HW_EscWriteDWord:%04X %08X\n", DBGARGS, Address, DWordValue);((((volatile UINT32 *)pEsc)[(Address)>>2]) = (DWordValue));}while(0);
#define     HW_EscWriteDWordIsr(DWordValue, Address)do{printf(DBGFMT "HW_EscWriteDWordIsr:%04X %08X\n", DBGARGS, Address, DWordValue);((((volatile UINT32 *)pEsc)[(Address)>>2]) = (DWordValue));}while(0);
#define     HW_EscWriteMbxMem(pData,Address,Len)    do{printf(DBGFMT "HW_EscWriteMbxMem:%04X\n", DBGARGS, Address);ESCMBXMEMCPY(&((ESC_MEM_ADDR ESCMEM *) pEsc)[((Address))],(MEM_ADDR *)(pData), (Len));}while(0);
#endif
#endif  // FPGA_LOCAL

#ifndef TIMER_INT_HEADER
#define    TIMER_INT_HEADER /**< \brief Compiler directive before the hardware timer ISR function*/
#endif

#define     ESC_RD                    0x02 /**< \brief Indicates a read access to ESC or EEPROM*/
#define     ESC_WR                    0x04 /**< \brief Indicates a write access to ESC or EEPROM.*/

#endif//_MCIHW_H_

#define QUEUED_COMMAND_NOTHING  0
#define QUEUED_COMMAND_COMMIT_4000  1

/*-----------------------------------------------------------------------------------------
------
------    Global variables
------
-----------------------------------------------------------------------------------------*/

#if _MCIHW_
    #define PROTO
#else
    #define PROTO extern
#endif

PROTO ESC_MEM_ADDR ESCMEM *            pEsc;            /**< \brief Pointer to the ESC.<br>Shall be initialized in HW_Init().*/

/*-----------------------------------------------------------------------------------------
------
------    Global functions
------
-----------------------------------------------------------------------------------------*/
PROTO void HW_SetLed(BOOL RunLed,BOOL ErrLed);

PROTO UINT16 HW_Init(void);
PROTO void HW_Release(void);
PROTO void HW_RestartTarget(void);

PROTO UINT8 eeprom_buffer[ESC_EEPROM_SIZE];
PROTO UINT16 HW_EEPROM_Read(UINT32 wordaddr);
PROTO UINT16 HW_EEPROM_Write(UINT32 wordaddr);
PROTO UINT16 HW_EEPROM_Reload(void);
PROTO void HW_EEPROM_Store(void);

// EoEprotocol
PROTO void HW_EoeReceive(UINT16 *pData, UINT16 length);
PROTO void HW_EoeSettingInd(UINT16 *pMac, UINT16 *pIp, UINT16 *pSubNet, UINT16 *pDefaultGateway, UINT16 *pDnsIp);
PROTO void HW_EoeEndTransmissionInd();
PROTO void HW_EoeSendFrame();

PROTO void HW_EoeOpen();
PROTO void HW_EoeClose();
int EoEfdRead;
int EoEfdWrite;
pthread_t *SSC_EoE_thp;

// FoE Protocol
PROTO UINT16 HW_FoeRead(UINT16 MBXMEM * pName, UINT16 nameSize, UINT32 password, UINT16 maxBlockSize, UINT16 *pData);
PROTO UINT16 HW_FoeReadData(UINT32 offset, UINT16 maxBlockSize, UINT16 *pData);
PROTO void HW_FoeError(UINT32 errorCode);
PROTO UINT16 HW_FoeWrite(UINT16 MBXMEM * pName, UINT16 nameSize, UINT32 password);
PROTO UINT16 HW_FoeWriteData(UINT16 MBXMEM * pData, UINT16 Size, BOOL bDataFollowing);
PROTO FILE *fd;
PROTO char FoEfileName[256];
PROTO BOOL FoEbusy;
PROTO int FoEcurrentOffset;

PROTO UINT32 HW_GetTimer();
PROTO void HW_ClearTimer();
PROTO void HW_ClearTimerComp(uint32_t reminder);
PROTO UINT32 HW_GetTimerResolution();

PROTO int gi_debug;

#undef PROTO
/** @}*/

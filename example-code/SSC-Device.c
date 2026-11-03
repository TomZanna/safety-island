/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
*/

/**
\addtogroup SSC-Device SSC-Device
@{
*/

/**
\file SSC-Device.c
\brief Implementation

\version 1.0.0.11
*/


/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#include <signal.h>
#include <limits.h>
#include <stdbool.h>
#include "ecat_def.h"
#include "applInterface.h"
#include "ini_file.h"

#include <sched.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <net/if.h>

#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h> // getuid()

// netlink
#include <linux/rtnetlink.h>

// ioctl
#include <net/if.h>
#include <netinet/in.h> // IPPROTO_UDP
#include <sys/ioctl.h>



#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>

//#define SET_INTERFACE_DOWN	it seems not to work properly

#define IS_MODBUS_UDP_SERVER
#ifdef IS_MODBUS_UDP_SERVER
#define READ_HOLDING_REGISTERS 3
#define WRITE_MULTIPLE_REGISTERS 16
#define WRITE_SINGLE_REGISTER 6
#define READ_INPUT_REGISTERS 4

#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define INVALID_SOCKET -1
#define MODBUS_UDP_PORT 502
#endif

#define _SSC_DEVICE_ 1
#include "SSC-Device.h"
#undef _SSC_DEVICE_

//#define MEASURE_IN_RATE
//#define MEASURE_OUT_RATE

#include "time.h"
struct timespec Otime, Itime;


extern uint16_t application_parameters[4];
extern int32_t safety_main(void);
extern void safety_app_run(void *cmdPtr, void *statPtr);



extern void eeprom_init(const char* str_name);

/*--------------------------------------------------------------------------------------
------
------    local types and defines
------
--------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------
------
------    local variables and constants
------
-----------------------------------------------------------------------------------------*/

//#define int32 INT32
//#define int16 INT16
//#include "PLIO06drv/PLIO06brd_direct.h"

char gstr_app_folder[PATH_MAX] = { 0 };

bool gb_op_ready = false;
int gi_opCnt = 0;
int gi_totalSize = 0;		//nr of bytes exchabegd in shared mem
int gi_out_totalSize = 0;	//nr of output bytes exchabegd in shared mem
uint8_t IObuf[64*2*8];		//max number of OUT PUT data
bool gb_PLIO06_0_inUsed = false;
bool gb_PLIO06_0_outUsed = false;
int gi_PLIO06_0_inOffset = 0;
int gi_PLIO06_0_outOffset = 0;
UINT8 gby_PLIO06_0_slotNr = 0;
bool gb_PLIO06_1_inUsed = false;
bool gb_PLIO06_1_outUsed = false;
int gi_PLIO06_1_inOffset = 0;
int gi_PLIO06_1_outOffset = 0;
UINT8 gby_PLIO06_1_slotNr = 0;
char gst_platform[5] = "UN65";   //global platforn

#ifdef USE_SHARED_MEM
//global pointers to shared memory buffers
//first byte of sharem buffer is the validity flag
caddr_t gp_memINptr;
caddr_t gp_memOUTptr;
sem_t* gp_semINptr;
sem_t* gp_semOUTptr;
#define USE_TRYWAIT
#define IN_MEM "/eX-CAT_IN"
#define IN_SEM "eX-CATlockIN"
#define OUT_MEM "/eX-CAT_OUT"
#define OUT_SEM "eX-CATlockOUT"

caddr_t gp_memSTATptr;
caddr_t gp_memCMDptr;
sem_t* gp_semSTATptr;
sem_t* gp_semCMDptr;
#define STAT_MEM "/eX-CAT_STAT"
#define STAT_SEM "eX-CATlockSTAT"
#define CMD_MEM "/eX-CAT_CMD"
#define CMD_SEM "eX-CATlockCMD"
#else
uint8_t gp_memINptr[256];
uint8_t gp_memOUTptr[256];
uint8_t gp_memCMDptr[256];
uint8_t gp_memSTATptr[256];
sem_t* gp_semINptr;
sem_t* gp_semOUTptr;
sem_t* gp_semSTATptr;
sem_t* gp_semCMDptr;
#endif

bool gb_FSoeForcedOut;

bool startIn;
bool startOut;

#define NUMBER_OF_ENTRIES 64
int ioctlfd = -1;
int netlinkfd = -1;

typedef struct {
	struct nlmsghdr nh;
	struct ifinfomsg ifi;
} Req_link;



//FSoE interface

#define FSoE_ON_SAFE_BOARD

#ifndef FSoE_ON_SAFE_BOARD
//extern void safety_app_run(void);
extern int safty_main(uint16_t address);
bool gb_FSoE_initOk;
bool gb_isFirst_FSoE_cycle;
#else
#include <linux/spi/spidev.h>
#include <gpiod.h>

#define SPIGO_SLAVE_GPIO      92
#define SPIGO_MASTER_GPIO_1     90
#define SPIGO_MASTER_GPIO_2     95
#define CPU_MASTER 0
#define CPU_SLAVE  1
#define DEFAULT_SPI_FREQ_HZ		4000000
#define SPIDEV_CPU_MASTER_1 		"/dev/spidev0.0"
#define SPIDEV_CPU_MASTER_2 		"/dev/spidev1.0"
#define SPIDEV_CPU_MASTER_1_1 		"/dev/spidev0.1"
#define SPIDEV_CPU_MASTER_2_1 		"/dev/spidev1.1"
//#define SPIDEV_CPU_SLAVE  		"/dev/spidev0.1"

//default priorities
#define DEF_ECAT_SLAVE_PRIO 98
#define DEF_SAFE_SPI_PRIO	99
#define DEF_SPI_SUPPORT_PRIO 99
#ifdef FSoE_ON_SAFE_BOARD
#define SPI0_IRQ	25
#define SPI1_IRQ	26
#else
#define SPI0_IRQ	63
#define SPI1_IRQ	64
#endif
#define DEF_FSoE_FRAME_LEN 11

static int ecatSlavePrio = DEF_ECAT_SLAVE_PRIO;
static int safeSpiPrio = DEF_SAFE_SPI_PRIO;
static int spiSupportPrio  = DEF_SPI_SUPPORT_PRIO;
static int FSoEFrameLen = DEF_FSoE_FRAME_LEN;

#define MAX_SLOT		4
#define UNT83A_TYPE		4
#define PL3057_TYPE		5
#define PL3057_PLUGIN_BIT 17
static UINT8 mountedFSoEBoardType = -1;
static UINT8 mountedFSoEBoardSlot = -1;

static int fdMaster = -1;
static int fdSlave = -1;
static struct spi_ioc_transfer xfer[1];
static unsigned long spiFrequencyHz;
static char spidevMaster[64];
static char spidevSlave[64];
//command line options
static int printDiffs;	//-v
static int doubleBuffer; //-d
static int isolatedCore; //-i


#pragma pack(1)
typedef struct{
	uint8_t Version;		//version of firmware of sender			64	128
	uint8_t ErrCod;			//error code TDB						65	129
	uint16_t JobNr;			//incrementing every cycle				66	130
	uint8_t Status;			//status of sender TBD					68	132
	uint8_t SafeBrdUID[12];	//UID (serial number) of SafeBoard		69	133
	uint8_t DbgArea[8];	//generic area )(free usage)				81	145
	uint8_t SafeFrameLen;	//actual length of FSoE frame			89	153
	uint8_t SafeFrame[DEF_FSoE_FRAME_LEN];	//FSoE frame			90	154
	// uint8_t  cmd													90	154
	// uint16_t safeI01												91	155
	// uint16_t crc1												93	157
	// uint16_t safeI02												95	159
	// uint16_t crc2												97	161
	// uint16_t connID												99	163
}s_SPIbuff;
#pragma pack()

s_SPIbuff SPITxBuf;
s_SPIbuff SPIRxBuf;
int cntSpiErr;
pthread_mutex_t FSinProtect;
pthread_mutex_t FSoutProtect;
pthread_t pthRX;
pthread_attr_t attr;
int firstTime = 1;
int spigoMasterGpio = SPIGO_MASTER_GPIO_1;
//int spigoSlaveGpio  = SPIGO_SLAVE_GPIO;

uint8_t FSinBuf2[64];		//FailSafe Input buffer
uint8_t FSoutBuf2[64];		//FailSafe Output buffer
int	FSinBufSw;
int	FSinBufReading;
int	FSinBuf2Reading;
int	FSoutBufSw;
int	FSoutBufReading;
int	FSoutBuf2Reading;
#endif
uint8_t FSinBuf[64];		//FailSafe Input buffer
uint8_t FSoutBuf[64];		//FailSafe Output buffer
bool gb_isFSoE;
bool gb_FSoE_is_on_SPI;
int gi_FSoE_outOffset = 0;
int gi_FSoE_inOffset = 0;



/*-----------------------------------------------------------------------------------------
------
------    application specific functions
------
-----------------------------------------------------------------------------------------*/
void FSoE_init_defaults()
{
   Obj.Safe_Address.Serial_Number = 0;
   Obj.FSOE_Communication_Parameter.Version = 0x3130;	//"01"
   Obj.FSOE_Communication_Parameter.Safe_Address = 0xffff-1;
   Obj.FSOE_Communication_Parameter.Watchdog_Time = 100;
   Obj.FSOE_Master_Frame_Elements_NrofIndexes = 4;
   Obj.FSOE_Slave_Frame_Elements_NrofIndexes = 4;
   Obj.FSOE_Slave_Safety_Inputs_NrofIndexes = 13;
   Obj.FSOE_Master_Safety_Outputs_NrofIndexes = 13;
   Obj.FSOE_Communication_Parameter_NrofIndexes = 8;
   Obj.Safe_Params_NrofIndexes = 2;
   Obj.Safe_Address_NrofIndexes = 2;
   Obj.SafeIOerror_NrofIndexes = 1;
}

void FSoE_init_default_address(uint16_t address)
{
   Obj.Safe_Address.FSoE_Address =  address; //0xffff-2;
   Obj.FSOE_Communication_Parameter.Safe_Address = address;
}


void updateFSout(uint16_t outWord)
{
	if (gb_FSoE_is_on_SPI){
		//printf("SPI FS OUT %X\n", outWord);
		//add simulation
	}else
		PLIO06_OutDig(gby_PLIO06_1_slotNr, &outWord);
}

uint16_t updateFSin(void)
{
	uint16_t inWord;
	if (gb_FSoE_is_on_SPI){
		//add simulation
		inWord = 0;
		//printf("SPI FS IN %X\n", inWord);
	}else{
		PLIO06_InDig(gby_PLIO06_0_slotNr, (UINT8 *)&inWord);
		PLIO06_DiagInDig(gby_PLIO06_0_slotNr, ((UINT8 *)&inWord)+1);
		inWord &= 0x7FF;
		//printf("FSIN: %04X\n", inWord);
	}
	return inWord;
}

/*-----------------------------------------------------------------------------------------
------
------    generic functions
------
-----------------------------------------------------------------------------------------*/
static int PLUGIN_IDS[] = {21, 22, 23, 24};

static int PluginInstalled(int slot, int board)
{
	int fd;

	char basename[200];
	char filename[200];
	sprintf(basename, "/dev/plugin%d", slot);
	sprintf(filename, "%s/installed", basename);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
	   sprintf(basename, "/sys/devices/platform/plugin%d", slot);
	   sprintf(filename, "%s/installed", basename);
	   fd = open(filename, O_RDONLY);
	   if (fd == -1)
	   {
		 sprintf(basename, "/sys/devices/soc0/plugin%d.%d", slot, PLUGIN_IDS[slot]);
		   sprintf(filename, "%s/installed", basename);
		   fd = open(filename, O_RDONLY);
		   if (fd == -1)
			   return 0;
	   }
	}
	char buf[128];
	size_t bytesRead = read(fd, buf, sizeof(buf));
	close(fd);

	if (bytesRead == 0)
	   return 0;

	int installed = atoi(buf);
	if (!installed)
	   return 0;

	sprintf(filename, "%s/func_bit_area", basename);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	   return 0;

	bytesRead = read(fd, buf, sizeof(buf));
	close(fd);

	if (bytesRead < sizeof(buf))
	   return 0;

	return (buf[board] != 0);
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\param     index               index of the requested object.
\param     subindex            subindex of the requested object.
\param     objSize             size of the requested object data, calculated with OBJ_GetObjectLength
\param     pData               Pointer to the buffer where the data can be copied to
\param     bCompleteAccess     Indicates if a complete read of all subindices of the
							   object shall be done or not

 \return    result of the read operation (0 (success) or an abort code (ABORTIDX_.... defined in
			sdosrv.h))
 *////////////////////////////////////////////////////////////////////////////////////////
UINT8 plc_read_600x(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM* pData, UINT8 bCompleteAccess)
{
	if (gi_debug & DBG_DEFAULT)
		printf(DBGFMT "%x.%x %x\n", DBGARGS, index, subindex, dataSize);

	switch(index){
	case 0x6000:
		if (bCompleteAccess)
		{
			if (subindex == 0){
				memcpy((void*)pData, &Obj.FSOE_Slave_Frame_Elements_NrofIndexes, sizeof(Obj.FSOE_Slave_Frame_Elements_NrofIndexes) + sizeof(Obj.FSOE_Slave_Frame_Elements));
				return 0;
			}
			if (subindex == 1){
				memcpy((void*)pData, &Obj.FSOE_Slave_Frame_Elements, sizeof(Obj.FSOE_Slave_Frame_Elements));
				return 0;
			}
			return ABORTIDX_UNSUPPORTED_ACCESS;
		}
		switch (subindex)
		{
		case 0:	memcpy((void*)pData, &Obj.FSOE_Slave_Frame_Elements_NrofIndexes, sizeof(Obj.FSOE_Slave_Frame_Elements_NrofIndexes)); return 0;
		case 1:	memcpy((void*)pData, &Obj.FSOE_Slave_Frame_Elements.Command, sizeof(Obj.FSOE_Slave_Frame_Elements.Command)); return 0;
		case 2:	memcpy((void*)pData, &Obj.FSOE_Slave_Frame_Elements.Connection_ID, sizeof(Obj.FSOE_Slave_Frame_Elements.Connection_ID)); return 0;
		case 3:	memcpy((void*)pData, &Obj.FSOE_Slave_Frame_Elements.CRC0, sizeof(Obj.FSOE_Slave_Frame_Elements.CRC0)); return 0;
		case 4:	memcpy((void*)pData, &Obj.FSOE_Slave_Frame_Elements.CRC1, sizeof(Obj.FSOE_Slave_Frame_Elements.CRC1)); return 0;
		default: return ABORTIDX_SUBINDEX_NOT_EXISTING;
		}
		break;
	case 0x6001:
		if (bCompleteAccess)
		{
			if (subindex == 0){
				memcpy((void*)pData, &Obj.FSOE_Slave_Safety_Inputs_NrofIndexes, sizeof(Obj.FSOE_Slave_Safety_Inputs_NrofIndexes) + sizeof(Obj.FSOE_Slave_Safety_Inputs));
				return 0;
			}
			if (subindex == 1){
				memcpy((void*)pData, &Obj.FSOE_Slave_Safety_Inputs, sizeof(Obj.FSOE_Slave_Safety_Inputs));
				return 0;
			}
			return ABORTIDX_UNSUPPORTED_ACCESS;
		}
		if (mountedFSoEBoardType == PL3057_TYPE){
			switch (subindex)
			{
			case 0:	memcpy((void*)pData, &Obj.FSOE_Slave_Safety_Inputs_NrofIndexes, sizeof(Obj.FSOE_Slave_Safety_Inputs_NrofIndexes)); return 0;
			case 1:	*pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput1 & 0x0001)?1:0; return 0;
			case 2:	*pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput1 & 0x0002)?1:0; return 0;
			case 3:	*pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput1 & 0x0004)?1:0; return 0;
			case 4:	*pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput1 & 0x0008)?1:0; return 0;
			case 5:	*pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput1 & 0x0010)?1:0; return 0;
			case 6:	*pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput1 & 0x0020)?1:0; return 0;
			case 7:	*pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput2 & 0x0001)?1:0; return 0;
			case 8:	*pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput2 & 0x0002)?1:0; return 0;
			case 9:	*pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput2 & 0x0004)?1:0; return 0;
			case 10: *pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput2 & 0x0008)?1:0; return 0;
			case 11: *pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput2 & 0x0010)?1:0; return 0;
			case 12: *pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput2 & 0x0020)?1:0; return 0;
			default: return ABORTIDX_SUBINDEX_NOT_EXISTING;
			}
		}else{
			switch (subindex)
			{
			case 0:	memcpy((void*)pData, &Obj.FSOE_Slave_Safety_Inputs_NrofIndexes, sizeof(Obj.FSOE_Slave_Safety_Inputs_NrofIndexes)); return 0;
			case 1:	*pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput1 & 0x0001)?1:0; return 0;
			case 2:	*pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput1 & 0x0002)?1:0; return 0;
			case 3:	*pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput1 & 0x0004)?1:0; return 0;
			case 4:	*pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput1 & 0x0008)?1:0; return 0;
			case 5:	*pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput1 & 0x0010)?1:0; return 0;
			case 6:	*pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput1 & 0x0020)?1:0; return 0;
			case 7:	*pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput1 & 0x0040)?1:0; return 0;
			case 8:	*pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput1 & 0x0080)?1:0; return 0;
			case 9:	*pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput1 & 0x0100)?1:0; return 0;
			case 10: *pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput1 & 0x0200)?1:0; return 0;
			case 11: *pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput1 & 0x0400)?1:0; return 0;
			case 12: *pData = (Obj.FSOE_Slave_Safety_Inputs.SafeInput1 & 0x0800)?1:0; return 0;
			case 13: memcpy((void*)pData, &Obj.FSOE_Slave_Safety_Inputs.SafeInput2, sizeof(Obj.FSOE_Slave_Safety_Inputs.SafeInput2)); return 0;
			default: return ABORTIDX_SUBINDEX_NOT_EXISTING;
			}
		}
		break;
	default: return ABORTIDX_OBJECT_NOT_EXISTING;
	}
	return 0;   // success
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
\param     index               index of the requested object.
\param     subindex            subindex of the requested object.
\param     objSize             size of the requested object data, calculated with OBJ_GetObjectLength
\param     pData               Pointer to the buffer where the data can be copied to
\param     bCompleteAccess     Indicates if a complete read of all subindices of the
							   object shall be done or not

 \return    result of the read operation (0 (success) or an abort code (ABORTIDX_.... defined in
			sdosrv.h))
 *////////////////////////////////////////////////////////////////////////////////////////
UINT8 plc_read_700x(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM* pData, UINT8 bCompleteAccess)
{
	if (gi_debug & DBG_DEFAULT)
		printf(DBGFMT "%x.%x %x\n", DBGARGS, index, subindex, dataSize);

	switch(index){
	case 0x7000:
		if (bCompleteAccess)
		{
			if (subindex == 0){
				memcpy((void*)pData, &Obj.FSOE_Master_Frame_Elements_NrofIndexes, sizeof(Obj.FSOE_Master_Frame_Elements_NrofIndexes) + sizeof(Obj.FSOE_Master_Frame_Elements));
				return 0;
			}
			if (subindex == 1){
				memcpy((void*)pData, &Obj.FSOE_Master_Frame_Elements, sizeof(Obj.FSOE_Master_Frame_Elements));
				return 0;
			}
			return ABORTIDX_UNSUPPORTED_ACCESS;
		}
		switch (subindex)
		{
		case 0:	memcpy((void*)pData, &Obj.FSOE_Master_Frame_Elements_NrofIndexes, sizeof(Obj.FSOE_Master_Frame_Elements_NrofIndexes)); return 0;
		case 1:	memcpy((void*)pData, &Obj.FSOE_Master_Frame_Elements.Command, sizeof(Obj.FSOE_Master_Frame_Elements.Command)); return 0;
		case 2:	memcpy((void*)pData, &Obj.FSOE_Master_Frame_Elements.Connection_ID, sizeof(Obj.FSOE_Master_Frame_Elements.Connection_ID)); return 0;
		case 3:	memcpy((void*)pData, &Obj.FSOE_Master_Frame_Elements.CRC0, sizeof(Obj.FSOE_Master_Frame_Elements.CRC0)); return 0;
		case 4:	memcpy((void*)pData, &Obj.FSOE_Master_Frame_Elements.CRC1, sizeof(Obj.FSOE_Master_Frame_Elements.CRC1)); return 0;
		default: return ABORTIDX_SUBINDEX_NOT_EXISTING;
		}
		break;
	case 0x7001:
		if (bCompleteAccess)
		{
			if (subindex == 0){
				memcpy((void*)pData, &Obj.FSOE_Master_Safety_Outputs_NrofIndexes, sizeof(Obj.FSOE_Master_Safety_Outputs_NrofIndexes) + sizeof(Obj.FSOE_Master_Safety_Outputs));
				return 0;
			}
			if (subindex == 1){
				memcpy((void*)pData, &Obj.FSOE_Master_Safety_Outputs, sizeof(Obj.FSOE_Master_Safety_Outputs));
				return 0;
			}
			return ABORTIDX_UNSUPPORTED_ACCESS;
		}
		if (mountedFSoEBoardType == PL3057_TYPE){
		switch (subindex)
		{
			case 0:	memcpy((void*)pData, &Obj.FSOE_Master_Safety_Outputs_NrofIndexes, sizeof(Obj.FSOE_Master_Safety_Outputs_NrofIndexes)); return 0;
			case 1:	*pData = (Obj.FSOE_Master_Safety_Outputs.SafeOutput1 & 0x0001)?1:0; return 0;
			case 2:	*pData = (Obj.FSOE_Master_Safety_Outputs.SafeOutput1 & 0x0002)?1:0; return 0;
			case 3:	*pData = (Obj.FSOE_Master_Safety_Outputs.SafeOutput1 & 0x0004)?1:0; return 0;
			case 4:	*pData = (Obj.FSOE_Master_Safety_Outputs.SafeOutput1 & 0x0008)?1:0; return 0;
			case 5:	*pData = (Obj.FSOE_Master_Safety_Outputs.SafeOutput2 & 0x0001)?1:0; return 0;
			case 6:	*pData = (Obj.FSOE_Master_Safety_Outputs.SafeOutput2 & 0x0002)?1:0; return 0;
			case 7:	*pData = (Obj.FSOE_Master_Safety_Outputs.SafeOutput2 & 0x0004)?1:0; return 0;
			case 8:	*pData = (Obj.FSOE_Master_Safety_Outputs.SafeOutput2 & 0x0008)?1:0; return 0;
			default: return ABORTIDX_SUBINDEX_NOT_EXISTING;
		}
		}else{
			switch (subindex)
			{
			case 0:	memcpy((void*)pData, &Obj.FSOE_Master_Safety_Outputs_NrofIndexes, sizeof(Obj.FSOE_Master_Safety_Outputs_NrofIndexes)); return 0;
			case 1:	*pData = (Obj.FSOE_Master_Safety_Outputs.SafeOutput1 & 0x0001)?1:0; return 0;
			case 2:	*pData = (Obj.FSOE_Master_Safety_Outputs.SafeOutput1 & 0x0002)?1:0; return 0;
			case 3:	*pData = (Obj.FSOE_Master_Safety_Outputs.SafeOutput1 & 0x0004)?1:0; return 0;
			case 4:	*pData = (Obj.FSOE_Master_Safety_Outputs.SafeOutput1 & 0x0008)?1:0; return 0;
			case 5:	*pData = (Obj.FSOE_Master_Safety_Outputs.SafeOutput1 & 0x0010)?1:0; return 0;
			case 6:	*pData = (Obj.FSOE_Master_Safety_Outputs.SafeOutput1 & 0x0020)?1:0; return 0;
			case 7:	*pData = (Obj.FSOE_Master_Safety_Outputs.SafeOutput1 & 0x0040)?1:0; return 0;
			case 8:	*pData = (Obj.FSOE_Master_Safety_Outputs.SafeOutput1 & 0x0080)?1:0; return 0;
			case 9:	*pData = (Obj.FSOE_Master_Safety_Outputs.SafeOutput1 & 0x0100)?1:0; return 0;
			case 10:	*pData = (Obj.FSOE_Master_Safety_Outputs.SafeOutput1 & 0x0200)?1:0; return 0;
			case 11:	*pData = (Obj.FSOE_Master_Safety_Outputs.SafeOutput1 & 0x0400)?1:0; return 0;
			case 12:	*pData = (Obj.FSOE_Master_Safety_Outputs.SafeOutput1 & 0x0800)?1:0; return 0;
			case 13: memcpy((void*)pData, &Obj.FSOE_Master_Safety_Outputs.SafeOutput2, sizeof(Obj.FSOE_Slave_Safety_Inputs.SafeInput2)); return 0;
			default: return ABORTIDX_SUBINDEX_NOT_EXISTING;
			}
		}
		break;
	default: return ABORTIDX_OBJECT_NOT_EXISTING;
	}
	return 0;   // success
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    The function is called when an error state was acknowledged by the master

*////////////////////////////////////////////////////////////////////////////////////////

void    APPL_AckErrorInd(UINT16 stateTrans)
{

}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from INIT to PREOP when
             all general settings were checked to start the mailbox handler. This function
             informs the application about the state transition, the application can refuse
             the state transition when returning an AL Status error code.
            The return code NOERROR_INWORK can be used, if the application cannot confirm
            the state transition immediately, in that case this function will be called cyclically
            until a value unequal NOERROR_INWORK is returned

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StartMailboxHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from PREEOP to INIT
             to stop the mailbox handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StopMailboxHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    pIntMask    pointer to the AL Event Mask which will be written to the AL event Mask
                        register (0x204) when this function is succeeded. The event mask can be adapted
                        in this function
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from PREOP to SAFEOP when
           all general settings were checked to start the input handler. This function
           informs the application about the state transition, the application can refuse
           the state transition when returning an AL Status error code.
           The return code NOERROR_INWORK can be used, if the application cannot confirm
           the state transition immediately, in that case the application need to be complete 
           the transition by calling ECAT_StateChange.
*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StartInputHandler(UINT16 *pIntMask)
{
	gp_memINptr[0] = 1;		//Operational
	return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from SAFEOP to PREEOP
             to stop the input handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StopInputHandler(void)
{
	gp_memINptr[0] = 0;		//not Operational
	return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from SAFEOP to OP when
             all general settings were checked to start the output handler. This function
             informs the application about the state transition, the application can refuse
             the state transition when returning an AL Status error code.
           The return code NOERROR_INWORK can be used, if the application cannot confirm
           the state transition immediately, in that case the application need to be complete 
           the transition by calling ECAT_StateChange.
*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StartOutputHandler(void)
{
	gp_memOUTptr[0] = 1;		//Operational
	return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from OP to SAFEOP
             to stop the output handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StopOutputHandler(void)
{
	gp_memOUTptr[0] = 0;		//not Operational
	return ALSTATUSCODE_NOERROR;
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
\return     0(ALSTATUSCODE_NOERROR), NOERROR_INWORK
\param      pInputSize  pointer to save the input process data length
\param      pOutputSize  pointer to save the output process data length

\brief    This function calculates the process data sizes from the actual SM-PDO-Assign
            and PDO mapping
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 APPL_GenerateMapping(UINT16 *pInputSize,UINT16 *pOutputSize)
{
    UINT16 result = ALSTATUSCODE_NOERROR;
    UINT16 InputSize = 0;
    UINT16 OutputSize = 0;

#if COE_SUPPORTED
    UINT16 PDOAssignEntryCnt = 0;
    OBJCONST TOBJECT OBJMEM * pPDO = NULL;
    UINT16 PDOSubindex0 = 0;
    UINT32 *pPDOEntry = NULL;
    UINT16 PDOEntryCnt = 0;

#if (MAX_PD_OUTPUT_SIZE > 0 || MAX_PD_INPUT_SIZE > 0)
	printf("refresh OP LIST\n");
	gb_op_ready = false;
	memset (IObuf, 0, sizeof(IObuf));
	if (gb_PLIO06_0_inUsed || gb_PLIO06_0_outUsed){
		uint8_t out = 0;
		PLIO06_OutDig(gby_PLIO06_0_slotNr, &out);
		PLIO06_close(gby_PLIO06_0_slotNr);
	}
	if (gb_PLIO06_1_inUsed || gb_PLIO06_1_outUsed){
		uint8_t out = 0;
		PLIO06_OutDig(gby_PLIO06_1_slotNr, &out);
		PLIO06_close(gby_PLIO06_1_slotNr);
	}
	gb_PLIO06_0_inUsed = false;
	gb_PLIO06_0_outUsed = false;
	gb_isFSoE = false;
	gb_FSoE_is_on_SPI = true;
	gb_PLIO06_1_inUsed = false;
	gb_PLIO06_1_outUsed = false;

	if (!gb_op_ready){
		int idx = 0;
		gi_opCnt = 0;
		gi_totalSize = 0;
		UINT16 assign_idx;
		printf ("Prepare PLC operations %d, %d\n", sTxPDOassign.u16SubIndex0, sRxPDOassign.u16SubIndex0);

		for (assign_idx = 0; assign_idx < sRxPDOassign.u16SubIndex0; ++assign_idx){
			UINT16 i_assign_index = sRxPDOassign.aEntries[assign_idx];
			UINT16 i_slot = i_assign_index & 0x0F;
			UINT16 i_slot_index = 0x8000 + (i_slot << 4);
			if (i_slot == 0)
				i_slot++;	//first PDO is reserved for slot 1

			OBJCONST TOBJECT OBJMEM* p_slot_obj = OBJ_GetObjectHandle(i_slot_index);
			printf("OUT operation AI:%X, SL:%d, SI:%X, PS:%p\n",i_assign_index,i_slot,i_slot_index, p_slot_obj);
			if (p_slot_obj){
				const TOBJECT *pObjEntry = OBJ_GetObjectHandle(0xF030);
				void * pVarPtr = pObjEntry->pVarPtr;
				UINT32 module = *((UINT32 *)(((UINT8 *)pVarPtr) + (OBJ_GetEntryOffset(i_slot, pObjEntry) >> 3)));
				UINT8 boardType = (module >> 24) & 0xFF;
				UINT8 direction = ((module >> 16) & 0xFF)-1; //0 = Input
				UINT8 maxNr = 1 << (module & 0xFF);
				UINT8 type = ((module >> 8) & 0xFF);
				if (type == 0)
					maxNr /= 8;
				else
					maxNr *= type;
				//check special I/O modules
                if (boardType == 1 || boardType == 3){
					bool PLIO06ready=false;
					UINT8 PLIO06slot = 0;
#if 0
                    UINT8 PLIO06debounceTime = 0;
					if (boardType == 3)
						maxNr = 0;
					if (boardType == 3 ||
							(OBJ_Read(i_slot_index, 1, 1, p_slot_obj, (UINT16*)&PLIO06slot, 0) == 0 &&
							OBJ_Read(i_slot_index, 2, 1, p_slot_obj, (UINT16*)&PLIO06debounceTime, 0) == 0)){
						if (PLIO06slot <= 3){
							if ((!gb_PLIO06_0_inUsed && !gb_PLIO06_0_outUsed) ||
								(!gb_PLIO06_1_inUsed && !gb_PLIO06_1_outUsed)){
								if (PLIO06_open(PLIO06slot)){
									uint8_t out = 0;
									PLIO06_DI_ParT parDI;
									parDI.FilterMask = 0xFFFFFFFF;
									switch(PLIO06debounceTime){
										case 0:
										default: parDI.FilterValue = 1; break;
										case 1: parDI.FilterValue = 30; break;
										case 2: parDI.FilterValue = 100; break;
										case 3: parDI.FilterValue = 200; break;
									}
									PLIO06_DigInpInit(&parDI,PLIO06slot);
									PLIO06_DiagInpInit(PLIO06slot);
									PLIO06_DigOutInit(PLIO06slot);
									PLIO06_OutDig(PLIO06slot, &out);
									PLIO06ready = true;
									printf("PLIO06 opened at slot %d Debounce = %d\n", PLIO06slot, PLIO06debounceTime);
								}else{
									//if (gi_debug & DBG_DEFAULT)
									printf(DBGFMT "failed to start PLIO06 at slot=%d for output\n", DBGARGS, PLIO06slot);
									return ALSTATUSCODE_EXT_HARDWARE_NOT_READY;
								}
							}
						}else{
							//if (gi_debug & DBG_DEFAULT)
								printf(DBGFMT "invalid  PLIO06 slot=%d\n", DBGARGS, PLIO06slot);
						}
					}else{
						//if (gi_debug & DBG_DEFAULT)
							printf(DBGFMT "Failed reading PLIO06 slot %04x\n", DBGARGS, i_slot_index);
					}
#endif
					if (PLIO06ready){
						if (!gb_PLIO06_0_outUsed && (!gb_PLIO06_0_inUsed || (gb_PLIO06_0_inUsed && gby_PLIO06_0_slotNr == PLIO06slot))){
							gb_PLIO06_0_outUsed = true;
							gby_PLIO06_0_slotNr = PLIO06slot;
							gi_PLIO06_0_outOffset = idx;
							printf("PLIO06 0 Output slot assigned offset: %d\n", gi_PLIO06_0_outOffset);
							if (boardType == 3){
								gb_isFSoE = true;
								gb_FSoE_is_on_SPI = false;
								gi_FSoE_outOffset = gi_PLIO06_0_outOffset;
							}
						}else if (!gb_PLIO06_1_outUsed && (!gb_PLIO06_1_inUsed || (gb_PLIO06_1_inUsed && gby_PLIO06_1_slotNr == PLIO06slot))){
							gb_PLIO06_1_outUsed = true;
							gby_PLIO06_1_slotNr = PLIO06slot;
							gi_PLIO06_1_outOffset = idx;
							printf("PLIO06 1 Output slot assigned offset: %d\n", gi_PLIO06_1_outOffset);
						}
					}
				}else if(boardType >= 4){
					if (boardType == mountedFSoEBoardType){
						gb_isFSoE = true;
						gb_FSoE_is_on_SPI = true;
						gi_FSoE_outOffset = idx;
					}else{
						//if (gi_debug & DBG_DEFAULT)
						printf(DBGFMT "SAFE IO plugin not found\n", DBGARGS);
						return ALSTATUSCODE_EXT_HARDWARE_NOT_READY;
					}
				}
				printf("RXPDO:module:%lX, brd:%d, dir:%d, type:%d, max:%d\n", module, boardType, direction, type, maxNr);
				idx += maxNr;
			}else{
				if (gi_debug & DBG_DEFAULT)
					printf(DBGFMT "OBJ_GetObjectHandle() failed %04x\n", DBGARGS, i_slot_index);
			}
		}
		gi_out_totalSize = idx;

		for (assign_idx = 0; assign_idx < sTxPDOassign.u16SubIndex0; ++assign_idx)    // 1C12 len
		{
			UINT16 i_assign_index = sTxPDOassign.aEntries[assign_idx];  // 1C13 elem
			UINT16 i_slot = i_assign_index & 0x0F;
			UINT16 i_slot_index = 0x8000 + (i_slot << 4);
			if (i_slot == 0)
				i_slot++;	//first PDO is reserved for slot 1

			OBJCONST TOBJECT OBJMEM* p_slot_obj = OBJ_GetObjectHandle(i_slot_index);
			printf("IN operation AI:%X, SL:%d, SI:%X, PS:%p\n",i_assign_index,i_slot,i_slot_index, p_slot_obj);
			if (p_slot_obj){
				const TOBJECT *pObjEntry = OBJ_GetObjectHandle(0xF030);
				void * pVarPtr = pObjEntry->pVarPtr;
				UINT32 module = *((UINT32 *)(((UINT8 *)pVarPtr) + (OBJ_GetEntryOffset(i_slot, pObjEntry) >> 3)));
				UINT8 boardType = (module >> 24) & 0xFF;
				UINT8 direction = ((module >> 16) & 0xFF)-1; //0 = Input
				UINT8 maxNr = 1 << (module & 0xFF);
				UINT8 type = ((module >> 8) & 0xFF);
				if (type == 0)
					maxNr /= 8;
				else
					maxNr *= type;
				//check special I/O modules
				if (boardType == 3)
					maxNr = 0;
				if (boardType == 1 || boardType == 3){
					bool PLIO06ready=false;
					UINT8 PLIO06slot = 0;
#if 0
					UINT8 PLIO06debounceTime = 0;
					if (boardType == 3 ||
							(OBJ_Read(i_slot_index, 1, 1, p_slot_obj, (UINT16*)&PLIO06slot, 0) == 0 &&
							OBJ_Read(i_slot_index, 2, 1, p_slot_obj, (UINT16*)&PLIO06debounceTime, 0) == 0)){
						if (PLIO06slot <= 3){
							if ((!gb_PLIO06_0_inUsed && !gb_PLIO06_0_outUsed) ||
								(!gb_PLIO06_1_inUsed && !gb_PLIO06_1_outUsed && PLIO06slot != gby_PLIO06_0_slotNr)){
								if (PLIO06_open(PLIO06slot)){
									uint8_t out = 0;
									PLIO06_DI_ParT parDI;
									parDI.FilterMask = 0xFFFFFFFF;
									switch(PLIO06debounceTime){
										case 0:
										default: parDI.FilterValue = 1; break;
										case 1: parDI.FilterValue = 30; break;
										case 2: parDI.FilterValue = 100; break;
										case 3: parDI.FilterValue = 200; break;
									}
									PLIO06_DigInpInit(&parDI,PLIO06slot);
									PLIO06_DiagInpInit(PLIO06slot);
									PLIO06_DigOutInit(PLIO06slot);
									PLIO06_OutDig(PLIO06slot, &out);
									PLIO06ready = true;
									printf("PLIO06 opened at slot %d\n", PLIO06slot);
								}else{
									//if (gi_debug & DBG_DEFAULT)
									printf(DBGFMT "failed to start PLIO06 at slot=%d for input\n", DBGARGS, PLIO06slot);
									return ALSTATUSCODE_EXT_HARDWARE_NOT_READY;
								}
							}
						}else{
							//if (gi_debug & DBG_DEFAULT)
								printf(DBGFMT "invalid  PLIO06 slot=%d\n", DBGARGS, PLIO06slot);
						}
					}else{
						//if (gi_debug & DBG_DEFAULT)
							printf(DBGFMT "Failed reading PLIO06 slot %04x\n", DBGARGS, i_slot_index);
                    }
#endif
					if (!gb_PLIO06_0_inUsed && ((!gb_PLIO06_0_outUsed && PLIO06ready) || (gb_PLIO06_0_outUsed && gby_PLIO06_0_slotNr == PLIO06slot))){
						gb_PLIO06_0_inUsed = true;
						gby_PLIO06_0_slotNr = PLIO06slot;
						gi_PLIO06_0_inOffset = idx - gi_out_totalSize;
						printf("PLIO06 0 Input slot assigned offset: %d\n", gi_PLIO06_0_inOffset);
						if (boardType == 3){
							gb_isFSoE = true;
							gb_FSoE_is_on_SPI = false;
							gi_FSoE_inOffset = gi_PLIO06_0_inOffset;
						}
					}else if (!gb_PLIO06_1_inUsed && ((!gb_PLIO06_1_outUsed && PLIO06ready) || (gb_PLIO06_1_outUsed && gby_PLIO06_1_slotNr == PLIO06slot))){
						gb_PLIO06_1_inUsed = true;
						gby_PLIO06_1_slotNr = PLIO06slot;
						gi_PLIO06_1_inOffset = idx - gi_out_totalSize;
						printf("PLIO06 1 Input slot assigned offset: %d\n", gi_PLIO06_1_inOffset);
					}
				}else if(boardType >= 4){
					if (boardType == mountedFSoEBoardType){
						gb_isFSoE = true;
						gb_FSoE_is_on_SPI = true;
						gi_FSoE_inOffset = idx - gi_out_totalSize;
					}else{
						//if (gi_debug & DBG_DEFAULT)
						printf(DBGFMT "SAFE IO plugin not found\n", DBGARGS);
						return ALSTATUSCODE_EXT_HARDWARE_NOT_READY;
					}
				}
				printf("TXPDO:module:%lX, brd:%d, dir:%d, type:%d, max:%d\n", module, boardType, direction, type, maxNr);
				idx += maxNr;
			}else{
				if (gi_debug & DBG_DEFAULT)
					printf(DBGFMT "OBJ_GetObjectHandle() failed %04x\n", DBGARGS, i_slot_index);
			}
		}
		gi_totalSize = idx;
		printf("START operation N:%d, OSZ:%d, SZ:%d\n",gi_opCnt, gi_out_totalSize, gi_totalSize);
		gb_op_ready = true;
	}

#endif

#if MAX_PD_OUTPUT_SIZE > 0

    /*Scan object 0x1C12 RXPDO assign*/
    for(PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sRxPDOassign.u16SubIndex0; PDOAssignEntryCnt++)
    {
        pPDO = OBJ_GetObjectHandle(sRxPDOassign.aEntries[PDOAssignEntryCnt]);
        if(pPDO != NULL)
        {
            PDOSubindex0 = *((UINT16 *)pPDO->pVarPtr);
            for(PDOEntryCnt = 0; PDOEntryCnt < PDOSubindex0; PDOEntryCnt++)
            {
                pPDOEntry = (UINT32 *)((UINT16 *)pPDO->pVarPtr + (OBJ_GetEntryOffset((PDOEntryCnt+1),pPDO)>>3)/2);    //goto PDO entry
                // we increment the expected output size depending on the mapped Entry
                OutputSize += (UINT16) ((*pPDOEntry) & 0xFF);
            }
        }
        else
        {
            /*assigned PDO was not found in object dictionary. return invalid mapping*/
            OutputSize = 0;
            result = ALSTATUSCODE_INVALIDOUTPUTMAPPING;
            break;
        }
    }

    OutputSize = (OutputSize + 7) >> 3;
#endif

#if MAX_PD_INPUT_SIZE > 0

	if(result == ALSTATUSCODE_NOERROR)
    {
        /*Scan Object 0x1C13 TXPDO assign*/
        for(PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sTxPDOassign.u16SubIndex0; PDOAssignEntryCnt++)
        {
            pPDO = OBJ_GetObjectHandle(sTxPDOassign.aEntries[PDOAssignEntryCnt]);
            if(pPDO != NULL)
            {
                PDOSubindex0 = *((UINT16 *)pPDO->pVarPtr);
                for(PDOEntryCnt = 0; PDOEntryCnt < PDOSubindex0; PDOEntryCnt++)
                {
                    pPDOEntry = (UINT32 *)((UINT16 *)pPDO->pVarPtr + (OBJ_GetEntryOffset((PDOEntryCnt+1),pPDO)>>3)/2);    //goto PDO entry
					// we increment the expected outputshared size depending on the mapped Entry
                    InputSize += (UINT16) ((*pPDOEntry) & 0xFF);
                }
            }
            else
            {
                /*assigned PDO was not found in object dictionary. return invalid mapping*/
                InputSize = 0;
                result = ALSTATUSCODE_INVALIDINPUTMAPPING;
                break;
            }
        }
    }
    InputSize = (InputSize + 7) >> 3;
#endif

#else
#if _WIN32
   #pragma message ("Warning: Define 'InputSize' and 'OutputSize'.")
#else
    #warning "Define 'InputSize' and 'OutputSize'."
#endif
#endif

    *pInputSize = InputSize;
    *pOutputSize = OutputSize;

    if (gi_debug & DBG_DEFAULT)
        printf(DBGFMT "inputsize=%d outputsize=%d\n", DBGARGS, InputSize, OutputSize);

    return result;
}
//uint8_t fakeIN[128];
//uint8_t fakeOUT[128];
#if 0
/////////////////////////////////////////////////////////////////////////////////////////
/**
\param      pData  pointer to input process data

\brief      This function will copies the inputs from the local memory to the ESC memory
			to the hardware
*////////////////////////////////////////////////////////////////////////////////////////
void APPL_InputMapping(UINT16* pData)
{
#ifdef MEASURE_IN_RATE
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	printf("SCC INput Mapping %d - %d\n", sTxPDOassign.u16SubIndex0, ((long long)now.tv_sec * 1000000000+now.tv_nsec)-((long long)Itime.tv_sec*1000000000+Itime.tv_nsec));
	Itime = now;
#endif
	if (sTxPDOassign.u16SubIndex0 > 0){   // we have some items to process
		UINT16 PLIO06_0_in;
		UINT16 PLIO06_1_in;
		int FSoffset=0;
		//process inputs

		if (gb_isFSoE)
			FSoffset = FSoEFrameLen;

		//read PLIO06 boards if present
		if (gb_PLIO06_0_inUsed && (!gb_isFSoE || (gb_isFSoE && gb_FSoE_is_on_SPI))){
			PLIO06_InDig(gby_PLIO06_0_slotNr, (UINT8 *)&PLIO06_0_in);
			PLIO06_DiagInDig(gby_PLIO06_0_slotNr, ((UINT8 *)&PLIO06_0_in)+1);
			PLIO06_0_in &= 0x7FF;
		}
		if (gb_PLIO06_1_inUsed){
			PLIO06_InDig(gby_PLIO06_1_slotNr, (UINT8 *)&PLIO06_1_in);
			PLIO06_DiagInDig(gby_PLIO06_1_slotNr, ((UINT8 *)&PLIO06_1_in)+1);
			PLIO06_1_in &= 0x7FF;
		}

//		fakeIN[0]++;
//		fakeIN[2]=fakeOUT[0];
		//read shared input area
//		memcpy (pData, fakeIN, gi_totalSize-gi_out_totalSize);
//		if (startIn)
		if (gp_memINptr){
			if (gb_isFSoE){
				if (doubleBuffer){
					if (FSinBufSw){
						FSinBuf2Reading=1;
						memcpy(&((UINT8 *)pData)[gi_FSoE_inOffset], FSinBuf2, FSoEFrameLen);
						FSinBuf2Reading=0;
					}else{
						FSinBufReading=1;
						memcpy(&((UINT8 *)pData)[gi_FSoE_inOffset], FSinBuf, FSoEFrameLen);
						FSinBufReading=0;
					}
				}else{
					pthread_mutex_lock(&FSinProtect);
					memcpy(&((UINT8 *)pData)[gi_FSoE_inOffset], FSinBuf, FSoEFrameLen);
					pthread_mutex_unlock(&FSinProtect);
				}
				_Rbuffer * fsoe_frame = (_Rbuffer *)&((UINT8 *)pData)[gi_FSoE_inOffset];
				Obj.FSOE_Slave_Frame_Elements.Command = fsoe_frame->FSOE_Slave_Frame_Elements.Command;
				Obj.FSOE_Slave_Frame_Elements.CRC0 = fsoe_frame->FSOE_Slave_Frame_Elements.CRC0;
				Obj.FSOE_Slave_Frame_Elements.CRC1 = fsoe_frame->FSOE_Slave_Frame_Elements.CRC1;
				Obj.FSOE_Slave_Frame_Elements.Connection_ID = fsoe_frame->FSOE_Slave_Frame_Elements.Connection_ID;
				if (Obj.FSOE_Slave_Frame_Elements.Command == 0x36){
					Obj.FSOE_Slave_Safety_Inputs.SafeInput1 = fsoe_frame->FSOE_Slave_Frame_Elements.SafeInput1;
					Obj.FSOE_Slave_Safety_Inputs.SafeInput2 = fsoe_frame->FSOE_Slave_Frame_Elements.SafeInput2;
				}else{
					Obj.FSOE_Slave_Safety_Inputs.SafeInput1 = 0;
					Obj.FSOE_Slave_Safety_Inputs.SafeInput2 = 0;
				}
			}
#ifdef USE_TRYWAIT
			if (sem_trywait(gp_semINptr) == 0){
				memcpy (pData+FSoffset, &gp_memINptr[1], gi_totalSize-gi_out_totalSize);
				if (gb_PLIO06_0_inUsed && (!gb_isFSoE || (gb_isFSoE && gb_FSoE_is_on_SPI))){
					if ((((UINT8 *)pData)[gi_PLIO06_0_inOffset+1] & 0x08) == 0)	//input not forced
						memcpy(&((UINT8 *)pData)[gi_PLIO06_0_inOffset], &PLIO06_0_in, 2);
				}
				if (gb_PLIO06_1_inUsed){
					if ((((UINT8 *)pData)[gi_PLIO06_1_inOffset+1] & 0x08) == 0)	//input not forced
						memcpy(&((UINT8 *)pData)[gi_PLIO06_1_inOffset], &PLIO06_1_in, 2);
				}
				sem_post(gp_semINptr);
			}//else
				//printf("semaphore gp_semINptr is busy - retry later\n");
#else
            sem_wait(gp_semINptr);
			memcpy (pData, &gp_memINptr[1], gi_totalSize-gi_out_totalSize);
			if (gb_PLIO06_0_inUsed){
				if ((((UINT8 *)pData)[gi_PLIO06_0_inOffset+1] & 0x08) == 0)	//input not forced
					memcpy(&((UINT8 *)pData)[gi_PLIO06_0_inOffset], &PLIO06_0_in, 2);
			}
			if (gb_PLIO06_1_inUsed){
				if ((((UINT8 *)pData)[gi_PLIO06_1_inOffset+1] & 0x08) == 0)	//input not forced
					memcpy(&((UINT8 *)pData)[gi_PLIO06_1_inOffset], &PLIO06_1_in, 2);
			}
            sem_post(gp_semINptr);
#endif
        }		
//		startIn = true;

		//		printf("IN: %02X, %d\n", pData[0], gi_totalSize-gi_out_totalSize);
		//copy new inpgi_totalSize-gi_out_totalSizeut data to IOBUF
		memcpy(&IObuf[gi_out_totalSize], pData, gi_totalSize-gi_out_totalSize);
	}else{
		// ok, we simply don't have any inputs assigned
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\param      pData  pointer to output process data

\brief    This function will copies the outputs from the ESC memory to the local memory
			to the hardware
*////////////////////////////////////////////////////////////////////////////////////////
void APPL_OutputMapping(UINT16* pData)
{
	int FSoffset=0;
#ifdef MEASURE_OUT_RATE
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	printf("SCC OUTput Mapping %d - %d\n", sRxPDOassign.u16SubIndex0, ((long long)now.tv_sec * 1000000000+now.tv_nsec)-((long long)Otime.tv_sec*1000000000+Otime.tv_nsec));
	Otime = now;
#endif
	if (sRxPDOassign.u16SubIndex0 > 0)
	{
		//check if PLIO06 are in use and update out if image is changed
		if (gb_PLIO06_0_outUsed && (!gb_isFSoE || (gb_isFSoE && gb_FSoE_is_on_SPI))){
			if (((UINT8 *)pData)[gi_PLIO06_0_outOffset] != IObuf[gi_PLIO06_0_outOffset]){
				PLIO06_OutDig(gby_PLIO06_0_slotNr, &((UINT8 *)pData)[gi_PLIO06_0_outOffset]);
			}
		}
		if (gb_PLIO06_1_outUsed){
			if (((UINT8 *)pData)[gi_PLIO06_1_outOffset] != IObuf[gi_PLIO06_1_outOffset]){
				PLIO06_OutDig(gby_PLIO06_1_slotNr, &((UINT8 *)pData)[gi_PLIO06_1_outOffset]);
			}
		}
		if (gb_isFSoE){
			if (doubleBuffer){
				if (FSoutBufSw){
					if (FSoutBufReading==0){
						memcpy(FSoutBuf, &((UINT8 *)pData)[gi_FSoE_outOffset], FSoEFrameLen);
						FSoutBufSw=0;
					}else{
						memcpy(FSoutBuf2, &((UINT8 *)pData)[gi_FSoE_outOffset], FSoEFrameLen);
					}
				}else{
					if (FSoutBuf2Reading==0){
						memcpy(FSoutBuf2, &((UINT8 *)pData)[gi_FSoE_outOffset], FSoEFrameLen);
						FSoutBufSw=1;
					}else{
						memcpy(FSoutBuf, &((UINT8 *)pData)[gi_FSoE_outOffset], FSoEFrameLen);
					}
				}
			}else{
				pthread_mutex_lock(&FSoutProtect);
				memcpy(FSoutBuf, &((UINT8 *)pData)[gi_FSoE_outOffset], FSoEFrameLen);
				pthread_mutex_unlock(&FSoutProtect);
			}
			_Wbuffer * rxFrame = (_Wbuffer *)&((UINT8 *)pData)[gi_FSoE_outOffset];
			Obj.FSOE_Master_Frame_Elements.Command =  rxFrame->FSOE_Master_Frame_Elements.Command;
			Obj.FSOE_Master_Frame_Elements.CRC0 = rxFrame->FSOE_Master_Frame_Elements.CRC0;
			Obj.FSOE_Master_Frame_Elements.CRC1 = rxFrame->FSOE_Master_Frame_Elements.CRC1;
			Obj.FSOE_Master_Frame_Elements.Connection_ID = rxFrame->FSOE_Master_Frame_Elements.Connection_ID;
			if (Obj.FSOE_Master_Frame_Elements.Command == 0x36){
				Obj.FSOE_Master_Safety_Outputs.SafeOutput1 = rxFrame->FSOE_Master_Frame_Elements.SafeOutput1;
				Obj.FSOE_Master_Safety_Outputs.SafeOutput2 = rxFrame->FSOE_Master_Frame_Elements.SafeOutput2;
			}else{
				Obj.FSOE_Master_Safety_Outputs.SafeOutput1 = 0;
				Obj.FSOE_Master_Safety_Outputs.SafeOutput2 = 0;
			}
			FSoffset = FSoEFrameLen;
		}
		memcpy(IObuf, pData+FSoffset, gi_out_totalSize);
		//transfer new output in shared memory
//		memcpy(fakeOUT, pData, gi_out_totalSize);
//		if (startOut)
		if (gp_memOUTptr){
#ifdef USE_TRYWAIT
			if (sem_trywait(gp_semOUTptr) == 0){
				memcpy (&gp_memOUTptr[1], pData, gi_totalSize-gi_out_totalSize);
				sem_post(gp_semOUTptr);
			}//else
				//printf("semaphore gp_semOUTptr is busy - retry later\n");
#else
            sem_wait(gp_semOUTptr);
			memcpy (&gp_memOUTptr[1], pData, gi_totalSize-gi_out_totalSize);
            sem_post(gp_semOUTptr);
#endif
        }
//		startOut = true;
//		printf("OUT: %02X, %d\n", fakeOUT[0], gi_out_totalSize);
	}
#ifndef FSoE_ON_SAFE_BOARD
	if (gb_isFSoE && gb_FSoE_initOk)
		safety_app_run();
#endif
}

void APPL_updateInputObjects()
{
	UINT16 PDOAssignEntryCnt = 0;
	OBJCONST TOBJECT OBJMEM * pPDO = NULL;
	UINT16 idx = gi_out_totalSize;
	/*Scan Object 0x1C13 TXPDO assign*/
	for(PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sTxPDOassign.u16SubIndex0; PDOAssignEntryCnt++){
		pPDO = OBJ_GetObjectHandle(sTxPDOassign.aEntries[PDOAssignEntryCnt]);
		if(pPDO != NULL)
		{
			UINT16 PDOSubindex0 = *((UINT16 *)pPDO->pVarPtr);
//			printf("PDO:%d index:%04X Nr:%d\n", PDOAssignEntryCnt, sTxPDOassign.aEntries[PDOAssignEntryCnt], PDOSubindex0);
			UINT16 PDOEntryCnt;
			UINT32 *pPDOEntry = NULL;
			for(PDOEntryCnt = 0; PDOEntryCnt < PDOSubindex0; PDOEntryCnt++)
			{
				UINT16 dataSize;
				UINT16 dataByteSize;
				UINT16 index;
				UINT8 subindex;
				const TOBJECT *pObjEntry;
				UINT32 *pPDOEntry = (UINT32 *)((UINT16 *)pPDO->pVarPtr + (OBJ_GetEntryOffset((PDOEntryCnt+1),pPDO)>>3)/2);    //goto PDO entry
				if (*pPDOEntry != 0){
					// we increment the expected output size depending on the mapped Entry
					dataByteSize = (UINT16) ((*pPDOEntry) & 0xFF)/8;
					index = (UINT16) ((*pPDOEntry) >> 16);
					if (index >= 0x6010){		//FSoE objects are updated directly
						subindex = (UINT8) (((*pPDOEntry) >> 8) & 0xFF);
						pObjEntry = OBJ_GetObjectHandle( index );
						//printf("UPDATE obj:%04X-%d size:%d val:%d %p %p\n", index, subindex, dataByteSize, ((UINT16)IObuf[idx+1]<<8)+IObuf[idx], pObjEntry->pVarPtr, pObjEntry->pVarPtr + (OBJ_GetEntryOffset(subindex, pObjEntry) >> 3));
						memcpy(pObjEntry->pVarPtr + (OBJ_GetEntryOffset(subindex, pObjEntry)>>3), &IObuf[idx], dataByteSize);
					}
					idx += dataByteSize;
				}
			}
		}
	}
}

void APPL_updateOutputObjects()
{
	UINT16 PDOAssignEntryCnt = 0;
	OBJCONST TOBJECT OBJMEM * pPDO = NULL;
	UINT16 idx = 0;
	/*Scan Object 0x1C13 TXPDO assign*/
	for(PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sRxPDOassign.u16SubIndex0; PDOAssignEntryCnt++){
		pPDO = OBJ_GetObjectHandle(sRxPDOassign.aEntries[PDOAssignEntryCnt]);
		if(pPDO != NULL)
		{
			UINT16 PDOSubindex0 = *((UINT16 *)pPDO->pVarPtr);
//			printf("PDO:%d index:%04X Nr:%d\n", PDOAssignEntryCnt, sRxPDOassign.aEntries[PDOAssignEntryCnt], PDOSubindex0);
			UINT16 PDOEntryCnt;
			UINT32 *pPDOEntry = NULL;
			for(PDOEntryCnt = 0; PDOEntryCnt < PDOSubindex0; PDOEntryCnt++)
			{
				UINT16 dataSize;
				UINT16 dataByteSize;
				UINT16 index;
				UINT8 subindex;
				const TOBJECT *pObjEntry;
				UINT32 *pPDOEntry = (UINT32 *)((UINT16 *)pPDO->pVarPtr + (OBJ_GetEntryOffset((PDOEntryCnt+1),pPDO)>>3)/2);    //goto PDO entry
				if (*pPDOEntry != 0){
					// we increment the expected output size depending on the mapped Entry
					dataByteSize = (UINT16) ((*pPDOEntry) & 0xFF)/8;
					index = (UINT16) ((*pPDOEntry) >> 16);
					if (index >= 0x7010){		//FSoE objects are updated directly
						subindex = (UINT8) (((*pPDOEntry) >> 8) & 0xFF);
						pObjEntry = OBJ_GetObjectHandle( index );
						//printf("UPDATE obj:%04X-%d size:%d val:%d %p %p\n", index, subindex, dataByteSize, ((UINT16)IObuf[idx+1]<<8)+IObuf[idx], pObjEntry->pVarPtr, pObjEntry->pVarPtr + (OBJ_GetEntryOffset(subindex, pObjEntry) >> 3));
						memcpy(pObjEntry->pVarPtr + (OBJ_GetEntryOffset(subindex, pObjEntry)>>3), &IObuf[idx], dataByteSize);
					}
					idx += dataByteSize;
				}
			}
		}
	}
}

void APPL_forceOutputObjects()
{
	UINT16 PDOAssignEntryCnt = 0;
	OBJCONST TOBJECT OBJMEM * pPDO = NULL;
	UINT16 idx = 0;
	/*Scan Object 0x1C13 TXPDO assign*/
	for(PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sRxPDOassign.u16SubIndex0; PDOAssignEntryCnt++){
		pPDO = OBJ_GetObjectHandle(sRxPDOassign.aEntries[PDOAssignEntryCnt]);
		if(pPDO != NULL)
		{
			UINT16 PDOSubindex0 = *((UINT16 *)pPDO->pVarPtr);
//			printf("PDO:%d index:%04X Nr:%d\n", PDOAssignEntryCnt, sRxPDOassign.aEntries[PDOAssignEntryCnt], PDOSubindex0);
			UINT16 PDOEntryCnt;
			UINT32 *pPDOEntry = NULL;
			for(PDOEntryCnt = 0; PDOEntryCnt < PDOSubindex0; PDOEntryCnt++)
			{
				UINT16 dataSize;
				UINT16 dataByteSize;
				UINT16 index;
				UINT8 subindex;
				const TOBJECT *pObjEntry;
				UINT32 *pPDOEntry = (UINT32 *)((UINT16 *)pPDO->pVarPtr + (OBJ_GetEntryOffset((PDOEntryCnt+1),pPDO)>>3)/2);    //goto PDO entry
				if (*pPDOEntry != 0){
					// we increment the expected output size depending on the mapped Entry
					dataByteSize = (UINT16) ((*pPDOEntry) & 0xFF)/8;
					index = (UINT16) ((*pPDOEntry) >> 16);
					if (index >= 0x7010){		//FSoE objects are updated directly
						subindex = (UINT8) (((*pPDOEntry) >> 8) & 0xFF);
						pObjEntry = OBJ_GetObjectHandle( index );
						memcpy(&IObuf[idx], pObjEntry->pVarPtr + (OBJ_GetEntryOffset(subindex, pObjEntry)>>3), dataByteSize);
						//printf("FORCE obj:%04X-%d size:%d val:%d %p %p\n", index, subindex, dataByteSize, ((UINT16)IObuf[idx+1]<<8)+IObuf[idx], pObjEntry->pVarPtr, pObjEntry->pVarPtr + (OBJ_GetEntryOffset(subindex, pObjEntry) >> 3));
					}
					idx += dataByteSize;
				}
			}
		}
	}
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////
/**
\brief    This function will called from the synchronisation ISR 
            or from the mainloop if no synchronisation is supported
*////////////////////////////////////////////////////////////////////////////////////////
void APPL_Application(void)
{
}

#if EXPLICIT_DEVICE_ID
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    The Explicit Device ID of the EtherCAT slave

 \brief     Calculate the Explicit Device ID
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 APPL_GetDeviceID()
{
    return initialize_station_alias();

    /* Explicit Device 5 is expected by Explicit Device ID conformance tests*/
	//return 0x5;
}
#endif


void putFrame(const void *pData, size_t FSoEFrameLen)
{
	if (doubleBuffer){
		if (FSoutBufSw){
			if (FSoutBufReading==0){
				memcpy(FSoutBuf, &((UINT8 *)pData)[gi_FSoE_outOffset], FSoEFrameLen);
				FSoutBufSw=0;
			}else{
				memcpy(FSoutBuf2, &((UINT8 *)pData)[gi_FSoE_outOffset], FSoEFrameLen);
			}
		}else{
			if (FSoutBuf2Reading==0){
				memcpy(FSoutBuf2, &((UINT8 *)pData)[gi_FSoE_outOffset], FSoEFrameLen);
				FSoutBufSw=1;
			}else{
				memcpy(FSoutBuf, &((UINT8 *)pData)[gi_FSoE_outOffset], FSoEFrameLen);
			}
		}
	}else{
		pthread_mutex_lock(&FSoutProtect);
		memcpy(FSoutBuf, &((UINT8 *)pData)[gi_FSoE_outOffset], FSoEFrameLen);
		pthread_mutex_unlock(&FSoutProtect);
	}
}

extern void getFrame(void * pData, size_t FSoEFrameLen)
{
	if (doubleBuffer){
		if (FSinBufSw){
			FSinBuf2Reading=1;
			memcpy(&((UINT8 *)pData)[gi_FSoE_inOffset], FSinBuf2, FSoEFrameLen);
			FSinBuf2Reading=0;
		}else{
			FSinBufReading=1;
			memcpy(&((UINT8 *)pData)[gi_FSoE_inOffset], FSinBuf, FSoEFrameLen);
			FSinBufReading=0;
		}
	}else{
		pthread_mutex_lock(&FSinProtect);
		memcpy(&((UINT8 *)pData)[gi_FSoE_inOffset], FSinBuf, FSoEFrameLen);
		pthread_mutex_unlock(&FSinProtect);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////

#if USE_DEFAULT_MAIN
/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    This is the main function

*////////////////////////////////////////////////////////////////////////////////////////

volatile int keepRunning = 1;
void SigIntHandler(int dummy) {
	(void)dummy;
	fprintf(stderr, "\nSigIntHandler %d\n", dummy);
	keepRunning = 0;
}

extern void eth_ports_init(const int i_configuration);
extern UINT16 FPGA_Init(void);

int skfd = -1;      /* AF_INET socket for ioctl() calls.*/
int set_if_flags(char *ifname, short flags)
{
	struct ifreq ifr;
	int res = 0;

	ifr.ifr_flags = flags;
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("socket error %s\n", strerror(errno));
		res = 1;
		goto out;
	}

	res = ioctl(skfd, SIOCSIFFLAGS, &ifr);
	if (res < 0) {
		printf("Interface '%s': Error: SIOCSIFFLAGS failed: %s\n",
			ifname, strerror(errno));
	} else {
		printf("Interface '%s': flags set to %04X.\n", ifname, flags);
	}
out:
	return res;
}

int get_if_flags(char *ifname, short *pFlags)
{
	struct ifreq ifr;
	int res = 0;

	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("socket error %s\n", strerror(errno));
		res = 1;
		goto out;
	}

	res = ioctl(skfd, SIOCGIFFLAGS, &ifr);
	if (res < 0) {
		printf("Interface '%s': Error: SIOCGIFFLAGS failed: %s\n",
			ifname, strerror(errno));
	} else {
		*pFlags = ifr.ifr_ifru.ifru_flags;
		printf("Interface '%s': flags are %04X.\n", ifname, *pFlags);
	}
out:
	return res;
}

static int set_if_up(char *ifname, short flags)
{
	return set_if_flags(ifname, flags | IFF_UP);
}

static int set_if_down(char *ifname, short flags)
{
	return set_if_flags(ifname, flags & ~IFF_UP);
}

void ioctl_init() {
	ioctlfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//	assert(ioctlfd == 3);
}

void ioctl_end() {
	close(ioctlfd);
	ioctlfd = -1;
}

void ioctl_flags(const bool up, const char* const dev) {
	assert(0 == getuid());
	struct ifreq ifr = {};
	strncpy(ifr.ifr_name, dev, IFNAMSIZ);
	assert(0 == ioctl(ioctlfd, SIOCGIFFLAGS, &ifr));
	if (up) ifr.ifr_flags |= IFF_UP;
	else ifr.ifr_flags &= (~((short)IFF_UP));
	assert(0 == ioctl(ioctlfd, SIOCSIFFLAGS, &ifr));
}

void netlink_init() {
	netlinkfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
//	assert(netlinkfd == 3);
	assert(0 == bind(netlinkfd, (struct sockaddr*)(&(struct sockaddr_nl) {
		.nl_family = AF_NETLINK,
			.nl_pad = 0,
			.nl_pid = getpid(),
			.nl_groups = 0
	}), sizeof(struct sockaddr_nl)));
}

void netlink_end() {
	assert(0 == close(netlinkfd));
	netlinkfd = -1;
}

void netlink_flags(const bool up, const char* const dev) {

	assert(0 == getuid());
	assert(dev && strlen(dev));
	const unsigned index = if_nametoindex(dev);
	assert(index > 0);

	assert(sizeof(Req_link) == send(netlinkfd, &(Req_link){
		.nh = {
		  .nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg)),
		  .nlmsg_type = RTM_NEWLINK,
		  .nlmsg_flags = NLM_F_REQUEST,
		  .nlmsg_seq = 0,
		  .nlmsg_pid = getpid()
		},
			.ifi = {
			  .ifi_family = AF_UNSPEC,
			  .ifi_type = 0,
			  .ifi_index = index,
			  .ifi_flags = up ? IFF_UP : 0,
			  // https://www.spinics.net/lists/netdev/msg598191.html
			  .ifi_change = IFF_UP
		}
	}, sizeof(Req_link), 0));

}

#ifdef FSoE_ON_SAFE_BOARD

BOOL enableDebug = TRUE;
void myLogToFile(char *str)
{
	if (enableDebug == 0) return;

	FILE *fp = fopen("/run/safety/safetyLog.txt", "a");
	if (fp != NULL) {
		fprintf(fp, "%s\n", str);
		fclose(fp);
	}
//	printf("%s\n", str);
}

static struct gpiod_line * lineArray[255];
static struct gpiod_chip * chipArray[32];
#define	CONSUMER	"Consumer"

int gpio_init(int gpioNum)
{
	struct gpiod_chip *chip;
	struct gpiod_line *line;
	int bankNum = (gpioNum/32);
	int lineNum = (gpioNum%32);

	if (enableDebug) printf("gpio_init: gpioNum %d bankNum %d lineNum %d\n", gpioNum, bankNum, lineNum);

	if (chipArray[bankNum] != NULL) {
		chip = chipArray[bankNum];
	} else {
		chip = gpiod_chip_open_by_number(bankNum);
		if (!chip) {
			printf("Open chip failed (gpio %d bank %d)\n", gpioNum, bankNum);
			return -1;
		}
		chipArray[bankNum] = chip;
	}

	line = gpiod_chip_get_line(chip, lineNum);
	if (!line) {
		printf("Get line failed (gpio %d bank %d line %d)\n", gpioNum, bankNum, lineNum);
		return -1;
	}

	lineArray[gpioNum] = line;
	return 0;
}


int gpio_request_events(int gpioNum)
{
//	long long startTime2 = myStartTime();
	struct gpiod_line *line = lineArray[gpioNum];

	gpiod_line_release(line);

//	ret = gpiod_line_request_rising_edge_events(line, CONSUMER);
	int ret = gpiod_line_request_both_edges_events(line, CONSUMER);
	if (ret < 0) {
		printf("Request event notification failed (gpio %d)\n", gpioNum);
		return -1;
	}
//	printf("gpio_request_events: gpio %d %d\n", gpioNum, (int)myGetElapsedTimeMs(startTime2));

	return 0;
}

// returns 0 if event received
// returns <0 if error or timeout
int gpio_wait_event(int gpioNum)
{
	struct timespec ts;
	struct gpiod_line_event event;
	struct gpiod_line *line = lineArray[gpioNum];

	ts.tv_sec = 1;
	ts.tv_nsec = 0;
	int ret = gpiod_line_event_wait(line, &ts);
	if (ret < 0) {
		printf("ERROR gpio%d wait event notification failed\n", gpioNum);
	} else if (ret == 0) {
		ret = -99;  // timeout
//		printf("TIMEOUT gpio%d wait event notification on line\n", gpioNum);
	} else if (ret == 1) {
//		printf("Wait event notification on line #%u \n", gpioNum);

		ret = gpiod_line_event_read(line, &event);
//		printf("Get event notification on line #%u\n", gpioNum);
		if (ret < 0) {
			printf("Read event notification failed gpio%d\n", gpioNum);
		}
	}
	return ret;
}

int gpio_purge_event(int gpioNum)
{
	struct timespec ts;
	struct gpiod_line_event event;
	struct gpiod_line *line = lineArray[gpioNum];

	ts.tv_sec = 0;
	ts.tv_nsec = 0;
	int ret = gpiod_line_event_wait(line, &ts);
	if (ret < 0) {
		printf("ERROR gpio%d wait event notification failed\n", gpioNum);
	} else if (ret == 0) {
		ret = -99;  // timeout
//		printf("TIMEOUT gpio%d wait event notification on line\n", gpioNum);
	} else if (ret == 1) {
//		printf("Wait event notification on line #%u \n", gpioNum);

		ret = gpiod_line_event_read(line, &event);
//		printf("Get event notification on line #%u\n", gpioNum);
		if (ret < 0) {
			printf("Read event notification failed gpio%d\n", gpioNum);
		}
	}
	return ret;
}

int gpio_request_input(int gpioNum)
{
//	long long startTime1 = myStartTime();

	struct gpiod_line *line = lineArray[gpioNum];
	gpiod_line_release(line);

	int ret = gpiod_line_request_input(line, CONSUMER);

	if (ret < 0) {
		printf("Gpio%d request input notification failed\n", gpioNum);
		return -1;
	}
//	printf("gpio_request_input: %d %d\n", gpioNum, (int)myGetElapsedTimeMs(startTime1));

	return 0;
}

int gpio_get_val(int gpioNum)
{
	struct gpiod_line *line = lineArray[gpioNum];
	int val = gpiod_line_get_value(line);
	return val;
}

int gpio_request_output(int gpioNum)
{
//	long long startTime1 = myStartTime();

	struct gpiod_line *line = lineArray[gpioNum];

	gpiod_line_release(line);

	int ret = gpiod_line_request_output(line, CONSUMER, 0);

	if (ret < 0) {
		printf("Gpio%d request input notification failed\n", gpioNum);
		return -1;
	}
//	printf("gpio_request_input: %d %d\n", gpioNum, (int)myGetElapsedTimeMs(startTime1));

	return 0;
}

int gpio_set_val(int gpioNum, int value)
{
	struct gpiod_line *line = lineArray[gpioNum];
	int ret = gpiod_line_set_value(line, value);
	return ret;
}

int SPI_Init(unsigned char safetyCpuId, char *spidev, unsigned long frequencyHz)
{
	spiFrequencyHz = frequencyHz;
	switch(safetyCpuId) {
		case CPU_MASTER:
			strcpy(spidevMaster, spidev);
			fdMaster = open(spidevMaster, O_RDWR);
			if (fdMaster < 0) {
				printf("ERROR opening %s !\n", spidevMaster);
			}
			return fdMaster;
		break;
		case CPU_SLAVE:
			strcpy(spidevSlave, spidev);
			fdSlave = open(spidevSlave, O_RDWR);
			if (fdSlave < 0) {
				printf("ERROR opening %s !\n", spidevSlave);
			}
			return fdSlave;
		break;
		default:
			printf("SPI_Init(), ERROR opening %u !\n", safetyCpuId);
			return -1;
	}
}


int SPI_TxRx(unsigned char safetyCpuId, char *txBuf, char *rxBuf, int len)
{
	int ret;
	memset(xfer, 0, sizeof xfer);
	xfer[0].tx_buf = (unsigned long)txBuf;
	xfer[0].len = len;
//    xfer[0].speed_hz=8000000;
//    xfer[0].speed_hz=1000000;  //  tbd così funziona, anche il pairing
//      xfer[0].speed_hz=10000000;  // tbd così funziona, anche il pairing
	xfer[0].speed_hz = spiFrequencyHz;
	xfer[0].rx_buf = (unsigned long)rxBuf;

	switch(safetyCpuId) {
		case CPU_MASTER:
			errno = 0;
			ret = ioctl(fdMaster, SPI_IOC_MESSAGE(1), xfer);
//			printf("SPI_TxRx master ret %d error %d\n", ret, errno);
		break;
		case CPU_SLAVE:
			errno = 0;
			ret = ioctl(fdSlave, SPI_IOC_MESSAGE(1), xfer);
//			printf("SPI_TxRx slave  ret %d error %d\n", ret, errno);
		break;
		default:
			return -1;
	}

	return ret;
}

//#define FAKE_SPI

#define PRINT_DIFFS
#ifdef PRINT_DIFFS
uint8_t prevFSoEtxFrame[DEF_FSoE_FRAME_LEN];
uint8_t prevFSoErxFrame[DEF_FSoE_FRAME_LEN];
#endif
uint64_t cycleNr;
uint32_t lostCyc;
uint32_t getUs()
{
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return  (uint32_t)(((uint64_t)now.tv_nsec + (uint64_t)now.tv_sec * 1000000000) / 1000);
}

static void spiTxRxSafety()
{
	static uint32_t startT;
	uint32_t prevSPIT = startT;
	int i;

	if (firstTime) {
		gpio_request_events(spigoMasterGpio);
		SPITxBuf.JobNr++;
		SPITxBuf.DbgArea[0] = Obj.Safe_Address.FSoE_Address & 0xFF;
		SPITxBuf.DbgArea[1] = Obj.Safe_Address.FSoE_Address >> 8;
		SPITxBuf.Status = 0;
	}
	lostCyc = 0;
	while(gpio_purge_event(spigoMasterGpio) != -99)
		lostCyc++;
	if (gpio_wait_event(spigoMasterGpio) == -99) {
		printf("%llu >>> SAFE SPI <<< wait event timeout\n", cycleNr);
		static int isFirstTimeout = 1;
		if (isFirstTimeout) {
			myLogToFile("SPIGO master timeout !");  // log only the first timeout
			isFirstTimeout = 0;
		}
		SPITxBuf.Status = 0;
		memset(&SPITxBuf.SafeFrame, 0, FSoEFrameLen);
		SPI_TxRx(CPU_MASTER, &SPITxBuf, &SPIRxBuf, sizeof(s_SPIbuff));
		if (printDiffs){
			printf("%llu >>> SAFE SPI <<< RX: ", cycleNr);
			for (i=0; i<sizeof(SPIRxBuf); i++)
				printf("%02X-", ((uint8_t *)&SPIRxBuf)[i]);
			printf("\n");
		}
	}else{
#ifdef FAKE_SPI
		startT = getUs();
		while (getUs()-startT < 250)
			;
#else
		SPITxBuf.SafeFrameLen = FSoEFrameLen;
		SPITxBuf.Version = 1;
		if (gb_FSoeForcedOut){
			if (sem_trywait(gp_semCMDptr) == 0){
				memcpy(&SPITxBuf.SafeFrame, &gp_memCMDptr[16], FSoEFrameLen);
				sem_post(gp_semCMDptr);
			}
		}else{
			if (doubleBuffer){
				if (FSoutBufSw){
					FSoutBuf2Reading=1;
					memcpy(&SPITxBuf.SafeFrame, FSoutBuf2, FSoEFrameLen);
					FSoutBuf2Reading=0;
				}else{
					FSoutBufReading=1;
					memcpy(&SPITxBuf.SafeFrame, FSoutBuf, FSoEFrameLen);
					FSoutBufReading=0;
				}
			}else{
				pthread_mutex_lock(&FSoutProtect);
				memcpy(&SPITxBuf.SafeFrame, FSoutBuf, FSoEFrameLen);
				pthread_mutex_unlock(&FSoutProtect);
			}
		}
		startT = getUs();
		if (SPI_TxRx(CPU_MASTER, &SPITxBuf, &SPIRxBuf, sizeof(s_SPIbuff)) < 0) {
			// i.e SPI communication error
			cntSpiErr++;
			printf("%llu >>> SAFE SPI <<< SPI error %d\n", cycleNr, cntSpiErr);
		} else {
			startT = getUs()-startT;
	//check jobNR
			if (SPITxBuf.JobNr != (uint16_t)(SPIRxBuf.JobNr+((uint16_t)1))){
				if (firstTime == 0){
					if (SPITxBuf.JobNr == SPIRxBuf.JobNr){
						printf("%llu >>> LOST JOB <<< TX: ", cycleNr);
						for (i=0; i<sizeof(SPITxBuf); i++)
							printf("%02X-", ((uint8_t *)&SPITxBuf)[i]);
						printf("\n");
						printf("%llu >>> LOST JOB <<< RX: ", cycleNr);
						for (i=0; i<sizeof(SPIRxBuf); i++)
							printf("%02X-", ((uint8_t *)&SPIRxBuf)[i]);
						printf("\n");
					}else{
						printf("%llu >>> WRONG JOB <<< TX: ", cycleNr);
						for (i=0; i<sizeof(SPITxBuf); i++)
							printf("%02X-", ((uint8_t *)&SPITxBuf)[i]);
						printf("\n");
						printf("%llu >>> WRONG JOB <<< RX: ", cycleNr);
						for (i=0; i<sizeof(SPIRxBuf); i++)
							printf("%02X-", ((uint8_t *)&SPIRxBuf)[i]);
						printf("\n");
					}
				}
			}else{
				SPITxBuf.Status = 1;
//				Obj.SafeIOerror = (uint16_t)SPIRxBuf.DbgArea[4] << 8 | SPIRxBuf.DbgArea[3];	//encode FEoE Error reported by the slave
				Obj.SafeIOerror = (uint16_t)SPIRxBuf.ErrCod;	//encode SAFE I/O Error reported by the slave
				if (doubleBuffer){
					if (FSinBufSw){
						if (FSinBufReading==0){
							memcpy(FSinBuf, &SPIRxBuf.SafeFrame, FSoEFrameLen);
							FSinBufSw=0;
						}else{
							memcpy(FSinBuf2, &SPIRxBuf.SafeFrame, FSoEFrameLen);
						}
					}else{
						if (FSinBuf2Reading==0){
							memcpy(FSinBuf2, &SPIRxBuf.SafeFrame, FSoEFrameLen);
							FSinBufSw=1;
						}else{
							memcpy(FSinBuf, &SPIRxBuf.SafeFrame, FSoEFrameLen);
						}
					}
				}else{
					pthread_mutex_lock(&FSinProtect);
					memcpy(FSinBuf, &SPIRxBuf.SafeFrame, FSoEFrameLen);
					pthread_mutex_unlock(&FSinProtect);
				}
			}
#ifdef PRINT_DIFFS
			if (printDiffs && memcmp(prevFSoEtxFrame, &SPITxBuf.SafeFrame, FSoEFrameLen) != 0){
				int i;
				printf("%llu >>> SAFE SPI <<< TX: ", cycleNr);
				for (i=0; i<sizeof(SPITxBuf); i++)
					printf("%02X-", ((uint8_t *)&SPITxBuf)[i]);
				printf("\n");
				memcpy(prevFSoEtxFrame, &SPITxBuf.SafeFrame, FSoEFrameLen);
			}
			if (printDiffs && memcmp(prevFSoErxFrame, &SPIRxBuf.SafeFrame, FSoEFrameLen) != 0){
				printf("%llu >>> SAFE SPI <<< RX: ", cycleNr);
				for (i=0; i<sizeof(SPIRxBuf); i++)
					printf("%02X-", ((uint8_t *)&SPIRxBuf)[i]);
				printf("\n");
				memcpy(prevFSoErxFrame, &SPIRxBuf.SafeFrame, FSoEFrameLen);
			}
#endif
#endif
		}
	}
#ifdef USE_TRYWAIT
	if (sem_trywait(gp_semSTATptr) == 0){
		memcpy (&gp_memSTATptr[64], &SPITxBuf, sizeof(SPITxBuf));
		memcpy (&gp_memSTATptr[128], &SPIRxBuf, sizeof(SPIRxBuf));
		sem_post(gp_semSTATptr);
	}//else
		//printf("semaphore gp_semSTATptr is busy - retry later\n");
#else
	sem_wait(gp_semSTATptr);
	memcpy (&gp_memSTATptr[128], &SPIRxBuf, sizeof(SPIRxBuf));
	sem_post(gp_semSTATptr);
#endif
	SPITxBuf.JobNr++;
	if (lostCyc)
		printf("%llu >>> lost cycles <<< nr of pending SPIGO found: %u last SPI duration = %u\n", cycleNr, lostCyc, prevSPIT);
	cycleNr++;
	firstTime = 0;
}

#include <syscall.h>
static int cpu_execute(const char* cmd, char* output, int outputSize)
{
	FILE *fp;

	//CPU_LOG("Executing %s\n", cmd);

	// Open the command for reading.
	fp = popen(cmd, "r");
	if (fp == NULL) {
		printf("Failed to run command\n" );
		return -1;
	}

	// Read the output a line at a time - output it.
	int len = 0;
	memset(output, 0, outputSize);
	while (fgets(&output[len], outputSize-len-1, fp) != NULL)
	{
		len = strlen(output);
	}

	//CPU_LOG("%s",output);

	// close
	pclose(fp);
	return strlen(output);
}

int cpu_moveRtProcesses(int pid, int core)
{
	char cmd[100];
	char output[1024];

	sprintf(cmd, "taskset -pc %d %d", core, pid);
	cpu_execute(cmd, output, sizeof(output));

	return (strlen(output) == 0)? 0 : -1;
}
static int cpu_getDevPid(const char *devName)
{
	char cmd[100];
	char output[1024];
	int ret;

	sprintf(cmd, "ps aux | grep %s | head -n 1 | awk '{ print $2 }'", devName);
	ret = cpu_execute(cmd, output, sizeof(output));
	if (ret < 0)
		return ret;
	return atoi(output);
}

static int cpu_getIRQPid(int irq)
{
	char cmd[100];
	char output[1024];
	int ret;

	sprintf(cmd, "ps aux | grep irq/%d | head -n 1 | awk '{ print $2 }'", irq);
	ret = cpu_execute(cmd, output, sizeof(output));
	if (ret < 0)
		return ret;
	return atoi(output);
}

static int cpu_getPidPriority(int pid)
{
	char cmd[100];
	char output[1024];
	int ret;

	sprintf(cmd, "chrt -p %d | grep priority | awk '{ print $6}'", pid);
	ret = cpu_execute(cmd, output, sizeof(output));
	if (ret < 0)
		return ret;
	return atoi(output);
}
static int cpu_getPidPolicy(int pid)
{
	char cmd[100];
	char output[1024];
	int ret;

	sprintf(cmd, "chrt -p %d | grep policy | awk '{ print $6}'", pid);
	ret = cpu_execute(cmd, output, sizeof(output));
	if (ret < 0)
		return ret;
	if (strcmp(output, "SCHED_BATCH\n")==0)
		return (int)'b';
	else if (strcmp(output, "SCHED_FIFO\n")==0)
		return (int)'f';
	else if (strcmp(output, "SCHED_IDLE\n")==0)
		return (int)'i';
	else if (strcmp(output, "SCHED_OTHER\n")==0)
		return (int)'o';
	else if (strcmp(output, "SCHED_RR\n")==0)
		return (int)'r';
	return -1;
}
static int cpu_setPidPriorityAndPolicy(int pid, int prio, int policy)
{
	char cmd[100];
	char output[1024];
	int ret;

	sprintf(cmd, "chrt -%c -p %d %d", policy, prio, pid);
	ret = cpu_execute(cmd, output, sizeof(output));
	return ret;
}

static void *spiTxRxThread (void *dummy)
{
	static int first = 0;
	if (first == 0){
		if (isolatedCore)
			cpu_moveRtProcesses(syscall(SYS_gettid), 1);
		first=1;
	}
	while (keepRunning){
//		if (gb_isFSoE)
			spiTxRxSafety();
//		else
//			sleep(1);
	}
	return 0;
}
void mySetPriority(pthread_t th, int prio)
{
	int rc, policy;
	struct sched_param    param;
	memset(&param, 0, sizeof(param));
	param.sched_priority = prio;
	rc = pthread_setschedparam(th, SCHED_FIFO, &param);

//    if (enableDebug) {
	if (1) {
		memset(&param, 0, sizeof(param));
		if (rc) printf("pthread_setschedparam res %d\n", rc);
		rc = pthread_getschedparam(th, &policy, &param);
		if (rc) printf("pthread_getschedparam res %d\n", rc);
	}
}
#endif


extern void eth_ports_init(const int i_configuration /*, int reset*/);
extern UINT16 FPGA_Init(void);

#define INTERFACE_ETHERNET	0
#define INTERFACE_ETHERCAT	1

#define INTERFACE_REGISTER	0x00010010

#if _PIC24
int main(void)
#else
void main(int argc, char *argv[])
#endif
{
	int i;
	bool initReg12 = true;
	short ifFlags;
#ifdef IS_MODBUS_UDP_SERVER
	int sock = INVALID_SOCKET;
	int one;
	struct sockaddr_in localaddr;
	struct sockaddr_in clientaddr;
	int clientlen;
	int rxLen;
	unsigned char rxBuf[300];
	unsigned char txBuf[300];
	int optVal=1;
#endif
	uint16_t address = 1;

#ifdef FSoE_ON_SAFE_BOARD
	unsigned long spiFreqHz = DEFAULT_SPI_FREQ_HZ;
	char spidevCpuMaster[128];
//	char spidevCpuSlave[128];
	strcpy(spidevCpuMaster,	SPIDEV_CPU_MASTER_1);
//	strcpy(spidevCpuSlave,	SPIDEV_CPU_SLAVE);
	//variables for SPI driver and IRQ priority management
	int devPID0, devPrio0, devPol0, irqPID0, irqPrio0, irqPol0;
	int devPID1, devPrio1, devPol1, irqPID1, irqPrio1, irqPol1;
	int isUN70 = 0;
#endif

    // signal handling (ctrl+c)
	signal(SIGINT, SigIntHandler);

	// folder containing application
    {
        char* p_last_sep = NULL;
        readlink("/proc/self/exe", gstr_app_folder, PATH_MAX);
        // we want the string to look like "/folder/subfolder1/subfolder2/"
        p_last_sep = strrchr(gstr_app_folder, '/');
        if (p_last_sep != NULL && p_last_sep != gstr_app_folder)
        {
            ++p_last_sep;
            *p_last_sep = 0;
        }
    }

#if 0
    // ini file
    ini_initialize("SSCapp.ini");
    ini_read();
    {
        char* str_debug = ini_get_value("debug");
        if (str_debug)
        {
            gi_debug = strtoul(str_debug, NULL, 0);
        }
    }
    if (gi_debug & DBG_DEFAULT)
        printf(DBGFMT "Application started\n", DBGARGS);

    // eeprom
    eeprom_init("eeprom.bin");

#if 0
    /* initialize the Hardware and the EtherCAT Slave Controller */
#if FC1100_HW
    if(HW_Init())
    {
        HW_Release();
        return;
    }
#else
    HW_Init();
#endif
//	printf("Interface = %s\n", interface == INTERFACE_ETHERNET ? "Ethernet" : "EtherCAT");
#ifdef SET_INTERFACE_DOWN
	get_if_flags(interface, &ifFlags);
	set_if_down(interface, ifFlags);
#endif

	/*
	if (strcmp(interface, "eth0")==0)
		eth_ports_init(0x01);
	else
		eth_ports_init(0x02);
	*/
	FPGA_Init();
#endif
#endif
#ifdef USE_SHARED_MEM
	//init shared area
	{
		int fd;
		shm_unlink(IN_MEM);
		fd = shm_open(IN_MEM, /* name from smem.h */
		O_RDWR | O_CREAT, /* read/write, create if needed */
		0x644); /* access permissions (0644) */
		if (fd < 0)
			printf("FAILED TO CREATE SHARED MEM %d\n", errno);
		else{
			ftruncate(fd, 256); /* get the bytes */
			gp_memINptr = (caddr_t )mmap(NULL, /* let system pick where to put segment */
				513, /* how many bytes */
				PROT_READ | PROT_WRITE, /* access protections */
				MAP_SHARED, /* mapping visible to other processes */
				fd, /* file descriptor */
				0); /* offset: start at 1st byte */
			if ((caddr_t) -1 == gp_memINptr){
				gp_memINptr = NULL;
				printf("FAILED TO MAP SHARED MEM %d \n", errno);
			}else{
				gp_memINptr[0] = 0;		//not Operational
				/* semaphore code to lock the shared mem */
				gp_semINptr = sem_open(IN_SEM, /* name */
				O_CREAT, /* create the semaphore */
				0x644, /* protection perms */
				0); /* initial value */
				if (gp_semINptr == (void*) -1){
					gp_semINptr = NULL;
					printf("FAILED TO CREATE SEMA %d\n", errno);
				}else{
					/* increment the semaphore so that memreader can read */
					sem_post(gp_semINptr);
					printf("IN shared mem %d address: %p [0..%d] sema:%p\n", fd, gp_memINptr, 256 - 1, gp_semINptr);
				}
			}
			close(fd);
		}
	}
	{
		int fd;
		shm_unlink(OUT_MEM);
		fd = shm_open(OUT_MEM, /* name from smem.h */
		O_RDWR | O_CREAT, /* read/write, create if needed */
		0x644); /* access permissions (0644) */
		if (fd < 0)
			printf("FAILED TO CREATE OUT SHARED MEM %d\n", errno);
		else{
			ftruncate(fd, 256); /* get the bytes */
			gp_memOUTptr = (caddr_t )mmap(NULL, /* let system pick where to put segment */
				513, /* how many bytes */
				PROT_READ | PROT_WRITE, /* access protections */
				MAP_SHARED, /* mapping visible to other processes */
				fd, /* file descriptor */
				0); /* offset: start at 1st byte */
			if ((caddr_t) -1 == gp_memOUTptr){
				gp_memOUTptr = NULL;
				printf("FAILED TO MAP SHARED MEM %d\n", errno);
			}else{
				gp_memOUTptr[0] = 0;		//not Operational
				/* semaphore code to lock the shared mem */
				gp_semOUTptr = sem_open(OUT_SEM, /* name */
				O_CREAT, /* create the semaphore */
				0x644, /* protection perms */
				0); /* initial value */
				if (gp_semOUTptr == (void*) -1){
					printf("FAILED TO CREATE SEMA %d\n", errno);
					gp_semOUTptr = NULL;
				}else{
					/* increment the semaphore so that memreader can read */
					sem_post(gp_semOUTptr);
					printf("OUT shared mem %d address: %p [0..%d] sema:%p\n", fd, gp_memOUTptr, 256 - 1, gp_semOUTptr);
				}
			}
			close(fd);
		}
	}
	{
		int fd;
		shm_unlink(CMD_MEM);
		fd = shm_open(CMD_MEM, /* name from smem.h */
		O_RDWR | O_CREAT, /* read/write, create if needed */
		0x644); /* access permissions (0644) */
		if (fd < 0)
			printf("FAILED TO CREATE OUT SHARED MEM %d\n", errno);
		else{
			ftruncate(fd, 256); /* get the bytes */
			gp_memCMDptr = (caddr_t )mmap(NULL, /* let system pick where to put segment */
				513, /* how many bytes */
				PROT_READ | PROT_WRITE, /* access protections */
				MAP_SHARED, /* mapping visible to other processes */
				fd, /* file descriptor */
				0); /* offset: start at 1st byte */
			if ((caddr_t) -1 == gp_memCMDptr){
				gp_memCMDptr = NULL;
				printf("FAILED TO MAP SHARED MEM %d\n", errno);
			}else{
				gp_memCMDptr[0] = 0;		//not Operational
				/* semaphore code to lock the shared mem */
				gp_semCMDptr = sem_open(CMD_SEM, /* name */
				O_CREAT, /* create the semaphore */
				0x644, /* protection perms */
				0); /* initial value */
				if (gp_semCMDptr == (void*) -1){
					printf("FAILED TO CREATE SEMA %d\n", errno);
					gp_semCMDptr = NULL;
				}else{
					/* increment the semaphore so that memreader can read */
					sem_post(gp_semCMDptr);
					printf("CMD shared mem %d address: %p [0..%d] sema:%p\n", fd, gp_memCMDptr, 256 - 1, gp_semCMDptr);
				}
			}
			close(fd);
		}
	}
	{
		int fd;
		shm_unlink(STAT_MEM);
		fd = shm_open(STAT_MEM, /* name from smem.h */
		O_RDWR | O_CREAT, /* read/write, create if needed */
		0x644); /* access permissions (0644) */
		if (fd < 0)
			printf("FAILED TO CREATE OUT SHARED MEM %d\n", errno);
		else{
			ftruncate(fd, 256); /* get the bytes */
			gp_memSTATptr = (caddr_t )mmap(NULL, /* let system pick where to put segment */
				513, /* how many bytes */
				PROT_READ | PROT_WRITE, /* access protections */
				MAP_SHARED, /* mapping visible to other processes */
				fd, /* file descriptor */
				0); /* offset: start at 1st byte */
			if ((caddr_t) -1 == gp_memSTATptr){
				gp_memSTATptr = NULL;
				printf("FAILED TO MAP SHARED MEM %d\n", errno);
			}else{
				gp_memSTATptr[0] = 0;		//not Operational
				/* semaphore code to lock the shared mem */
				gp_semSTATptr = sem_open(STAT_SEM, /* name */
				O_CREAT, /* create the semaphore */
				0x644, /* protection perms */
				0); /* initial value */
				if (gp_semSTATptr == (void*) -1){
					printf("FAILED TO CREATE SEMA %d\n", errno);
					gp_semSTATptr = NULL;
				}else{
					/* increment the semaphore so that memreader can read */
					sem_post(gp_semSTATptr);
					printf("STAT shared mem %d address: %p [0..%d] sema:%p\n", fd, gp_memSTATptr, 256 - 1, gp_semSTATptr);
				}
			}
			close(fd);
		}
	}
#else
    /* semaphore code to lock the shared mem */
    gp_semINptr = sem_open("IN_SEM", /* name */
    O_CREAT, /* create the semaphore */
    0x644, /* protection perms */
    0); /* initial value */
    if (gp_semINptr == (void*) -1){
        gp_semINptr = NULL;
        printf("FAILED TO CREATE SEMA %d\n", errno);
    }else{
        /* increment the semaphore so that memreader can read */
        sem_post(gp_semINptr);
        printf("IN shared mem address: %p [0..%d] sema:%p\n", gp_memINptr, 256 - 1, gp_semINptr);
    }
    /* semaphore code to lock the shared mem */
    gp_semOUTptr = sem_open("OUT_SEM", /* name */
    O_CREAT, /* create the semaphore */
    0x644, /* protection perms */
    0); /* initial value */
    if (gp_semOUTptr == (void*) -1){
        printf("FAILED TO CREATE SEMA %d\n", errno);
        gp_semOUTptr = NULL;
    }else{
        /* increment the semaphore so that memreader can read */
        sem_post(gp_semOUTptr);
        printf("OUT shared mem address: %p [0..%d] sema:%p\n", gp_memOUTptr, 256 - 1, gp_semOUTptr);
    }
    /* semaphore code to lock the shared mem */
    gp_semCMDptr = sem_open("CMD_SEM", /* name */
    O_CREAT, /* create the semaphore */
    0x644, /* protection perms */
    0); /* initial value */
    if (gp_semCMDptr == (void*) -1){
        printf("FAILED TO CREATE SEMA %d\n", errno);
        gp_semCMDptr = NULL;
    }else{
        /* increment the semaphore so that memreader can read */
        sem_post(gp_semCMDptr);
        printf("CMD shared mem address: %p [0..%d] sema:%p\n", gp_memCMDptr, 256 - 1, gp_semCMDptr);
    }
    /* semaphore code to lock the shared mem */
    gp_semSTATptr = sem_open("STAT_SEM", /* name */
    O_CREAT, /* create the semaphore */
    0x644, /* protection perms */
    0); /* initial value */
    if (gp_semSTATptr == (void*) -1){
        printf("FAILED TO CREATE SEMA %d\n", errno);
        gp_semSTATptr = NULL;
    }else{
        /* increment the semaphore so that memreader can read */
        sem_post(gp_semSTATptr);
        printf("STAT shared mem address: %p [0..%d] sema:%p\n", gp_memSTATptr, 256 - 1, gp_semSTATptr);
    }
#endif
//	MainInit();
	FSoE_init_defaults();

	application_parameters[0] = 0x00;
	application_parameters[1] = 0x00;
	application_parameters[2] = 0x00;
	application_parameters[3] = 0x00;

	safety_main();
#if 0
	//check presence of SAFE Plugin
	for(i = 0; i < MAX_SLOT;i++)
	{
		if (PluginInstalled(i, PL3057_PLUGIN_BIT))
		{
			printf("SAFE I/O Plugin 3057-FSoE found in slot %d\n", i);
//			Obj.FSOE_Slave_Safety_Inputs_NrofIndexes = 12;
//			Obj.FSOE_Master_Safety_Outputs_NrofIndexes = 8;
//			Obj.Safe_Params_NrofIndexes = 31;
#ifdef FSOE_OBJ_DYNAMIC_ASSIGN
//			COE_AdjFSoEObjectsToDic(PL3057_TYPE);
#endif
			mountedFSoEBoardType = PL3057_TYPE;
			mountedFSoEBoardSlot = i;
			if (i >= 2)
				strcpy(spidevCpuMaster,	SPIDEV_CPU_MASTER_2);
		}
	}
#else
    strcpy(spidevCpuMaster,	SPIDEV_CPU_MASTER_2);
#endif

//    HW_EEPROM_Reload();
	// dvm test
	//eth_ports_init(0x02);

	// doesn't belong to Ethercat interface, but we can reuse the macro
//	HW_EscWriteDWord(INTERFACE_ETHERCAT, INTERFACE_REGISTER);
//	HW_EoeOpen();
#if 0
	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> FSoE support init <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	{
		char* str_FSoEadd = ini_get_value("FSoEadd");
		if (str_FSoEadd)
		{
			address = strtoul(str_FSoEadd, NULL, 0);
			printf("FSoE address set to %d by stored parameter\n", address);
		}
	}
#endif
	if (argc > 2){
		int i = 1;
		while (i < argc){
			if (strncmp(argv[i], "-a", 2) == 0 && (i+1) < argc){
				address = strtoul(argv[i+1],NULL,10);
				printf("FSoE address set to %d by command line parameter\n", address);
				i++;
			}else if (strncmp(argv[i], "-p", 2) == 0 && (i+1) < argc){
				ecatSlavePrio = strtoul(argv[i+1],NULL,10);
				printf("ECAT slave thread priority set to %d\n", ecatSlavePrio);
				i++;
#ifdef FSoE_ON_SAFE_BOARD
			//}else if (strncmp(argv[i], "-s", 2) == 0 && (i+1) < argc){
			//	spigoSlaveGpio = strtoul(argv[i+1],NULL,10);
			//	printf("SPIGO Slave GPIO index set to %d\n", spigoSlaveGpio);
			//	i++;
			//}else if (strncmp(argv[i], "-m", 2) == 0 && (i+1) < argc){
			//	spigoMasterGpio = strtoul(argv[i+1],NULL,10);
			//	printf("SPIGO Master GPIO index set to %d\n", spigoMasterGpio);
			//	i++;
			//}else if (strncmp(argv[i], "-S", 2) == 0 && (i+1) < argc){
			//	strcpy(spidevCpuSlave, argv[i+1]);
			//	printf("SPI Slave Device set to \"%s\"\n", spidevCpuSlave);
			//	i++;
			}else if (strncmp(argv[i], "-M", 2) == 0 && (i+1) < argc){
				strcpy(spidevCpuMaster, argv[i+1]);
				printf("SPI Master Device set to \"%s\"\n", spidevCpuMaster);
				i++;
			}else if (strncmp(argv[i], "-f", 2) == 0 && (i+1) < argc){
				spiFreqHz = strtoul(argv[i+1],NULL,10);
				printf("SPI Frequency set to %d\n", spiFreqHz);
				i++;
			}else if (strncmp(argv[i], "-v", 2) == 0){
				printDiffs = 1;
				printf("print FSoe Buffers  when changed\n");
			}else if (strncmp(argv[i], "-d", 2) == 0){
				doubleBuffer = 1;
				printf("use double buffers for data exchange\n");
			}else if (strncmp(argv[i], "-i", 2) == 0){
				isolatedCore = 1;
				printf("run RT process on isolated core\n");
			}else if (strncmp(argv[i], "-P", 2) == 0 && (i+1) < argc){
				safeSpiPrio = strtoul(argv[i+1],NULL,10);
				printf("SAFE SPI thread priority set to %d\n", safeSpiPrio);
				i++;
			}else if (strncmp(argv[i], "-Q", 2) == 0 && (i+1) < argc){
				spiSupportPrio = strtoul(argv[i+1],NULL,10);
				printf("spi IRQ and support thread priority set to %d\n", spiSupportPrio);
				i++;
#endif
			}else{
				printf("Invalid Parameter - FSoE address set to %d\n", address);
#ifdef FSoE_ON_SAFE_BOARD
//				printf("-s slave SPIGO index\n");
//				printf("-m master SPIGO index\n");
//				printf("-S slave SPI device\n");
//				printf("-M master SPI device\n");
//				printf("-f SPI frequency\n");
#endif
			}
			i += 1;
		}
	}
	printf("FSoE parameters\n");
	printf("FSoE address = %d\n", address);
	printf("ECAT slave thread priority = %d\n", ecatSlavePrio);
#ifdef FSoE_ON_SAFE_BOARD
//	printf("SPIGO Slave GPIO index = %d\n", spigoSlaveGpio);
//	printf("SPIGO Master GPIO index = %d\n", spigoMasterGpio);
//	printf("SPI Slave Device = \"%s\"\n", spidevCpuSlave);
	printf("SPI Master Device = \"%s\"\n", spidevCpuMaster);
	printf("SPI Frequency = %d\n", spiFreqHz);
	printf("SAFE SPI thread priority = %d\n", safeSpiPrio);
	printf("SPI IRQ and support thread priority = %d\n", spiSupportPrio);
	memset(FSinBuf2, 0, sizeof(FSinBuf));
	memset(FSoutBuf2, 0, sizeof(FSoutBuf));
	FSinBufSw = 0;
	FSoutBufSw = 0;
	FSinBufReading = 0;
	FSinBuf2Reading = 0;
	FSoutBufReading = 0;
	FSoutBuf2Reading = 0;
#endif

#if 1
	{
		struct sched_param sch;
		sched_getparam(getpid(), &sch);
		sch.sched_priority = ecatSlavePrio;    //highest
		if (sched_setscheduler(getpid(), SCHED_RR, &sch)) {
			printf("Failed to setschedparam: %d\n", errno);
		}
	}
#endif
	FSoE_init_default_address(address);

	gb_isFSoE = false;
	memset(FSinBuf, 0, sizeof(FSinBuf));
	memset(FSoutBuf, 0, sizeof(FSoutBuf));
#ifndef FSoE_ON_SAFE_BOARD
	gb_FSoE_is_on_SPI = true;
	gb_isFirst_FSoE_cycle = true;
	gb_FSoE_initOk = false;
#else
	{
		int res;
		char dummy[]="ciao";

		if (strcmp(spidevCpuMaster, SPIDEV_CPU_MASTER_1) != 0){
			res = SPI_Init(CPU_MASTER, SPIDEV_CPU_MASTER_2_1, spiFreqHz);
		}else{
			res = SPI_Init(CPU_MASTER, SPIDEV_CPU_MASTER_1_1, spiFreqHz);
		}
		SPI_TxRx(CPU_MASTER, dummy, dummy, strlen(dummy));	//purge TX buffer
		if (fdMaster)
			close (fdMaster);

		res = SPI_Init(CPU_MASTER, spidevCpuMaster, spiFreqHz);
		printf("SPI_INIT %s = %d\n", spidevCpuMaster, res);
		//res = SPI_Init(CPU_SLAVE,  spidevCpuSlave,  spiFreqHz);
		//printf("SPI_INIT %s = %d\n", spidevCpuSlave, res);
		if (strcmp(spidevCpuMaster, SPIDEV_CPU_MASTER_1) != 0)
			spigoMasterGpio = SPIGO_MASTER_GPIO_2;
		printf("SPIGO Master GPIO index = %d\n", spigoMasterGpio);
		res = gpio_init(spigoMasterGpio);
		printf(">>> SAFE_SPI <<< - GPIO master init: %d %d\n", spigoMasterGpio, res);
		//if ((res = gpio_init(spigoSlaveGpio) >= 0)) {
		//	printf(">>> SAFE_SPI <<< - GPIO slave init: %d %d\n", spigoSlaveGpio, res);
		//	gpio_request_input(spigoSlaveGpio);  // always in polling
		//}
	}

	//raise priority of SPI dev task and irq
	if (!strcmp(gst_platform, "UN70"))
		isUN70 = 1;
	if (strcmp(spidevCpuMaster, SPIDEV_CPU_MASTER_1) != 0){
	   devPID1 = cpu_getDevPid("spi1");
	   if (devPID1){
		   devPrio1 = cpu_getPidPriority(devPID1);
		   devPol1 = cpu_getPidPolicy(devPID1);
		   cpu_setPidPriorityAndPolicy(devPID1, spiSupportPrio, 'f');
		   if (!isUN70){
			   irqPID1 = cpu_getIRQPid(SPI1_IRQ);
			   irqPrio1 = cpu_getPidPriority(irqPID1);
			   irqPol1 = cpu_getPidPolicy(irqPID1);
			   cpu_setPidPriorityAndPolicy(irqPID1, spiSupportPrio, 'f');
		   }
	   }
	}else{
	   devPID0 = cpu_getDevPid("spi0");
	   if (devPID0){
		   devPrio0 = cpu_getPidPriority(devPID0);
		   devPol0 = cpu_getPidPolicy(devPID0);
		   cpu_setPidPriorityAndPolicy(devPID0, spiSupportPrio, 'f');
		   if (!isUN70){
			   irqPID0 = cpu_getIRQPid(SPI0_IRQ);
			   irqPrio0 = cpu_getPidPriority(irqPID0);
			   irqPol0 = cpu_getPidPolicy(irqPID0);
			   cpu_setPidPriorityAndPolicy(irqPID0, spiSupportPrio, 'f');
		   }
	   }
	}
	pthread_mutex_init(&FSinProtect, NULL);
	pthread_mutex_init(&FSoutProtect, NULL);
	{
		int s = pthread_attr_init(&attr);
		if (s != 0) {
			printf("pthread_attr_init error !\n");
            return;
		}
		s = pthread_attr_setstacksize(&attr, 1024*1024);
		if (s != 0) {
			printf("pthread_attr_setstacksize error !\n");
            return;
		}
	}
	pthread_create(&pthRX, &attr, spiTxRxThread, "SafeSpiTrh");
	mySetPriority(pthRX, safeSpiPrio);
	pthread_setname_np(pthRX, "SAFE_SPI");
#endif

#ifdef IS_MODBUS_UDP_SERVER
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	one=fcntl(sock, F_GETFL, 0);
	fcntl(sock, F_SETFL, one | O_NONBLOCK); // set to non block
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,(const void *)&optVal, sizeof(int));
	memset(&localaddr, 0, sizeof(struct sockaddr_in));
	localaddr.sin_family = AF_INET;
	localaddr.sin_port = htons(MODBUS_UDP_PORT);
	localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (struct sockaddr *) &localaddr, sizeof(struct sockaddr_in)) < 0){
		close(sock);
		sock = INVALID_SOCKET;
		printf("UDP socket BIND error\n");
	}
	clientlen = sizeof(clientaddr);
#endif
	do{
#ifndef FSoE_ON_SAFE_BOARD
		//FSoE application
		if (gb_isFSoE && gb_isFirst_FSoE_cycle){
			gb_isFirst_FSoE_cycle = false;
			gb_FSoE_initOk = (safty_main(Obj.Safe_Address.FSoE_Address) == 0);
			if (gb_FSoE_initOk)
				printf("FailSafe Firmware initialized correctly\n");
			else
				printf("failed to initialize FailSafe Firmware\n");
		}
#endif
//		MainLoop();
		safety_app_run(gp_memCMDptr, gp_memSTATptr);
		usleep(4000);

		//update slave status
//		gp_memSTATptr[0] = nAlStatus;
//		gp_memSTATptr[1] = u8EcatRunLed;
//		gp_memSTATptr[2] = u8EcatErrorLed;
#if 0
		if (initReg12){
			UINT16 stationAlias;
			HW_EscReadWord(stationAlias, ESC_STATION_ALIAS);
			printf("station_alias: %04X\n", stationAlias);
			initReg12 = false;
		}
#endif


#ifdef IS_MODBUS_UDP_SERVER
		if (sock != INVALID_SOCKET){
			rxLen = 0;
			rxLen = recvfrom(sock, rxBuf, sizeof(rxBuf),0,(struct sockaddr *)&clientaddr,(socklen_t *)&clientlen);
			//printf ("check Modbus request. %d %d\n", sock, rxLen);
			if (rxLen > 0){
				int i;
				int index;
				int nBytes;
				bool isOut;
				bool isWrite;
				bool isSingleWrite;
				unsigned char *mem;
				sem_t * pSem;
				uint8_t cmd = rxBuf[7];
#if 0
				printf ("Modbus request received. %d\n", rxLen);
				for (i=0;i<rxLen;i++){
					printf("%02X,", rxBuf[i]);
				}
				printf("\n");
#endif				
				//analize and respond
				if (cmd != READ_HOLDING_REGISTERS && cmd != READ_INPUT_REGISTERS && cmd != WRITE_MULTIPLE_REGISTERS && cmd != WRITE_SINGLE_REGISTER ){
					//send exception "Function not supported"
					memcpy(txBuf, rxBuf, 8);
					txBuf[5]=3;
					txBuf[7] |= 0x80;
					txBuf[8] = 1;
					sendto(sock,txBuf,9,0,(const struct sockaddr *)&clientaddr,clientlen);
				}else{
					isOut = (cmd == READ_INPUT_REGISTERS)?false:true;
					isWrite = (cmd==WRITE_MULTIPLE_REGISTERS)?true:false;
					isSingleWrite = (cmd==WRITE_SINGLE_REGISTER)?true:false;
					index = (int)rxBuf[8]*256 + rxBuf[9];
					if (isSingleWrite)
						nBytes = 2;
					else
						nBytes = ((int)rxBuf[10]*256 + rxBuf[11])*2;
					if (index*2 + nBytes > 512){
						//send exception "Illegal Data Address"
						memcpy(txBuf, rxBuf, 8);
						txBuf[5]=3;
						txBuf[7] |= 0x80;
						txBuf[8] = 2;
						sendto(sock,txBuf,9,0,(const struct sockaddr *)&clientaddr,clientlen);
					}else{
						if ((isOut)?gp_memINptr[0]==0:gp_memOUTptr[0]==0){
							//send exception "Server Busy"
							memcpy(txBuf, rxBuf, 8);
							txBuf[5]=3;
							txBuf[7] |= 0x80;
							txBuf[8] = 6;
							sendto(sock,txBuf,9,0,(const struct sockaddr *)&clientaddr,clientlen);
						}else{
							mem = (isOut) ? (uint8_t *)&gp_memINptr[1+index*2] : (uint8_t *)&gp_memOUTptr[1+index*2];
							pSem = (isOut)?gp_semINptr:gp_semOUTptr;
							if (!isWrite && !isSingleWrite){
								memcpy(txBuf, rxBuf, 8);
								txBuf[5]=3+nBytes;
								txBuf[8]=nBytes;
#ifdef USE_TRYWAIT
								if (sem_trywait(pSem) == 0){
									for (i=0;i<nBytes;i+=2){
										txBuf[9+i]=mem[i+1];
										txBuf[9+i+1]=mem[i];
									}
									sem_post(pSem);
									sendto(sock,txBuf,6+3+nBytes,0,(const struct sockaddr *)&clientaddr,clientlen);
								}else{
									printf("semaphore is busy - retry later\n");
									//send exception "Server Busy"
									memcpy(txBuf, rxBuf, 8);
									txBuf[5]=3;
									txBuf[7] |= 0x80;
									txBuf[8] = 6;
									sendto(sock,txBuf,9,0,(const struct sockaddr *)&clientaddr,clientlen);
								}
#else
								sem_wait(pSem);
								for (i=0;i<nBytes;i+=2){
									txBuf[9+i]=mem[i+1];
									txBuf[9+i+1]=mem[i];
								}
								sem_post(pSem);
								sendto(sock,txBuf,6+3+nBytes,0,(const struct sockaddr *)&clientaddr,clientlen);
#endif
							}else if (isWrite){
#ifdef USE_TRYWAIT
								if (sem_trywait(pSem) == 0){
									for (i=0;i<nBytes;i+=2){
										mem[i+1]=rxBuf[13+i];
										mem[i]=rxBuf[13+i+1];
									}
									sem_post(pSem);
									memcpy(txBuf, rxBuf, 12);
									txBuf[5]=6;
									sendto(sock,txBuf,12,0,(const struct sockaddr *)&clientaddr,clientlen);
								}else{
									printf("semaphore is busy - retry later\n");
									//send exception "Server Busy"
									memcpy(txBuf, rxBuf, 8);
									txBuf[5]=3;
									txBuf[7] |= 0x80;
									txBuf[8] = 6;
									sendto(sock,txBuf,9,0,(const struct sockaddr *)&clientaddr,clientlen);
								}
#else
								sem_wait(pSem);
								for (i=0;i<nBytes;i+=2){
									mem[i+1]=rxBuf[13+i];
									mem[i]=rxBuf[13+i+1];
								}
								sem_post(pSem);
								memcpy(txBuf, rxBuf, 12);
								txBuf[5]=6;
								sendto(sock,txBuf,12,0,(const struct sockaddr *)&clientaddr,clientlen);
#endif
							}else{	//single write
#ifdef USE_TRYWAIT
								if (sem_trywait(pSem) == 0){
									mem[1]=rxBuf[10];
									mem[0]=rxBuf[11];
									sem_post(pSem);
									sendto(sock,rxBuf,12,0,(const struct sockaddr *)&clientaddr,clientlen);
								}else{
									printf("semaphore is busy - retry later\n");
									//send exception "Server Busy"
									memcpy(txBuf, rxBuf, 8);
									txBuf[5]=3;
									txBuf[7] |= 0x80;
									txBuf[8] = 6;
									sendto(sock,txBuf,9,0,(const struct sockaddr *)&clientaddr,clientlen);
								}
#else
								sem_wait(pSem);
								mem[1]=rxBuf[10];
								mem[0]=rxBuf[11];
								sem_post(pSem);
								sendto(sock,rxBuf,12,0,(const struct sockaddr *)&clientaddr,clientlen);
#endif
							}
						}
					}
				}
			}
		}
#endif
		//check commands
		if (gp_memCMDptr[0]){
			if (sem_trywait(gp_semCMDptr) == 0){
				switch (gp_memCMDptr[0]){
					case 1:
						keepRunning = 0;
						printf("exit command\n");
						break;
#if 0
					case 2:
						gb_FSoeForcedOut = true;
						break;
					case 3:
						gb_FSoeForcedOut = false;
						break;
#endif
					case 4:
						memcpy(application_parameters, &gp_memCMDptr[20], 8);	//update parameters and restart the master
						printf("apply new parameters %04X %04X %04X %04X and restart the master\n", application_parameters[0], application_parameters[1],application_parameters[2],application_parameters[3]);
						safety_main();
				}
				gp_memCMDptr[0] = 0;
				sem_post(gp_semCMDptr);
			}
		}
	} while (keepRunning);
#ifdef IS_MODBUS_UDP_SERVER
	if (sock != INVALID_SOCKET)
		close(sock);
#endif

#ifdef FSoE_ON_SAFE_BOARD
	pthread_mutex_destroy(&FSinProtect);
	pthread_mutex_destroy(&FSoutProtect);
	if (fdMaster)
		close (fdMaster);
#endif
#if 0
	//close special I/O drivers if in use
	if (gb_PLIO06_0_inUsed || gb_PLIO06_0_outUsed){
		uint8_t out = 0;
		PLIO06_OutDig(gby_PLIO06_0_slotNr, &out);
		PLIO06_close(gby_PLIO06_0_slotNr);
	}
	if (gb_PLIO06_1_inUsed || gb_PLIO06_1_outUsed){
		uint8_t out = 0;
		PLIO06_OutDig(gby_PLIO06_1_slotNr, &out);
		PLIO06_close(gby_PLIO06_1_slotNr);
	}
#endif
#ifdef USE_SHARED_MEM
	//deinit shared area
	if (gp_semINptr){
		sem_close(gp_semINptr);
		gp_semINptr = NULL;
		sem_unlink(IN_SEM);
	}
	if (gp_semOUTptr){
		sem_close(gp_semOUTptr);
		gp_semOUTptr = NULL;
		sem_unlink(OUT_SEM);
	}
	if (gp_semCMDptr){
		sem_close(gp_semCMDptr);
		gp_semCMDptr = NULL;
		sem_unlink(CMD_SEM);
	}
	if (gp_semSTATptr){
		sem_close(gp_semSTATptr);
		gp_semSTATptr = NULL;
		sem_unlink(STAT_SEM);
	}
	if (gp_memINptr){
		munmap(gp_memINptr, 257);
		shm_unlink(IN_MEM);
		gp_memINptr = NULL;
	}
	if (gp_memOUTptr){
		munmap(gp_memOUTptr, 257);
		shm_unlink(OUT_MEM);
		gp_memOUTptr = NULL;
	}
	if (gp_memCMDptr){
		munmap(gp_memCMDptr, 257);
		shm_unlink(CMD_MEM);
		gp_memCMDptr = NULL;
	}
	if (gp_memSTATptr){
		munmap(gp_memSTATptr, 257);
		shm_unlink(STAT_MEM);
		gp_memSTATptr = NULL;
	}
#else
    if (gp_semINptr){
        sem_close(gp_semINptr);
        gp_semINptr = NULL;
        sem_unlink("IN_SEM");
    }
    if (gp_semOUTptr){
        sem_close(gp_semOUTptr);
        gp_semOUTptr = NULL;
        sem_unlink("OUT_SEM");
    }
    if (gp_semCMDptr){
        sem_close(gp_semCMDptr);
        gp_semCMDptr = NULL;
        sem_unlink("CMD_SEM");
    }
    if (gp_semSTATptr){
        sem_close(gp_semSTATptr);
        gp_semSTATptr = NULL;
        sem_unlink("STAT_SEM");
    }
#endif
	printf("Restore Hardware settings and Exit\n");

//	HW_EoeClose();
//	HW_EscWriteDWord(INTERFACE_ETHERNET, INTERFACE_REGISTER);
//	ioctl_init();
//	ioctl_flags(true, "eth1");
//	ioctl_flags(true, "eth2");
//	ioctl_end();

	// dvm test eth_ports_init(0x00);
//	HW_Release();

#ifdef SET_INTERFACE_DOWN
//	set_if_up(interface, ifFlags);
#endif

//	if (gi_debug & DBG_DEFAULT)
        printf(DBGFMT "Exit\n", DBGARGS);
#if _PIC24
    return 0;
#endif
}
#endif //#if USE_DEFAULT_MAIN
/** @} */

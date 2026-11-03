/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
*/

/**
\addtogroup MCI_HW Parallel ESC Access
@{
*/

/**
\file mcihw.c
\author EthercatSSC@beckhoff.com
\brief Implementation
This file contains the interface to the ESC via MCI

\version 5.10

<br>Changes to version V5.01:<br>
V5.10 HW1: Add 32Bit test access during hardware initialization (only for 32Bit controller)<br>
V5.10 HW4: Add volatile directive for direct ESC DWORD/WORD/BYTE access<br>
           Add missing swapping in mcihw.c<br>
           Add "volatile" directive vor dummy vairables in enable and disable SyncManger functions<br>
           Add missing swapping in EL9800hw files<br>
<br>Changes to version V5.0:<br>
V5.01 HW1: Invalid ESC access function was used<br>
<br>Changes to version V4.30:<br>
V5.0 ESC4: Save SM disable/Enable. Operation may be pending due to frame handling.<br>
<br>Changes to version V4.20:<br>
V4.30 ESM: if mailbox SyncManager is disabled and bMbxRunning is true the SyncManger settings need to be revalidate<br>
V4.30 SYNC: change synchronisation control function. Add usage of 0x1C32:12 [SM missed counter].<br>
Calculate bus cycle time (0x1C32:02 ; 0x1C33:02) CalcSMCycleTime()<br>
V4.30 PDO: rename PDO specific functions (COE_xxMapping -> PDO_xxMapping and COE_Application -> ECAT_Application)<br>
V4.20 PCI 1: PC_APPLICATION removed<br>
V4.10 PCI 1: Support for PCI EtherCAT Slave<br>
V4.10 MCI 1: Support for PCI EtherCAT Slave<br>
<br>Changes to version V4.07:<br>
V4.08 ECAT 3: The AlStatusCode is changed as parameter of the function AL_ControlInd<br>
<br>Changes to version V4.06:<br>
V4.07 ECAT 1: The sources for SPI and MCI were merged (in ecat_def.h<br>
                   set the switch MCI_HW to 1 when using the MCI,<br>
                   set the switch SPI_HW (obsolete now EL9800_HW)to 1 when using the SPI<br>
V4.07 COEAPPL 2: The example is working for the NIOS with the evaluation board DBC2C20 V1.2<br>
                       which is available by Altera<br>
<br>Changes to version V3.20:<br>
V4.00 MCI 1: When the MAILBOX_QUEUE-switch was set, the mailbox access<br>
             in boot mode was not working correctly<br>
V4.00 MCI 2: The mailbox data link layer was not working correctly<br>
V4.00 MCI 3: if no outputs are supported, the watchdog has to be triggered by the SM3-Event<br>
V4.00 ECAT 1: The handling of the Sync Manager Parameter was included according to<br>
              the EtherCAT Guidelines and Protocol Enhancements Specification<br>
V4.00 APPL 1: The watchdog checking should be done by a microcontroller<br>
                 timer because the watchdog trigger of the ESC will be reset too<br>
                 if only a part of the sync manager data is written<br>
V4.00 APPL 4: The EEPROM access through the ESC is added
*/

/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "ecat_def.h"
#include "foeappl.h"
#include "ecatfoe.h"
#include "applInterface.h"

#define    _MCIHW_ 1
#include "mcihw.h"
#undef _MCIHW_
#define    _MCIHW_ 0

//#define FOE_LOG
//#define EOE_LOG
//#define EOE_DUMP
//#define USE_SELECT

#include "ecatslv.h"
#include "ecatappl.h"
#include "ini_file.h"
#include "semaphore.h"
#define USE_SEMAPHORE
sem_t sem_EOT;

void HW_EcatIsr(void);

int gi_debug = DBG_NONE;
//int gi_debug = DBG_DEFAULT;
int i_dw_idx;
static unsigned char packet[1536];
static unsigned short usFrameLength;
static bool bFramePending;
static bool bTxThreadWaiting;
static bool keepTxRunning;
static bool receiverIsOver;
extern volatile int keepRunning;
//interface to LEDs
static FILE *fpR = NULL;
static FILE *fpE = NULL;

/*--------------------------------------------------------------------------------------
------
------    local Types and Defines
------
--------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------
------
------    local variables and constants
------
-----------------------------------------------------------------------------------------*/
int i_dev_mem_fd = -1;
uint8_t* p_map_base = NULL;
#if defined(FPGA_PCIE)
int i_uio_fd = -1;
int i_config_fd = -1;
#endif

TSYNCMAN    TmpSyncMan;

struct timespec g_ts_timer_clear = { 0, 0 };
UINT64 g_ns_clear = 0;
UINT64 g_ms_resolution = 0;	// initialize at startup

/*-----------------------------------------------------------------------------------------
------
------    local functions
------
-----------------------------------------------------------------------------------------*/
UINT64 timespec_to_ns(struct timespec* p_ts)
{
	UINT64 ns = (p_ts->tv_sec * 1000000000) + p_ts->tv_nsec;
	return ns;
}



/*-----------------------------------------------------------------------------------------
------
------    functions
------
-----------------------------------------------------------------------------------------*/

UINT32 HW_GetTimer()
{
    struct timespec ts_now;
	UINT64 ns_now;
	UINT64 ns_elapsed;

    clock_gettime(ESC_HW_CLOCK, &ts_now);
	ns_now = timespec_to_ns(&ts_now);
	ns_elapsed = ns_now - g_ns_clear;

//	printf(DBGFMT "%08x us\n", DBGARGS, (UINT32)us_elapsed);
	return (UINT32)ns_elapsed;
}

void HW_ClearTimer()
{
    clock_gettime(ESC_HW_CLOCK, &g_ts_timer_clear);
	g_ns_clear = timespec_to_ns(&g_ts_timer_clear);

//    printf(DBGFMT "\n", DBGARGS);
}
void HW_ClearTimerComp(uint32_t reminder)
{
	clock_gettime(ESC_HW_CLOCK, &g_ts_timer_clear);
	g_ns_clear = timespec_to_ns(&g_ts_timer_clear) - reminder;
}

UINT32 HW_GetTimerResolution()
{
	//printf(DBGFMT "%08x ms\n", DBGARGS, (UINT32)g_ms_resolution);
    return (UINT32)g_ms_resolution;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\return     0 if initialization was successful

 \brief    This function initialize the EtherCAT Slave Interface.
*////////////////////////////////////////////////////////////////////////////////////////

UINT16 HW_Init(void)
{
    /* the memory interface to the ESC, the ESC-interrupt and the ECAT-timer for the
       watchdog monitoring should be initialized here microcontroller specific*/

    /* initialize ESC DPRAM pointer microcontroller specific to the beginning of the physical memory of the ESC,
        the macro MAKE_PTR_TO_ESC should be defined in mcihw.h */
#if defined(FPGA_PCIE)
    int err = 0;
    unsigned char command_high = 0;

	//gi_debug = DBG_DEFAULT;

    i_uio_fd = open("/dev/uio0", O_RDWR);
    if (i_uio_fd == -1)
    {   // failed to open
        if (gi_debug & DBG_DEFAULT)
            printf(DBGFMT "open uio0 error %d\n", DBGARGS, errno);
        return 1;
    }

    i_config_fd = open("/sys/class/uio/uio0/device/config", O_RDWR);
    if (i_config_fd == -1)
    {   // failed to open
        if (gi_debug & DBG_DEFAULT)
            printf(DBGFMT "open uio0 config error %d\n", DBGARGS, errno);
        return 1;
    }

    /* Read and cache command value */
    err = pread(i_config_fd, &command_high, 1, 5);
    if (err != 1)
    {
        perror("command config read:");
        return errno;
    }

    i_dev_mem_fd = open("/sys/class/uio/uio0/device/resource0", O_RDWR /* | O_SYNC*/);
//bar1    i_dev_mem_fd1 = open("/sys/class/uio/uio0/device/resource1", O_RDWR /* | O_SYNC*/);
#elif defined(FPGA_LOCAL)
    i_dev_mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
#endif
    if (i_dev_mem_fd == -1)
    {   // failed to open
        if (gi_debug & DBG_DEFAULT)
            printf(DBGFMT "open error %d\n", DBGARGS, errno);
        return 1;
    }
	FoEbusy = FALSE;
	FoEfileName[0] = '\0';
    return 0;
}

UINT16 FPGA_Init(void)
{
    UINT32 intMask = 0;

#if defined(FPGA_PCIE)
    p_map_base = (uint8_t*)mmap(0, ESC_MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, i_dev_mem_fd, 0);
// bar1    p_map_base1 = (uint8_t*)mmap(0, 1024 /* dvm test ESC_MAP_SIZE*/, PROT_READ | PROT_WRITE, MAP_SHARED, i_dev_mem_fd1, 0);
#elif defined(FPGA_LOCAL)
    p_map_base = (uint8_t*)mmap(0, ESC_MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, i_dev_mem_fd, ESC_BASE & ~ESC_MAP_MASK);
#endif
    if (p_map_base == (void*)-1)
    {
        if (gi_debug & DBG_DEFAULT)
            printf(DBGFMT "mmap error %d\n", DBGARGS, errno);
        close(i_dev_mem_fd);
        i_dev_mem_fd = -1;
        return 1;
    }

#if defined(FPGA_PCIE)
    pEsc = (ESC_MEM_ADDR ESCMEM*) (p_map_base);
#elif defined(FPGA_LOCAL)
    pEsc = (ESC_MEM_ADDR ESCMEM * ) (p_map_base + (ESC_BASE & ESC_MAP_MASK));
#endif

    {
        struct timespec ts_res;
        UINT64 ns_res;

        clock_getres(ESC_HW_CLOCK, &ts_res);
        ns_res = ts_res.tv_sec * 1000000000ul + ts_res.tv_nsec;
		g_ms_resolution = (UINT64) (1. / (ns_res / 1000000.));
    }

    {
        UINT32 u16PdiCtrl = 0;

        do
        {
            HW_EscReadWord(u16PdiCtrl,ESC_PDI_CONTROL_OFFSET);
            u16PdiCtrl = SWAPDWORD(u16PdiCtrl);
            if (!keepRunning)
                break;
        } while ((u16PdiCtrl & 0xFF) != 0x80);  // DVM 0x80 for Avalon interface
    }

    /* initialize the PDI - interrupt source*/
    INIT_ESC_INT;

    /* initialize the AL_Event Mask Register */
    /* the AL Event-Mask register is initialized with 0, so that no ESC interrupt is generated yet,
       the AL Event-Mask register will be adapted in the function StartInputHandler in ecatslv.c
        when the state transition from PREOP to SAFEOP is made */
    HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);

    /* enable the ESC-interrupt microcontroller specific,
        the macro ENABLE_ESC_INT should be defined in ecat_def.h */
    ENABLE_ESC_INT();

    return 0;
}

void HW_Release(void)
{
    if (p_map_base)
    {
        munmap(p_map_base, ESC_MAP_SIZE);
        p_map_base = NULL;
    }

    if (i_dev_mem_fd != -1)
    {
        close(i_dev_mem_fd);
        i_dev_mem_fd = -1;
    }

	//close LED interface
	if (fpE != NULL)
		fclose(fpE);
	if (fpR != NULL)
		close(fpR);

    pEsc = NULL;

}

/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    This function resets the hardware
*////////////////////////////////////////////////////////////////////////////////////////

void     HW_RestartTarget(void)
{
}


#define AGPIO0_BASE	0xFF210010  // FPGA/Avalon base address of GPIO0 core + FPGA/CPU bridge translation
#define AGPIO0_MAP_SIZE    4096 // not sure we can map less than one page
#define AGPIO0_MAP_MASK    (AGPIO0_MAP_SIZE-1)
#define AGPIO0_ETH_BIT_POSITION 23
#define AGPIO0_RESET_BIT_POSITION   25
#if defined(FPGA_PCIE)
#define AGPIO0_ETH_BIT_MASK 0x00800000  // bit 23
#else
#define AGPIO0_ETH_BIT_MASK 0x01800000  // bits 24 and 23
#endif

/* Note: must be called before accessing any of the FPGA registers (in FPGA_Init() or elsewhere),
* because it initializes one of FPGA clocks.
*/
void eth_ports_init(const int i_configuration)
{
    uint32_t* p_agpio0 = NULL;
    uint32_t gpio_values = 0;

    uint8_t* p_agpio0_base = (uint8_t*)mmap(0, AGPIO0_MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, i_dev_mem_fd, AGPIO0_BASE & ~AGPIO0_MAP_MASK);
    if (p_agpio0_base == (void*)-1)
    {
        if (gi_debug & DBG_DEFAULT)
            printf(DBGFMT "mmap error %d\n", DBGARGS, errno);

        return;
    }

    p_agpio0 = (uint32_t*)(p_agpio0_base + (AGPIO0_BASE & AGPIO0_MAP_MASK));
    gpio_values = *p_agpio0;

    gpio_values &= ~AGPIO0_ETH_BIT_MASK;    // clear bits
	uint32_t mask = 0;
	if (i_configuration == 1)
		mask = (1 << AGPIO0_ETH_BIT_POSITION) +1;
	else if (i_configuration == 2)
		mask = 1 << AGPIO0_ETH_BIT_POSITION;
	gpio_values |= mask; // set new value

    *p_agpio0 = gpio_values;

    munmap(p_agpio0_base, AGPIO0_MAP_SIZE);
}


UINT8 eeprom_buffer[ESC_EEPROM_SIZE] = { 0 };
char str_eeprom_file_name[PATH_MAX] = { 0 };
extern char gstr_app_folder[PATH_MAX];

void eeprom_init(const char* str_name)
{
    strcpy(str_eeprom_file_name, gstr_app_folder);
    strcat(str_eeprom_file_name, str_name);
}

UINT16 HW_EEPROM_Read(UINT32 wordaddr)
{
    UINT16* pData = (UINT16*)eeprom_buffer;

    if (wordaddr <= (ESC_EEPROM_SIZE >> 1))
    {
        // from pData to ESC...OFFSET
        HW_EscWrite((MEM_ADDR*)&pData[(wordaddr)], ESC_EEPROM_DATA_OFFSET, EepromReadSize);

        {
            UINT32 u32 = *(UINT32*)(MEM_ADDR*)&pData[(wordaddr)];
            if (gi_debug & DBG_DEFAULT)
				printf(DBGFMT "addr=%04lX val=%08lX\n", DBGARGS, wordaddr, u32);
        }

        return 0;
    }
    else
    {
        if (gi_debug & DBG_DEFAULT)
			printf(DBGFMT "bad waddress %08lX\n", DBGARGS, wordaddr);
        return 1;
    }
}

UINT16 HW_EEPROM_Write(UINT32 wordaddr)
{
    UINT16* pData = (UINT16*)eeprom_buffer;

    if (wordaddr <= (ESC_EEPROM_SIZE >> 1))
    {
        // from ESC...OFFSET to pData
#if (EEPROM_WRITE_SIZE == 1)
	   HW_EscReadByte(*((MEM_ADDR*)&pData[(wordaddr)]), ESC_EEPROM_DATA_OFFSET);
#elif (EEPROM_WRITE_SIZE == 2)
		HW_EscReadWord(*(UINT16 *)((MEM_ADDR*)&pData[(wordaddr)]), ESC_EEPROM_DATA_OFFSET);
#elif (EEPROM_WRITE_SIZE == 4)
	   HW_EscReadDWord(*(UINT32 *)((MEM_ADDR*)&pData[(wordaddr)]), ESC_EEPROM_DATA_OFFSET);
#else
		HW_EscRead((MEM_ADDR*)&pData[(wordaddr)], ESC_EEPROM_DATA_OFFSET, EEPROM_WRITE_SIZE);
#endif

        if (gi_debug & DBG_DEFAULT)
			printf(DBGFMT "addr=%04lX val=%04X\n", DBGARGS, wordaddr, pData[wordaddr]);

        return 0;
    }
    else
    {
        if (gi_debug & DBG_DEFAULT)
			printf(DBGFMT "bad waddress %08lX\n", DBGARGS, wordaddr);
        return 1;
    }
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
\return     0 if reload was successful

 \brief    This function is called when the master has request an EEPROM reload during EEPROM emulation

*////////////////////////////////////////////////////////////////////////////////////////
extern UINT16 initialize_station_alias();
extern UINT16 u16EepromStoreTimeoutValue;

UINT16 HW_EEPROM_Reload (void)
{
    UINT32 i_configuration = 0;
    UINT16* p_eeprom_words = NULL;
	int i_file;
	if (u16EepromStoreTimeoutValue != 0)	//save EEPROM is pending
		return 0;

	i_file = open(str_eeprom_file_name, O_RDONLY);

    if (i_file == -1)
    {
        // not a fatal error, maybe file was just never saved yet or user intentionally deleted it
        if (gi_debug & DBG_DEFAULT)
            printf(DBGFMT "no saved eeprom file, using default content\n", DBGARGS);
        memcpy(eeprom_buffer, pEEPROM, ESC_EEPROM_SIZE);    // copy default content
    }
    else
    {
        if (gi_debug & DBG_DEFAULT)
            printf(DBGFMT "reading eeprom from file\n", DBGARGS);
        read(i_file, eeprom_buffer, ESC_EEPROM_SIZE);
        close(i_file);
    }

    p_eeprom_words = (UINT16*)eeprom_buffer;
    // bits 15..0 configured station alias
    i_configuration = initialize_station_alias();
    // bit 16 enhanced link detection for all ports; bit 1 in eeprom word 0 high byte
    i_configuration |= (p_eeprom_words[0] & 0x0200) ? 0x00010000 : 0x00000000;
    // bits 20..17 enhanced link detection for individual ports; bits 7..4 (15..12) in eeprom word 0 high byte
    i_configuration |= ((UINT32)(p_eeprom_words[0] & 0xF000)) << (17-12);
    // bits 24..21 ESC DL configuration; eeprom word 5 bits 7..4
    i_configuration |= ((UINT32)(p_eeprom_words[5] & 0x00F0)) << (21-4);
    // bits 27..25 FIFO size reduction; eeprom word 5 bits 11..9 inverted
    i_configuration |= ( ~(((UINT32)(p_eeprom_words[5] & 0x0E00)) << (25 - 9)) ) & 0x0E000000;

    // set register 0x508
	HW_EscWriteDWord(i_configuration, ESC_EEPROM_DATA_OFFSET);
    if (gi_debug & DBG_DEFAULT)
		printf(DBGFMT "eepromcfg=0x%08lX\n", DBGARGS, i_configuration);

    return 0;
}

void HW_EEPROM_Store(void)
{
    int i_file = open(str_eeprom_file_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (i_file == -1)
    {
        if (gi_debug & DBG_DEFAULT)
            printf(DBGFMT "failed opening/creating eeprom file\n", DBGARGS);
    }
    else
    {
        if (gi_debug & DBG_DEFAULT)
            printf(DBGFMT "writing eeprom to file\n", DBGARGS);
        write(i_file, eeprom_buffer, ESC_EEPROM_SIZE);
        close(i_file);
		sync();
    }

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    Interrupt service routine for the interrupts from the EtherCAT Slave Controller
*////////////////////////////////////////////////////////////////////////////////////////

void HW_EcatIsr(void)
{
    PDI_Isr();
}


void HW_SetLed(BOOL RunLed,BOOL ErrLed)
{
	static BOOL RunLedPrev = true;
	static BOOL ErrLedPrev = true;
	char c;

//	struct timespec now;
//	clock_gettime(CLOCK_MONOTONIC, &now);
//	printf("%15lld\n", ((uint64_t)now.tv_sec * 1000000000 + now.tv_nsec)/1000);

	if (ErrLed != ErrLedPrev){
		ErrLedPrev = ErrLed;
		if (fpE == NULL){
			fpE = fopen("/sys/class/leds/ecat1/brightness","w");
			if (fpE == NULL){
				printf("failed opening access to ERROR LED\n");
				return;
			}
		}
		c = (ErrLed)?'1':'0';
		fwrite(&c, 1, 1, fpE);
		fflush(fpE);
	}

	if (RunLed != RunLedPrev){
		RunLedPrev = RunLed;
		if (fpR == NULL){
			fpR = fopen("/sys/class/leds/ecat2/brightness","w");
			if (fpR == NULL){
				printf("failed opening access to RUN LED\n");
				return;
			}
		}
		c = (RunLed)?'1':'0';
		fwrite(&c, 1, 1, fpR);
		fflush(fpR);
	}
}

int gi_queued_command = QUEUED_COMMAND_NOTHING;
extern void plc_commit_4000();

void my_main_loop()
{
    static bool b_coop_level_initialized = false;
    static UINT32 i_coop_level = 500;   // default; results in about 10% CPU use; HMI uses about 20%

    if (bEscIntEnabled)
    {
        HW_EcatIsr();
    }

    if (!b_coop_level_initialized)
    {
        char* str_level = ini_get_value("cooperative_level");
        if (str_level)
        {
            i_coop_level = strtoul(str_level, NULL, 0);
        }
        b_coop_level_initialized = true;
    }

    switch (gi_queued_command)
    {
    case QUEUED_COMMAND_NOTHING:
        usleep(i_coop_level);
        break;

    case QUEUED_COMMAND_COMMIT_4000:
        plc_commit_4000();
        break;

    default:
        // error, unrecognized command
        break;
    }
    gi_queued_command = QUEUED_COMMAND_NOTHING;
}

//EoE interface
/* ioctl commands */
#define XETHM_VS_IOCTL_ADD_NIC			1100
#define XETHM_VS_IOCTL_REMOVE_NIC		1101

/* structure for ioctls */
struct nic_definition
{
		unsigned char master_instance;
		unsigned char slave_pos;
};

int net_execute(const char* cmd, char* output, int outputSize)
{
	FILE *fp;
#ifdef EOE_LOG
	printf("Executing %s\n", cmd);
#endif
	// Open the command for reading.
	fp = popen(cmd, "r");
	if (fp == NULL) {
#ifdef EOE_LOG
		printf("Failed to run command\n" );
#endif
		return -1;
	}
	if (output == NULL)
		return 0;

	// Read the output a line at a time - output it.
	int len = 0;
	memset(output, 0, outputSize);
	while (fgets(&output[len], outputSize-len-1, fp) != NULL)
	{
		len = strlen(output);
	}
#ifdef EOE_LOG
	printf(output);
#endif
	// close
	pclose(fp);
	return strlen(output);
}
int net_networkDown(char* ifname)
{
	char cmd[100];
	char output[1024];

	sprintf(cmd, "ifconfig %s down", ifname);
	int ret = net_execute(cmd, output, sizeof(output));
	if (ret < 0)
		return ret;

	return 0;
}

int net_networkUp(char* ifname)
{
	char cmd[100];
	char output[1024];

	sprintf(cmd, "ifconfig %s up", ifname);
	int ret = net_execute(cmd, output, sizeof(output));
	if (ret < 0)
		return ret;

	return 0;
}

int net_addNIC(int instance, int pos)
{
	int fd = open("/dev/xethm_vs", O_RDWR);
	if (fd < 0)
		return -1;

	struct nic_definition nic;
	nic.master_instance = instance;
	nic.slave_pos = pos;

	ioctl(fd, XETHM_VS_IOCTL_ADD_NIC, &nic);
	close(fd);

	return 0;
}

int net_bridgeExists(char* name)
{
	char cmd[100];
	char output[1024];

	sprintf(cmd, "brctl show | grep %s | awk '{ print $1 }'", name);
	int ret = net_execute(cmd, output, sizeof(output));
	if (ret < 0)
		return ret;

	return (strlen(output) > 0);
}

int net_IFExists(char* bridge, char* ifname)
{
	char cmd[100];
	char output[1024];

	sprintf(cmd, "brctl show %s | grep %s", bridge, ifname);
	int ret = net_execute(cmd, output, sizeof(output));
	if (ret < 0)
		return ret;

	return (strlen(output) > 0);
}

int net_addBridge(char* name)
{
	char cmd[100];
	char output[1024];

	sprintf(cmd, "brctl addbr %s", name);
	int ret = net_execute(cmd, output, sizeof(output));
	if (ret < 0)
		return ret;

	return 0;
}

int net_addBridgeInterface(char* bridge, const char* ifname)
{
	char cmd[100];
	char output[1024];

	sprintf(cmd, "brctl addif %s %s", bridge, ifname);
	int ret = net_execute(cmd, output, sizeof(output));
	if (ret < 0)
		return ret;

	return 0;
}

int net_moduleLoaded()
{
	char cmd[100];
	char output[1024];

	sprintf(cmd, "lsmod | grep xethm_vs");
	int ret = net_execute(cmd, output, sizeof(output));
	if (ret < 0)
		return 0;

	return (strlen(output) > 0);
}

int net_loadModule()
{
	char cmd[100];
	char output[1024];

	//sprintf(cmd, "insmod xethm_vs.ko");
	sprintf(cmd, "modprobe xethm_vs");
	int ret = net_execute(cmd, output, sizeof(output));

	return ret;
}
int net_unloadModule()
{
	char cmd[100];
	char output[1024];

	sprintf(cmd, "rmmod xethm_vs.ko");
	int ret = net_execute(cmd, output, sizeof(output));

	return ret;
}
int net_extractValue(char* output, char* delim, char* value)
{
	strcpy(value, "");

	char* ptr = strstr(output, delim);
	if (ptr != NULL)
	{
		ptr += strlen(delim);
		char* end = strchr(ptr, ' ');
		if (end == NULL)
			end = strchr(ptr, '\n');
		if (end == NULL)
			end = strchr(ptr, '\n');
		if (end == NULL)
			end = strchr(ptr, '\t');

		if (end != NULL)
		{
			*end = '\0';
			strcpy(value, ptr);

			*end = ' ';
		}
	}
	return strlen(value);
}


int net_getIPAddress(const char* ifname, char* addr, char* netmask, char* bcast)
{
	char cmd[100];
	char output[1024];

	sprintf(cmd, "ifconfig %s", ifname);
	int ret = net_execute(cmd, output, sizeof(output));
	if (ret < 0)
		return ret;

	net_extractValue(output, "inet addr:", addr);
	net_extractValue(output, "Bcast:", bcast);
	net_extractValue(output, "Mask:", netmask);

	return 0;
}

int net_setHwAddress(const char* ifname, char * hwAddr)
{
	char cmd[100];
	char output[1024];

	if (strlen(hwAddr) > 0)
	{
		sprintf(cmd, "ip link set dev %s address %s", ifname, hwAddr);
		int ret = net_execute(cmd, output, sizeof(output));
		if (ret < 0)
			return ret;
	}
}
int net_setIPAddress(const char* ifname, char* addr, char* netmask, char* bcast)
{
	char cmd[100];
	char output[1024];

	if (strlen(addr) > 0)
	{
		sprintf(cmd, "ifconfig %s %s", ifname, addr);
		int ret = net_execute(cmd, output, sizeof(output));
		if (ret < 0)
			return ret;
	}

	if (strlen(netmask) > 0)
	{
		sprintf(cmd, "ifconfig %s netmask %s", ifname, netmask);
		int ret = net_execute(cmd, output, sizeof(output));
		if (ret < 0)
			return ret;
	}

	if (strlen(bcast) > 0)
	{
		sprintf(cmd, "ifconfig %s broadcast %s", ifname, bcast);
		int ret = net_execute(cmd, output, sizeof(output));
		if (ret < 0)
			return ret;
	}

	return 0;
}

int net_setDefaultGateway(const char* defGtw)
{
	char cmd[100];
	char output[1024];

	if (strlen(defGtw) > 0 && strcmp(defGtw, "0.0.0.0")!=0)
	{
		sprintf(cmd, "route add default gw %s", defGtw);
		int ret = net_execute(cmd, output, sizeof(output));
		if (ret < 0)
			return ret;
	}
	return 0;
}

void *HW_EoeTransmit(void *p)
{
	UINT16 res;
	receiverIsOver = false;
	keepTxRunning = true;
//	unsigned char packet[1514];
	while (keepTxRunning){
		if (EoEfdRead > 0){
			UINT16 length = 0;
#ifdef USE_SELECT
			fd_set set;
			struct timeval timeout;
			int rv;
#ifdef EOE_LOG
//			printf("EOE check transmit F:%d\n", EoEfdRead);
#endif
			FD_ZERO(&set); /* clear the set */
			FD_SET(EoEfdRead, &set); /* add our file descriptor to the set */
			timeout.tv_sec = 0;
			timeout.tv_usec = 100000;
			rv = select(EoEfdRead + 1, &set, NULL, NULL, &timeout);
//			printf("after select %d\n", rv);
			if(rv == 1){
				printf("after select %d\n", rv);
				if(FD_ISSET(EoEfdRead, &set)){
					printf("before read\n");
#else
#ifdef EOE_LOG
//			printf("EOE check transmit F:%d\n", EoEfdRead);
#endif
#endif
					length = read(EoEfdRead, packet, 1536);
#ifdef EOE_LOG
//					printf("read returned  %d\n", length);
#endif
					if (length != 0 && length != 0xFFFF){
#ifdef ADD_PADDING
						if (length < 64){ //padding
							memset(&packet[length], 0, 64-length);
							length = 64;
						}
#endif
#ifdef EOE_LOG
						{
							struct timespec now;
							clock_gettime(CLOCK_MONOTONIC, &now);
							printf("%15lld EOE transmit %d : ", ((uint64_t)now.tv_sec * 1000000000 + now.tv_nsec)/1000000, length);
						}
#ifdef EOE_DUMP
						{
							int i;
							for (i=0; i<length;i++)
//							for (i=0; i<36;i++)
								printf("%02X-", packet[i]);
						}
#endif
						printf("\n");
#endif

//						printf(" <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< EOE wait for SEND to complete\n");
						bTxThreadWaiting = true;
						usFrameLength = length;
						bFramePending = true;
						sem_wait(&sem_EOT);
//						printf(" >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> EOE signal SEND completed\n");
					}
				}
				//else
				//	 printf("receive timeout: %d\n",rv);
#ifdef USE_SELECT
			}
		}
#endif
	}
	receiverIsOver = true;
#ifdef EOE_LOG
	printf("SCC_EOE exited\n");
#endif
	return NULL;
}

void HW_EoeReceive(UINT16 *pData, UINT16 length)
{
	if (EoEfdWrite > 0){
		int res;
		if (length > 1514)
			length = 1514;
#ifdef EOE_LOG
		struct timespec now;
		clock_gettime(CLOCK_MONOTONIC, &now);
		printf("%15lld EOE receive %d : ", ((uint64_t)now.tv_sec * 1000000000 + now.tv_nsec)/1000000, length);
#ifdef EOE_DUMP
		{
			int i;
			unsigned char *p=pData;
			for (i=0; i<length;i++)
//			for (i=0; i<36;i++)
				printf("%02X-", p[i]);
		}	
#endif
		printf("\n");
#endif
		res = write(EoEfdWrite , pData, length);
#ifdef EOE_LOG
//		printf("EoEReceive completed\n");
#endif
	}
}
void HW_EoeEndTransmissionInd()
{
//	printf("EOE SEND completed\n");
	if (bTxThreadWaiting){
		bTxThreadWaiting = false;
		sem_post(&sem_EOT);
	}
}

void HW_EoeSendFrame()
{
	if (bFramePending){
		if (EOE_SendFrameRequest((UINT16 *)packet, usFrameLength) == 0){
			bFramePending = false;
		}
	}
}

void HW_EoeSettingInd(UINT16 *pMac, UINT16 *pIp, UINT16 *pSubNet, UINT16 *pDefaultGateway, UINT16 *pDnsIp)
{
	int ret;
	char strIp[16];
	char strSubNet[16];
	char strBcast[16];
	char strDefGtw[16];
	char strMAC[18];
	char *pbIp = (char *)pIp;
	char *pbSubNet = (char *)pSubNet;
	char *pbDefGtw = (char *)pDefaultGateway;
	char *pbMac = (char *)pMac;
	char bCast[4];
#if 0
	if (pIp[0]==0 && pIp[1]==0){
		printf("setting interface with default values\n");
		pbIp[0]=192;
		pbIp[1]=168;
		pbIp[2]=0;
		pbIp[3]=250;
		pbSubNet[0]=255;
		pbSubNet[1]=255;
		pbSubNet[2]=255;
		pbSubNet[3]=0;
		pbMac[0] = 0x00;
		pbMac[1] = 0x55;
		pbMac[2] = 0x66;
		pbMac[3] = 0x77;
		pbMac[4] = 0x88;
		pbMac[5] = 0x99;
	}
#endif
	bCast[0] = pbIp[0] | ~(pbSubNet[0]);
	bCast[1] = pbIp[1] | ~(pbSubNet[1]);
	bCast[2] = pbIp[2] | ~(pbSubNet[2]);
	bCast[3] = pbIp[3] | ~(pbSubNet[3]);

	sprintf(strIp, "%d.%d.%d.%d", pbIp[0], pbIp[1], pbIp[2], pbIp[3]);
	sprintf(strSubNet, "%d.%d.%d.%d", pbSubNet[0], pbSubNet[1], pbSubNet[2], pbSubNet[3]);
	sprintf(strMAC, "%02X:%02X:%02X:%02X:%02X:%02X", pbMac[0], pbMac[1], pbMac[2], pbMac[3],  pbMac[4],  pbMac[5]);
	sprintf(strBcast, "%d.%d.%d.%d", bCast[0], bCast[1], bCast[2], bCast[3]);
	sprintf(strDefGtw, "%d.%d.%d.%d", pbDefGtw[0], pbDefGtw[1], pbDefGtw[2], pbDefGtw[3]);

	if (!net_moduleLoaded())
		net_loadModule();
	if (!net_moduleLoaded()){
#ifdef EOE_LOG
		printf("** Unable to load network module xethm_vs\n");
#endif
		return;
	}
	ret = net_addNIC(0, 0);
	if (ret < 0){
#ifdef EOE_LOG
		printf("** Unable to create NIC %d.%d. Check that xethm_vs.ko kernel module has been properly loaded\n", 0, 0);
#endif
		return;
	}
	net_setHwAddress("eoe0a0", strMAC);
#ifdef USE_BRIDGE
	ret = net_bridgeExists("eoebr");
	if (ret == 0)
	{
		printf("Bridge does no exist. It will be created now\n");
		// bridge does not exist. get iface configuration (IP address)
#if 0
		char addr[30];
		char netmask[30];
		char bcast[30];
		memset(addr, 0, sizeof(addr));
		memset(netmask, 0, sizeof(netmask));
		memset(bcast, 0, sizeof(bcast));
		ret = net_getIPAddress("eth1", addr, netmask, bcast);
#endif
		// create bridge
		ret = net_addBridge("eoebr");
		// add iface
		ret = net_addBridgeInterface("eoebr", "eth1");
		// add virtual NIC
		ret = net_addBridgeInterface("eoebr", "eoe0a0");
		// set bride's IP address
		ret = net_setIPAddress("eth1", "0.0.0.0", "", "");
		ret = net_setIPAddress("eoe0a0", "0.0.0.0", "", "");
	}else{
		printf("Bridge already exists\n");

		// bridge exists: check if the iterface is already in the bridge
		ret = net_IFExists("eoebr", "eoe0a0");
		if (ret == 0)
		{
			printf("Adding NIC to the bridge\n");
			// bridge down
			ret = net_networkDown("eoebr");
			// add virtual NIC
			ret = net_addBridgeInterface("eoebr", "eoe0a0");
			ret = net_setIPAddress("eoe0a0", "0.0.0.0", "", "");
		}
	}
	net_setIPAddress("eoebr", strIp, strSubNet, strBcast);
	// bridge up
	net_networkUp("eoebr");
#else
	net_setIPAddress("eoe0a0", strIp, strSubNet, strBcast);
#endif
	net_setDefaultGateway(strDefGtw);
#ifdef EOE_LOG
	printf ("HW_EoeSettingInd - %d ip=%s mask=%s bCast=%s MAC=%s\n", ret, strIp, strSubNet, strBcast, strMAC);
#endif
#ifdef EOE_LOG
	printf("init access files to virtual NIC\n");
#endif
	if (EoEfdRead <= 0)
		EoEfdRead = open("/dev/eoe0a0", O_RDONLY);
	if (EoEfdWrite <= 0)
		EoEfdWrite = open("/dev/eoe0a0", O_RDWR);
#ifdef EOE_LOG
	printf("init access files to virtual NIC %d %d \n", EoEfdRead, EoEfdWrite);
#endif
	//create and start thread for managing answers
	if (!keepTxRunning){
		pthread_attr_t attr;
		SSC_EoE_thp = (pthread_t*)malloc(sizeof(pthread_t));
		pthread_attr_init(&attr);
		pthread_attr_setstacksize(&attr, 65536);
		ret = pthread_create(SSC_EoE_thp, &attr, HW_EoeTransmit, NULL);
		if (ret < 0) {
			return 0;
		}

		struct sched_param schparam;
		schparam.sched_priority = sched_get_priority_max(SCHED_RR);
		pthread_setschedparam(*SSC_EoE_thp, SCHED_RR, &schparam);

		pthread_setname_np(*SSC_EoE_thp, "SCC_EoE");
	}
}
void HW_EoeOpen()
{
	SSC_EoE_thp = NULL;
	sem_init(&sem_EOT, 0, 0);
	EoEfdRead = -1;
	EoEfdWrite = -1;
}
#define SIGUSR1 10
void HW_EoeClose()
{
	keepTxRunning = false;
#ifdef USE_SELECT
	while (!receiverIsOver){
		printf("waiting receiver to close\n");
		usleep(10000);
	}
	printf("finished waiting receiver to close\n");
#else
	if (SSC_EoE_thp)
		pthread_kill(SSC_EoE_thp, SIGUSR1);	//??????????????????????????? cleanup closing of parallel thread
#endif
	if (SSC_EoE_thp)
		free(SSC_EoE_thp);

	if (EoEfdRead > 0){
		close(EoEfdRead);
		EoEfdRead = -1;
	}
	if (EoEfdWrite > 0){
		close(EoEfdWrite);
		EoEfdWrite = -1;
	}
	net_unloadModule();
	HW_EoeEndTransmissionInd();
	sem_destroy(&sem_EOT);
}

//FOE interface
UINT16 HW_FoeRead(UINT16 MBXMEM * pName, UINT16 nameSize, UINT32 password, UINT16 maxBlockSize, UINT16 *pData)
{
	if (FoEbusy){
		return ECAT_FOE_ERRCODE_ACCESS;
	}
	if (pName){
		strncpy (FoEfileName, pName, (nameSize < sizeof(FoEfileName)-1)?nameSize:sizeof(FoEfileName)-1);
#ifdef FOE_LOG
		printf("FoeREAD file: %s -  chunk:%d\n", pName, maxBlockSize);
#endif
	}
	if (FoEfileName[0] == '\0')
		return ECAT_FOE_ERRCODE_NOTDEFINED;
	fd = fopen(FoEfileName, "rb");
	if (fd == NULL){
		FoEfileName[0] = '\0';
		return ECAT_FOE_ERRCODE_NOTFOUND;
	}
	FoEbusy = true;
	FoEcurrentOffset = 0;
	size_t len = fread(pData, 1, maxBlockSize, fd);
	if (len < maxBlockSize){
		fclose (fd);
		FoEfileName[0] = '\0';
		FoEbusy = false;
		if (len < 0){
			//decode errno ???????????????
#ifdef FOE_LOG
			printf ("read error: %d\n", len);
#endif
			return ECAT_FOE_ERRCODE_ACCESS;
		}
#ifdef FOE_LOG
		printf ("read file completed\n");
#endif
	}
	FoEcurrentOffset += len;
//	printf ("read success: %d\n", len);
	return len;
}
UINT16 HW_FoeReadData(UINT32 offset, UINT16 maxBlockSize, UINT16 *pData)
{
	if (fd != NULL){
		//printf ("read next at: %d\n", offset);
		if (FoEcurrentOffset != offset){
			fclose (fd);
			FoEfileName[0] = '\0';
			FoEbusy = false;
#ifdef FOE_LOG
			printf ("read error out of sequence: %d / %d\n", offset, FoEcurrentOffset);
#endif
			return ECAT_FOE_ERRCODE_PACKENO;
		}
		fseek(fd, offset, SEEK_SET);
		size_t len = fread(pData, 1, maxBlockSize, fd);
		if (len < maxBlockSize){
			fclose (fd);
			FoEfileName[0] = '\0';
			FoEbusy = false;
			if (len < 0){
				//decode errno ???????????????
#ifdef FOE_LOG
				printf ("read error: %d\n", len);
#endif
				return ECAT_FOE_ERRCODE_ACCESS;
			}
#ifdef FOE_LOG
			printf ("read file completed\n");
#endif
		}
		FoEcurrentOffset += len;
//		printf ("read next success: %d\n", len);
		return len;
	}
	return ECAT_FOE_ERRCODE_NOTDEFINED;
}
void HW_FoeError(UINT32 errorCode)
{
	printf ("FoE abort received error code: %X\n", errorCode);
}
UINT16 HW_FoeWrite(UINT16 MBXMEM * pName, UINT16 nameSize, UINT32 password)
{
	if (FoEbusy){
		return ECAT_FOE_ERRCODE_ACCESS;
	}
	strncpy (FoEfileName, pName, (nameSize < sizeof(FoEfileName)-1)?nameSize:sizeof(FoEfileName)-1);
#ifdef FOE_LOG
	printf("FoeWrite file: %s\n", pName);
#endif
	if (FoEfileName[0] == '\0')
		return ECAT_FOE_ERRCODE_NOTDEFINED;
	fd = fopen(FoEfileName, "wb");
	if (fd == NULL){
		FoEfileName[0] = '\0';
		return ECAT_FOE_ERRCODE_EXISTS;	//decode errno ???????????????????
	}
	FoEbusy = true;
	FoEcurrentOffset = 0;
	return 0;
}
UINT16 HW_FoeWriteData(UINT16 MBXMEM * pData, UINT16 Size, BOOL bDataFollowing)
{
	if (fd != NULL){
//		printf ("write next len: %d\n", Size);
		size_t len = fwrite(pData, 1, Size, fd);
		if (len != Size){
			fclose (fd);
			sync();
			FoEfileName[0] = '\0';
			FoEbusy = false;
#ifdef FOE_LOG
			printf ("write error: %d\n", len);
#endif
			return ECAT_FOE_ERRCODE_DISKFULL;
		}
		if (bDataFollowing == FALSE){
			fclose (fd);
			sync();
			FoEfileName[0] = '\0';
			FoEbusy = false;
#ifdef FOE_LOG
			printf ("write file completed\n");
#endif
		}
//		printf ("write next success: %d\n", len);
		return 0;
	}
	return ECAT_FOE_ERRCODE_NOTDEFINED;
}


/** @} */

#define ESC_MII_CONTROL_STATUS  0x0510  // 2 bytes
#define ESC_MII_PHY_ADDRESS 0x0512  // 1 byte
#define ESC_MII_PHY_REGISTER_ADDRESS    0x0513  // 1 byte
#define ESC_MII_PHY_DATA    0x0514  // 2 bytes
#define ESC_MII_ECAT_ACCESS_STATE    0x0516  // 1 byte
#define ESC_MII_PDI_ACCESS_STATE    0x0517  // 1 byte
#define ESC_MII_PHY_PORT_STATUS 0x0518  // 4 bytes

#define MII_CMD_READ    (0x0001 << 8)
#define MII_CMD_WRITE   (0x0002 << 8)
#define MII_STATUS_BUSY (0x0001 << 15)

/* returns 0 on success, otherwise various error codes */

int mdio_read(UINT8 i_phy_addr, UINT8 i_register, UINT16* pi_value)
{
    UINT8 i_ecat_access_state = 0;
    UINT8 i_pdi_access_state = 0;
    UINT16 i_control = MII_CMD_READ;
    int i_iteration = 0;

    HW_EscReadByte(i_ecat_access_state, ESC_MII_ECAT_ACCESS_STATE);
    HW_EscReadByte(i_pdi_access_state, ESC_MII_PDI_ACCESS_STATE);
    if (((i_ecat_access_state & 0x01) == 0x00) &&
        ((i_pdi_access_state & 0x01) == 0x00))
    {
        i_pdi_access_state = 0x01;
        HW_EscWriteByte(i_pdi_access_state, ESC_MII_PDI_ACCESS_STATE);

        HW_EscWriteByte(i_phy_addr, ESC_MII_PHY_ADDRESS);
        HW_EscWriteByte(i_register, ESC_MII_PHY_REGISTER_ADDRESS);
        HW_EscWriteWord(i_control, ESC_MII_CONTROL_STATUS);

        for (i_iteration = 0; i_iteration < 100; ++i_iteration)
        {
            HW_EscReadWord(i_control, ESC_MII_CONTROL_STATUS);
            if (i_control & MII_STATUS_BUSY)
                usleep(1000);
            else
                break;
        }
        
        if (i_control & MII_STATUS_BUSY)
        {
            if (gi_debug & DBG_DEFAULT)
                printf(DBGFMT "mdio read wait failed status=%x\n", DBGARGS, i_control);
            return 1;
        }
        if (i_control & (1 << 13))
        {
            if (gi_debug & DBG_DEFAULT)
                printf(DBGFMT "mdio read command failed status=%x\n", DBGARGS, i_control);
            return 2;
        }
        if (i_control & (1 << 14))
        {
            if (gi_debug & DBG_DEFAULT)
                printf(DBGFMT "mdio read command error status=%x\n", DBGARGS, i_control);
            return 3;
        }

        HW_EscReadWord(*pi_value, ESC_MII_PHY_DATA);
        return 0;
    }
    else
    {
        if (gi_debug & DBG_DEFAULT)
            printf(DBGFMT "mdio no access ecat=%x pdi=%x\n", DBGARGS, i_ecat_access_state, i_pdi_access_state);

        return 4;
    }
}

int mdio_write(UINT8 i_phy_addr, UINT8 i_register, UINT16 i_value)
{
    UINT8 i_ecat_access_state = 0;
    UINT8 i_pdi_access_state = 0;
    UINT16 i_control = MII_CMD_WRITE;
    int i_iteration = 0;

    HW_EscReadByte(i_ecat_access_state, ESC_MII_ECAT_ACCESS_STATE);
    HW_EscReadByte(i_pdi_access_state, ESC_MII_PDI_ACCESS_STATE);
    if (((i_ecat_access_state & 0x01) == 0x00) &&
        ((i_pdi_access_state & 0x01) == 0x00))
    {
        i_pdi_access_state = 0x01;
        HW_EscWriteByte(i_pdi_access_state, ESC_MII_PDI_ACCESS_STATE);

        HW_EscWriteByte(i_phy_addr, ESC_MII_PHY_ADDRESS);
        HW_EscWriteByte(i_register, ESC_MII_PHY_REGISTER_ADDRESS);
        HW_EscWriteWord(i_value, ESC_MII_PHY_DATA);
        HW_EscWriteWord(i_control, ESC_MII_CONTROL_STATUS);

        for (i_iteration = 0; i_iteration < 100; ++i_iteration)
        {
            HW_EscReadWord(i_control, ESC_MII_CONTROL_STATUS);
            if (i_control & MII_STATUS_BUSY)
                usleep(1000);
            else
                break;
        }

        if (i_control & MII_STATUS_BUSY)
        {
            if (gi_debug & DBG_DEFAULT)
                printf(DBGFMT "mdio read wait failed status=%x\n", DBGARGS, i_control);
            return 1;
        }
        if (i_control & (1 << 13))
        {
            if (gi_debug & DBG_DEFAULT)
                printf(DBGFMT "mdio read command failed status=%x\n", DBGARGS, i_control);
            return 2;
        }
        if (i_control & (1 << 14))
        {
            if (gi_debug & DBG_DEFAULT)
                printf(DBGFMT "mdio read command error status=%x\n", DBGARGS, i_control);
            return 3;
        }

        return 0;
    }
    else
    {
        if (gi_debug & DBG_DEFAULT)
            printf(DBGFMT "mdio no access ecat=%x pdi=%x\n", DBGARGS, i_ecat_access_state, i_pdi_access_state);

        return 4;
    }
}

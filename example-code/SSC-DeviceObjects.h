/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
*/

/**
* \addtogroup SSC-Device SSC-Device
* @{
*/

/**
\file SSC-DeviceObjects
\author ET9300Utilities.ApplicationHandler (Version 1.5.0.0) | EthercatSSC@beckhoff.com

\brief SSC-Device specific objects<br>
\brief NOTE : This file will be overwritten if a new object dictionary is generated!<br>
*/

#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
#define PROTO
#else
#define PROTO extern
#endif

//FSoe support
#include "utypes.h"
/* Application variables */
PROTO _Objects    Obj;

#if 0
/******************************************************************************
*                    Object 0x1600 : RxPDO_mapping_1600
******************************************************************************/
/**
* \addtogroup 0x1600 0x1600 | RxPDO_mapping_1600
* @{
* \brief Object 0x1600 (RxPDO_mapping_1600) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
* SubIndex 30<br>
* SubIndex 31<br>
* SubIndex 32<br>
* SubIndex 33<br>
* SubIndex 34<br>
* SubIndex 35<br>
* SubIndex 36<br>
* SubIndex 37<br>
* SubIndex 38<br>
* SubIndex 39<br>
* SubIndex 40<br>
* SubIndex 41<br>
* SubIndex 42<br>
* SubIndex 43<br>
* SubIndex 44<br>
* SubIndex 45<br>
* SubIndex 46<br>
* SubIndex 47<br>
* SubIndex 48<br>
* SubIndex 49<br>
* SubIndex 50<br>
* SubIndex 51<br>
* SubIndex 52<br>
* SubIndex 53<br>
* SubIndex 54<br>
* SubIndex 55<br>
* SubIndex 56<br>
* SubIndex 57<br>
* SubIndex 58<br>
* SubIndex 59<br>
* SubIndex 60<br>
* SubIndex 61<br>
* SubIndex 62<br>
* SubIndex 63<br>
* SubIndex 64<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1600[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex29 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex30 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex31 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex32 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex33 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex34 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex35 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex36 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex37 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex38 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex39 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex40 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex41 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex42 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex43 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex44 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex45 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex46 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex47 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex48 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex49 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex50 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex51 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex52 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex53 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex54 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex55 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex56 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex57 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex58 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex59 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex60 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex61 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex62 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex63 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }}; /* Subindex64 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1600[] = "RxPDO mapping of Output Register 1\000"
"1st receive PDO mapped\000"
"2nd receive PDO mapped\000"
"3rd receive PDO mapped\000"
"4th receive PDO mapped\000"
"5th receive PDO mapped\000"
"6th receive PDO mapped\000"
"7th receive PDO mapped\000"
"8th receive PDO mapped\000"
"9th receive PDO mapped\000"
"10th receive PDO mapped\000"
"11th receive PDO mapped\000"
"12nd receive PDO mapped\000"
"13th receive PDO mapped\000"
"14th receive PDO mapped\000"
"15th receive PDO mapped\000"
"16th receive PDO mapped\000"
"17th receive PDO mapped\000"
"18th receive PDO mapped\000"
"19th receive PDO mapped\000"
"20th receive PDO mapped\000"
"21st receive PDO mapped\000"
"22nd receive PDO mapped\000"
"23rd receive PDO mapped\000"
"24th receive PDO mapped\000"
"25th receive PDO mapped\000"
"26th receive PDO mapped\000"
"27th receive PDO mapped\000"
"28th receive PDO mapped\000"
"29th receive PDO mapped\000"
"30th receive PDO mapped\000"
"31st receive PDO mapped\000"
"32nd receive PDO mapped\000"
"33rd receive PDO mapped\000"
"34th receive PDO mapped\000"
"35th receive PDO mapped\000"
"36th receive PDO mapped\000"
"37th receive PDO mapped\000"
"38th receive PDO mapped\000"
"39th receive PDO mapped\000"
"40th receive PDO mapped\000"
"41st receive PDO mapped\000"
"42nd receive PDO mapped\000"
"43rd receive PDO mapped\000"
"44th receive PDO mapped\000"
"45th receive PDO mapped\000"
"46th receive PDO mapped\000"
"47th receive PDO mapped\000"
"48th receive PDO mapped\000"
"49th receive PDO mapped\000"
"50th receive PDO mapped\000"
"51st receive PDO mapped\000"
"52nd receive PDO mapped\000"
"53rd receive PDO mapped\000"
"54th receive PDO mapped\000"
"55th receive PDO mapped\000"
"56th receive PDO mapped\000"
"57th receive PDO mapped\000"
"58th receive PDO mapped\000"
"59th receive PDO mapped\000"
"60th receive PDO mapped\000"
"61st receive PDO mapped\000"
"62nd receive PDO mapped\000"
"63rd receive PDO mapped\000"
"64th receive PDO mapped\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */
UINT32 SI30; /* Subindex30 -  */
UINT32 SI31; /* Subindex31 -  */
UINT32 SI32; /* Subindex32 -  */
UINT32 SI33; /* Subindex33 -  */
UINT32 SI34; /* Subindex34 -  */
UINT32 SI35; /* Subindex35 -  */
UINT32 SI36; /* Subindex36 -  */
UINT32 SI37; /* Subindex37 -  */
UINT32 SI38; /* Subindex38 -  */
UINT32 SI39; /* Subindex39 -  */
UINT32 SI40; /* Subindex40 -  */
UINT32 SI41; /* Subindex41 -  */
UINT32 SI42; /* Subindex42 -  */
UINT32 SI43; /* Subindex43 -  */
UINT32 SI44; /* Subindex44 -  */
UINT32 SI45; /* Subindex45 -  */
UINT32 SI46; /* Subindex46 -  */
UINT32 SI47; /* Subindex47 -  */
UINT32 SI48; /* Subindex48 -  */
UINT32 SI49; /* Subindex49 -  */
UINT32 SI50; /* Subindex50 -  */
UINT32 SI51; /* Subindex51 -  */
UINT32 SI52; /* Subindex52 -  */
UINT32 SI53; /* Subindex53 -  */
UINT32 SI54; /* Subindex54 -  */
UINT32 SI55; /* Subindex55 -  */
UINT32 SI56; /* Subindex56 -  */
UINT32 SI57; /* Subindex57 -  */
UINT32 SI58; /* Subindex58 -  */
UINT32 SI59; /* Subindex59 -  */
UINT32 SI60; /* Subindex60 -  */
UINT32 SI61; /* Subindex61 -  */
UINT32 SI62; /* Subindex62 -  */
UINT32 SI63; /* Subindex63 -  */
UINT32 SI64; /* Subindex64 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1600;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1600 RxPDO_mapping_16000x1600
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/
#endif

/******************************************************************************
*                    Object 0x1600 : RxPDO_mapping_1600
******************************************************************************/
/**
* \addtogroup 0x1600 0x1600 | RxPDO_mapping_1600
* @{
* \brief Object 0x1600 (RxPDO_mapping_1600) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1600[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }}; /* Subindex18 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1600[] = "RxPDO mapping of FSoE Outputs\000"
"Command\000"
"SafeOut1\000"
"SafeOut2\000"
"SafeOut3\000"
"SafeOut4\000"
"SafeOut5\000"
"SafeOut6\000"
"SafeOut7\000"
"SafeOut8\000"
"SafeOut9\000"
"SafeOut10\000"
"SafeOut11\000"
"SafeOut12\000"
"SafeOutDummy\000"
"CRC0\000"
"SafeOutput2\000"
"CRC1\000"
"Connection ID\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1600;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1600 RxPDO_mapping_16000x1600
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={18,0x70000108,0x70010101,0x70010201,0x70010301,0x70010401,0x70010501,0x70010601,0x70010701,0x70010801,0x70010901,0x70010A01,0x70010B01,0x70010C01,0x00000004,0x70000310,0x70010D10,0x70000410,0x70000210}
#endif
;
/** @}*/


/******************************************************************************
*                    Object 0x1600 : RxPDO_mapping_1600 for 3057 plugin
******************************************************************************/
/**
* \addtogroup 0x1600 0x1600 | RxPDO_mapping_1600
* @{
* \brief Object 0x1600 (RxPDO_mapping_1600) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1600_5[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }}; /* Subindex14 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1600_5[] = "RxPDO mapping of FSoE Outputs\000"
"Command\000"
"SafeOut1\000"
"SafeOut2\000"
"SafeOut3\000"
"SafeOut4\000"
"SafeOutDummy1\000"
"CRC0\000"
"SafeOut5\000"
"SafeOut6\000"
"SafeOut7\000"
"SafeOut8\000"
"SafeOutDummy2\000"
"CRC1\000"
"Connection ID\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1600_5;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1600_5 RxPDO_mapping_16000x1600_5
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={14,0x70000108,0x70010101,0x70010201,0x70010301,0x70010401,0x0000000C,0x70000310,0x70010501,0x70010601,0x70010701,0x70010801,0x0000000C,0x70000410,0x70000210}
#endif
;
/** @}*/





/******************************************************************************
*                    Object 0x1601 : RxPDO_mapping_1601
******************************************************************************/
/**
* \addtogroup 0x1601 0x1601 | RxPDO_mapping_1601
* @{
* \brief Object 0x1601 (RxPDO_mapping_1601) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
* SubIndex 30<br>
* SubIndex 31<br>
* SubIndex 32<br>
* SubIndex 33<br>
* SubIndex 34<br>
* SubIndex 35<br>
* SubIndex 36<br>
* SubIndex 37<br>
* SubIndex 38<br>
* SubIndex 39<br>
* SubIndex 40<br>
* SubIndex 41<br>
* SubIndex 42<br>
* SubIndex 43<br>
* SubIndex 44<br>
* SubIndex 45<br>
* SubIndex 46<br>
* SubIndex 47<br>
* SubIndex 48<br>
* SubIndex 49<br>
* SubIndex 50<br>
* SubIndex 51<br>
* SubIndex 52<br>
* SubIndex 53<br>
* SubIndex 54<br>
* SubIndex 55<br>
* SubIndex 56<br>
* SubIndex 57<br>
* SubIndex 58<br>
* SubIndex 59<br>
* SubIndex 60<br>
* SubIndex 61<br>
* SubIndex 62<br>
* SubIndex 63<br>
* SubIndex 64<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1601[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex29 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex30 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex31 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex32 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex33 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex34 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex35 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex36 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex37 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex38 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex39 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex40 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex41 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex42 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex43 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex44 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex45 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex46 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex47 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex48 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex49 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex50 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex51 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex52 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex53 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex54 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex55 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex56 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex57 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex58 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex59 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex60 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex61 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex62 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex63 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }}; /* Subindex64 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1601[] = "RxPDO Mapping of Output Register 1\000"
		"1st receive PDO mapped\000"
		"2nd receive PDO mapped\000"
		"3rd receive PDO mapped\000"
		"4th receive PDO mapped\000"
		"5th receive PDO mapped\000"
		"6th receive PDO mapped\000"
		"7th receive PDO mapped\000"
		"8th receive PDO mapped\000"
		"9th receive PDO mapped\000"
		"10th receive PDO mapped\000"
		"11th receive PDO mapped\000"
		"12nd receive PDO mapped\000"
		"13th receive PDO mapped\000"
		"14th receive PDO mapped\000"
		"15th receive PDO mapped\000"
		"16th receive PDO mapped\000"
		"17th receive PDO mapped\000"
		"18th receive PDO mapped\000"
		"19th receive PDO mapped\000"
		"20th receive PDO mapped\000"
		"21st receive PDO mapped\000"
		"22nd receive PDO mapped\000"
		"23rd receive PDO mapped\000"
		"24th receive PDO mapped\000"
		"25th receive PDO mapped\000"
		"26th receive PDO mapped\000"
		"27th receive PDO mapped\000"
		"28th receive PDO mapped\000"
		"29th receive PDO mapped\000"
		"30th receive PDO mapped\000"
		"31st receive PDO mapped\000"
		"32nd receive PDO mapped\000"
		"33rd receive PDO mapped\000"
		"34th receive PDO mapped\000"
		"35th receive PDO mapped\000"
		"36th receive PDO mapped\000"
		"37th receive PDO mapped\000"
		"38th receive PDO mapped\000"
		"39th receive PDO mapped\000"
		"40th receive PDO mapped\000"
		"41st receive PDO mapped\000"
		"42nd receive PDO mapped\000"
		"43rd receive PDO mapped\000"
		"44th receive PDO mapped\000"
		"45th receive PDO mapped\000"
		"46th receive PDO mapped\000"
		"47th receive PDO mapped\000"
		"48th receive PDO mapped\000"
		"49th receive PDO mapped\000"
		"50th receive PDO mapped\000"
		"51st receive PDO mapped\000"
		"52nd receive PDO mapped\000"
		"53rd receive PDO mapped\000"
		"54th receive PDO mapped\000"
		"55th receive PDO mapped\000"
		"56th receive PDO mapped\000"
		"57th receive PDO mapped\000"
		"58th receive PDO mapped\000"
		"59th receive PDO mapped\000"
		"60th receive PDO mapped\000"
		"61st receive PDO mapped\000"
		"62nd receive PDO mapped\000"
		"63rd receive PDO mapped\000"
		"64th receive PDO mapped\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */
UINT32 SI30; /* Subindex30 -  */
UINT32 SI31; /* Subindex31 -  */
UINT32 SI32; /* Subindex32 -  */
UINT32 SI33; /* Subindex33 -  */
UINT32 SI34; /* Subindex34 -  */
UINT32 SI35; /* Subindex35 -  */
UINT32 SI36; /* Subindex36 -  */
UINT32 SI37; /* Subindex37 -  */
UINT32 SI38; /* Subindex38 -  */
UINT32 SI39; /* Subindex39 -  */
UINT32 SI40; /* Subindex40 -  */
UINT32 SI41; /* Subindex41 -  */
UINT32 SI42; /* Subindex42 -  */
UINT32 SI43; /* Subindex43 -  */
UINT32 SI44; /* Subindex44 -  */
UINT32 SI45; /* Subindex45 -  */
UINT32 SI46; /* Subindex46 -  */
UINT32 SI47; /* Subindex47 -  */
UINT32 SI48; /* Subindex48 -  */
UINT32 SI49; /* Subindex49 -  */
UINT32 SI50; /* Subindex50 -  */
UINT32 SI51; /* Subindex51 -  */
UINT32 SI52; /* Subindex52 -  */
UINT32 SI53; /* Subindex53 -  */
UINT32 SI54; /* Subindex54 -  */
UINT32 SI55; /* Subindex55 -  */
UINT32 SI56; /* Subindex56 -  */
UINT32 SI57; /* Subindex57 -  */
UINT32 SI58; /* Subindex58 -  */
UINT32 SI59; /* Subindex59 -  */
UINT32 SI60; /* Subindex60 -  */
UINT32 SI61; /* Subindex61 -  */
UINT32 SI62; /* Subindex62 -  */
UINT32 SI63; /* Subindex63 -  */
UINT32 SI64; /* Subindex64 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1601;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1601 RxPDO_mapping_16010x1601
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1602 : RxPDO_mapping_1602
******************************************************************************/
/**
* \addtogroup 0x1602 0x1602 | RxPDO_mapping_1602
* @{
* \brief Object 0x1602 (RxPDO_mapping_1602) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
* SubIndex 30<br>
* SubIndex 31<br>
* SubIndex 32<br>
* SubIndex 33<br>
* SubIndex 34<br>
* SubIndex 35<br>
* SubIndex 36<br>
* SubIndex 37<br>
* SubIndex 38<br>
* SubIndex 39<br>
* SubIndex 40<br>
* SubIndex 41<br>
* SubIndex 42<br>
* SubIndex 43<br>
* SubIndex 44<br>
* SubIndex 45<br>
* SubIndex 46<br>
* SubIndex 47<br>
* SubIndex 48<br>
* SubIndex 49<br>
* SubIndex 50<br>
* SubIndex 51<br>
* SubIndex 52<br>
* SubIndex 53<br>
* SubIndex 54<br>
* SubIndex 55<br>
* SubIndex 56<br>
* SubIndex 57<br>
* SubIndex 58<br>
* SubIndex 59<br>
* SubIndex 60<br>
* SubIndex 61<br>
* SubIndex 62<br>
* SubIndex 63<br>
* SubIndex 64<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1602[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex29 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex30 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex31 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex32 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex33 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex34 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex35 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex36 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex37 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex38 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex39 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex40 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex41 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex42 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex43 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex44 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex45 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex46 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex47 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex48 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex49 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex50 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex51 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex52 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex53 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex54 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex55 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex56 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex57 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex58 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex59 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex60 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex61 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex62 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex63 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }}; /* Subindex64 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1602[] = "RxPDO Mapping of Output Register 2\000"
		"1st receive PDO mapped\000"
		"2nd receive PDO mapped\000"
		"3rd receive PDO mapped\000"
		"4th receive PDO mapped\000"
		"5th receive PDO mapped\000"
		"6th receive PDO mapped\000"
		"7th receive PDO mapped\000"
		"8th receive PDO mapped\000"
		"9th receive PDO mapped\000"
		"10th receive PDO mapped\000"
		"11th receive PDO mapped\000"
		"12nd receive PDO mapped\000"
		"13th receive PDO mapped\000"
		"14th receive PDO mapped\000"
		"15th receive PDO mapped\000"
		"16th receive PDO mapped\000"
		"17th receive PDO mapped\000"
		"18th receive PDO mapped\000"
		"19th receive PDO mapped\000"
		"20th receive PDO mapped\000"
		"21st receive PDO mapped\000"
		"22nd receive PDO mapped\000"
		"23rd receive PDO mapped\000"
		"24th receive PDO mapped\000"
		"25th receive PDO mapped\000"
		"26th receive PDO mapped\000"
		"27th receive PDO mapped\000"
		"28th receive PDO mapped\000"
		"29th receive PDO mapped\000"
		"30th receive PDO mapped\000"
		"31st receive PDO mapped\000"
		"32nd receive PDO mapped\000"
		"33rd receive PDO mapped\000"
		"34th receive PDO mapped\000"
		"35th receive PDO mapped\000"
		"36th receive PDO mapped\000"
		"37th receive PDO mapped\000"
		"38th receive PDO mapped\000"
		"39th receive PDO mapped\000"
		"40th receive PDO mapped\000"
		"41st receive PDO mapped\000"
		"42nd receive PDO mapped\000"
		"43rd receive PDO mapped\000"
		"44th receive PDO mapped\000"
		"45th receive PDO mapped\000"
		"46th receive PDO mapped\000"
		"47th receive PDO mapped\000"
		"48th receive PDO mapped\000"
		"49th receive PDO mapped\000"
		"50th receive PDO mapped\000"
		"51st receive PDO mapped\000"
		"52nd receive PDO mapped\000"
		"53rd receive PDO mapped\000"
		"54th receive PDO mapped\000"
		"55th receive PDO mapped\000"
		"56th receive PDO mapped\000"
		"57th receive PDO mapped\000"
		"58th receive PDO mapped\000"
		"59th receive PDO mapped\000"
		"60th receive PDO mapped\000"
		"61st receive PDO mapped\000"
		"62nd receive PDO mapped\000"
		"63rd receive PDO mapped\000"
		"64th receive PDO mapped\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */
UINT32 SI30; /* Subindex30 -  */
UINT32 SI31; /* Subindex31 -  */
UINT32 SI32; /* Subindex32 -  */
UINT32 SI33; /* Subindex33 -  */
UINT32 SI34; /* Subindex34 -  */
UINT32 SI35; /* Subindex35 -  */
UINT32 SI36; /* Subindex36 -  */
UINT32 SI37; /* Subindex37 -  */
UINT32 SI38; /* Subindex38 -  */
UINT32 SI39; /* Subindex39 -  */
UINT32 SI40; /* Subindex40 -  */
UINT32 SI41; /* Subindex41 -  */
UINT32 SI42; /* Subindex42 -  */
UINT32 SI43; /* Subindex43 -  */
UINT32 SI44; /* Subindex44 -  */
UINT32 SI45; /* Subindex45 -  */
UINT32 SI46; /* Subindex46 -  */
UINT32 SI47; /* Subindex47 -  */
UINT32 SI48; /* Subindex48 -  */
UINT32 SI49; /* Subindex49 -  */
UINT32 SI50; /* Subindex50 -  */
UINT32 SI51; /* Subindex51 -  */
UINT32 SI52; /* Subindex52 -  */
UINT32 SI53; /* Subindex53 -  */
UINT32 SI54; /* Subindex54 -  */
UINT32 SI55; /* Subindex55 -  */
UINT32 SI56; /* Subindex56 -  */
UINT32 SI57; /* Subindex57 -  */
UINT32 SI58; /* Subindex58 -  */
UINT32 SI59; /* Subindex59 -  */
UINT32 SI60; /* Subindex60 -  */
UINT32 SI61; /* Subindex61 -  */
UINT32 SI62; /* Subindex62 -  */
UINT32 SI63; /* Subindex63 -  */
UINT32 SI64; /* Subindex64 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1602;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1602 RxPDO_mapping_16020x1602
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1603 : RxPDO_mapping_1603
******************************************************************************/
/**
* \addtogroup 0x1603 0x1603 | RxPDO_mapping_1603
* @{
* \brief Object 0x1603 (RxPDO_mapping_1603) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
* SubIndex 30<br>
* SubIndex 31<br>
* SubIndex 32<br>
* SubIndex 33<br>
* SubIndex 34<br>
* SubIndex 35<br>
* SubIndex 36<br>
* SubIndex 37<br>
* SubIndex 38<br>
* SubIndex 39<br>
* SubIndex 40<br>
* SubIndex 41<br>
* SubIndex 42<br>
* SubIndex 43<br>
* SubIndex 44<br>
* SubIndex 45<br>
* SubIndex 46<br>
* SubIndex 47<br>
* SubIndex 48<br>
* SubIndex 49<br>
* SubIndex 50<br>
* SubIndex 51<br>
* SubIndex 52<br>
* SubIndex 53<br>
* SubIndex 54<br>
* SubIndex 55<br>
* SubIndex 56<br>
* SubIndex 57<br>
* SubIndex 58<br>
* SubIndex 59<br>
* SubIndex 60<br>
* SubIndex 61<br>
* SubIndex 62<br>
* SubIndex 63<br>
* SubIndex 64<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1603[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex29 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex30 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex31 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex32 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex33 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex34 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex35 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex36 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex37 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex38 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex39 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex40 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex41 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex42 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex43 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex44 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex45 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex46 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex47 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex48 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex49 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex50 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex51 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex52 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex53 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex54 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex55 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex56 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex57 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex58 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex59 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex60 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex61 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex62 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex63 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }}; /* Subindex64 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1603[] = "RxPDO Mapping of Output Register 3\000"
		"1st receive PDO mapped\000"
		"2nd receive PDO mapped\000"
		"3rd receive PDO mapped\000"
		"4th receive PDO mapped\000"
		"5th receive PDO mapped\000"
		"6th receive PDO mapped\000"
		"7th receive PDO mapped\000"
		"8th receive PDO mapped\000"
		"9th receive PDO mapped\000"
		"10th receive PDO mapped\000"
		"11th receive PDO mapped\000"
		"12nd receive PDO mapped\000"
		"13th receive PDO mapped\000"
		"14th receive PDO mapped\000"
		"15th receive PDO mapped\000"
		"16th receive PDO mapped\000"
		"17th receive PDO mapped\000"
		"18th receive PDO mapped\000"
		"19th receive PDO mapped\000"
		"20th receive PDO mapped\000"
		"21st receive PDO mapped\000"
		"22nd receive PDO mapped\000"
		"23rd receive PDO mapped\000"
		"24th receive PDO mapped\000"
		"25th receive PDO mapped\000"
		"26th receive PDO mapped\000"
		"27th receive PDO mapped\000"
		"28th receive PDO mapped\000"
		"29th receive PDO mapped\000"
		"30th receive PDO mapped\000"
		"31st receive PDO mapped\000"
		"32nd receive PDO mapped\000"
		"33rd receive PDO mapped\000"
		"34th receive PDO mapped\000"
		"35th receive PDO mapped\000"
		"36th receive PDO mapped\000"
		"37th receive PDO mapped\000"
		"38th receive PDO mapped\000"
		"39th receive PDO mapped\000"
		"40th receive PDO mapped\000"
		"41st receive PDO mapped\000"
		"42nd receive PDO mapped\000"
		"43rd receive PDO mapped\000"
		"44th receive PDO mapped\000"
		"45th receive PDO mapped\000"
		"46th receive PDO mapped\000"
		"47th receive PDO mapped\000"
		"48th receive PDO mapped\000"
		"49th receive PDO mapped\000"
		"50th receive PDO mapped\000"
		"51st receive PDO mapped\000"
		"52nd receive PDO mapped\000"
		"53rd receive PDO mapped\000"
		"54th receive PDO mapped\000"
		"55th receive PDO mapped\000"
		"56th receive PDO mapped\000"
		"57th receive PDO mapped\000"
		"58th receive PDO mapped\000"
		"59th receive PDO mapped\000"
		"60th receive PDO mapped\000"
		"61st receive PDO mapped\000"
		"62nd receive PDO mapped\000"
		"63rd receive PDO mapped\000"
		"64th receive PDO mapped\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */
UINT32 SI30; /* Subindex30 -  */
UINT32 SI31; /* Subindex31 -  */
UINT32 SI32; /* Subindex32 -  */
UINT32 SI33; /* Subindex33 -  */
UINT32 SI34; /* Subindex34 -  */
UINT32 SI35; /* Subindex35 -  */
UINT32 SI36; /* Subindex36 -  */
UINT32 SI37; /* Subindex37 -  */
UINT32 SI38; /* Subindex38 -  */
UINT32 SI39; /* Subindex39 -  */
UINT32 SI40; /* Subindex40 -  */
UINT32 SI41; /* Subindex41 -  */
UINT32 SI42; /* Subindex42 -  */
UINT32 SI43; /* Subindex43 -  */
UINT32 SI44; /* Subindex44 -  */
UINT32 SI45; /* Subindex45 -  */
UINT32 SI46; /* Subindex46 -  */
UINT32 SI47; /* Subindex47 -  */
UINT32 SI48; /* Subindex48 -  */
UINT32 SI49; /* Subindex49 -  */
UINT32 SI50; /* Subindex50 -  */
UINT32 SI51; /* Subindex51 -  */
UINT32 SI52; /* Subindex52 -  */
UINT32 SI53; /* Subindex53 -  */
UINT32 SI54; /* Subindex54 -  */
UINT32 SI55; /* Subindex55 -  */
UINT32 SI56; /* Subindex56 -  */
UINT32 SI57; /* Subindex57 -  */
UINT32 SI58; /* Subindex58 -  */
UINT32 SI59; /* Subindex59 -  */
UINT32 SI60; /* Subindex60 -  */
UINT32 SI61; /* Subindex61 -  */
UINT32 SI62; /* Subindex62 -  */
UINT32 SI63; /* Subindex63 -  */
UINT32 SI64; /* Subindex64 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1603;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1603 RxPDO_mapping_16030x1603
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1604 : RxPDO_mapping_1604
******************************************************************************/
/**
* \addtogroup 0x1604 0x1604 | RxPDO_mapping_1604
* @{
* \brief Object 0x1604 (RxPDO_mapping_1604) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
* SubIndex 30<br>
* SubIndex 31<br>
* SubIndex 32<br>
* SubIndex 33<br>
* SubIndex 34<br>
* SubIndex 35<br>
* SubIndex 36<br>
* SubIndex 37<br>
* SubIndex 38<br>
* SubIndex 39<br>
* SubIndex 40<br>
* SubIndex 41<br>
* SubIndex 42<br>
* SubIndex 43<br>
* SubIndex 44<br>
* SubIndex 45<br>
* SubIndex 46<br>
* SubIndex 47<br>
* SubIndex 48<br>
* SubIndex 49<br>
* SubIndex 50<br>
* SubIndex 51<br>
* SubIndex 52<br>
* SubIndex 53<br>
* SubIndex 54<br>
* SubIndex 55<br>
* SubIndex 56<br>
* SubIndex 57<br>
* SubIndex 58<br>
* SubIndex 59<br>
* SubIndex 60<br>
* SubIndex 61<br>
* SubIndex 62<br>
* SubIndex 63<br>
* SubIndex 64<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1604[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex29 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex30 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex31 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex32 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex33 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex34 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex35 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex36 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex37 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex38 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex39 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex40 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex41 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex42 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex43 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex44 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex45 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex46 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex47 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex48 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex49 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex50 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex51 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex52 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex53 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex54 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex55 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex56 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex57 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex58 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex59 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex60 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex61 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex62 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex63 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }}; /* Subindex64 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1604[] = "RxPDO Mapping of Output Register 4\000"
		"1st receive PDO mapped\000"
		"2nd receive PDO mapped\000"
		"3rd receive PDO mapped\000"
		"4th receive PDO mapped\000"
		"5th receive PDO mapped\000"
		"6th receive PDO mapped\000"
		"7th receive PDO mapped\000"
		"8th receive PDO mapped\000"
		"9th receive PDO mapped\000"
		"10th receive PDO mapped\000"
		"11th receive PDO mapped\000"
		"12nd receive PDO mapped\000"
		"13th receive PDO mapped\000"
		"14th receive PDO mapped\000"
		"15th receive PDO mapped\000"
		"16th receive PDO mapped\000"
		"17th receive PDO mapped\000"
		"18th receive PDO mapped\000"
		"19th receive PDO mapped\000"
		"20th receive PDO mapped\000"
		"21st receive PDO mapped\000"
		"22nd receive PDO mapped\000"
		"23rd receive PDO mapped\000"
		"24th receive PDO mapped\000"
		"25th receive PDO mapped\000"
		"26th receive PDO mapped\000"
		"27th receive PDO mapped\000"
		"28th receive PDO mapped\000"
		"29th receive PDO mapped\000"
		"30th receive PDO mapped\000"
		"31st receive PDO mapped\000"
		"32nd receive PDO mapped\000"
		"33rd receive PDO mapped\000"
		"34th receive PDO mapped\000"
		"35th receive PDO mapped\000"
		"36th receive PDO mapped\000"
		"37th receive PDO mapped\000"
		"38th receive PDO mapped\000"
		"39th receive PDO mapped\000"
		"40th receive PDO mapped\000"
		"41st receive PDO mapped\000"
		"42nd receive PDO mapped\000"
		"43rd receive PDO mapped\000"
		"44th receive PDO mapped\000"
		"45th receive PDO mapped\000"
		"46th receive PDO mapped\000"
		"47th receive PDO mapped\000"
		"48th receive PDO mapped\000"
		"49th receive PDO mapped\000"
		"50th receive PDO mapped\000"
		"51st receive PDO mapped\000"
		"52nd receive PDO mapped\000"
		"53rd receive PDO mapped\000"
		"54th receive PDO mapped\000"
		"55th receive PDO mapped\000"
		"56th receive PDO mapped\000"
		"57th receive PDO mapped\000"
		"58th receive PDO mapped\000"
		"59th receive PDO mapped\000"
		"60th receive PDO mapped\000"
		"61st receive PDO mapped\000"
		"62nd receive PDO mapped\000"
		"63rd receive PDO mapped\000"
		"64th receive PDO mapped\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */
UINT32 SI30; /* Subindex30 -  */
UINT32 SI31; /* Subindex31 -  */
UINT32 SI32; /* Subindex32 -  */
UINT32 SI33; /* Subindex33 -  */
UINT32 SI34; /* Subindex34 -  */
UINT32 SI35; /* Subindex35 -  */
UINT32 SI36; /* Subindex36 -  */
UINT32 SI37; /* Subindex37 -  */
UINT32 SI38; /* Subindex38 -  */
UINT32 SI39; /* Subindex39 -  */
UINT32 SI40; /* Subindex40 -  */
UINT32 SI41; /* Subindex41 -  */
UINT32 SI42; /* Subindex42 -  */
UINT32 SI43; /* Subindex43 -  */
UINT32 SI44; /* Subindex44 -  */
UINT32 SI45; /* Subindex45 -  */
UINT32 SI46; /* Subindex46 -  */
UINT32 SI47; /* Subindex47 -  */
UINT32 SI48; /* Subindex48 -  */
UINT32 SI49; /* Subindex49 -  */
UINT32 SI50; /* Subindex50 -  */
UINT32 SI51; /* Subindex51 -  */
UINT32 SI52; /* Subindex52 -  */
UINT32 SI53; /* Subindex53 -  */
UINT32 SI54; /* Subindex54 -  */
UINT32 SI55; /* Subindex55 -  */
UINT32 SI56; /* Subindex56 -  */
UINT32 SI57; /* Subindex57 -  */
UINT32 SI58; /* Subindex58 -  */
UINT32 SI59; /* Subindex59 -  */
UINT32 SI60; /* Subindex60 -  */
UINT32 SI61; /* Subindex61 -  */
UINT32 SI62; /* Subindex62 -  */
UINT32 SI63; /* Subindex63 -  */
UINT32 SI64; /* Subindex64 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1604;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1604 RxPDO_mapping_16040x1604
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1605 : RxPDO_mapping_1605
******************************************************************************/
/**
* \addtogroup 0x1605 0x1605 | RxPDO_mapping_1605
* @{
* \brief Object 0x1605 (RxPDO_mapping_1605) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
* SubIndex 30<br>
* SubIndex 31<br>
* SubIndex 32<br>
* SubIndex 33<br>
* SubIndex 34<br>
* SubIndex 35<br>
* SubIndex 36<br>
* SubIndex 37<br>
* SubIndex 38<br>
* SubIndex 39<br>
* SubIndex 40<br>
* SubIndex 41<br>
* SubIndex 42<br>
* SubIndex 43<br>
* SubIndex 44<br>
* SubIndex 45<br>
* SubIndex 46<br>
* SubIndex 47<br>
* SubIndex 48<br>
* SubIndex 49<br>
* SubIndex 50<br>
* SubIndex 51<br>
* SubIndex 52<br>
* SubIndex 53<br>
* SubIndex 54<br>
* SubIndex 55<br>
* SubIndex 56<br>
* SubIndex 57<br>
* SubIndex 58<br>
* SubIndex 59<br>
* SubIndex 60<br>
* SubIndex 61<br>
* SubIndex 62<br>
* SubIndex 63<br>
* SubIndex 64<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1605[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex29 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex30 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex31 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex32 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex33 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex34 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex35 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex36 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex37 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex38 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex39 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex40 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex41 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex42 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex43 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex44 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex45 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex46 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex47 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex48 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex49 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex50 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex51 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex52 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex53 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex54 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex55 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex56 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex57 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex58 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex59 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex60 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex61 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex62 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex63 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }}; /* Subindex64 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1605[] = "RxPDO Mapping of Output Register 5\000"
		"1st receive PDO mapped\000"
		"2nd receive PDO mapped\000"
		"3rd receive PDO mapped\000"
		"4th receive PDO mapped\000"
		"5th receive PDO mapped\000"
		"6th receive PDO mapped\000"
		"7th receive PDO mapped\000"
		"8th receive PDO mapped\000"
		"9th receive PDO mapped\000"
		"10th receive PDO mapped\000"
		"11th receive PDO mapped\000"
		"12nd receive PDO mapped\000"
		"13th receive PDO mapped\000"
		"14th receive PDO mapped\000"
		"15th receive PDO mapped\000"
		"16th receive PDO mapped\000"
		"17th receive PDO mapped\000"
		"18th receive PDO mapped\000"
		"19th receive PDO mapped\000"
		"20th receive PDO mapped\000"
		"21st receive PDO mapped\000"
		"22nd receive PDO mapped\000"
		"23rd receive PDO mapped\000"
		"24th receive PDO mapped\000"
		"25th receive PDO mapped\000"
		"26th receive PDO mapped\000"
		"27th receive PDO mapped\000"
		"28th receive PDO mapped\000"
		"29th receive PDO mapped\000"
		"30th receive PDO mapped\000"
		"31st receive PDO mapped\000"
		"32nd receive PDO mapped\000"
		"33rd receive PDO mapped\000"
		"34th receive PDO mapped\000"
		"35th receive PDO mapped\000"
		"36th receive PDO mapped\000"
		"37th receive PDO mapped\000"
		"38th receive PDO mapped\000"
		"39th receive PDO mapped\000"
		"40th receive PDO mapped\000"
		"41st receive PDO mapped\000"
		"42nd receive PDO mapped\000"
		"43rd receive PDO mapped\000"
		"44th receive PDO mapped\000"
		"45th receive PDO mapped\000"
		"46th receive PDO mapped\000"
		"47th receive PDO mapped\000"
		"48th receive PDO mapped\000"
		"49th receive PDO mapped\000"
		"50th receive PDO mapped\000"
		"51st receive PDO mapped\000"
		"52nd receive PDO mapped\000"
		"53rd receive PDO mapped\000"
		"54th receive PDO mapped\000"
		"55th receive PDO mapped\000"
		"56th receive PDO mapped\000"
		"57th receive PDO mapped\000"
		"58th receive PDO mapped\000"
		"59th receive PDO mapped\000"
		"60th receive PDO mapped\000"
		"61st receive PDO mapped\000"
		"62nd receive PDO mapped\000"
		"63rd receive PDO mapped\000"
		"64th receive PDO mapped\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */
UINT32 SI30; /* Subindex30 -  */
UINT32 SI31; /* Subindex31 -  */
UINT32 SI32; /* Subindex32 -  */
UINT32 SI33; /* Subindex33 -  */
UINT32 SI34; /* Subindex34 -  */
UINT32 SI35; /* Subindex35 -  */
UINT32 SI36; /* Subindex36 -  */
UINT32 SI37; /* Subindex37 -  */
UINT32 SI38; /* Subindex38 -  */
UINT32 SI39; /* Subindex39 -  */
UINT32 SI40; /* Subindex40 -  */
UINT32 SI41; /* Subindex41 -  */
UINT32 SI42; /* Subindex42 -  */
UINT32 SI43; /* Subindex43 -  */
UINT32 SI44; /* Subindex44 -  */
UINT32 SI45; /* Subindex45 -  */
UINT32 SI46; /* Subindex46 -  */
UINT32 SI47; /* Subindex47 -  */
UINT32 SI48; /* Subindex48 -  */
UINT32 SI49; /* Subindex49 -  */
UINT32 SI50; /* Subindex50 -  */
UINT32 SI51; /* Subindex51 -  */
UINT32 SI52; /* Subindex52 -  */
UINT32 SI53; /* Subindex53 -  */
UINT32 SI54; /* Subindex54 -  */
UINT32 SI55; /* Subindex55 -  */
UINT32 SI56; /* Subindex56 -  */
UINT32 SI57; /* Subindex57 -  */
UINT32 SI58; /* Subindex58 -  */
UINT32 SI59; /* Subindex59 -  */
UINT32 SI60; /* Subindex60 -  */
UINT32 SI61; /* Subindex61 -  */
UINT32 SI62; /* Subindex62 -  */
UINT32 SI63; /* Subindex63 -  */
UINT32 SI64; /* Subindex64 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1605;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1605 RxPDO_mapping_16050x1605
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1606 : RxPDO_mapping_1606
******************************************************************************/
/**
* \addtogroup 0x1606 0x1606 | RxPDO_mapping_1606
* @{
* \brief Object 0x1606 (RxPDO_mapping_1606) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
* SubIndex 30<br>
* SubIndex 31<br>
* SubIndex 32<br>
* SubIndex 33<br>
* SubIndex 34<br>
* SubIndex 35<br>
* SubIndex 36<br>
* SubIndex 37<br>
* SubIndex 38<br>
* SubIndex 39<br>
* SubIndex 40<br>
* SubIndex 41<br>
* SubIndex 42<br>
* SubIndex 43<br>
* SubIndex 44<br>
* SubIndex 45<br>
* SubIndex 46<br>
* SubIndex 47<br>
* SubIndex 48<br>
* SubIndex 49<br>
* SubIndex 50<br>
* SubIndex 51<br>
* SubIndex 52<br>
* SubIndex 53<br>
* SubIndex 54<br>
* SubIndex 55<br>
* SubIndex 56<br>
* SubIndex 57<br>
* SubIndex 58<br>
* SubIndex 59<br>
* SubIndex 60<br>
* SubIndex 61<br>
* SubIndex 62<br>
* SubIndex 63<br>
* SubIndex 64<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1606[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex29 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex30 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex31 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex32 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex33 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex34 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex35 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex36 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex37 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex38 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex39 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex40 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex41 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex42 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex43 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex44 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex45 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex46 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex47 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex48 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex49 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex50 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex51 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex52 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex53 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex54 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex55 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex56 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex57 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex58 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex59 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex60 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex61 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex62 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex63 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }}; /* Subindex64 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1606[] = "RxPDO Mapping of Output Register 6\000"
		"1st receive PDO mapped\000"
		"2nd receive PDO mapped\000"
		"3rd receive PDO mapped\000"
		"4th receive PDO mapped\000"
		"5th receive PDO mapped\000"
		"6th receive PDO mapped\000"
		"7th receive PDO mapped\000"
		"8th receive PDO mapped\000"
		"9th receive PDO mapped\000"
		"10th receive PDO mapped\000"
		"11th receive PDO mapped\000"
		"12nd receive PDO mapped\000"
		"13th receive PDO mapped\000"
		"14th receive PDO mapped\000"
		"15th receive PDO mapped\000"
		"16th receive PDO mapped\000"
		"17th receive PDO mapped\000"
		"18th receive PDO mapped\000"
		"19th receive PDO mapped\000"
		"20th receive PDO mapped\000"
		"21st receive PDO mapped\000"
		"22nd receive PDO mapped\000"
		"23rd receive PDO mapped\000"
		"24th receive PDO mapped\000"
		"25th receive PDO mapped\000"
		"26th receive PDO mapped\000"
		"27th receive PDO mapped\000"
		"28th receive PDO mapped\000"
		"29th receive PDO mapped\000"
		"30th receive PDO mapped\000"
		"31st receive PDO mapped\000"
		"32nd receive PDO mapped\000"
		"33rd receive PDO mapped\000"
		"34th receive PDO mapped\000"
		"35th receive PDO mapped\000"
		"36th receive PDO mapped\000"
		"37th receive PDO mapped\000"
		"38th receive PDO mapped\000"
		"39th receive PDO mapped\000"
		"40th receive PDO mapped\000"
		"41st receive PDO mapped\000"
		"42nd receive PDO mapped\000"
		"43rd receive PDO mapped\000"
		"44th receive PDO mapped\000"
		"45th receive PDO mapped\000"
		"46th receive PDO mapped\000"
		"47th receive PDO mapped\000"
		"48th receive PDO mapped\000"
		"49th receive PDO mapped\000"
		"50th receive PDO mapped\000"
		"51st receive PDO mapped\000"
		"52nd receive PDO mapped\000"
		"53rd receive PDO mapped\000"
		"54th receive PDO mapped\000"
		"55th receive PDO mapped\000"
		"56th receive PDO mapped\000"
		"57th receive PDO mapped\000"
		"58th receive PDO mapped\000"
		"59th receive PDO mapped\000"
		"60th receive PDO mapped\000"
		"61st receive PDO mapped\000"
		"62nd receive PDO mapped\000"
		"63rd receive PDO mapped\000"
		"64th receive PDO mapped\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */
UINT32 SI30; /* Subindex30 -  */
UINT32 SI31; /* Subindex31 -  */
UINT32 SI32; /* Subindex32 -  */
UINT32 SI33; /* Subindex33 -  */
UINT32 SI34; /* Subindex34 -  */
UINT32 SI35; /* Subindex35 -  */
UINT32 SI36; /* Subindex36 -  */
UINT32 SI37; /* Subindex37 -  */
UINT32 SI38; /* Subindex38 -  */
UINT32 SI39; /* Subindex39 -  */
UINT32 SI40; /* Subindex40 -  */
UINT32 SI41; /* Subindex41 -  */
UINT32 SI42; /* Subindex42 -  */
UINT32 SI43; /* Subindex43 -  */
UINT32 SI44; /* Subindex44 -  */
UINT32 SI45; /* Subindex45 -  */
UINT32 SI46; /* Subindex46 -  */
UINT32 SI47; /* Subindex47 -  */
UINT32 SI48; /* Subindex48 -  */
UINT32 SI49; /* Subindex49 -  */
UINT32 SI50; /* Subindex50 -  */
UINT32 SI51; /* Subindex51 -  */
UINT32 SI52; /* Subindex52 -  */
UINT32 SI53; /* Subindex53 -  */
UINT32 SI54; /* Subindex54 -  */
UINT32 SI55; /* Subindex55 -  */
UINT32 SI56; /* Subindex56 -  */
UINT32 SI57; /* Subindex57 -  */
UINT32 SI58; /* Subindex58 -  */
UINT32 SI59; /* Subindex59 -  */
UINT32 SI60; /* Subindex60 -  */
UINT32 SI61; /* Subindex61 -  */
UINT32 SI62; /* Subindex62 -  */
UINT32 SI63; /* Subindex63 -  */
UINT32 SI64; /* Subindex64 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1606;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1606 RxPDO_mapping_16060x1606
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1607 : RxPDO_mapping_1607
******************************************************************************/
/**
* \addtogroup 0x1607 0x1607 | RxPDO_mapping_1607
* @{
* \brief Object 0x1607 (RxPDO_mapping_1607) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
* SubIndex 30<br>
* SubIndex 31<br>
* SubIndex 32<br>
* SubIndex 33<br>
* SubIndex 34<br>
* SubIndex 35<br>
* SubIndex 36<br>
* SubIndex 37<br>
* SubIndex 38<br>
* SubIndex 39<br>
* SubIndex 40<br>
* SubIndex 41<br>
* SubIndex 42<br>
* SubIndex 43<br>
* SubIndex 44<br>
* SubIndex 45<br>
* SubIndex 46<br>
* SubIndex 47<br>
* SubIndex 48<br>
* SubIndex 49<br>
* SubIndex 50<br>
* SubIndex 51<br>
* SubIndex 52<br>
* SubIndex 53<br>
* SubIndex 54<br>
* SubIndex 55<br>
* SubIndex 56<br>
* SubIndex 57<br>
* SubIndex 58<br>
* SubIndex 59<br>
* SubIndex 60<br>
* SubIndex 61<br>
* SubIndex 62<br>
* SubIndex 63<br>
* SubIndex 64<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1607[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex29 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex30 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex31 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex32 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex33 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex34 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex35 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex36 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex37 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex38 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex39 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex40 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex41 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex42 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex43 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex44 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex45 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex46 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex47 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex48 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex49 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex50 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex51 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex52 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex53 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex54 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex55 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex56 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex57 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex58 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex59 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex60 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex61 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex62 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex63 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }}; /* Subindex64 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1607[] = "RxPDO Mapping of Output Register 7\000"
		"1st receive PDO mapped\000"
		"2nd receive PDO mapped\000"
		"3rd receive PDO mapped\000"
		"4th receive PDO mapped\000"
		"5th receive PDO mapped\000"
		"6th receive PDO mapped\000"
		"7th receive PDO mapped\000"
		"8th receive PDO mapped\000"
		"9th receive PDO mapped\000"
		"10th receive PDO mapped\000"
		"11th receive PDO mapped\000"
		"12nd receive PDO mapped\000"
		"13th receive PDO mapped\000"
		"14th receive PDO mapped\000"
		"15th receive PDO mapped\000"
		"16th receive PDO mapped\000"
		"17th receive PDO mapped\000"
		"18th receive PDO mapped\000"
		"19th receive PDO mapped\000"
		"20th receive PDO mapped\000"
		"21st receive PDO mapped\000"
		"22nd receive PDO mapped\000"
		"23rd receive PDO mapped\000"
		"24th receive PDO mapped\000"
		"25th receive PDO mapped\000"
		"26th receive PDO mapped\000"
		"27th receive PDO mapped\000"
		"28th receive PDO mapped\000"
		"29th receive PDO mapped\000"
		"30th receive PDO mapped\000"
		"31st receive PDO mapped\000"
		"32nd receive PDO mapped\000"
		"33rd receive PDO mapped\000"
		"34th receive PDO mapped\000"
		"35th receive PDO mapped\000"
		"36th receive PDO mapped\000"
		"37th receive PDO mapped\000"
		"38th receive PDO mapped\000"
		"39th receive PDO mapped\000"
		"40th receive PDO mapped\000"
		"41st receive PDO mapped\000"
		"42nd receive PDO mapped\000"
		"43rd receive PDO mapped\000"
		"44th receive PDO mapped\000"
		"45th receive PDO mapped\000"
		"46th receive PDO mapped\000"
		"47th receive PDO mapped\000"
		"48th receive PDO mapped\000"
		"49th receive PDO mapped\000"
		"50th receive PDO mapped\000"
		"51st receive PDO mapped\000"
		"52nd receive PDO mapped\000"
		"53rd receive PDO mapped\000"
		"54th receive PDO mapped\000"
		"55th receive PDO mapped\000"
		"56th receive PDO mapped\000"
		"57th receive PDO mapped\000"
		"58th receive PDO mapped\000"
		"59th receive PDO mapped\000"
		"60th receive PDO mapped\000"
		"61st receive PDO mapped\000"
		"62nd receive PDO mapped\000"
		"63rd receive PDO mapped\000"
		"64th receive PDO mapped\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */
UINT32 SI30; /* Subindex30 -  */
UINT32 SI31; /* Subindex31 -  */
UINT32 SI32; /* Subindex32 -  */
UINT32 SI33; /* Subindex33 -  */
UINT32 SI34; /* Subindex34 -  */
UINT32 SI35; /* Subindex35 -  */
UINT32 SI36; /* Subindex36 -  */
UINT32 SI37; /* Subindex37 -  */
UINT32 SI38; /* Subindex38 -  */
UINT32 SI39; /* Subindex39 -  */
UINT32 SI40; /* Subindex40 -  */
UINT32 SI41; /* Subindex41 -  */
UINT32 SI42; /* Subindex42 -  */
UINT32 SI43; /* Subindex43 -  */
UINT32 SI44; /* Subindex44 -  */
UINT32 SI45; /* Subindex45 -  */
UINT32 SI46; /* Subindex46 -  */
UINT32 SI47; /* Subindex47 -  */
UINT32 SI48; /* Subindex48 -  */
UINT32 SI49; /* Subindex49 -  */
UINT32 SI50; /* Subindex50 -  */
UINT32 SI51; /* Subindex51 -  */
UINT32 SI52; /* Subindex52 -  */
UINT32 SI53; /* Subindex53 -  */
UINT32 SI54; /* Subindex54 -  */
UINT32 SI55; /* Subindex55 -  */
UINT32 SI56; /* Subindex56 -  */
UINT32 SI57; /* Subindex57 -  */
UINT32 SI58; /* Subindex58 -  */
UINT32 SI59; /* Subindex59 -  */
UINT32 SI60; /* Subindex60 -  */
UINT32 SI61; /* Subindex61 -  */
UINT32 SI62; /* Subindex62 -  */
UINT32 SI63; /* Subindex63 -  */
UINT32 SI64; /* Subindex64 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1607;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1607 RxPDO_mapping_16070x1607
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/

/******************************************************************************
*                    Object 0x1608 : RxPDO_mapping_1608
******************************************************************************/
/**
* \addtogroup 0x1608 0x1608 | RxPDO_mapping_1608
* @{
* \brief Object 0x1608 (RxPDO_mapping_1608) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
* SubIndex 30<br>
* SubIndex 31<br>
* SubIndex 32<br>
* SubIndex 33<br>
* SubIndex 34<br>
* SubIndex 35<br>
* SubIndex 36<br>
* SubIndex 37<br>
* SubIndex 38<br>
* SubIndex 39<br>
* SubIndex 40<br>
* SubIndex 41<br>
* SubIndex 42<br>
* SubIndex 43<br>
* SubIndex 44<br>
* SubIndex 45<br>
* SubIndex 46<br>
* SubIndex 47<br>
* SubIndex 48<br>
* SubIndex 49<br>
* SubIndex 50<br>
* SubIndex 51<br>
* SubIndex 52<br>
* SubIndex 53<br>
* SubIndex 54<br>
* SubIndex 55<br>
* SubIndex 56<br>
* SubIndex 57<br>
* SubIndex 58<br>
* SubIndex 59<br>
* SubIndex 60<br>
* SubIndex 61<br>
* SubIndex 62<br>
* SubIndex 63<br>
* SubIndex 64<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1608[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex29 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex30 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex31 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex32 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex33 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex34 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex35 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex36 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex37 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex38 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex39 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex40 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex41 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex42 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex43 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex44 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex45 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex46 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex47 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex48 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex49 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex50 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex51 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex52 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex53 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex54 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex55 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex56 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex57 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex58 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex59 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex60 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex61 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex62 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex63 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }}; /* Subindex64 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1608[] = "RxPDO Mapping of Output Register 8\000"
		"1st receive PDO mapped\000"
		"2nd receive PDO mapped\000"
		"3rd receive PDO mapped\000"
		"4th receive PDO mapped\000"
		"5th receive PDO mapped\000"
		"6th receive PDO mapped\000"
		"7th receive PDO mapped\000"
		"8th receive PDO mapped\000"
		"9th receive PDO mapped\000"
		"10th receive PDO mapped\000"
		"11th receive PDO mapped\000"
		"12nd receive PDO mapped\000"
		"13th receive PDO mapped\000"
		"14th receive PDO mapped\000"
		"15th receive PDO mapped\000"
		"16th receive PDO mapped\000"
		"17th receive PDO mapped\000"
		"18th receive PDO mapped\000"
		"19th receive PDO mapped\000"
		"20th receive PDO mapped\000"
		"21st receive PDO mapped\000"
		"22nd receive PDO mapped\000"
		"23rd receive PDO mapped\000"
		"24th receive PDO mapped\000"
		"25th receive PDO mapped\000"
		"26th receive PDO mapped\000"
		"27th receive PDO mapped\000"
		"28th receive PDO mapped\000"
		"29th receive PDO mapped\000"
		"30th receive PDO mapped\000"
		"31st receive PDO mapped\000"
		"32nd receive PDO mapped\000"
		"33rd receive PDO mapped\000"
		"34th receive PDO mapped\000"
		"35th receive PDO mapped\000"
		"36th receive PDO mapped\000"
		"37th receive PDO mapped\000"
		"38th receive PDO mapped\000"
		"39th receive PDO mapped\000"
		"40th receive PDO mapped\000"
		"41st receive PDO mapped\000"
		"42nd receive PDO mapped\000"
		"43rd receive PDO mapped\000"
		"44th receive PDO mapped\000"
		"45th receive PDO mapped\000"
		"46th receive PDO mapped\000"
		"47th receive PDO mapped\000"
		"48th receive PDO mapped\000"
		"49th receive PDO mapped\000"
		"50th receive PDO mapped\000"
		"51st receive PDO mapped\000"
		"52nd receive PDO mapped\000"
		"53rd receive PDO mapped\000"
		"54th receive PDO mapped\000"
		"55th receive PDO mapped\000"
		"56th receive PDO mapped\000"
		"57th receive PDO mapped\000"
		"58th receive PDO mapped\000"
		"59th receive PDO mapped\000"
		"60th receive PDO mapped\000"
		"61st receive PDO mapped\000"
		"62nd receive PDO mapped\000"
		"63rd receive PDO mapped\000"
		"64th receive PDO mapped\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */
UINT32 SI30; /* Subindex30 -  */
UINT32 SI31; /* Subindex31 -  */
UINT32 SI32; /* Subindex32 -  */
UINT32 SI33; /* Subindex33 -  */
UINT32 SI34; /* Subindex34 -  */
UINT32 SI35; /* Subindex35 -  */
UINT32 SI36; /* Subindex36 -  */
UINT32 SI37; /* Subindex37 -  */
UINT32 SI38; /* Subindex38 -  */
UINT32 SI39; /* Subindex39 -  */
UINT32 SI40; /* Subindex40 -  */
UINT32 SI41; /* Subindex41 -  */
UINT32 SI42; /* Subindex42 -  */
UINT32 SI43; /* Subindex43 -  */
UINT32 SI44; /* Subindex44 -  */
UINT32 SI45; /* Subindex45 -  */
UINT32 SI46; /* Subindex46 -  */
UINT32 SI47; /* Subindex47 -  */
UINT32 SI48; /* Subindex48 -  */
UINT32 SI49; /* Subindex49 -  */
UINT32 SI50; /* Subindex50 -  */
UINT32 SI51; /* Subindex51 -  */
UINT32 SI52; /* Subindex52 -  */
UINT32 SI53; /* Subindex53 -  */
UINT32 SI54; /* Subindex54 -  */
UINT32 SI55; /* Subindex55 -  */
UINT32 SI56; /* Subindex56 -  */
UINT32 SI57; /* Subindex57 -  */
UINT32 SI58; /* Subindex58 -  */
UINT32 SI59; /* Subindex59 -  */
UINT32 SI60; /* Subindex60 -  */
UINT32 SI61; /* Subindex61 -  */
UINT32 SI62; /* Subindex62 -  */
UINT32 SI63; /* Subindex63 -  */
UINT32 SI64; /* Subindex64 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1608;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1608 RxPDO_mapping_16080x1608
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/



#if 0
/******************************************************************************
*                    Object 0x1A00 : TxPDO_mapping_1A00
******************************************************************************/
/**
* \addtogroup 0x1A00 0x1A00 | TxPDO_mapping_1A00
* @{
* \brief Object 0x1A00 (TxPDO_mapping_1A00) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
* SubIndex 30<br>
* SubIndex 31<br>
* SubIndex 32<br>
* SubIndex 33<br>
* SubIndex 34<br>
* SubIndex 35<br>
* SubIndex 36<br>
* SubIndex 37<br>
* SubIndex 38<br>
* SubIndex 39<br>
* SubIndex 40<br>
* SubIndex 41<br>
* SubIndex 42<br>
* SubIndex 43<br>
* SubIndex 44<br>
* SubIndex 45<br>
* SubIndex 46<br>
* SubIndex 47<br>
* SubIndex 48<br>
* SubIndex 49<br>
* SubIndex 50<br>
* SubIndex 51<br>
* SubIndex 52<br>
* SubIndex 53<br>
* SubIndex 54<br>
* SubIndex 55<br>
* SubIndex 56<br>
* SubIndex 57<br>
* SubIndex 58<br>
* SubIndex 59<br>
* SubIndex 60<br>
* SubIndex 61<br>
* SubIndex 62<br>
* SubIndex 63<br>
* SubIndex 64<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1A00[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex29 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex30 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex31 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex32 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex33 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex34 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex35 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex36 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex37 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex38 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex39 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex40 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex41 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex42 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex43 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex44 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex45 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex46 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex47 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex48 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex49 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex50 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex51 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex52 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex53 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex54 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex55 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex56 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex57 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex58 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex59 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex60 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex61 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex62 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex63 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }}; /* Subindex64 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1A00[] = "TxPDO mapping of Input Register 1\000"
		"1st transmit PDO mapped\000"
		"2nd transmit PDO mapped\000"
		"3rd transmit PDO mapped\000"
		"4th transmit PDO mapped\000"
		"5th transmit PDO mapped\000"
		"6th transmit PDO mapped\000"
		"7th transmit PDO mapped\000"
		"8th transmit PDO mapped\000"
		"9th transmit PDO mapped\000"
		"10th transmit PDO mapped\000"
		"11th transmit PDO mapped\000"
		"12nd transmit PDO mapped\000"
		"13th transmit PDO mapped\000"
		"14th transmit PDO mapped\000"
		"15th transmit PDO mapped\000"
		"16th transmit PDO mapped\000"
		"17th transmit PDO mapped\000"
		"18th transmit PDO mapped\000"
		"19th transmit PDO mapped\000"
		"20th transmit PDO mapped\000"
		"21st transmit PDO mapped\000"
		"22nd transmit PDO mapped\000"
		"23rd transmit PDO mapped\000"
		"24th transmit PDO mapped\000"
		"25th transmit PDO mapped\000"
		"26th transmit PDO mapped\000"
		"27th transmit PDO mapped\000"
		"28th transmit PDO mapped\000"
		"29th transmit PDO mapped\000"
		"30th transmit PDO mapped\000"
		"31st transmit PDO mapped\000"
		"32nd transmit PDO mapped\000"
		"33rd transmit PDO mapped\000"
		"34th transmit PDO mapped\000"
		"35th transmit PDO mapped\000"
		"36th transmit PDO mapped\000"
		"37th transmit PDO mapped\000"
		"38th transmit PDO mapped\000"
		"39th transmit PDO mapped\000"
		"40th transmit PDO mapped\000"
		"41st transmit PDO mapped\000"
		"42nd transmit PDO mapped\000"
		"43rd transmit PDO mapped\000"
		"44th transmit PDO mapped\000"
		"45th transmit PDO mapped\000"
		"46th transmit PDO mapped\000"
		"47th transmit PDO mapped\000"
		"48th transmit PDO mapped\000"
		"49th transmit PDO mapped\000"
		"50th transmit PDO mapped\000"
		"51st transmit PDO mapped\000"
		"52nd transmit PDO mapped\000"
		"53rd transmit PDO mapped\000"
		"54th transmit PDO mapped\000"
		"55th transmit PDO mapped\000"
		"56th transmit PDO mapped\000"
		"57th transmit PDO mapped\000"
		"58th transmit PDO mapped\000"
		"59th transmit PDO mapped\000"
		"60th transmit PDO mapped\000"
		"61st transmit PDO mapped\000"
		"62nd transmit PDO mapped\000"
		"63rd transmit PDO mapped\000"
		"64th transmit PDO mapped\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */
UINT32 SI30; /* Subindex30 -  */
UINT32 SI31; /* Subindex31 -  */
UINT32 SI32; /* Subindex32 -  */
UINT32 SI33; /* Subindex33 -  */
UINT32 SI34; /* Subindex34 -  */
UINT32 SI35; /* Subindex35 -  */
UINT32 SI36; /* Subindex36 -  */
UINT32 SI37; /* Subindex37 -  */
UINT32 SI38; /* Subindex38 -  */
UINT32 SI39; /* Subindex39 -  */
UINT32 SI40; /* Subindex40 -  */
UINT32 SI41; /* Subindex41 -  */
UINT32 SI42; /* Subindex42 -  */
UINT32 SI43; /* Subindex43 -  */
UINT32 SI44; /* Subindex44 -  */
UINT32 SI45; /* Subindex45 -  */
UINT32 SI46; /* Subindex46 -  */
UINT32 SI47; /* Subindex47 -  */
UINT32 SI48; /* Subindex48 -  */
UINT32 SI49; /* Subindex49 -  */
UINT32 SI50; /* Subindex50 -  */
UINT32 SI51; /* Subindex51 -  */
UINT32 SI52; /* Subindex52 -  */
UINT32 SI53; /* Subindex53 -  */
UINT32 SI54; /* Subindex54 -  */
UINT32 SI55; /* Subindex55 -  */
UINT32 SI56; /* Subindex56 -  */
UINT32 SI57; /* Subindex57 -  */
UINT32 SI58; /* Subindex58 -  */
UINT32 SI59; /* Subindex59 -  */
UINT32 SI60; /* Subindex60 -  */
UINT32 SI61; /* Subindex61 -  */
UINT32 SI62; /* Subindex62 -  */
UINT32 SI63; /* Subindex63 -  */
UINT32 SI64; /* Subindex64 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1A00;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1A00 TxPDO_mapping_1A000x1A00
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/

#endif
/******************************************************************************
*                    Object 0x1A00 : TxPDO_mapping_1A00
******************************************************************************/
/**
* \addtogroup 0x1A00 0x1A00 | TxPDO_mapping_1A00
* @{
* \brief Object 0x1A00 (TxPDO_mapping_1A00) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1A00[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }}; /* Subindex18 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1A00[] = "TxPDO mapping of FSoE Inputs\000"
"Command\000"
"SafeInp1\000"
"SafeInp2\000"
"SafeInp3\000"
"SafeInp4\000"
"SafeInp5\000"
"SafeInp6\000"
"SafeInp7\000"
"SafeInp8\000"
"SafeInp9\000"
"SafeInp10\000"
"SafeInp11\000"
"SafeInp12\000"
"SafeInpDummy\000"
"CRC0\000"
"SafeInput2\000"
"CRC1\000"
"Connection ID\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1A00;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1A00 TxPDO_mapping_1A000x1A00
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={18,0x60000108,0x60010101,0x60010201,0x60010301,0x60010401,0x60010501,0x60010601,0x60010701,0x60010801,0x60010901,0x60010A01,0x60010B01,0x60010C01,0x00000004,0x60000310,0x60010D10,0x60000410,0x60000210}
#endif
;

/******************************************************************************
*                    Object 0x1A00 : TxPDO_mapping_1A00 for 3057 plugin
******************************************************************************/
/**
* \addtogroup 0x1A00 0x1A00 | TxPDO_mapping_1A00
* @{
* \brief Object 0x1A00 (TxPDO_mapping_1A00) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1A00_5[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }}; /* Subindex18 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1A00_5[] = "TxPDO mapping of FSoE Inputs\000"
"Command\000"
"SafeInp1\000"
"SafeInp2\000"
"SafeInp3\000"
"SafeInp4\000"
"SafeInp5\000"
"SafeInp6\000"
"SafeInpDummy1\000"
"CRC0\000"
"SafeInp7\000"
"SafeInp8\000"
"SafeInp9\000"
"SafeInp10\000"
"SafeInp11\000"
"SafeInp12\000"
"SafeInpDummy2\000"
"CRC1\000"
"Connection ID\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1A00_5;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1A00_5 TxPDO_mapping_1A000x1A00_5
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={18,0x60000108,0x60010101,0x60010201,0x60010301,0x60010401,0x60010501,0x60010601,0x0000000A,0x60000310,0x60010701,0x60010801,0x60010901,0x60010A01,0x60010B01,0x60010C01,0x0000000A,0x60000410,0x60000210}
#endif
;

/** @}*/



/******************************************************************************
*                    Object 0x1A01 : TxPDO_mapping_1A01
******************************************************************************/
/**
* \addtogroup 0x1A01 0x1A01 | TxPDO_mapping_1A01
* @{
* \brief Object 0x1A01 (TxPDO_mapping_1A01) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
* SubIndex 30<br>
* SubIndex 31<br>
* SubIndex 32<br>
* SubIndex 33<br>
* SubIndex 34<br>
* SubIndex 35<br>
* SubIndex 36<br>
* SubIndex 37<br>
* SubIndex 38<br>
* SubIndex 39<br>
* SubIndex 40<br>
* SubIndex 41<br>
* SubIndex 42<br>
* SubIndex 43<br>
* SubIndex 44<br>
* SubIndex 45<br>
* SubIndex 46<br>
* SubIndex 47<br>
* SubIndex 48<br>
* SubIndex 49<br>
* SubIndex 50<br>
* SubIndex 51<br>
* SubIndex 52<br>
* SubIndex 53<br>
* SubIndex 54<br>
* SubIndex 55<br>
* SubIndex 56<br>
* SubIndex 57<br>
* SubIndex 58<br>
* SubIndex 59<br>
* SubIndex 60<br>
* SubIndex 61<br>
* SubIndex 62<br>
* SubIndex 63<br>
* SubIndex 64<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1A01[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex29 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex30 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex31 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex32 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex33 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex34 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex35 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex36 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex37 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex38 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex39 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex40 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex41 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex42 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex43 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex44 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex45 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex46 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex47 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex48 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex49 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex50 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex51 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex52 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex53 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex54 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex55 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex56 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex57 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex58 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex59 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex60 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex61 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex62 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex63 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }}; /* Subindex64 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1A01[] = "TxPDO Mapping of Input Register 1\000"
		"1st transmit PDO mapped\000"
		"2nd transmit PDO mapped\000"
		"3rd transmit PDO mapped\000"
		"4th transmit PDO mapped\000"
		"5th transmit PDO mapped\000"
		"6th transmit PDO mapped\000"
		"7th transmit PDO mapped\000"
		"8th transmit PDO mapped\000"
		"9th transmit PDO mapped\000"
		"10th transmit PDO mapped\000"
		"11th transmit PDO mapped\000"
		"12nd transmit PDO mapped\000"
		"13th transmit PDO mapped\000"
		"14th transmit PDO mapped\000"
		"15th transmit PDO mapped\000"
		"16th transmit PDO mapped\000"
		"17th transmit PDO mapped\000"
		"18th transmit PDO mapped\000"
		"19th transmit PDO mapped\000"
		"20th transmit PDO mapped\000"
		"21st transmit PDO mapped\000"
		"22nd transmit PDO mapped\000"
		"23rd transmit PDO mapped\000"
		"24th transmit PDO mapped\000"
		"25th transmit PDO mapped\000"
		"26th transmit PDO mapped\000"
		"27th transmit PDO mapped\000"
		"28th transmit PDO mapped\000"
		"29th transmit PDO mapped\000"
		"30th transmit PDO mapped\000"
		"31st transmit PDO mapped\000"
		"32nd transmit PDO mapped\000"
		"33rd transmit PDO mapped\000"
		"34th transmit PDO mapped\000"
		"35th transmit PDO mapped\000"
		"36th transmit PDO mapped\000"
		"37th transmit PDO mapped\000"
		"38th transmit PDO mapped\000"
		"39th transmit PDO mapped\000"
		"40th transmit PDO mapped\000"
		"41st transmit PDO mapped\000"
		"42nd transmit PDO mapped\000"
		"43rd transmit PDO mapped\000"
		"44th transmit PDO mapped\000"
		"45th transmit PDO mapped\000"
		"46th transmit PDO mapped\000"
		"47th transmit PDO mapped\000"
		"48th transmit PDO mapped\000"
		"49th transmit PDO mapped\000"
		"50th transmit PDO mapped\000"
		"51st transmit PDO mapped\000"
		"52nd transmit PDO mapped\000"
		"53rd transmit PDO mapped\000"
		"54th transmit PDO mapped\000"
		"55th transmit PDO mapped\000"
		"56th transmit PDO mapped\000"
		"57th transmit PDO mapped\000"
		"58th transmit PDO mapped\000"
		"59th transmit PDO mapped\000"
		"60th transmit PDO mapped\000"
		"61st transmit PDO mapped\000"
		"62nd transmit PDO mapped\000"
		"63rd transmit PDO mapped\000"
		"64th transmit PDO mapped\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */
UINT32 SI30; /* Subindex30 -  */
UINT32 SI31; /* Subindex31 -  */
UINT32 SI32; /* Subindex32 -  */
UINT32 SI33; /* Subindex33 -  */
UINT32 SI34; /* Subindex34 -  */
UINT32 SI35; /* Subindex35 -  */
UINT32 SI36; /* Subindex36 -  */
UINT32 SI37; /* Subindex37 -  */
UINT32 SI38; /* Subindex38 -  */
UINT32 SI39; /* Subindex39 -  */
UINT32 SI40; /* Subindex40 -  */
UINT32 SI41; /* Subindex41 -  */
UINT32 SI42; /* Subindex42 -  */
UINT32 SI43; /* Subindex43 -  */
UINT32 SI44; /* Subindex44 -  */
UINT32 SI45; /* Subindex45 -  */
UINT32 SI46; /* Subindex46 -  */
UINT32 SI47; /* Subindex47 -  */
UINT32 SI48; /* Subindex48 -  */
UINT32 SI49; /* Subindex49 -  */
UINT32 SI50; /* Subindex50 -  */
UINT32 SI51; /* Subindex51 -  */
UINT32 SI52; /* Subindex52 -  */
UINT32 SI53; /* Subindex53 -  */
UINT32 SI54; /* Subindex54 -  */
UINT32 SI55; /* Subindex55 -  */
UINT32 SI56; /* Subindex56 -  */
UINT32 SI57; /* Subindex57 -  */
UINT32 SI58; /* Subindex58 -  */
UINT32 SI59; /* Subindex59 -  */
UINT32 SI60; /* Subindex60 -  */
UINT32 SI61; /* Subindex61 -  */
UINT32 SI62; /* Subindex62 -  */
UINT32 SI63; /* Subindex63 -  */
UINT32 SI64; /* Subindex64 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1A01;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1A01 TxPDO_mapping_1A010x1A01
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1A02 : TxPDO_mapping_1A02
******************************************************************************/
/**
* \addtogroup 0x1A02 0x1A02 | TxPDO_mapping_1A02
* @{
* \brief Object 0x1A02 (TxPDO_mapping_1A02) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
* SubIndex 30<br>
* SubIndex 31<br>
* SubIndex 32<br>
* SubIndex 33<br>
* SubIndex 34<br>
* SubIndex 35<br>
* SubIndex 36<br>
* SubIndex 37<br>
* SubIndex 38<br>
* SubIndex 39<br>
* SubIndex 40<br>
* SubIndex 41<br>
* SubIndex 42<br>
* SubIndex 43<br>
* SubIndex 44<br>
* SubIndex 45<br>
* SubIndex 46<br>
* SubIndex 47<br>
* SubIndex 48<br>
* SubIndex 49<br>
* SubIndex 50<br>
* SubIndex 51<br>
* SubIndex 52<br>
* SubIndex 53<br>
* SubIndex 54<br>
* SubIndex 55<br>
* SubIndex 56<br>
* SubIndex 57<br>
* SubIndex 58<br>
* SubIndex 59<br>
* SubIndex 60<br>
* SubIndex 61<br>
* SubIndex 62<br>
* SubIndex 63<br>
* SubIndex 64<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1A02[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex29 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex30 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex31 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex32 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex33 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex34 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex35 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex36 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex37 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex38 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex39 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex40 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex41 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex42 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex43 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex44 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex45 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex46 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex47 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex48 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex49 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex50 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex51 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex52 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex53 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex54 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex55 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex56 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex57 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex58 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex59 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex60 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex61 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex62 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex63 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }}; /* Subindex64 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1A02[] = "TxPDO Mapping of Input Register 2\000"
		"1st transmit PDO mapped\000"
		"2nd transmit PDO mapped\000"
		"3rd transmit PDO mapped\000"
		"4th transmit PDO mapped\000"
		"5th transmit PDO mapped\000"
		"6th transmit PDO mapped\000"
		"7th transmit PDO mapped\000"
		"8th transmit PDO mapped\000"
		"9th transmit PDO mapped\000"
		"10th transmit PDO mapped\000"
		"11th transmit PDO mapped\000"
		"12nd transmit PDO mapped\000"
		"13th transmit PDO mapped\000"
		"14th transmit PDO mapped\000"
		"15th transmit PDO mapped\000"
		"16th transmit PDO mapped\000"
		"17th transmit PDO mapped\000"
		"18th transmit PDO mapped\000"
		"19th transmit PDO mapped\000"
		"20th transmit PDO mapped\000"
		"21st transmit PDO mapped\000"
		"22nd transmit PDO mapped\000"
		"23rd transmit PDO mapped\000"
		"24th transmit PDO mapped\000"
		"25th transmit PDO mapped\000"
		"26th transmit PDO mapped\000"
		"27th transmit PDO mapped\000"
		"28th transmit PDO mapped\000"
		"29th transmit PDO mapped\000"
		"30th transmit PDO mapped\000"
		"31st transmit PDO mapped\000"
		"32nd transmit PDO mapped\000"
		"33rd transmit PDO mapped\000"
		"34th transmit PDO mapped\000"
		"35th transmit PDO mapped\000"
		"36th transmit PDO mapped\000"
		"37th transmit PDO mapped\000"
		"38th transmit PDO mapped\000"
		"39th transmit PDO mapped\000"
		"40th transmit PDO mapped\000"
		"41st transmit PDO mapped\000"
		"42nd transmit PDO mapped\000"
		"43rd transmit PDO mapped\000"
		"44th transmit PDO mapped\000"
		"45th transmit PDO mapped\000"
		"46th transmit PDO mapped\000"
		"47th transmit PDO mapped\000"
		"48th transmit PDO mapped\000"
		"49th transmit PDO mapped\000"
		"50th transmit PDO mapped\000"
		"51st transmit PDO mapped\000"
		"52nd transmit PDO mapped\000"
		"53rd transmit PDO mapped\000"
		"54th transmit PDO mapped\000"
		"55th transmit PDO mapped\000"
		"56th transmit PDO mapped\000"
		"57th transmit PDO mapped\000"
		"58th transmit PDO mapped\000"
		"59th transmit PDO mapped\000"
		"60th transmit PDO mapped\000"
		"61st transmit PDO mapped\000"
		"62nd transmit PDO mapped\000"
		"63rd transmit PDO mapped\000"
		"64th transmit PDO mapped\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */
UINT32 SI30; /* Subindex30 -  */
UINT32 SI31; /* Subindex31 -  */
UINT32 SI32; /* Subindex32 -  */
UINT32 SI33; /* Subindex33 -  */
UINT32 SI34; /* Subindex34 -  */
UINT32 SI35; /* Subindex35 -  */
UINT32 SI36; /* Subindex36 -  */
UINT32 SI37; /* Subindex37 -  */
UINT32 SI38; /* Subindex38 -  */
UINT32 SI39; /* Subindex39 -  */
UINT32 SI40; /* Subindex40 -  */
UINT32 SI41; /* Subindex41 -  */
UINT32 SI42; /* Subindex42 -  */
UINT32 SI43; /* Subindex43 -  */
UINT32 SI44; /* Subindex44 -  */
UINT32 SI45; /* Subindex45 -  */
UINT32 SI46; /* Subindex46 -  */
UINT32 SI47; /* Subindex47 -  */
UINT32 SI48; /* Subindex48 -  */
UINT32 SI49; /* Subindex49 -  */
UINT32 SI50; /* Subindex50 -  */
UINT32 SI51; /* Subindex51 -  */
UINT32 SI52; /* Subindex52 -  */
UINT32 SI53; /* Subindex53 -  */
UINT32 SI54; /* Subindex54 -  */
UINT32 SI55; /* Subindex55 -  */
UINT32 SI56; /* Subindex56 -  */
UINT32 SI57; /* Subindex57 -  */
UINT32 SI58; /* Subindex58 -  */
UINT32 SI59; /* Subindex59 -  */
UINT32 SI60; /* Subindex60 -  */
UINT32 SI61; /* Subindex61 -  */
UINT32 SI62; /* Subindex62 -  */
UINT32 SI63; /* Subindex63 -  */
UINT32 SI64; /* Subindex64 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1A02;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1A02 TxPDO_mapping_1A020x1A02
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1A03 : TxPDO_mapping_1A03
******************************************************************************/
/**
* \addtogroup 0x1A03 0x1A03 | TxPDO_mapping_1A03
* @{
* \brief Object 0x1A03 (TxPDO_mapping_1A03) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
* SubIndex 30<br>
* SubIndex 31<br>
* SubIndex 32<br>
* SubIndex 33<br>
* SubIndex 34<br>
* SubIndex 35<br>
* SubIndex 36<br>
* SubIndex 37<br>
* SubIndex 38<br>
* SubIndex 39<br>
* SubIndex 40<br>
* SubIndex 41<br>
* SubIndex 42<br>
* SubIndex 43<br>
* SubIndex 44<br>
* SubIndex 45<br>
* SubIndex 46<br>
* SubIndex 47<br>
* SubIndex 48<br>
* SubIndex 49<br>
* SubIndex 50<br>
* SubIndex 51<br>
* SubIndex 52<br>
* SubIndex 53<br>
* SubIndex 54<br>
* SubIndex 55<br>
* SubIndex 56<br>
* SubIndex 57<br>
* SubIndex 58<br>
* SubIndex 59<br>
* SubIndex 60<br>
* SubIndex 61<br>
* SubIndex 62<br>
* SubIndex 63<br>
* SubIndex 64<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1A03[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex29 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex30 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex31 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex32 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex33 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex34 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex35 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex36 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex37 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex38 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex39 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex40 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex41 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex42 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex43 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex44 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex45 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex46 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex47 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex48 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex49 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex50 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex51 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex52 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex53 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex54 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex55 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex56 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex57 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex58 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex59 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex60 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex61 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex62 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex63 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }}; /* Subindex64 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1A03[] = "TxPDO Mapping of Input Register 3\000"
		"1st transmit PDO mapped\000"
		"2nd transmit PDO mapped\000"
		"3rd transmit PDO mapped\000"
		"4th transmit PDO mapped\000"
		"5th transmit PDO mapped\000"
		"6th transmit PDO mapped\000"
		"7th transmit PDO mapped\000"
		"8th transmit PDO mapped\000"
		"9th transmit PDO mapped\000"
		"10th transmit PDO mapped\000"
		"11th transmit PDO mapped\000"
		"12nd transmit PDO mapped\000"
		"13th transmit PDO mapped\000"
		"14th transmit PDO mapped\000"
		"15th transmit PDO mapped\000"
		"16th transmit PDO mapped\000"
		"17th transmit PDO mapped\000"
		"18th transmit PDO mapped\000"
		"19th transmit PDO mapped\000"
		"20th transmit PDO mapped\000"
		"21st transmit PDO mapped\000"
		"22nd transmit PDO mapped\000"
		"23rd transmit PDO mapped\000"
		"24th transmit PDO mapped\000"
		"25th transmit PDO mapped\000"
		"26th transmit PDO mapped\000"
		"27th transmit PDO mapped\000"
		"28th transmit PDO mapped\000"
		"29th transmit PDO mapped\000"
		"30th transmit PDO mapped\000"
		"31st transmit PDO mapped\000"
		"32nd transmit PDO mapped\000"
		"33rd transmit PDO mapped\000"
		"34th transmit PDO mapped\000"
		"35th transmit PDO mapped\000"
		"36th transmit PDO mapped\000"
		"37th transmit PDO mapped\000"
		"38th transmit PDO mapped\000"
		"39th transmit PDO mapped\000"
		"40th transmit PDO mapped\000"
		"41st transmit PDO mapped\000"
		"42nd transmit PDO mapped\000"
		"43rd transmit PDO mapped\000"
		"44th transmit PDO mapped\000"
		"45th transmit PDO mapped\000"
		"46th transmit PDO mapped\000"
		"47th transmit PDO mapped\000"
		"48th transmit PDO mapped\000"
		"49th transmit PDO mapped\000"
		"50th transmit PDO mapped\000"
		"51st transmit PDO mapped\000"
		"52nd transmit PDO mapped\000"
		"53rd transmit PDO mapped\000"
		"54th transmit PDO mapped\000"
		"55th transmit PDO mapped\000"
		"56th transmit PDO mapped\000"
		"57th transmit PDO mapped\000"
		"58th transmit PDO mapped\000"
		"59th transmit PDO mapped\000"
		"60th transmit PDO mapped\000"
		"61st transmit PDO mapped\000"
		"62nd transmit PDO mapped\000"
		"63rd transmit PDO mapped\000"
		"64th transmit PDO mapped\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */
UINT32 SI30; /* Subindex30 -  */
UINT32 SI31; /* Subindex31 -  */
UINT32 SI32; /* Subindex32 -  */
UINT32 SI33; /* Subindex33 -  */
UINT32 SI34; /* Subindex34 -  */
UINT32 SI35; /* Subindex35 -  */
UINT32 SI36; /* Subindex36 -  */
UINT32 SI37; /* Subindex37 -  */
UINT32 SI38; /* Subindex38 -  */
UINT32 SI39; /* Subindex39 -  */
UINT32 SI40; /* Subindex40 -  */
UINT32 SI41; /* Subindex41 -  */
UINT32 SI42; /* Subindex42 -  */
UINT32 SI43; /* Subindex43 -  */
UINT32 SI44; /* Subindex44 -  */
UINT32 SI45; /* Subindex45 -  */
UINT32 SI46; /* Subindex46 -  */
UINT32 SI47; /* Subindex47 -  */
UINT32 SI48; /* Subindex48 -  */
UINT32 SI49; /* Subindex49 -  */
UINT32 SI50; /* Subindex50 -  */
UINT32 SI51; /* Subindex51 -  */
UINT32 SI52; /* Subindex52 -  */
UINT32 SI53; /* Subindex53 -  */
UINT32 SI54; /* Subindex54 -  */
UINT32 SI55; /* Subindex55 -  */
UINT32 SI56; /* Subindex56 -  */
UINT32 SI57; /* Subindex57 -  */
UINT32 SI58; /* Subindex58 -  */
UINT32 SI59; /* Subindex59 -  */
UINT32 SI60; /* Subindex60 -  */
UINT32 SI61; /* Subindex61 -  */
UINT32 SI62; /* Subindex62 -  */
UINT32 SI63; /* Subindex63 -  */
UINT32 SI64; /* Subindex64 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1A03;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1A03 TxPDO_mapping_1A030x1A03
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1A04 : TxPDO_mapping_1A04
******************************************************************************/
/**
* \addtogroup 0x1A04 0x1A04 | TxPDO_mapping_1A04
* @{
* \brief Object 0x1A04 (TxPDO_mapping_1A04) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
* SubIndex 30<br>
* SubIndex 31<br>
* SubIndex 32<br>
* SubIndex 33<br>
* SubIndex 34<br>
* SubIndex 35<br>
* SubIndex 36<br>
* SubIndex 37<br>
* SubIndex 38<br>
* SubIndex 39<br>
* SubIndex 40<br>
* SubIndex 41<br>
* SubIndex 42<br>
* SubIndex 43<br>
* SubIndex 44<br>
* SubIndex 45<br>
* SubIndex 46<br>
* SubIndex 47<br>
* SubIndex 48<br>
* SubIndex 49<br>
* SubIndex 50<br>
* SubIndex 51<br>
* SubIndex 52<br>
* SubIndex 53<br>
* SubIndex 54<br>
* SubIndex 55<br>
* SubIndex 56<br>
* SubIndex 57<br>
* SubIndex 58<br>
* SubIndex 59<br>
* SubIndex 60<br>
* SubIndex 61<br>
* SubIndex 62<br>
* SubIndex 63<br>
* SubIndex 64<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1A04[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex29 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex30 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex31 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex32 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex33 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex34 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex35 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex36 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex37 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex38 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex39 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex40 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex41 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex42 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex43 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex44 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex45 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex46 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex47 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex48 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex49 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex50 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex51 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex52 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex53 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex54 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex55 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex56 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex57 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex58 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex59 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex60 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex61 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex62 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex63 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }}; /* Subindex64 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1A04[] = "TxPDO Mapping of Input Register 4\000"
		"1st transmit PDO mapped\000"
		"2nd transmit PDO mapped\000"
		"3rd transmit PDO mapped\000"
		"4th transmit PDO mapped\000"
		"5th transmit PDO mapped\000"
		"6th transmit PDO mapped\000"
		"7th transmit PDO mapped\000"
		"8th transmit PDO mapped\000"
		"9th transmit PDO mapped\000"
		"10th transmit PDO mapped\000"
		"11th transmit PDO mapped\000"
		"12nd transmit PDO mapped\000"
		"13th transmit PDO mapped\000"
		"14th transmit PDO mapped\000"
		"15th transmit PDO mapped\000"
		"16th transmit PDO mapped\000"
		"17th transmit PDO mapped\000"
		"18th transmit PDO mapped\000"
		"19th transmit PDO mapped\000"
		"20th transmit PDO mapped\000"
		"21st transmit PDO mapped\000"
		"22nd transmit PDO mapped\000"
		"23rd transmit PDO mapped\000"
		"24th transmit PDO mapped\000"
		"25th transmit PDO mapped\000"
		"26th transmit PDO mapped\000"
		"27th transmit PDO mapped\000"
		"28th transmit PDO mapped\000"
		"29th transmit PDO mapped\000"
		"30th transmit PDO mapped\000"
		"31st transmit PDO mapped\000"
		"32nd transmit PDO mapped\000"
		"33rd transmit PDO mapped\000"
		"34th transmit PDO mapped\000"
		"35th transmit PDO mapped\000"
		"36th transmit PDO mapped\000"
		"37th transmit PDO mapped\000"
		"38th transmit PDO mapped\000"
		"39th transmit PDO mapped\000"
		"40th transmit PDO mapped\000"
		"41st transmit PDO mapped\000"
		"42nd transmit PDO mapped\000"
		"43rd transmit PDO mapped\000"
		"44th transmit PDO mapped\000"
		"45th transmit PDO mapped\000"
		"46th transmit PDO mapped\000"
		"47th transmit PDO mapped\000"
		"48th transmit PDO mapped\000"
		"49th transmit PDO mapped\000"
		"50th transmit PDO mapped\000"
		"51st transmit PDO mapped\000"
		"52nd transmit PDO mapped\000"
		"53rd transmit PDO mapped\000"
		"54th transmit PDO mapped\000"
		"55th transmit PDO mapped\000"
		"56th transmit PDO mapped\000"
		"57th transmit PDO mapped\000"
		"58th transmit PDO mapped\000"
		"59th transmit PDO mapped\000"
		"60th transmit PDO mapped\000"
		"61st transmit PDO mapped\000"
		"62nd transmit PDO mapped\000"
		"63rd transmit PDO mapped\000"
		"64th transmit PDO mapped\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */
UINT32 SI30; /* Subindex30 -  */
UINT32 SI31; /* Subindex31 -  */
UINT32 SI32; /* Subindex32 -  */
UINT32 SI33; /* Subindex33 -  */
UINT32 SI34; /* Subindex34 -  */
UINT32 SI35; /* Subindex35 -  */
UINT32 SI36; /* Subindex36 -  */
UINT32 SI37; /* Subindex37 -  */
UINT32 SI38; /* Subindex38 -  */
UINT32 SI39; /* Subindex39 -  */
UINT32 SI40; /* Subindex40 -  */
UINT32 SI41; /* Subindex41 -  */
UINT32 SI42; /* Subindex42 -  */
UINT32 SI43; /* Subindex43 -  */
UINT32 SI44; /* Subindex44 -  */
UINT32 SI45; /* Subindex45 -  */
UINT32 SI46; /* Subindex46 -  */
UINT32 SI47; /* Subindex47 -  */
UINT32 SI48; /* Subindex48 -  */
UINT32 SI49; /* Subindex49 -  */
UINT32 SI50; /* Subindex50 -  */
UINT32 SI51; /* Subindex51 -  */
UINT32 SI52; /* Subindex52 -  */
UINT32 SI53; /* Subindex53 -  */
UINT32 SI54; /* Subindex54 -  */
UINT32 SI55; /* Subindex55 -  */
UINT32 SI56; /* Subindex56 -  */
UINT32 SI57; /* Subindex57 -  */
UINT32 SI58; /* Subindex58 -  */
UINT32 SI59; /* Subindex59 -  */
UINT32 SI60; /* Subindex60 -  */
UINT32 SI61; /* Subindex61 -  */
UINT32 SI62; /* Subindex62 -  */
UINT32 SI63; /* Subindex63 -  */
UINT32 SI64; /* Subindex64 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1A04;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1A04 TxPDO_mapping_1A040x1A04
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1A05 : TxPDO_mapping_1A05
******************************************************************************/
/**
* \addtogroup 0x1A05 0x1A05 | TxPDO_mapping_1A05
* @{
* \brief Object 0x1A05 (TxPDO_mapping_1A05) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
* SubIndex 30<br>
* SubIndex 31<br>
* SubIndex 32<br>
* SubIndex 33<br>
* SubIndex 34<br>
* SubIndex 35<br>
* SubIndex 36<br>
* SubIndex 37<br>
* SubIndex 38<br>
* SubIndex 39<br>
* SubIndex 40<br>
* SubIndex 41<br>
* SubIndex 42<br>
* SubIndex 43<br>
* SubIndex 44<br>
* SubIndex 45<br>
* SubIndex 46<br>
* SubIndex 47<br>
* SubIndex 48<br>
* SubIndex 49<br>
* SubIndex 50<br>
* SubIndex 51<br>
* SubIndex 52<br>
* SubIndex 53<br>
* SubIndex 54<br>
* SubIndex 55<br>
* SubIndex 56<br>
* SubIndex 57<br>
* SubIndex 58<br>
* SubIndex 59<br>
* SubIndex 60<br>
* SubIndex 61<br>
* SubIndex 62<br>
* SubIndex 63<br>
* SubIndex 64<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1A05[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex29 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex30 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex31 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex32 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex33 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex34 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex35 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex36 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex37 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex38 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex39 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex40 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex41 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex42 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex43 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex44 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex45 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex46 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex47 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex48 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex49 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex50 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex51 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex52 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex53 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex54 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex55 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex56 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex57 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex58 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex59 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex60 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex61 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex62 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex63 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }}; /* Subindex64 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1A05[] = "TxPDO Mapping of Input Register 5\000"
		"1st transmit PDO mapped\000"
		"2nd transmit PDO mapped\000"
		"3rd transmit PDO mapped\000"
		"4th transmit PDO mapped\000"
		"5th transmit PDO mapped\000"
		"6th transmit PDO mapped\000"
		"7th transmit PDO mapped\000"
		"8th transmit PDO mapped\000"
		"9th transmit PDO mapped\000"
		"10th transmit PDO mapped\000"
		"11th transmit PDO mapped\000"
		"12nd transmit PDO mapped\000"
		"13th transmit PDO mapped\000"
		"14th transmit PDO mapped\000"
		"15th transmit PDO mapped\000"
		"16th transmit PDO mapped\000"
		"17th transmit PDO mapped\000"
		"18th transmit PDO mapped\000"
		"19th transmit PDO mapped\000"
		"20th transmit PDO mapped\000"
		"21st transmit PDO mapped\000"
		"22nd transmit PDO mapped\000"
		"23rd transmit PDO mapped\000"
		"24th transmit PDO mapped\000"
		"25th transmit PDO mapped\000"
		"26th transmit PDO mapped\000"
		"27th transmit PDO mapped\000"
		"28th transmit PDO mapped\000"
		"29th transmit PDO mapped\000"
		"30th transmit PDO mapped\000"
		"31st transmit PDO mapped\000"
		"32nd transmit PDO mapped\000"
		"33rd transmit PDO mapped\000"
		"34th transmit PDO mapped\000"
		"35th transmit PDO mapped\000"
		"36th transmit PDO mapped\000"
		"37th transmit PDO mapped\000"
		"38th transmit PDO mapped\000"
		"39th transmit PDO mapped\000"
		"40th transmit PDO mapped\000"
		"41st transmit PDO mapped\000"
		"42nd transmit PDO mapped\000"
		"43rd transmit PDO mapped\000"
		"44th transmit PDO mapped\000"
		"45th transmit PDO mapped\000"
		"46th transmit PDO mapped\000"
		"47th transmit PDO mapped\000"
		"48th transmit PDO mapped\000"
		"49th transmit PDO mapped\000"
		"50th transmit PDO mapped\000"
		"51st transmit PDO mapped\000"
		"52nd transmit PDO mapped\000"
		"53rd transmit PDO mapped\000"
		"54th transmit PDO mapped\000"
		"55th transmit PDO mapped\000"
		"56th transmit PDO mapped\000"
		"57th transmit PDO mapped\000"
		"58th transmit PDO mapped\000"
		"59th transmit PDO mapped\000"
		"60th transmit PDO mapped\000"
		"61st transmit PDO mapped\000"
		"62nd transmit PDO mapped\000"
		"63rd transmit PDO mapped\000"
		"64th transmit PDO mapped\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */
UINT32 SI30; /* Subindex30 -  */
UINT32 SI31; /* Subindex31 -  */
UINT32 SI32; /* Subindex32 -  */
UINT32 SI33; /* Subindex33 -  */
UINT32 SI34; /* Subindex34 -  */
UINT32 SI35; /* Subindex35 -  */
UINT32 SI36; /* Subindex36 -  */
UINT32 SI37; /* Subindex37 -  */
UINT32 SI38; /* Subindex38 -  */
UINT32 SI39; /* Subindex39 -  */
UINT32 SI40; /* Subindex40 -  */
UINT32 SI41; /* Subindex41 -  */
UINT32 SI42; /* Subindex42 -  */
UINT32 SI43; /* Subindex43 -  */
UINT32 SI44; /* Subindex44 -  */
UINT32 SI45; /* Subindex45 -  */
UINT32 SI46; /* Subindex46 -  */
UINT32 SI47; /* Subindex47 -  */
UINT32 SI48; /* Subindex48 -  */
UINT32 SI49; /* Subindex49 -  */
UINT32 SI50; /* Subindex50 -  */
UINT32 SI51; /* Subindex51 -  */
UINT32 SI52; /* Subindex52 -  */
UINT32 SI53; /* Subindex53 -  */
UINT32 SI54; /* Subindex54 -  */
UINT32 SI55; /* Subindex55 -  */
UINT32 SI56; /* Subindex56 -  */
UINT32 SI57; /* Subindex57 -  */
UINT32 SI58; /* Subindex58 -  */
UINT32 SI59; /* Subindex59 -  */
UINT32 SI60; /* Subindex60 -  */
UINT32 SI61; /* Subindex61 -  */
UINT32 SI62; /* Subindex62 -  */
UINT32 SI63; /* Subindex63 -  */
UINT32 SI64; /* Subindex64 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1A05;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1A05 TxPDO_mapping_1A050x1A05
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1A06 : TxPDO_mapping_1A06
******************************************************************************/
/**
* \addtogroup 0x1A06 0x1A06 | TxPDO_mapping_1A06
* @{
* \brief Object 0x1A06 (TxPDO_mapping_1A06) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
* SubIndex 30<br>
* SubIndex 31<br>
* SubIndex 32<br>
* SubIndex 33<br>
* SubIndex 34<br>
* SubIndex 35<br>
* SubIndex 36<br>
* SubIndex 37<br>
* SubIndex 38<br>
* SubIndex 39<br>
* SubIndex 40<br>
* SubIndex 41<br>
* SubIndex 42<br>
* SubIndex 43<br>
* SubIndex 44<br>
* SubIndex 45<br>
* SubIndex 46<br>
* SubIndex 47<br>
* SubIndex 48<br>
* SubIndex 49<br>
* SubIndex 50<br>
* SubIndex 51<br>
* SubIndex 52<br>
* SubIndex 53<br>
* SubIndex 54<br>
* SubIndex 55<br>
* SubIndex 56<br>
* SubIndex 57<br>
* SubIndex 58<br>
* SubIndex 59<br>
* SubIndex 60<br>
* SubIndex 61<br>
* SubIndex 62<br>
* SubIndex 63<br>
* SubIndex 64<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1A06[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex29 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex30 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex31 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex32 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex33 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex34 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex35 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex36 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex37 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex38 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex39 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex40 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex41 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex42 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex43 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex44 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex45 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex46 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex47 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex48 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex49 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex50 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex51 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex52 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex53 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex54 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex55 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex56 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex57 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex58 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex59 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex60 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex61 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex62 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex63 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }}; /* Subindex64 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1A06[] = "TxPDO Mapping of Input Register 6\000"
		"1st transmit PDO mapped\000"
		"2nd transmit PDO mapped\000"
		"3rd transmit PDO mapped\000"
		"4th transmit PDO mapped\000"
		"5th transmit PDO mapped\000"
		"6th transmit PDO mapped\000"
		"7th transmit PDO mapped\000"
		"8th transmit PDO mapped\000"
		"9th transmit PDO mapped\000"
		"10th transmit PDO mapped\000"
		"11th transmit PDO mapped\000"
		"12nd transmit PDO mapped\000"
		"13th transmit PDO mapped\000"
		"14th transmit PDO mapped\000"
		"15th transmit PDO mapped\000"
		"16th transmit PDO mapped\000"
		"17th transmit PDO mapped\000"
		"18th transmit PDO mapped\000"
		"19th transmit PDO mapped\000"
		"20th transmit PDO mapped\000"
		"21st transmit PDO mapped\000"
		"22nd transmit PDO mapped\000"
		"23rd transmit PDO mapped\000"
		"24th transmit PDO mapped\000"
		"25th transmit PDO mapped\000"
		"26th transmit PDO mapped\000"
		"27th transmit PDO mapped\000"
		"28th transmit PDO mapped\000"
		"29th transmit PDO mapped\000"
		"30th transmit PDO mapped\000"
		"31st transmit PDO mapped\000"
		"32nd transmit PDO mapped\000"
		"33rd transmit PDO mapped\000"
		"34th transmit PDO mapped\000"
		"35th transmit PDO mapped\000"
		"36th transmit PDO mapped\000"
		"37th transmit PDO mapped\000"
		"38th transmit PDO mapped\000"
		"39th transmit PDO mapped\000"
		"40th transmit PDO mapped\000"
		"41st transmit PDO mapped\000"
		"42nd transmit PDO mapped\000"
		"43rd transmit PDO mapped\000"
		"44th transmit PDO mapped\000"
		"45th transmit PDO mapped\000"
		"46th transmit PDO mapped\000"
		"47th transmit PDO mapped\000"
		"48th transmit PDO mapped\000"
		"49th transmit PDO mapped\000"
		"50th transmit PDO mapped\000"
		"51st transmit PDO mapped\000"
		"52nd transmit PDO mapped\000"
		"53rd transmit PDO mapped\000"
		"54th transmit PDO mapped\000"
		"55th transmit PDO mapped\000"
		"56th transmit PDO mapped\000"
		"57th transmit PDO mapped\000"
		"58th transmit PDO mapped\000"
		"59th transmit PDO mapped\000"
		"60th transmit PDO mapped\000"
		"61st transmit PDO mapped\000"
		"62nd transmit PDO mapped\000"
		"63rd transmit PDO mapped\000"
		"64th transmit PDO mapped\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */
UINT32 SI30; /* Subindex30 -  */
UINT32 SI31; /* Subindex31 -  */
UINT32 SI32; /* Subindex32 -  */
UINT32 SI33; /* Subindex33 -  */
UINT32 SI34; /* Subindex34 -  */
UINT32 SI35; /* Subindex35 -  */
UINT32 SI36; /* Subindex36 -  */
UINT32 SI37; /* Subindex37 -  */
UINT32 SI38; /* Subindex38 -  */
UINT32 SI39; /* Subindex39 -  */
UINT32 SI40; /* Subindex40 -  */
UINT32 SI41; /* Subindex41 -  */
UINT32 SI42; /* Subindex42 -  */
UINT32 SI43; /* Subindex43 -  */
UINT32 SI44; /* Subindex44 -  */
UINT32 SI45; /* Subindex45 -  */
UINT32 SI46; /* Subindex46 -  */
UINT32 SI47; /* Subindex47 -  */
UINT32 SI48; /* Subindex48 -  */
UINT32 SI49; /* Subindex49 -  */
UINT32 SI50; /* Subindex50 -  */
UINT32 SI51; /* Subindex51 -  */
UINT32 SI52; /* Subindex52 -  */
UINT32 SI53; /* Subindex53 -  */
UINT32 SI54; /* Subindex54 -  */
UINT32 SI55; /* Subindex55 -  */
UINT32 SI56; /* Subindex56 -  */
UINT32 SI57; /* Subindex57 -  */
UINT32 SI58; /* Subindex58 -  */
UINT32 SI59; /* Subindex59 -  */
UINT32 SI60; /* Subindex60 -  */
UINT32 SI61; /* Subindex61 -  */
UINT32 SI62; /* Subindex62 -  */
UINT32 SI63; /* Subindex63 -  */
UINT32 SI64; /* Subindex64 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1A06;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1A06 TxPDO_mapping_1A060x1A06
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1A07 : TxPDO_mapping_1A07
******************************************************************************/
/**
* \addtogroup 0x1A07 0x1A07 | TxPDO_mapping_1A07
* @{
* \brief Object 0x1A07 (TxPDO_mapping_1A07) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
* SubIndex 30<br>
* SubIndex 31<br>
* SubIndex 32<br>
* SubIndex 33<br>
* SubIndex 34<br>
* SubIndex 35<br>
* SubIndex 36<br>
* SubIndex 37<br>
* SubIndex 38<br>
* SubIndex 39<br>
* SubIndex 40<br>
* SubIndex 41<br>
* SubIndex 42<br>
* SubIndex 43<br>
* SubIndex 44<br>
* SubIndex 45<br>
* SubIndex 46<br>
* SubIndex 47<br>
* SubIndex 48<br>
* SubIndex 49<br>
* SubIndex 50<br>
* SubIndex 51<br>
* SubIndex 52<br>
* SubIndex 53<br>
* SubIndex 54<br>
* SubIndex 55<br>
* SubIndex 56<br>
* SubIndex 57<br>
* SubIndex 58<br>
* SubIndex 59<br>
* SubIndex 60<br>
* SubIndex 61<br>
* SubIndex 62<br>
* SubIndex 63<br>
* SubIndex 64<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1A07[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex29 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex30 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex31 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex32 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex33 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex34 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex35 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex36 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex37 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex38 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex39 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex40 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex41 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex42 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex43 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex44 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex45 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex46 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex47 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex48 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex49 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex50 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex51 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex52 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex53 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex54 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex55 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex56 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex57 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex58 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex59 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex60 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex61 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex62 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex63 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }}; /* Subindex64 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1A07[] = "TxPDO Mapping of Input Register 7\000"
		"1st transmit PDO mapped\000"
		"2nd transmit PDO mapped\000"
		"3rd transmit PDO mapped\000"
		"4th transmit PDO mapped\000"
		"5th transmit PDO mapped\000"
		"6th transmit PDO mapped\000"
		"7th transmit PDO mapped\000"
		"8th transmit PDO mapped\000"
		"9th transmit PDO mapped\000"
		"10th transmit PDO mapped\000"
		"11th transmit PDO mapped\000"
		"12nd transmit PDO mapped\000"
		"13th transmit PDO mapped\000"
		"14th transmit PDO mapped\000"
		"15th transmit PDO mapped\000"
		"16th transmit PDO mapped\000"
		"17th transmit PDO mapped\000"
		"18th transmit PDO mapped\000"
		"19th transmit PDO mapped\000"
		"20th transmit PDO mapped\000"
		"21st transmit PDO mapped\000"
		"22nd transmit PDO mapped\000"
		"23rd transmit PDO mapped\000"
		"24th transmit PDO mapped\000"
		"25th transmit PDO mapped\000"
		"26th transmit PDO mapped\000"
		"27th transmit PDO mapped\000"
		"28th transmit PDO mapped\000"
		"29th transmit PDO mapped\000"
		"30th transmit PDO mapped\000"
		"31st transmit PDO mapped\000"
		"32nd transmit PDO mapped\000"
		"33rd transmit PDO mapped\000"
		"34th transmit PDO mapped\000"
		"35th transmit PDO mapped\000"
		"36th transmit PDO mapped\000"
		"37th transmit PDO mapped\000"
		"38th transmit PDO mapped\000"
		"39th transmit PDO mapped\000"
		"40th transmit PDO mapped\000"
		"41st transmit PDO mapped\000"
		"42nd transmit PDO mapped\000"
		"43rd transmit PDO mapped\000"
		"44th transmit PDO mapped\000"
		"45th transmit PDO mapped\000"
		"46th transmit PDO mapped\000"
		"47th transmit PDO mapped\000"
		"48th transmit PDO mapped\000"
		"49th transmit PDO mapped\000"
		"50th transmit PDO mapped\000"
		"51st transmit PDO mapped\000"
		"52nd transmit PDO mapped\000"
		"53rd transmit PDO mapped\000"
		"54th transmit PDO mapped\000"
		"55th transmit PDO mapped\000"
		"56th transmit PDO mapped\000"
		"57th transmit PDO mapped\000"
		"58th transmit PDO mapped\000"
		"59th transmit PDO mapped\000"
		"60th transmit PDO mapped\000"
		"61st transmit PDO mapped\000"
		"62nd transmit PDO mapped\000"
		"63rd transmit PDO mapped\000"
		"64th transmit PDO mapped\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */
UINT32 SI30; /* Subindex30 -  */
UINT32 SI31; /* Subindex31 -  */
UINT32 SI32; /* Subindex32 -  */
UINT32 SI33; /* Subindex33 -  */
UINT32 SI34; /* Subindex34 -  */
UINT32 SI35; /* Subindex35 -  */
UINT32 SI36; /* Subindex36 -  */
UINT32 SI37; /* Subindex37 -  */
UINT32 SI38; /* Subindex38 -  */
UINT32 SI39; /* Subindex39 -  */
UINT32 SI40; /* Subindex40 -  */
UINT32 SI41; /* Subindex41 -  */
UINT32 SI42; /* Subindex42 -  */
UINT32 SI43; /* Subindex43 -  */
UINT32 SI44; /* Subindex44 -  */
UINT32 SI45; /* Subindex45 -  */
UINT32 SI46; /* Subindex46 -  */
UINT32 SI47; /* Subindex47 -  */
UINT32 SI48; /* Subindex48 -  */
UINT32 SI49; /* Subindex49 -  */
UINT32 SI50; /* Subindex50 -  */
UINT32 SI51; /* Subindex51 -  */
UINT32 SI52; /* Subindex52 -  */
UINT32 SI53; /* Subindex53 -  */
UINT32 SI54; /* Subindex54 -  */
UINT32 SI55; /* Subindex55 -  */
UINT32 SI56; /* Subindex56 -  */
UINT32 SI57; /* Subindex57 -  */
UINT32 SI58; /* Subindex58 -  */
UINT32 SI59; /* Subindex59 -  */
UINT32 SI60; /* Subindex60 -  */
UINT32 SI61; /* Subindex61 -  */
UINT32 SI62; /* Subindex62 -  */
UINT32 SI63; /* Subindex63 -  */
UINT32 SI64; /* Subindex64 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1A07;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1A07 TxPDO_mapping_1A070x1A07
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/

/******************************************************************************
*                    Object 0x1A08 : TxPDO_mapping_1A08
******************************************************************************/
/**
* \addtogroup 0x1A08 0x1A08 | TxPDO_mapping_1A08
* @{
* \brief Object 0x1A08 (TxPDO_mapping_1A08) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
* SubIndex 30<br>
* SubIndex 31<br>
* SubIndex 32<br>
* SubIndex 33<br>
* SubIndex 34<br>
* SubIndex 35<br>
* SubIndex 36<br>
* SubIndex 37<br>
* SubIndex 38<br>
* SubIndex 39<br>
* SubIndex 40<br>
* SubIndex 41<br>
* SubIndex 42<br>
* SubIndex 43<br>
* SubIndex 44<br>
* SubIndex 45<br>
* SubIndex 46<br>
* SubIndex 47<br>
* SubIndex 48<br>
* SubIndex 49<br>
* SubIndex 50<br>
* SubIndex 51<br>
* SubIndex 52<br>
* SubIndex 53<br>
* SubIndex 54<br>
* SubIndex 55<br>
* SubIndex 56<br>
* SubIndex 57<br>
* SubIndex 58<br>
* SubIndex 59<br>
* SubIndex 60<br>
* SubIndex 61<br>
* SubIndex 62<br>
* SubIndex 63<br>
* SubIndex 64<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1A08[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex29 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex30 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex31 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex32 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex33 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex34 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex35 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex36 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex37 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex38 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex39 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex40 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex41 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex42 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex43 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex44 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex45 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex46 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex47 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex48 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex49 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex50 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex51 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex52 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex53 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex54 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex55 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex56 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex57 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex58 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex59 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex60 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex61 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex62 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }, /* Subindex63 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }}; /* Subindex64 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1A08[] = "TxPDO Mapping of Input Register 8\000"
		"1st transmit PDO mapped\000"
		"2nd transmit PDO mapped\000"
		"3rd transmit PDO mapped\000"
		"4th transmit PDO mapped\000"
		"5th transmit PDO mapped\000"
		"6th transmit PDO mapped\000"
		"7th transmit PDO mapped\000"
		"8th transmit PDO mapped\000"
		"9th transmit PDO mapped\000"
		"10th transmit PDO mapped\000"
		"11th transmit PDO mapped\000"
		"12nd transmit PDO mapped\000"
		"13th transmit PDO mapped\000"
		"14th transmit PDO mapped\000"
		"15th transmit PDO mapped\000"
		"16th transmit PDO mapped\000"
		"17th transmit PDO mapped\000"
		"18th transmit PDO mapped\000"
		"19th transmit PDO mapped\000"
		"20th transmit PDO mapped\000"
		"21st transmit PDO mapped\000"
		"22nd transmit PDO mapped\000"
		"23rd transmit PDO mapped\000"
		"24th transmit PDO mapped\000"
		"25th transmit PDO mapped\000"
		"26th transmit PDO mapped\000"
		"27th transmit PDO mapped\000"
		"28th transmit PDO mapped\000"
		"29th transmit PDO mapped\000"
		"30th transmit PDO mapped\000"
		"31st transmit PDO mapped\000"
		"32nd transmit PDO mapped\000"
		"33rd transmit PDO mapped\000"
		"34th transmit PDO mapped\000"
		"35th transmit PDO mapped\000"
		"36th transmit PDO mapped\000"
		"37th transmit PDO mapped\000"
		"38th transmit PDO mapped\000"
		"39th transmit PDO mapped\000"
		"40th transmit PDO mapped\000"
		"41st transmit PDO mapped\000"
		"42nd transmit PDO mapped\000"
		"43rd transmit PDO mapped\000"
		"44th transmit PDO mapped\000"
		"45th transmit PDO mapped\000"
		"46th transmit PDO mapped\000"
		"47th transmit PDO mapped\000"
		"48th transmit PDO mapped\000"
		"49th transmit PDO mapped\000"
		"50th transmit PDO mapped\000"
		"51st transmit PDO mapped\000"
		"52nd transmit PDO mapped\000"
		"53rd transmit PDO mapped\000"
		"54th transmit PDO mapped\000"
		"55th transmit PDO mapped\000"
		"56th transmit PDO mapped\000"
		"57th transmit PDO mapped\000"
		"58th transmit PDO mapped\000"
		"59th transmit PDO mapped\000"
		"60th transmit PDO mapped\000"
		"61st transmit PDO mapped\000"
		"62nd transmit PDO mapped\000"
		"63rd transmit PDO mapped\000"
		"64th transmit PDO mapped\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */
UINT32 SI30; /* Subindex30 -  */
UINT32 SI31; /* Subindex31 -  */
UINT32 SI32; /* Subindex32 -  */
UINT32 SI33; /* Subindex33 -  */
UINT32 SI34; /* Subindex34 -  */
UINT32 SI35; /* Subindex35 -  */
UINT32 SI36; /* Subindex36 -  */
UINT32 SI37; /* Subindex37 -  */
UINT32 SI38; /* Subindex38 -  */
UINT32 SI39; /* Subindex39 -  */
UINT32 SI40; /* Subindex40 -  */
UINT32 SI41; /* Subindex41 -  */
UINT32 SI42; /* Subindex42 -  */
UINT32 SI43; /* Subindex43 -  */
UINT32 SI44; /* Subindex44 -  */
UINT32 SI45; /* Subindex45 -  */
UINT32 SI46; /* Subindex46 -  */
UINT32 SI47; /* Subindex47 -  */
UINT32 SI48; /* Subindex48 -  */
UINT32 SI49; /* Subindex49 -  */
UINT32 SI50; /* Subindex50 -  */
UINT32 SI51; /* Subindex51 -  */
UINT32 SI52; /* Subindex52 -  */
UINT32 SI53; /* Subindex53 -  */
UINT32 SI54; /* Subindex54 -  */
UINT32 SI55; /* Subindex55 -  */
UINT32 SI56; /* Subindex56 -  */
UINT32 SI57; /* Subindex57 -  */
UINT32 SI58; /* Subindex58 -  */
UINT32 SI59; /* Subindex59 -  */
UINT32 SI60; /* Subindex60 -  */
UINT32 SI61; /* Subindex61 -  */
UINT32 SI62; /* Subindex62 -  */
UINT32 SI63; /* Subindex63 -  */
UINT32 SI64; /* Subindex64 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1A08;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1A08 TxPDO_mapping_1A080x1A08
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/




/******************************************************************************
*                    Object 0x1C12 : RxPDO_assignment
******************************************************************************/
/**
* \addtogroup 0x1C12 0x1C12 | RxPDO_assignment
* @{
* \brief Object 0x1C12 (RxPDO_assignment) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1C12[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP},
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x1C12[] = "Sync Manager 2 PDO Assignment\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[9];  /**< \brief Subindex 1 - 8 */
} OBJ_STRUCT_PACKED_END
TOBJ1C12;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1C12 sRxPDOassign
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={0,{0,0,0,0,0,0,0,0,0}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1C13 : TxPDO_assignment
******************************************************************************/
/**
* \addtogroup 0x1C13 0x1C13 | TxPDO_assignment
* @{
* \brief Object 0x1C13 (TxPDO_assignment) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1C13[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_NOPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x1C13[] = "Sync Manager 3 PDO Assignment\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[9];  /**< \brief Subindex 1 - 8 */
} OBJ_STRUCT_PACKED_END
TOBJ1C13;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1C13 sTxPDOassign
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={0,{0,0,0,0,0,0,0,0,0}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x2001 : Component_Name
******************************************************************************/
/**
* \addtogroup 0x2001 0x2001 | Component_Name
* @{
* \brief Object 0x2001 (Component_Name) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x2001 = { DEFTYPE_VISIBLESTRING , 0x100 , ACCESS_READ | OBJACCESS_NOPDOMAPPING };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x2001[] = "Component Name\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO CHAR Component_Name0x2001[32];
/** @}*/



/******************************************************************************
*                    Object 0x2002 : Vendor_Name
******************************************************************************/
/**
* \addtogroup 0x2002 0x2002 | Vendor_Name
* @{
* \brief Object 0x2002 (Vendor_Name) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x2002 = { DEFTYPE_VISIBLESTRING , 0x200 , ACCESS_READ | OBJACCESS_NOPDOMAPPING };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x2002[] = "Vendor Name\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO CHAR Vendor_Name0x2002[64];
/** @}*/



/******************************************************************************
*                    Object 0x2003 : Vendor_URL
******************************************************************************/
/**
* \addtogroup 0x2003 0x2003 | Vendor_URL
* @{
* \brief Object 0x2003 (Vendor_URL) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x2003 = { DEFTYPE_VISIBLESTRING , 0x200 , ACCESS_READ | OBJACCESS_NOPDOMAPPING };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x2003[] = "Vendor URL\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO CHAR Vendor_URL0x2003[64];
/** @}*/



/******************************************************************************
*                    Object 0x2004 : Order_Number
******************************************************************************/
/**
* \addtogroup 0x2004 0x2004 | Order_Number
* @{
* \brief Object 0x2004 (Order_Number) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x2004 = { DEFTYPE_VISIBLESTRING , 0x100 , ACCESS_READ | OBJACCESS_NOPDOMAPPING };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x2004[] = "Order Number\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO CHAR Order_Number0x2004[32];
/** @}*/



/******************************************************************************
*                    Object 0x2005 : Manufacturing_Date
******************************************************************************/
/**
* \addtogroup 0x2005 0x2005 | Manufacturing_Date
* @{
* \brief Object 0x2005 (Manufacturing_Date) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x2005 = { DEFTYPE_VISIBLESTRING , 0x100 , ACCESS_READ | OBJACCESS_NOPDOMAPPING };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x2005[] = "Manufacturing Date\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO CHAR Manufacturing_Date0x2005[32];
/** @}*/



/******************************************************************************
*                    Object 0x2006 : QS_Date
******************************************************************************/
/**
* \addtogroup 0x2006 0x2006 | QS_Date
* @{
* \brief Object 0x2006 (QS_Date) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x2006 = { DEFTYPE_VISIBLESTRING , 0x100 , ACCESS_READ | OBJACCESS_NOPDOMAPPING };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x2006[] = "QS Date\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO CHAR QS_Date0x2006[32];
/** @}*/



/******************************************************************************
*                    Object 0x2007 : Installation_Location
******************************************************************************/
/**
* \addtogroup 0x2007 0x2007 | Installation_Location
* @{
* \brief Object 0x2007 (Installation_Location) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x2007 = { DEFTYPE_VISIBLESTRING , 0x200 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x2007[] = "Installation Location\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO CHAR Installation_Location0x2007[64];
/** @}*/



/******************************************************************************
*                    Object 0x2008 : Operational_Hours
******************************************************************************/
/**
* \addtogroup 0x2008 0x2008 | Operational_Hours
* @{
* \brief Object 0x2008 (Operational_Hours) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x2008 = { DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x2008[] = "Operational Hours\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT32 Operational_Hours0x2008
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x2009 : Service_Date
******************************************************************************/
/**
* \addtogroup 0x2009 0x2009 | Service_Date
* @{
* \brief Object 0x2009 (Service_Date) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x2009 = { DEFTYPE_VISIBLESTRING , 0x100 , ACCESS_READ | OBJACCESS_NOPDOMAPPING };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x2009[] = "Service Date\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO CHAR Service_Date0x2009[32];
/** @}*/



/******************************************************************************
*                    Object 0x200A : Equipment_Ident
******************************************************************************/
/**
* \addtogroup 0x200A 0x200A | Equipment_Ident
* @{
* \brief Object 0x200A (Equipment_Ident) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x200A = { DEFTYPE_VISIBLESTRING , 0x200 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x200A[] = "Equipment Ident\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO CHAR Equipment_Ident0x200A[64];
/** @}*/



/******************************************************************************
*                    Object 0x3740 : Station_Alias_Setup_High
******************************************************************************/
/**
* \addtogroup 0x3740 0x3740 | Station_Alias_Setup_High
* @{
* \brief Object 0x3740 (Station_Alias_Setup_High) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x3740 = { DEFTYPE_INTEGER16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x3740[] = "Station Alias Setup (High)\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT16 Station_Alias_Setup_High0x3740
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
= 0x0000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x3741 : Station_Alias_Selection
******************************************************************************/
/**
* \addtogroup 0x3741 0x3741 | Station_Alias_Selection
* @{
* \brief Object 0x3741 (Station_Alias_Selection) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x3741 = { DEFTYPE_INTEGER16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x3741[] = "Station Alias Selection\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT16 Station_Alias_Selection0x3741
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
= 0x0010
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x4000 : Service_Data_Request
******************************************************************************/
/**
* \addtogroup 0x4000 0x4000 | Service_Data_Request
* @{
* \brief Object 0x4000 (Service_Data_Request) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Operation<br>
* SubIndex 2<br>
* SubIndex 3 - Request_Register_Area_Selection<br>
* SubIndex 4 - Request_Number_of_Registers<br>
* SubIndex 5 - Request_Register_Area_Offset<br>
* SubIndex 6 - Write_Value1<br>
* SubIndex 7 - Write_Value2<br>
* SubIndex 8 - Write_Value3<br>
* SubIndex 9 - Write_Value4<br>
* SubIndex 10 - Write_Value5<br>
* SubIndex 11 - Write_Value6<br>
* SubIndex 12 - Write_Value7<br>
* SubIndex 13 - Write_Value8<br>
* SubIndex 14 - Write_Value9<br>
* SubIndex 15 - Write_Value10<br>
* SubIndex 16 - Write_Value11<br>
* SubIndex 17 - Write_Value12<br>
* SubIndex 18 - Write_Value13<br>
* SubIndex 19 - Write_Value14<br>
* SubIndex 20 - Write_Value15<br>
* SubIndex 21 - Write_Value16<br>
* SubIndex 22 - Write_Value17<br>
* SubIndex 23 - Write_Value18<br>
* SubIndex 24 - Write_Value19<br>
* SubIndex 25 - Write_Value20<br>
* SubIndex 26 - Write_Value21<br>
* SubIndex 27 - Write_Value22<br>
* SubIndex 28 - Write_Value23<br>
* SubIndex 29 - Write_Value24<br>
* SubIndex 30 - Write_Value25<br>
* SubIndex 31 - Write_Value26<br>
* SubIndex 32 - Write_Value27<br>
* SubIndex 33 - Write_Value28<br>
* SubIndex 34 - Write_Value29<br>
* SubIndex 35 - Write_Value30<br>
* SubIndex 36 - Write_Value31<br>
* SubIndex 37 - Write_Value32<br>
* SubIndex 38 - Write_Value33<br>
* SubIndex 39 - Write_Value34<br>
* SubIndex 40 - Write_Value35<br>
* SubIndex 41 - Write_Value36<br>
* SubIndex 42 - Write_Value37<br>
* SubIndex 43 - Write_Value38<br>
* SubIndex 44 - Write_Value39<br>
* SubIndex 45 - Write_Value40<br>
* SubIndex 46 - Write_Value41<br>
* SubIndex 47 - Write_Value42<br>
* SubIndex 48 - Write_Value43<br>
* SubIndex 49 - Write_Value44<br>
* SubIndex 50 - Write_Value45<br>
* SubIndex 51 - Write_Value46<br>
* SubIndex 52 - Write_Value47<br>
* SubIndex 53 - Write_Value48<br>
* SubIndex 54 - Write_Value49<br>
* SubIndex 55 - Write_Value50<br>
* SubIndex 56 - Write_Value51<br>
* SubIndex 57 - Write_Value52<br>
* SubIndex 58 - Write_Value53<br>
* SubIndex 59 - Write_Value54<br>
* SubIndex 60 - Write_Value55<br>
* SubIndex 61 - Write_Value56<br>
* SubIndex 62 - Write_Value57<br>
* SubIndex 63 - Write_Value58<br>
* SubIndex 64 - Write_Value59<br>
* SubIndex 65 - Write_Value60<br>
* SubIndex 66 - Write_Value61<br>
* SubIndex 67 - Write_Value62<br>
* SubIndex 68 - Write_Value63<br>
* SubIndex 69 - Write_Value64<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x4000[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex1 - Operation */
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex3 - Request_Register_Area_Selection */
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex4 - Request_Number_of_Registers */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex5 - Request_Register_Area_Offset */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex6 - Write_Value1 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex7 - Write_Value2 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex8 - Write_Value3 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex9 - Write_Value4 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex10 - Write_Value5 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex11 - Write_Value6 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex12 - Write_Value7 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex13 - Write_Value8 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex14 - Write_Value9 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex15 - Write_Value10 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex16 - Write_Value11 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex17 - Write_Value12 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex18 - Write_Value13 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex19 - Write_Value14 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex20 - Write_Value15 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex21 - Write_Value16 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex22 - Write_Value17 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex23 - Write_Value18 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex24 - Write_Value19 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex25 - Write_Value20 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex26 - Write_Value21 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex27 - Write_Value22 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex28 - Write_Value23 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex29 - Write_Value24 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex30 - Write_Value25 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex31 - Write_Value26 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex32 - Write_Value27 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex33 - Write_Value28 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex34 - Write_Value29 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex35 - Write_Value30 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex36 - Write_Value31 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex37 - Write_Value32 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex38 - Write_Value33 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex39 - Write_Value34 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex40 - Write_Value35 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex41 - Write_Value36 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex42 - Write_Value37 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex43 - Write_Value38 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex44 - Write_Value39 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex45 - Write_Value40 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex46 - Write_Value41 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex47 - Write_Value42 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex48 - Write_Value43 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex49 - Write_Value44 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex50 - Write_Value45 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex51 - Write_Value46 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex52 - Write_Value47 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex53 - Write_Value48 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex54 - Write_Value49 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex55 - Write_Value50 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex56 - Write_Value51 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex57 - Write_Value52 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex58 - Write_Value53 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex59 - Write_Value54 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex60 - Write_Value55 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex61 - Write_Value56 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex62 - Write_Value57 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex63 - Write_Value58 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex64 - Write_Value59 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex65 - Write_Value60 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex66 - Write_Value61 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex67 - Write_Value62 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex68 - Write_Value63 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }}; /* Subindex69 - Write_Value64 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x4000[] = "Service Data Request\000"
"Operation\000"
"Reserved\000"
"Register Area Selection\000"
"Number of Registers\000"
"Register Area Offset\000"
"Value of Reg1\000"
"Value of Reg2\000"
"Value of Reg3\000"
"Value of Reg4\000"
"Value of Reg5\000"
"Value of Reg6\000"
"Value of Reg7\000"
"Value of Reg8\000"
"Value of Reg9\000"
"Value of Reg10\000"
"Value of Reg11\000"
"Value of Reg12\000"
"Value of Reg13\000"
"Value of Reg14\000"
"Value of Reg15\000"
"Value of Reg16\000"
"Value of Reg17\000"
"Value of Reg18\000"
"Value of Reg19\000"
"Value of Reg20\000"
"Value of Reg21\000"
"Value of Reg22\000"
"Value of Reg23\000"
"Value of Reg24\000"
"Value of Reg25\000"
"Value of Reg26\000"
"Value of Reg27\000"
"Value of Reg28\000"
"Value of Reg29\000"
"Value of Reg30\000"
"Value of Reg31\000"
"Value of Reg32\000"
"Value of Reg33\000"
"Value of Reg34\000"
"Value of Reg35\000"
"Value of Reg36\000"
"Value of Reg37\000"
"Value of Reg38\000"
"Value of Reg39\000"
"Value of Reg40\000"
"Value of Reg41\000"
"Value of Reg42\000"
"Value of Reg43\000"
"Value of Reg44\000"
"Value of Reg45\000"
"Value of Reg46\000"
"Value of Reg47\000"
"Value of Reg48\000"
"Value of Reg49\000"
"Value of Reg50\000"
"Value of Reg51\000"
"Value of Reg52\000"
"Value of Reg53\000"
"Value of Reg54\000"
"Value of Reg55\000"
"Value of Reg56\000"
"Value of Reg57\000"
"Value of Reg58\000"
"Value of Reg59\000"
"Value of Reg60\000"
"Value of Reg61\000"
"Value of Reg62\000"
"Value of Reg63\000"
"Value of Reg64\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT8 Operation; /* Subindex1 - Operation */
UINT8 SubIndex002; /* Subindex2 - SubIndex 002 */
UINT8 Request_Register_Area_Selection; /* Subindex3 - Request_Register_Area_Selection */
UINT8 Request_Number_of_Registers; /* Subindex4 - Request_Number_of_Registers */
UINT32 Request_Register_Area_Offset; /* Subindex5 - Request_Register_Area_Offset */
UINT16 Write_Value1; /* Subindex6 - Write_Value1 */
UINT16 Write_Value2; /* Subindex7 - Write_Value2 */
UINT16 Write_Value3; /* Subindex8 - Write_Value3 */
UINT16 Write_Value4; /* Subindex9 - Write_Value4 */
UINT16 Write_Value5; /* Subindex10 - Write_Value5 */
UINT16 Write_Value6; /* Subindex11 - Write_Value6 */
UINT16 Write_Value7; /* Subindex12 - Write_Value7 */
UINT16 Write_Value8; /* Subindex13 - Write_Value8 */
UINT16 Write_Value9; /* Subindex14 - Write_Value9 */
UINT16 Write_Value10; /* Subindex15 - Write_Value10 */
UINT16 Write_Value11; /* Subindex16 - Write_Value11 */
UINT16 Write_Value12; /* Subindex17 - Write_Value12 */
UINT16 Write_Value13; /* Subindex18 - Write_Value13 */
UINT16 Write_Value14; /* Subindex19 - Write_Value14 */
UINT16 Write_Value15; /* Subindex20 - Write_Value15 */
UINT16 Write_Value16; /* Subindex21 - Write_Value16 */
UINT16 Write_Value17; /* Subindex22 - Write_Value17 */
UINT16 Write_Value18; /* Subindex23 - Write_Value18 */
UINT16 Write_Value19; /* Subindex24 - Write_Value19 */
UINT16 Write_Value20; /* Subindex25 - Write_Value20 */
UINT16 Write_Value21; /* Subindex26 - Write_Value21 */
UINT16 Write_Value22; /* Subindex27 - Write_Value22 */
UINT16 Write_Value23; /* Subindex28 - Write_Value23 */
UINT16 Write_Value24; /* Subindex29 - Write_Value24 */
UINT16 Write_Value25; /* Subindex30 - Write_Value25 */
UINT16 Write_Value26; /* Subindex31 - Write_Value26 */
UINT16 Write_Value27; /* Subindex32 - Write_Value27 */
UINT16 Write_Value28; /* Subindex33 - Write_Value28 */
UINT16 Write_Value29; /* Subindex34 - Write_Value29 */
UINT16 Write_Value30; /* Subindex35 - Write_Value30 */
UINT16 Write_Value31; /* Subindex36 - Write_Value31 */
UINT16 Write_Value32; /* Subindex37 - Write_Value32 */
UINT16 Write_Value33; /* Subindex38 - Write_Value33 */
UINT16 Write_Value34; /* Subindex39 - Write_Value34 */
UINT16 Write_Value35; /* Subindex40 - Write_Value35 */
UINT16 Write_Value36; /* Subindex41 - Write_Value36 */
UINT16 Write_Value37; /* Subindex42 - Write_Value37 */
UINT16 Write_Value38; /* Subindex43 - Write_Value38 */
UINT16 Write_Value39; /* Subindex44 - Write_Value39 */
UINT16 Write_Value40; /* Subindex45 - Write_Value40 */
UINT16 Write_Value41; /* Subindex46 - Write_Value41 */
UINT16 Write_Value42; /* Subindex47 - Write_Value42 */
UINT16 Write_Value43; /* Subindex48 - Write_Value43 */
UINT16 Write_Value44; /* Subindex49 - Write_Value44 */
UINT16 Write_Value45; /* Subindex50 - Write_Value45 */
UINT16 Write_Value46; /* Subindex51 - Write_Value46 */
UINT16 Write_Value47; /* Subindex52 - Write_Value47 */
UINT16 Write_Value48; /* Subindex53 - Write_Value48 */
UINT16 Write_Value49; /* Subindex54 - Write_Value49 */
UINT16 Write_Value50; /* Subindex55 - Write_Value50 */
UINT16 Write_Value51; /* Subindex56 - Write_Value51 */
UINT16 Write_Value52; /* Subindex57 - Write_Value52 */
UINT16 Write_Value53; /* Subindex58 - Write_Value53 */
UINT16 Write_Value54; /* Subindex59 - Write_Value54 */
UINT16 Write_Value55; /* Subindex60 - Write_Value55 */
UINT16 Write_Value56; /* Subindex61 - Write_Value56 */
UINT16 Write_Value57; /* Subindex62 - Write_Value57 */
UINT16 Write_Value58; /* Subindex63 - Write_Value58 */
UINT16 Write_Value59; /* Subindex64 - Write_Value59 */
UINT16 Write_Value60; /* Subindex65 - Write_Value60 */
UINT16 Write_Value61; /* Subindex66 - Write_Value61 */
UINT16 Write_Value62; /* Subindex67 - Write_Value62 */
UINT16 Write_Value63; /* Subindex68 - Write_Value63 */
UINT16 Write_Value64; /* Subindex69 - Write_Value64 */
} OBJ_STRUCT_PACKED_END
TOBJ4000;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ4000 Service_Data_Request0x4000
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={69,0x10,0x00,0x11,0x10,0x00000000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x4001 : Service_Data_Response
******************************************************************************/
/**
* \addtogroup 0x4001 0x4001 | Service_Data_Response
* @{
* \brief Object 0x4001 (Service_Data_Response) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Status<br>
* SubIndex 2 - Error_Code<br>
* SubIndex 3 - Response_Register_Area_Selection<br>
* SubIndex 4 - Response_Number_of_Registers<br>
* SubIndex 5 - Response_Register_Area_Offset<br>
* SubIndex 6 - Read_Value1<br>
* SubIndex 7 - Read_Value2<br>
* SubIndex 8 - Read_Value3<br>
* SubIndex 9 - Read_Value4<br>
* SubIndex 10 - Read_Value5<br>
* SubIndex 11 - Read_Value6<br>
* SubIndex 12 - Read_Value7<br>
* SubIndex 13 - Read_Value8<br>
* SubIndex 14 - Read_Value9<br>
* SubIndex 15 - Read_Value10<br>
* SubIndex 16 - Read_Value11<br>
* SubIndex 17 - Read_Value12<br>
* SubIndex 18 - Read_Value13<br>
* SubIndex 19 - Read_Value14<br>
* SubIndex 20 - Read_Value15<br>
* SubIndex 21 - Read_Value16<br>
* SubIndex 22 - Read_Value17<br>
* SubIndex 23 - Read_Value18<br>
* SubIndex 24 - Read_Value19<br>
* SubIndex 25 - Read_Value20<br>
* SubIndex 26 - Read_Value21<br>
* SubIndex 27 - Read_Value22<br>
* SubIndex 28 - Read_Value23<br>
* SubIndex 29 - Read_Value24<br>
* SubIndex 30 - Read_Value25<br>
* SubIndex 31 - Read_Value26<br>
* SubIndex 32 - Read_Value27<br>
* SubIndex 33 - Read_Value28<br>
* SubIndex 34 - Read_Value29<br>
* SubIndex 35 - Read_Value30<br>
* SubIndex 36 - Read_Value31<br>
* SubIndex 37 - Read_Value32<br>
* SubIndex 38 - Read_Value33<br>
* SubIndex 39 - Read_Value34<br>
* SubIndex 40 - Read_Value35<br>
* SubIndex 41 - Read_Value36<br>
* SubIndex 42 - Read_Value37<br>
* SubIndex 43 - Read_Value38<br>
* SubIndex 44 - Read_Value39<br>
* SubIndex 45 - Read_Value40<br>
* SubIndex 46 - Read_Value41<br>
* SubIndex 47 - Read_Value42<br>
* SubIndex 48 - Read_Value43<br>
* SubIndex 49 - Read_Value44<br>
* SubIndex 50 - Read_Value45<br>
* SubIndex 51 - Read_Value46<br>
* SubIndex 52 - Read_Value47<br>
* SubIndex 53 - Read_Value48<br>
* SubIndex 54 - Read_Value49<br>
* SubIndex 55 - Read_Value50<br>
* SubIndex 56 - Read_Value51<br>
* SubIndex 57 - Read_Value52<br>
* SubIndex 58 - Read_Value53<br>
* SubIndex 59 - Read_Value54<br>
* SubIndex 60 - Read_Value55<br>
* SubIndex 61 - Read_Value56<br>
* SubIndex 62 - Read_Value57<br>
* SubIndex 63 - Read_Value58<br>
* SubIndex 64 - Read_Value59<br>
* SubIndex 65 - Read_Value60<br>
* SubIndex 66 - Read_Value61<br>
* SubIndex 67 - Read_Value62<br>
* SubIndex 68 - Read_Value63<br>
* SubIndex 69 - Read_Value64<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x4001[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex1 - Status */
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex2 - Error_Code */
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex3 - Response_Register_Area_Selection */
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex4 - Response_Number_of_Registers */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex5 - Response_Register_Area_Offset */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex6 - Read_Value1 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex7 - Read_Value2 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex8 - Read_Value3 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex9 - Read_Value4 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex10 - Read_Value5 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex11 - Read_Value6 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex12 - Read_Value7 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex13 - Read_Value8 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex14 - Read_Value9 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex15 - Read_Value10 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex16 - Read_Value11 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex17 - Read_Value12 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex18 - Read_Value13 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex19 - Read_Value14 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex20 - Read_Value15 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex21 - Read_Value16 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex22 - Read_Value17 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex23 - Read_Value18 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex24 - Read_Value19 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex25 - Read_Value20 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex26 - Read_Value21 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex27 - Read_Value22 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex28 - Read_Value23 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex29 - Read_Value24 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex30 - Read_Value25 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex31 - Read_Value26 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex32 - Read_Value27 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex33 - Read_Value28 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex34 - Read_Value29 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex35 - Read_Value30 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex36 - Read_Value31 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex37 - Read_Value32 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex38 - Read_Value33 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex39 - Read_Value34 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex40 - Read_Value35 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex41 - Read_Value36 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex42 - Read_Value37 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex43 - Read_Value38 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex44 - Read_Value39 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex45 - Read_Value40 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex46 - Read_Value41 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex47 - Read_Value42 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex48 - Read_Value43 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex49 - Read_Value44 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex50 - Read_Value45 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex51 - Read_Value46 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex52 - Read_Value47 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex53 - Read_Value48 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex54 - Read_Value49 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex55 - Read_Value50 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex56 - Read_Value51 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex57 - Read_Value52 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex58 - Read_Value53 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex59 - Read_Value54 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex60 - Read_Value55 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex61 - Read_Value56 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex62 - Read_Value57 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex63 - Read_Value58 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex64 - Read_Value59 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex65 - Read_Value60 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex66 - Read_Value61 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex67 - Read_Value62 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }, /* Subindex68 - Read_Value63 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }}; /* Subindex69 - Read_Value64 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x4001[] = "Service Data Response\000"
"Status\000"
"Error Code\000"
"Register Area Selection\000"
"Number of Registers\000"
"Register Area Offset\000"
"Value of Reg1\000"
"Value of Reg2\000"
"Value of Reg3\000"
"Value of Reg4\000"
"Value of Reg5\000"
"Value of Reg6\000"
"Value of Reg7\000"
"Value of Reg8\000"
"Value of Reg9\000"
"Value of Reg10\000"
"Value of Reg11\000"
"Value of Reg12\000"
"Value of Reg13\000"
"Value of Reg14\000"
"Value of Reg15\000"
"Value of Reg16\000"
"Value of Reg17\000"
"Value of Reg18\000"
"Value of Reg19\000"
"Value of Reg20\000"
"Value of Reg21\000"
"Value of Reg22\000"
"Value of Reg23\000"
"Value of Reg24\000"
"Value of Reg25\000"
"Value of Reg26\000"
"Value of Reg27\000"
"Value of Reg28\000"
"Value of Reg29\000"
"Value of Reg30\000"
"Value of Reg31\000"
"Value of Reg32\000"
"Value of Reg33\000"
"Value of Reg34\000"
"Value of Reg35\000"
"Value of Reg36\000"
"Value of Reg37\000"
"Value of Reg38\000"
"Value of Reg39\000"
"Value of Reg40\000"
"Value of Reg41\000"
"Value of Reg42\000"
"Value of Reg43\000"
"Value of Reg44\000"
"Value of Reg45\000"
"Value of Reg46\000"
"Value of Reg47\000"
"Value of Reg48\000"
"Value of Reg49\000"
"Value of Reg50\000"
"Value of Reg51\000"
"Value of Reg52\000"
"Value of Reg53\000"
"Value of Reg54\000"
"Value of Reg55\000"
"Value of Reg56\000"
"Value of Reg57\000"
"Value of Reg58\000"
"Value of Reg59\000"
"Value of Reg60\000"
"Value of Reg61\000"
"Value of Reg62\000"
"Value of Reg63\000"
"Value of Reg64\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT8 Status; /* Subindex1 - Status */
UINT8 Error_Code; /* Subindex2 - Error_Code */
UINT8 Response_Register_Area_Selection; /* Subindex3 - Response_Register_Area_Selection */
UINT8 Response_Number_of_Registers; /* Subindex4 - Response_Number_of_Registers */
UINT32 Response_Register_Area_Offset; /* Subindex5 - Response_Register_Area_Offset */
UINT16 Read_Value1; /* Subindex6 - Read_Value1 */
UINT16 Read_Value2; /* Subindex7 - Read_Value2 */
UINT16 Read_Value3; /* Subindex8 - Read_Value3 */
UINT16 Read_Value4; /* Subindex9 - Read_Value4 */
UINT16 Read_Value5; /* Subindex10 - Read_Value5 */
UINT16 Read_Value6; /* Subindex11 - Read_Value6 */
UINT16 Read_Value7; /* Subindex12 - Read_Value7 */
UINT16 Read_Value8; /* Subindex13 - Read_Value8 */
UINT16 Read_Value9; /* Subindex14 - Read_Value9 */
UINT16 Read_Value10; /* Subindex15 - Read_Value10 */
UINT16 Read_Value11; /* Subindex16 - Read_Value11 */
UINT16 Read_Value12; /* Subindex17 - Read_Value12 */
UINT16 Read_Value13; /* Subindex18 - Read_Value13 */
UINT16 Read_Value14; /* Subindex19 - Read_Value14 */
UINT16 Read_Value15; /* Subindex20 - Read_Value15 */
UINT16 Read_Value16; /* Subindex21 - Read_Value16 */
UINT16 Read_Value17; /* Subindex22 - Read_Value17 */
UINT16 Read_Value18; /* Subindex23 - Read_Value18 */
UINT16 Read_Value19; /* Subindex24 - Read_Value19 */
UINT16 Read_Value20; /* Subindex25 - Read_Value20 */
UINT16 Read_Value21; /* Subindex26 - Read_Value21 */
UINT16 Read_Value22; /* Subindex27 - Read_Value22 */
UINT16 Read_Value23; /* Subindex28 - Read_Value23 */
UINT16 Read_Value24; /* Subindex29 - Read_Value24 */
UINT16 Read_Value25; /* Subindex30 - Read_Value25 */
UINT16 Read_Value26; /* Subindex31 - Read_Value26 */
UINT16 Read_Value27; /* Subindex32 - Read_Value27 */
UINT16 Read_Value28; /* Subindex33 - Read_Value28 */
UINT16 Read_Value29; /* Subindex34 - Read_Value29 */
UINT16 Read_Value30; /* Subindex35 - Read_Value30 */
UINT16 Read_Value31; /* Subindex36 - Read_Value31 */
UINT16 Read_Value32; /* Subindex37 - Read_Value32 */
UINT16 Read_Value33; /* Subindex38 - Read_Value33 */
UINT16 Read_Value34; /* Subindex39 - Read_Value34 */
UINT16 Read_Value35; /* Subindex40 - Read_Value35 */
UINT16 Read_Value36; /* Subindex41 - Read_Value36 */
UINT16 Read_Value37; /* Subindex42 - Read_Value37 */
UINT16 Read_Value38; /* Subindex43 - Read_Value38 */
UINT16 Read_Value39; /* Subindex44 - Read_Value39 */
UINT16 Read_Value40; /* Subindex45 - Read_Value40 */
UINT16 Read_Value41; /* Subindex46 - Read_Value41 */
UINT16 Read_Value42; /* Subindex47 - Read_Value42 */
UINT16 Read_Value43; /* Subindex48 - Read_Value43 */
UINT16 Read_Value44; /* Subindex49 - Read_Value44 */
UINT16 Read_Value45; /* Subindex50 - Read_Value45 */
UINT16 Read_Value46; /* Subindex51 - Read_Value46 */
UINT16 Read_Value47; /* Subindex52 - Read_Value47 */
UINT16 Read_Value48; /* Subindex53 - Read_Value48 */
UINT16 Read_Value49; /* Subindex54 - Read_Value49 */
UINT16 Read_Value50; /* Subindex55 - Read_Value50 */
UINT16 Read_Value51; /* Subindex56 - Read_Value51 */
UINT16 Read_Value52; /* Subindex57 - Read_Value52 */
UINT16 Read_Value53; /* Subindex58 - Read_Value53 */
UINT16 Read_Value54; /* Subindex59 - Read_Value54 */
UINT16 Read_Value55; /* Subindex60 - Read_Value55 */
UINT16 Read_Value56; /* Subindex61 - Read_Value56 */
UINT16 Read_Value57; /* Subindex62 - Read_Value57 */
UINT16 Read_Value58; /* Subindex63 - Read_Value58 */
UINT16 Read_Value59; /* Subindex64 - Read_Value59 */
UINT16 Read_Value60; /* Subindex65 - Read_Value60 */
UINT16 Read_Value61; /* Subindex66 - Read_Value61 */
UINT16 Read_Value62; /* Subindex67 - Read_Value62 */
UINT16 Read_Value63; /* Subindex68 - Read_Value63 */
UINT16 Read_Value64; /* Subindex69 - Read_Value64 */
} OBJ_STRUCT_PACKED_END
TOBJ4001;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ4001 Service_Data_Response0x4001
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={69,0x00,0x00,0x00,0x00,0x00000000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}
#endif
;
/** @}*/


#if 0
/******************************************************************************
*                    Object 0x6000 : Input_Data_6000
******************************************************************************/
/**
* \addtogroup 0x6000 0x6000 | Input_Data_6000
* @{
* \brief Object 0x6000 (Input_Data_6000) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6000[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x6000[] = "FSOE Slave Frame Elements\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[64];  /**< \brief Subindex 1 - 64 */
} OBJ_STRUCT_PACKED_END
TOBJ6000;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6000 Input_Data_60000x6000
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}
#endif
;
/** @}*/

#endif
/******************************************************************************
*                    Object 0x6000 : FSoE_Slave_Data_6000
******************************************************************************/
/**
* \addtogroup 0x6000 0x6000 | FSoE_Slave_Data_6000
* @{
* \brief Object 0x6000 (FSoE_Slave_Data_6000) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
*
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6000[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x6000[] = "FSOE Slave Frame Elements\000"
"Command\000"
"Connection ID\000"
"CRC0\000""CRC1\000\377";
#endif //#ifdef _OBJD_

/** @}*/

/******************************************************************************
*                    Object 0x6001 : FSoE_Slave_Inputs_6001
******************************************************************************/
/**
* \addtogroup 0x6001 0x6001 | FSoE_Slave_Inputs_6001
* @{
* \brief Object 0x6001 (FSoE_Slave_Inputs_6001) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
*
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6001[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x6001[] = "FSOE Slave Safety Inputs\000"
"SafeInp1\000"
"SafeInp2\000"
"SafeInp3\000"
"SafeInp4\000"
"SafeInp5\000"
"SafeInp6\000"
"SafeInp7\000"
"SafeInp8\000"
"SafeInp9\000"
"SafeInp10\000"
"SafeInp11\000"
"SafeInp12\000"
"SafeInput2\000\377";
#endif //#ifdef _OBJD_

/******************************************************************************
*                    Object 0x6001 : FSoE_Slave_Inputs_6001 for 3057 plugin
******************************************************************************/
/**
* \addtogroup 0x6001 0x6001 | FSoE_Slave_Inputs_6001
* @{
* \brief Object 0x6001 (FSoE_Slave_Inputs_6001) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
*
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6001_5[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01	 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x6001_5[] = "FSOE Slave Safety Inputs\000"
"SafeInp1\000"
"SafeInp2\000"
"SafeInp3\000"
"SafeInp4\000"
"SafeInp5\000"
"SafeInp6\000"
"SafeInp7\000"
"SafeInp8\000"
"SafeInp9\000"
"SafeInp10\000"
"SafeInp11\000"
"SafeInp12\000\377";
#endif //#ifdef _OBJD_

/** @}*/


/******************************************************************************
*                    Object 0x6010 : Input_Data_6010
******************************************************************************/
/**
* \addtogroup 0x6010 0x6010 | Input_Data_6010
* @{
* \brief Object 0x6010 (Input_Data_6010) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6010[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Input Data XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x6010[] = "Input 1\000"
		"Input Data 1\000"
		"Input Data 2\000"
		"Input Data 3\000"
		"Input Data 4\000"
		"Input Data 5\000"
		"Input Data 6\000"
		"Input Data 7\000"
		"Input Data 8\000"
		"Input Data 9\000"
		"Input Data 10\000"
		"Input Data 11\000"
		"Input Data 12\000"
		"Input Data 13\000"
		"Input Data 14\000"
		"Input Data 15\000"
		"Input Data 16\000"
		"Input Data 17\000"
		"Input Data 18\000"
		"Input Data 19\000"
		"Input Data 20\000"
		"Input Data 21\000"
		"Input Data 22\000"
		"Input Data 23\000"
		"Input Data 24\000"
		"Input Data 25\000"
		"Input Data 26\000"
		"Input Data 27\000"
		"Input Data 28\000"
		"Input Data 29\000"
		"Input Data 30\000"
		"Input Data 31\000"
		"Input Data 32\000"
		"Input Data 33\000"
		"Input Data 34\000"
		"Input Data 35\000"
		"Input Data 36\000"
		"Input Data 37\000"
		"Input Data 38\000"
		"Input Data 39\000"
		"Input Data 40\000"
		"Input Data 41\000"
		"Input Data 42\000"
		"Input Data 43\000"
		"Input Data 44\000"
		"Input Data 45\000"
		"Input Data 46\000"
		"Input Data 47\000"
		"Input Data 48\000"
		"Input Data 49\000"
		"Input Data 50\000"
		"Input Data 51\000"
		"Input Data 52\000"
		"Input Data 53\000"
		"Input Data 54\000"
		"Input Data 55\000"
		"Input Data 56\000"
		"Input Data 57\000"
		"Input Data 58\000"
		"Input Data 59\000"
		"Input Data 60\000"
		"Input Data 61\000"
		"Input Data 62\000"
		"Input Data 63\000"
		"Input Data 64\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[64];  /**< \brief Subindex 1 - 64 */
} OBJ_STRUCT_PACKED_END
TOBJ6010;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6010 Input_Data_60100x6010
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6020 : Input_Data_6020
******************************************************************************/
/**
* \addtogroup 0x6020 0x6020 | Input_Data_6020
* @{
* \brief Object 0x6020 (Input_Data_6020) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6020[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Input Data XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x6020[] = "Input 2\000"
		"Input Data 1\000"
		"Input Data 2\000"
		"Input Data 3\000"
		"Input Data 4\000"
		"Input Data 5\000"
		"Input Data 6\000"
		"Input Data 7\000"
		"Input Data 8\000"
		"Input Data 9\000"
		"Input Data 10\000"
		"Input Data 11\000"
		"Input Data 12\000"
		"Input Data 13\000"
		"Input Data 14\000"
		"Input Data 15\000"
		"Input Data 16\000"
		"Input Data 17\000"
		"Input Data 18\000"
		"Input Data 19\000"
		"Input Data 20\000"
		"Input Data 21\000"
		"Input Data 22\000"
		"Input Data 23\000"
		"Input Data 24\000"
		"Input Data 25\000"
		"Input Data 26\000"
		"Input Data 27\000"
		"Input Data 28\000"
		"Input Data 29\000"
		"Input Data 30\000"
		"Input Data 31\000"
		"Input Data 32\000"
		"Input Data 33\000"
		"Input Data 34\000"
		"Input Data 35\000"
		"Input Data 36\000"
		"Input Data 37\000"
		"Input Data 38\000"
		"Input Data 39\000"
		"Input Data 40\000"
		"Input Data 41\000"
		"Input Data 42\000"
		"Input Data 43\000"
		"Input Data 44\000"
		"Input Data 45\000"
		"Input Data 46\000"
		"Input Data 47\000"
		"Input Data 48\000"
		"Input Data 49\000"
		"Input Data 50\000"
		"Input Data 51\000"
		"Input Data 52\000"
		"Input Data 53\000"
		"Input Data 54\000"
		"Input Data 55\000"
		"Input Data 56\000"
		"Input Data 57\000"
		"Input Data 58\000"
		"Input Data 59\000"
		"Input Data 60\000"
		"Input Data 61\000"
		"Input Data 62\000"
		"Input Data 63\000"
		"Input Data 64\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[64];  /**< \brief Subindex 1 - 64 */
} OBJ_STRUCT_PACKED_END
TOBJ6020;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6020 Input_Data_60200x6020
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6030 : Input_Data_6030
******************************************************************************/
/**
* \addtogroup 0x6030 0x6030 | Input_Data_6030
* @{
* \brief Object 0x6030 (Input_Data_6030) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6030[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Input Data XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x6030[] = "Input 3\000"
		"Input Data 1\000"
		"Input Data 2\000"
		"Input Data 3\000"
		"Input Data 4\000"
		"Input Data 5\000"
		"Input Data 6\000"
		"Input Data 7\000"
		"Input Data 8\000"
		"Input Data 9\000"
		"Input Data 10\000"
		"Input Data 11\000"
		"Input Data 12\000"
		"Input Data 13\000"
		"Input Data 14\000"
		"Input Data 15\000"
		"Input Data 16\000"
		"Input Data 17\000"
		"Input Data 18\000"
		"Input Data 19\000"
		"Input Data 20\000"
		"Input Data 21\000"
		"Input Data 22\000"
		"Input Data 23\000"
		"Input Data 24\000"
		"Input Data 25\000"
		"Input Data 26\000"
		"Input Data 27\000"
		"Input Data 28\000"
		"Input Data 29\000"
		"Input Data 30\000"
		"Input Data 31\000"
		"Input Data 32\000"
		"Input Data 33\000"
		"Input Data 34\000"
		"Input Data 35\000"
		"Input Data 36\000"
		"Input Data 37\000"
		"Input Data 38\000"
		"Input Data 39\000"
		"Input Data 40\000"
		"Input Data 41\000"
		"Input Data 42\000"
		"Input Data 43\000"
		"Input Data 44\000"
		"Input Data 45\000"
		"Input Data 46\000"
		"Input Data 47\000"
		"Input Data 48\000"
		"Input Data 49\000"
		"Input Data 50\000"
		"Input Data 51\000"
		"Input Data 52\000"
		"Input Data 53\000"
		"Input Data 54\000"
		"Input Data 55\000"
		"Input Data 56\000"
		"Input Data 57\000"
		"Input Data 58\000"
		"Input Data 59\000"
		"Input Data 60\000"
		"Input Data 61\000"
		"Input Data 62\000"
		"Input Data 63\000"
		"Input Data 64\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[64];  /**< \brief Subindex 1 - 64 */
} OBJ_STRUCT_PACKED_END
TOBJ6030;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6030 Input_Data_60300x6030
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6040 : Input_Data_6040
******************************************************************************/
/**
* \addtogroup 0x6040 0x6040 | Input_Data_6040
* @{
* \brief Object 0x6040 (Input_Data_6040) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6040[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Input Data XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x6040[] = "Input 4\000"
		"Input Data 1\000"
		"Input Data 2\000"
		"Input Data 3\000"
		"Input Data 4\000"
		"Input Data 5\000"
		"Input Data 6\000"
		"Input Data 7\000"
		"Input Data 8\000"
		"Input Data 9\000"
		"Input Data 10\000"
		"Input Data 11\000"
		"Input Data 12\000"
		"Input Data 13\000"
		"Input Data 14\000"
		"Input Data 15\000"
		"Input Data 16\000"
		"Input Data 17\000"
		"Input Data 18\000"
		"Input Data 19\000"
		"Input Data 20\000"
		"Input Data 21\000"
		"Input Data 22\000"
		"Input Data 23\000"
		"Input Data 24\000"
		"Input Data 25\000"
		"Input Data 26\000"
		"Input Data 27\000"
		"Input Data 28\000"
		"Input Data 29\000"
		"Input Data 30\000"
		"Input Data 31\000"
		"Input Data 32\000"
		"Input Data 33\000"
		"Input Data 34\000"
		"Input Data 35\000"
		"Input Data 36\000"
		"Input Data 37\000"
		"Input Data 38\000"
		"Input Data 39\000"
		"Input Data 40\000"
		"Input Data 41\000"
		"Input Data 42\000"
		"Input Data 43\000"
		"Input Data 44\000"
		"Input Data 45\000"
		"Input Data 46\000"
		"Input Data 47\000"
		"Input Data 48\000"
		"Input Data 49\000"
		"Input Data 50\000"
		"Input Data 51\000"
		"Input Data 52\000"
		"Input Data 53\000"
		"Input Data 54\000"
		"Input Data 55\000"
		"Input Data 56\000"
		"Input Data 57\000"
		"Input Data 58\000"
		"Input Data 59\000"
		"Input Data 60\000"
		"Input Data 61\000"
		"Input Data 62\000"
		"Input Data 63\000"
		"Input Data 64\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[64];  /**< \brief Subindex 1 - 64 */
} OBJ_STRUCT_PACKED_END
TOBJ6040;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6040 Input_Data_60400x6040
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6050 : Input_Data_6050
******************************************************************************/
/**
* \addtogroup 0x6050 0x6050 | Input_Data_6050
* @{
* \brief Object 0x6050 (Input_Data_6050) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6050[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Input Data XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x6050[] = "Input 5\000"
		"Input Data 1\000"
		"Input Data 2\000"
		"Input Data 3\000"
		"Input Data 4\000"
		"Input Data 5\000"
		"Input Data 6\000"
		"Input Data 7\000"
		"Input Data 8\000"
		"Input Data 9\000"
		"Input Data 10\000"
		"Input Data 11\000"
		"Input Data 12\000"
		"Input Data 13\000"
		"Input Data 14\000"
		"Input Data 15\000"
		"Input Data 16\000"
		"Input Data 17\000"
		"Input Data 18\000"
		"Input Data 19\000"
		"Input Data 20\000"
		"Input Data 21\000"
		"Input Data 22\000"
		"Input Data 23\000"
		"Input Data 24\000"
		"Input Data 25\000"
		"Input Data 26\000"
		"Input Data 27\000"
		"Input Data 28\000"
		"Input Data 29\000"
		"Input Data 30\000"
		"Input Data 31\000"
		"Input Data 32\000"
		"Input Data 33\000"
		"Input Data 34\000"
		"Input Data 35\000"
		"Input Data 36\000"
		"Input Data 37\000"
		"Input Data 38\000"
		"Input Data 39\000"
		"Input Data 40\000"
		"Input Data 41\000"
		"Input Data 42\000"
		"Input Data 43\000"
		"Input Data 44\000"
		"Input Data 45\000"
		"Input Data 46\000"
		"Input Data 47\000"
		"Input Data 48\000"
		"Input Data 49\000"
		"Input Data 50\000"
		"Input Data 51\000"
		"Input Data 52\000"
		"Input Data 53\000"
		"Input Data 54\000"
		"Input Data 55\000"
		"Input Data 56\000"
		"Input Data 57\000"
		"Input Data 58\000"
		"Input Data 59\000"
		"Input Data 60\000"
		"Input Data 61\000"
		"Input Data 62\000"
		"Input Data 63\000"
		"Input Data 64\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[64];  /**< \brief Subindex 1 - 64 */
} OBJ_STRUCT_PACKED_END
TOBJ6050;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6050 Input_Data_60500x6050
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6060 : Input_Data_6060
******************************************************************************/
/**
* \addtogroup 0x6060 0x6060 | Input_Data_6060
* @{
* \brief Object 0x6060 (Input_Data_6060) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6060[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Input Data XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x6060[] = "Input 6\000"
		"Input Data 1\000"
		"Input Data 2\000"
		"Input Data 3\000"
		"Input Data 4\000"
		"Input Data 5\000"
		"Input Data 6\000"
		"Input Data 7\000"
		"Input Data 8\000"
		"Input Data 9\000"
		"Input Data 10\000"
		"Input Data 11\000"
		"Input Data 12\000"
		"Input Data 13\000"
		"Input Data 14\000"
		"Input Data 15\000"
		"Input Data 16\000"
		"Input Data 17\000"
		"Input Data 18\000"
		"Input Data 19\000"
		"Input Data 20\000"
		"Input Data 21\000"
		"Input Data 22\000"
		"Input Data 23\000"
		"Input Data 24\000"
		"Input Data 25\000"
		"Input Data 26\000"
		"Input Data 27\000"
		"Input Data 28\000"
		"Input Data 29\000"
		"Input Data 30\000"
		"Input Data 31\000"
		"Input Data 32\000"
		"Input Data 33\000"
		"Input Data 34\000"
		"Input Data 35\000"
		"Input Data 36\000"
		"Input Data 37\000"
		"Input Data 38\000"
		"Input Data 39\000"
		"Input Data 40\000"
		"Input Data 41\000"
		"Input Data 42\000"
		"Input Data 43\000"
		"Input Data 44\000"
		"Input Data 45\000"
		"Input Data 46\000"
		"Input Data 47\000"
		"Input Data 48\000"
		"Input Data 49\000"
		"Input Data 50\000"
		"Input Data 51\000"
		"Input Data 52\000"
		"Input Data 53\000"
		"Input Data 54\000"
		"Input Data 55\000"
		"Input Data 56\000"
		"Input Data 57\000"
		"Input Data 58\000"
		"Input Data 59\000"
		"Input Data 60\000"
		"Input Data 61\000"
		"Input Data 62\000"
		"Input Data 63\000"
		"Input Data 64\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[64];  /**< \brief Subindex 1 - 64 */
} OBJ_STRUCT_PACKED_END
TOBJ6060;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6060 Input_Data_60600x6060
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6070 : Input_Data_6070
******************************************************************************/
/**
* \addtogroup 0x6070 0x6070 | Input_Data_6070
* @{
* \brief Object 0x6070 (Input_Data_6070) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6070[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Input Data XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x6070[] = "Input 7\000"
		"Input Data 1\000"
		"Input Data 2\000"
		"Input Data 3\000"
		"Input Data 4\000"
		"Input Data 5\000"
		"Input Data 6\000"
		"Input Data 7\000"
		"Input Data 8\000"
		"Input Data 9\000"
		"Input Data 10\000"
		"Input Data 11\000"
		"Input Data 12\000"
		"Input Data 13\000"
		"Input Data 14\000"
		"Input Data 15\000"
		"Input Data 16\000"
		"Input Data 17\000"
		"Input Data 18\000"
		"Input Data 19\000"
		"Input Data 20\000"
		"Input Data 21\000"
		"Input Data 22\000"
		"Input Data 23\000"
		"Input Data 24\000"
		"Input Data 25\000"
		"Input Data 26\000"
		"Input Data 27\000"
		"Input Data 28\000"
		"Input Data 29\000"
		"Input Data 30\000"
		"Input Data 31\000"
		"Input Data 32\000"
		"Input Data 33\000"
		"Input Data 34\000"
		"Input Data 35\000"
		"Input Data 36\000"
		"Input Data 37\000"
		"Input Data 38\000"
		"Input Data 39\000"
		"Input Data 40\000"
		"Input Data 41\000"
		"Input Data 42\000"
		"Input Data 43\000"
		"Input Data 44\000"
		"Input Data 45\000"
		"Input Data 46\000"
		"Input Data 47\000"
		"Input Data 48\000"
		"Input Data 49\000"
		"Input Data 50\000"
		"Input Data 51\000"
		"Input Data 52\000"
		"Input Data 53\000"
		"Input Data 54\000"
		"Input Data 55\000"
		"Input Data 56\000"
		"Input Data 57\000"
		"Input Data 58\000"
		"Input Data 59\000"
		"Input Data 60\000"
		"Input Data 61\000"
		"Input Data 62\000"
		"Input Data 63\000"
		"Input Data 64\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[64];  /**< \brief Subindex 1 - 64 */
} OBJ_STRUCT_PACKED_END
TOBJ6070;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6070 Input_Data_60700x6070
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}
#endif
;
/** @}*/

/******************************************************************************
*                    Object 0x6080 : Input_Data_6080
******************************************************************************/
/**
* \addtogroup 0x6080 0x6080 | Input_Data_6080
* @{
* \brief Object 0x6080 (Input_Data_6080) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
*
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6080[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Input Data XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x6080[] = "Input 8\000"
		"Input Data 1\000"
		"Input Data 2\000"
		"Input Data 3\000"
		"Input Data 4\000"
		"Input Data 5\000"
		"Input Data 6\000"
		"Input Data 7\000"
		"Input Data 8\000"
		"Input Data 9\000"
		"Input Data 10\000"
		"Input Data 11\000"
		"Input Data 12\000"
		"Input Data 13\000"
		"Input Data 14\000"
		"Input Data 15\000"
		"Input Data 16\000"
		"Input Data 17\000"
		"Input Data 18\000"
		"Input Data 19\000"
		"Input Data 20\000"
		"Input Data 21\000"
		"Input Data 22\000"
		"Input Data 23\000"
		"Input Data 24\000"
		"Input Data 25\000"
		"Input Data 26\000"
		"Input Data 27\000"
		"Input Data 28\000"
		"Input Data 29\000"
		"Input Data 30\000"
		"Input Data 31\000"
		"Input Data 32\000"
		"Input Data 33\000"
		"Input Data 34\000"
		"Input Data 35\000"
		"Input Data 36\000"
		"Input Data 37\000"
		"Input Data 38\000"
		"Input Data 39\000"
		"Input Data 40\000"
		"Input Data 41\000"
		"Input Data 42\000"
		"Input Data 43\000"
		"Input Data 44\000"
		"Input Data 45\000"
		"Input Data 46\000"
		"Input Data 47\000"
		"Input Data 48\000"
		"Input Data 49\000"
		"Input Data 50\000"
		"Input Data 51\000"
		"Input Data 52\000"
		"Input Data 53\000"
		"Input Data 54\000"
		"Input Data 55\000"
		"Input Data 56\000"
		"Input Data 57\000"
		"Input Data 58\000"
		"Input Data 59\000"
		"Input Data 60\000"
		"Input Data 61\000"
		"Input Data 62\000"
		"Input Data 63\000"
		"Input Data 64\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[64];  /**< \brief Subindex 1 - 64 */
} OBJ_STRUCT_PACKED_END
TOBJ6080;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6080 Input_Data_60800x6080
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}
#endif
;
/** @}*/



#if 0
/******************************************************************************
*                    Object 0x7000 : Output_Data_7000
******************************************************************************/
/**
* \addtogroup 0x7000 0x7000 | Output_Data_7000
* @{
* \brief Object 0x7000 (Output_Data_7000) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7000[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x7000[] = "Output 1\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[64];  /**< \brief Subindex 1 - 64 */
} OBJ_STRUCT_PACKED_END
TOBJ7000;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ7000 Output_Data_70000x7000
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}
#endif
;
/** @}*/

#endif
/******************************************************************************
*                    Object 0x7000 : FSoE_Slave_Data_7000
******************************************************************************/
/**
* \addtogroup 0x7000 0x7000 | FSoE_Slave_Data_7000
* @{
* \brief Object 0x7000 (FSoE_Slave_Data_7000) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
*
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7000[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_RXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x7000[] = "FSOE Master Frame Elements\000"
"Command\000"
"Connection ID\000"
"CRC0\000"
"CRC1\000\377";
#endif //#ifdef _OBJD_

/** @}*/

/******************************************************************************
*                    Object 0x7001 : FSoE_Slave_Inputs_7001
******************************************************************************/
/**
* \addtogroup 0x7001 0x7001 | FSoE_Slave_Inputs_7001
* @{
* \brief Object 0x7001 (FSoE_Slave_Inputs_7001) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
*
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7001[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_RXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x7001[] = "FSOE Slave Safety Outputs\000"
"SafeOut1\000"
"SafeOut2\000"
"SafeOut3\000"
"SafeOut4\000"
"SafeOut5\000"
"SafeOut6\000"
"SafeOut7\000"
"SafeOut8\000"
"SafeOut9\000"
"SafeOut10\000"
"SafeOut11\000"
"SafeOut12\000"
"SafeOutput2\000\377";
#endif //#ifdef _OBJD_

/******************************************************************************
*                    Object 0x7001 : FSoE_Slave_Inputs_7001 for 3057 plugin
******************************************************************************/
/**
* \addtogroup 0x7001 0x7001 | FSoE_Slave_Inputs_7001
* @{
* \brief Object 0x7001 (FSoE_Slave_Inputs_7001) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
*
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7001_5[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_RXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x7001_5[] = "FSOE Slave Safety Outputs\000"
"SafeOut1\000"
"SafeOut2\000"
"SafeOut3\000"
"SafeOut4\000"
"SafeOut5\000"
"SafeOut6\000"
"SafeOut7\000"
"SafeOut8\000\377";
#endif //#ifdef _OBJD_

/** @}*/


/******************************************************************************
*                    Object 0x7010 : Output_Data_7010
******************************************************************************/
/**
* \addtogroup 0x7010 0x7010 | Output_Data_7010
* @{
* \brief Object 0x7010 (Output_Data_7010) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7010[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x7010[] = "Output 1\000"
		"Output Data 1\000"
		"Output Data 2\000"
		"Output Data 3\000"
		"Output Data 4\000"
		"Output Data 5\000"
		"Output Data 6\000"
		"Output Data 7\000"
		"Output Data 8\000"
		"Output Data 9\000"
		"Output Data 10\000"
		"Output Data 11\000"
		"Output Data 12\000"
		"Output Data 13\000"
		"Output Data 14\000"
		"Output Data 15\000"
		"Output Data 16\000"
		"Output Data 17\000"
		"Output Data 18\000"
		"Output Data 19\000"
		"Output Data 20\000"
		"Output Data 21\000"
		"Output Data 22\000"
		"Output Data 23\000"
		"Output Data 24\000"
		"Output Data 25\000"
		"Output Data 26\000"
		"Output Data 27\000"
		"Output Data 28\000"
		"Output Data 29\000"
		"Output Data 30\000"
		"Output Data 31\000"
		"Output Data 32\000"
		"Output Data 33\000"
		"Output Data 34\000"
		"Output Data 35\000"
		"Output Data 36\000"
		"Output Data 37\000"
		"Output Data 38\000"
		"Output Data 39\000"
		"Output Data 40\000"
		"Output Data 41\000"
		"Output Data 42\000"
		"Output Data 43\000"
		"Output Data 44\000"
		"Output Data 45\000"
		"Output Data 46\000"
		"Output Data 47\000"
		"Output Data 48\000"
		"Output Data 49\000"
		"Output Data 50\000"
		"Output Data 51\000"
		"Output Data 52\000"
		"Output Data 53\000"
		"Output Data 54\000"
		"Output Data 55\000"
		"Output Data 56\000"
		"Output Data 57\000"
		"Output Data 58\000"
		"Output Data 59\000"
		"Output Data 60\000"
		"Output Data 61\000"
		"Output Data 62\000"
		"Output Data 63\000"
		"Output Data 64\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[64];  /**< \brief Subindex 1 - 64 */
} OBJ_STRUCT_PACKED_END
TOBJ7010;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ7010 Output_Data_70100x7010
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x7020 : Output_Data_7020
******************************************************************************/
/**
* \addtogroup 0x7020 0x7020 | Output_Data_7020
* @{
* \brief Object 0x7020 (Output_Data_7020) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7020[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x7020[] = "Output 2\000"
		"Output Data 1\000"
		"Output Data 2\000"
		"Output Data 3\000"
		"Output Data 4\000"
		"Output Data 5\000"
		"Output Data 6\000"
		"Output Data 7\000"
		"Output Data 8\000"
		"Output Data 9\000"
		"Output Data 10\000"
		"Output Data 11\000"
		"Output Data 12\000"
		"Output Data 13\000"
		"Output Data 14\000"
		"Output Data 15\000"
		"Output Data 16\000"
		"Output Data 17\000"
		"Output Data 18\000"
		"Output Data 19\000"
		"Output Data 20\000"
		"Output Data 21\000"
		"Output Data 22\000"
		"Output Data 23\000"
		"Output Data 24\000"
		"Output Data 25\000"
		"Output Data 26\000"
		"Output Data 27\000"
		"Output Data 28\000"
		"Output Data 29\000"
		"Output Data 30\000"
		"Output Data 31\000"
		"Output Data 32\000"
		"Output Data 33\000"
		"Output Data 34\000"
		"Output Data 35\000"
		"Output Data 36\000"
		"Output Data 37\000"
		"Output Data 38\000"
		"Output Data 39\000"
		"Output Data 40\000"
		"Output Data 41\000"
		"Output Data 42\000"
		"Output Data 43\000"
		"Output Data 44\000"
		"Output Data 45\000"
		"Output Data 46\000"
		"Output Data 47\000"
		"Output Data 48\000"
		"Output Data 49\000"
		"Output Data 50\000"
		"Output Data 51\000"
		"Output Data 52\000"
		"Output Data 53\000"
		"Output Data 54\000"
		"Output Data 55\000"
		"Output Data 56\000"
		"Output Data 57\000"
		"Output Data 58\000"
		"Output Data 59\000"
		"Output Data 60\000"
		"Output Data 61\000"
		"Output Data 62\000"
		"Output Data 63\000"
		"Output Data 64\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[64];  /**< \brief Subindex 1 - 64 */
} OBJ_STRUCT_PACKED_END
TOBJ7020;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ7020 Output_Data_70200x7020
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x7030 : Output_Data_7030
******************************************************************************/
/**
* \addtogroup 0x7030 0x7030 | Output_Data_7030
* @{
* \brief Object 0x7030 (Output_Data_7030) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7030[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x7030[] = "Output 3\000"
		"Output Data 1\000"
		"Output Data 2\000"
		"Output Data 3\000"
		"Output Data 4\000"
		"Output Data 5\000"
		"Output Data 6\000"
		"Output Data 7\000"
		"Output Data 8\000"
		"Output Data 9\000"
		"Output Data 10\000"
		"Output Data 11\000"
		"Output Data 12\000"
		"Output Data 13\000"
		"Output Data 14\000"
		"Output Data 15\000"
		"Output Data 16\000"
		"Output Data 17\000"
		"Output Data 18\000"
		"Output Data 19\000"
		"Output Data 20\000"
		"Output Data 21\000"
		"Output Data 22\000"
		"Output Data 23\000"
		"Output Data 24\000"
		"Output Data 25\000"
		"Output Data 26\000"
		"Output Data 27\000"
		"Output Data 28\000"
		"Output Data 29\000"
		"Output Data 30\000"
		"Output Data 31\000"
		"Output Data 32\000"
		"Output Data 33\000"
		"Output Data 34\000"
		"Output Data 35\000"
		"Output Data 36\000"
		"Output Data 37\000"
		"Output Data 38\000"
		"Output Data 39\000"
		"Output Data 40\000"
		"Output Data 41\000"
		"Output Data 42\000"
		"Output Data 43\000"
		"Output Data 44\000"
		"Output Data 45\000"
		"Output Data 46\000"
		"Output Data 47\000"
		"Output Data 48\000"
		"Output Data 49\000"
		"Output Data 50\000"
		"Output Data 51\000"
		"Output Data 52\000"
		"Output Data 53\000"
		"Output Data 54\000"
		"Output Data 55\000"
		"Output Data 56\000"
		"Output Data 57\000"
		"Output Data 58\000"
		"Output Data 59\000"
		"Output Data 60\000"
		"Output Data 61\000"
		"Output Data 62\000"
		"Output Data 63\000"
		"Output Data 64\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[64];  /**< \brief Subindex 1 - 64 */
} OBJ_STRUCT_PACKED_END
TOBJ7030;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ7030 Output_Data_70300x7030
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x7040 : Output_Data_7040
******************************************************************************/
/**
* \addtogroup 0x7040 0x7040 | Output_Data_7040
* @{
* \brief Object 0x7040 (Output_Data_7040) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7040[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x7040[] = "Output 4\000"
		"Output Data 1\000"
		"Output Data 2\000"
		"Output Data 3\000"
		"Output Data 4\000"
		"Output Data 5\000"
		"Output Data 6\000"
		"Output Data 7\000"
		"Output Data 8\000"
		"Output Data 9\000"
		"Output Data 10\000"
		"Output Data 11\000"
		"Output Data 12\000"
		"Output Data 13\000"
		"Output Data 14\000"
		"Output Data 15\000"
		"Output Data 16\000"
		"Output Data 17\000"
		"Output Data 18\000"
		"Output Data 19\000"
		"Output Data 20\000"
		"Output Data 21\000"
		"Output Data 22\000"
		"Output Data 23\000"
		"Output Data 24\000"
		"Output Data 25\000"
		"Output Data 26\000"
		"Output Data 27\000"
		"Output Data 28\000"
		"Output Data 29\000"
		"Output Data 30\000"
		"Output Data 31\000"
		"Output Data 32\000"
		"Output Data 33\000"
		"Output Data 34\000"
		"Output Data 35\000"
		"Output Data 36\000"
		"Output Data 37\000"
		"Output Data 38\000"
		"Output Data 39\000"
		"Output Data 40\000"
		"Output Data 41\000"
		"Output Data 42\000"
		"Output Data 43\000"
		"Output Data 44\000"
		"Output Data 45\000"
		"Output Data 46\000"
		"Output Data 47\000"
		"Output Data 48\000"
		"Output Data 49\000"
		"Output Data 50\000"
		"Output Data 51\000"
		"Output Data 52\000"
		"Output Data 53\000"
		"Output Data 54\000"
		"Output Data 55\000"
		"Output Data 56\000"
		"Output Data 57\000"
		"Output Data 58\000"
		"Output Data 59\000"
		"Output Data 60\000"
		"Output Data 61\000"
		"Output Data 62\000"
		"Output Data 63\000"
		"Output Data 64\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[64];  /**< \brief Subindex 1 - 64 */
} OBJ_STRUCT_PACKED_END
TOBJ7040;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ7040 Output_Data_70400x7040
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x7050 : Output_Data_7050
******************************************************************************/
/**
* \addtogroup 0x7050 0x7050 | Output_Data_7050
* @{
* \brief Object 0x7050 (Output_Data_7050) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7050[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x7050[] = "Output 5\000"
		"Output Data 1\000"
		"Output Data 2\000"
		"Output Data 3\000"
		"Output Data 4\000"
		"Output Data 5\000"
		"Output Data 6\000"
		"Output Data 7\000"
		"Output Data 8\000"
		"Output Data 9\000"
		"Output Data 10\000"
		"Output Data 11\000"
		"Output Data 12\000"
		"Output Data 13\000"
		"Output Data 14\000"
		"Output Data 15\000"
		"Output Data 16\000"
		"Output Data 17\000"
		"Output Data 18\000"
		"Output Data 19\000"
		"Output Data 20\000"
		"Output Data 21\000"
		"Output Data 22\000"
		"Output Data 23\000"
		"Output Data 24\000"
		"Output Data 25\000"
		"Output Data 26\000"
		"Output Data 27\000"
		"Output Data 28\000"
		"Output Data 29\000"
		"Output Data 30\000"
		"Output Data 31\000"
		"Output Data 32\000"
		"Output Data 33\000"
		"Output Data 34\000"
		"Output Data 35\000"
		"Output Data 36\000"
		"Output Data 37\000"
		"Output Data 38\000"
		"Output Data 39\000"
		"Output Data 40\000"
		"Output Data 41\000"
		"Output Data 42\000"
		"Output Data 43\000"
		"Output Data 44\000"
		"Output Data 45\000"
		"Output Data 46\000"
		"Output Data 47\000"
		"Output Data 48\000"
		"Output Data 49\000"
		"Output Data 50\000"
		"Output Data 51\000"
		"Output Data 52\000"
		"Output Data 53\000"
		"Output Data 54\000"
		"Output Data 55\000"
		"Output Data 56\000"
		"Output Data 57\000"
		"Output Data 58\000"
		"Output Data 59\000"
		"Output Data 60\000"
		"Output Data 61\000"
		"Output Data 62\000"
		"Output Data 63\000"
		"Output Data 64\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[64];  /**< \brief Subindex 1 - 64 */
} OBJ_STRUCT_PACKED_END
TOBJ7050;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ7050 Output_Data_70500x7050
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x7060 : Output_Data_7060
******************************************************************************/
/**
* \addtogroup 0x7060 0x7060 | Output_Data_7060
* @{
* \brief Object 0x7060 (Output_Data_7060) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7060[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x7060[] = "Output 6\000"
		"Output Data 1\000"
		"Output Data 2\000"
		"Output Data 3\000"
		"Output Data 4\000"
		"Output Data 5\000"
		"Output Data 6\000"
		"Output Data 7\000"
		"Output Data 8\000"
		"Output Data 9\000"
		"Output Data 10\000"
		"Output Data 11\000"
		"Output Data 12\000"
		"Output Data 13\000"
		"Output Data 14\000"
		"Output Data 15\000"
		"Output Data 16\000"
		"Output Data 17\000"
		"Output Data 18\000"
		"Output Data 19\000"
		"Output Data 20\000"
		"Output Data 21\000"
		"Output Data 22\000"
		"Output Data 23\000"
		"Output Data 24\000"
		"Output Data 25\000"
		"Output Data 26\000"
		"Output Data 27\000"
		"Output Data 28\000"
		"Output Data 29\000"
		"Output Data 30\000"
		"Output Data 31\000"
		"Output Data 32\000"
		"Output Data 33\000"
		"Output Data 34\000"
		"Output Data 35\000"
		"Output Data 36\000"
		"Output Data 37\000"
		"Output Data 38\000"
		"Output Data 39\000"
		"Output Data 40\000"
		"Output Data 41\000"
		"Output Data 42\000"
		"Output Data 43\000"
		"Output Data 44\000"
		"Output Data 45\000"
		"Output Data 46\000"
		"Output Data 47\000"
		"Output Data 48\000"
		"Output Data 49\000"
		"Output Data 50\000"
		"Output Data 51\000"
		"Output Data 52\000"
		"Output Data 53\000"
		"Output Data 54\000"
		"Output Data 55\000"
		"Output Data 56\000"
		"Output Data 57\000"
		"Output Data 58\000"
		"Output Data 59\000"
		"Output Data 60\000"
		"Output Data 61\000"
		"Output Data 62\000"
		"Output Data 63\000"
		"Output Data 64\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[64];  /**< \brief Subindex 1 - 64 */
} OBJ_STRUCT_PACKED_END
TOBJ7060;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ7060 Output_Data_70600x7060
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x7070 : Output_Data_7070
******************************************************************************/
/**
* \addtogroup 0x7070 0x7070 | Output_Data_7070
* @{
* \brief Object 0x7070 (Output_Data_7070) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7070[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x7070[] = "Output 7\000"
		"Output Data 1\000"
		"Output Data 2\000"
		"Output Data 3\000"
		"Output Data 4\000"
		"Output Data 5\000"
		"Output Data 6\000"
		"Output Data 7\000"
		"Output Data 8\000"
		"Output Data 9\000"
		"Output Data 10\000"
		"Output Data 11\000"
		"Output Data 12\000"
		"Output Data 13\000"
		"Output Data 14\000"
		"Output Data 15\000"
		"Output Data 16\000"
		"Output Data 17\000"
		"Output Data 18\000"
		"Output Data 19\000"
		"Output Data 20\000"
		"Output Data 21\000"
		"Output Data 22\000"
		"Output Data 23\000"
		"Output Data 24\000"
		"Output Data 25\000"
		"Output Data 26\000"
		"Output Data 27\000"
		"Output Data 28\000"
		"Output Data 29\000"
		"Output Data 30\000"
		"Output Data 31\000"
		"Output Data 32\000"
		"Output Data 33\000"
		"Output Data 34\000"
		"Output Data 35\000"
		"Output Data 36\000"
		"Output Data 37\000"
		"Output Data 38\000"
		"Output Data 39\000"
		"Output Data 40\000"
		"Output Data 41\000"
		"Output Data 42\000"
		"Output Data 43\000"
		"Output Data 44\000"
		"Output Data 45\000"
		"Output Data 46\000"
		"Output Data 47\000"
		"Output Data 48\000"
		"Output Data 49\000"
		"Output Data 50\000"
		"Output Data 51\000"
		"Output Data 52\000"
		"Output Data 53\000"
		"Output Data 54\000"
		"Output Data 55\000"
		"Output Data 56\000"
		"Output Data 57\000"
		"Output Data 58\000"
		"Output Data 59\000"
		"Output Data 60\000"
		"Output Data 61\000"
		"Output Data 62\000"
		"Output Data 63\000"
		"Output Data 64\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[64];  /**< \brief Subindex 1 - 64 */
} OBJ_STRUCT_PACKED_END
TOBJ7070;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ7070 Output_Data_70700x7070
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x7080 : Output_Data_7080
******************************************************************************/
/**
* \addtogroup 0x7080 0x7080 | Output_Data_7080
* @{
* \brief Object 0x7080 (Output_Data_7080) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
*
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7080[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Output Data XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x7080[] = "Output 8\000"
		"Output Data 1\000"
		"Output Data 2\000"
		"Output Data 3\000"
		"Output Data 4\000"
		"Output Data 5\000"
		"Output Data 6\000"
		"Output Data 7\000"
		"Output Data 8\000"
		"Output Data 9\000"
		"Output Data 10\000"
		"Output Data 11\000"
		"Output Data 12\000"
		"Output Data 13\000"
		"Output Data 14\000"
		"Output Data 15\000"
		"Output Data 16\000"
		"Output Data 17\000"
		"Output Data 18\000"
		"Output Data 19\000"
		"Output Data 20\000"
		"Output Data 21\000"
		"Output Data 22\000"
		"Output Data 23\000"
		"Output Data 24\000"
		"Output Data 25\000"
		"Output Data 26\000"
		"Output Data 27\000"
		"Output Data 28\000"
		"Output Data 29\000"
		"Output Data 30\000"
		"Output Data 31\000"
		"Output Data 32\000"
		"Output Data 33\000"
		"Output Data 34\000"
		"Output Data 35\000"
		"Output Data 36\000"
		"Output Data 37\000"
		"Output Data 38\000"
		"Output Data 39\000"
		"Output Data 40\000"
		"Output Data 41\000"
		"Output Data 42\000"
		"Output Data 43\000"
		"Output Data 44\000"
		"Output Data 45\000"
		"Output Data 46\000"
		"Output Data 47\000"
		"Output Data 48\000"
		"Output Data 49\000"
		"Output Data 50\000"
		"Output Data 51\000"
		"Output Data 52\000"
		"Output Data 53\000"
		"Output Data 54\000"
		"Output Data 55\000"
		"Output Data 56\000"
		"Output Data 57\000"
		"Output Data 58\000"
		"Output Data 59\000"
		"Output Data 60\000"
		"Output Data 61\000"
		"Output Data 62\000"
		"Output Data 63\000"
		"Output Data 64\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[64];  /**< \brief Subindex 1 - 64 */
} OBJ_STRUCT_PACKED_END
TOBJ7080;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ7080 Output_Data_70800x7080
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={64,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}
#endif
;
/** @}*/


#if 0
/******************************************************************************
*                    Object 0x8000 : Configuration_8000
******************************************************************************/
/**
* \addtogroup 0x8000 0x8000 | Configuration_8000
* @{
* \brief Object 0x8000 (Configuration_8000) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Register_Area_Selection_8000<br>
* SubIndex 2 - Number_of_Registers_8000<br>
* SubIndex 3 - Register_Area_Offset_8000<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x8000[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex1 - Register_Area_Selection_8000 */
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex2 - Number_of_Registers_8000 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }}; /* Subindex3 - Register_Area_Offset_8000 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x8000[] = "Configuration 1\000"
"Register Area Selection\000"
"Number of Registers\000"
"Register Area Offset\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT8 Register_Area_Selection_8000; /* Subindex1 - Register_Area_Selection_8000 */
UINT8 Number_of_Registers_8000; /* Subindex2 - Number_of_Registers_8000 */
UINT32 Register_Area_Offset_8000; /* Subindex3 - Register_Area_Offset_8000 */
} OBJ_STRUCT_PACKED_END
TOBJ8000;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ8000 Configuration_80000x8000
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={3,11,0,0x00000000}
#endif
;
/** @}*/
#endif
/******************************************************************************
*                    Object 0x8000 : Configuration_8000
******************************************************************************/
/**
* \addtogroup 0x8000 0x8000 | Configuration_8000
* @{
* \brief Object 0x8000 (Configuration_8000) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - SafeParameter1<br>
* SubIndex 2 - SafeParameter2<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x8000[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex1 - SafeParameter1 */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }}; /* Subindex2 - SafeParameter2 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x8000[] = "SafetyParams\000"
		"SafeParameter1\000"
		"SafeParameter2\000\377";
#endif //#ifdef _OBJD_

/******************************************************************************
*                    Object 0x8000 : Configuration_8000 for 3057 plugin
******************************************************************************/
/**
* \addtogroup 0x8000 0x8000 | Configuration_8000
* @{
* \brief Object 0x8000 (Configuration_8000) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - SafeParameter1<br>
* SubIndex 2 - SafeParameter2<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x8000_5[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
#if 0
	{ 2049 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
	{ 2049 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
	{ 2049 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
	{ 2049 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
	{ 2049 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
	{ 2049 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
	{ DEFTYPE_BIT4 , 0x4 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
	{ 2050 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
	{ 2050 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
	{ 2050 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
	{ 2050 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
	{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
	{ 2051 , 0x1 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
	{ 2051 , 0x1 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
	{ 2051 , 0x1 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
	{ 2051 , 0x1 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
	{ 2051 , 0x1 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
	{ 2051 , 0x1 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex18 */
	{ DEFTYPE_BIT2 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex19 */
	{ 2052 , 0x1 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex20 */
	{ 2052 , 0x1 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex21 */
	{ 2052 , 0x1 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex22 */
	{ 2052 , 0x1 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex23 */
	{ DEFTYPE_BIT4 , 0x4 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex24 */
	{ 2053 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex25 */
	{ 2053 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex26 */
	{ 2053 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex27 */
	{ 2053 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex28 */
	{ 2053 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex29 */
	{ 2053 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex30 */
#else
{ DEFTYPE_BIT2 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex1 */
{ DEFTYPE_BIT2 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex2 */
{ DEFTYPE_BIT2 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex3 */
{ DEFTYPE_BIT2 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex4 */
{ DEFTYPE_BIT2 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex5 */
{ DEFTYPE_BIT2 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex6 */
{ DEFTYPE_BIT4 , 0x4 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex7 */
{ DEFTYPE_BIT2 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex8 */
{ DEFTYPE_BIT2 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex9 */
{ DEFTYPE_BIT2 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex10 */
{ DEFTYPE_BIT2 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex11 */
{ DEFTYPE_BYTE , 0x8 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex12 */
{ DEFTYPE_BIT1 , 0x1 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex13 */
{ DEFTYPE_BIT1 , 0x1 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex14 */
{ DEFTYPE_BIT1 , 0x1 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex15 */
{ DEFTYPE_BIT1 , 0x1 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex16 */
{ DEFTYPE_BIT1 , 0x1 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex17 */
{ DEFTYPE_BIT1 , 0x1 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex18 */
{ DEFTYPE_BIT2 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex19 */
{ DEFTYPE_BIT1 , 0x1 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex20 */
{ DEFTYPE_BIT1 , 0x1 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex21 */
{ DEFTYPE_BIT1 , 0x1 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex22 */
{ DEFTYPE_BIT1 , 0x1 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex23 */
{ DEFTYPE_BIT4 , 0x4 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex24 */
{ DEFTYPE_BIT2 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex25 */
{ DEFTYPE_BIT2 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex26 */
{ DEFTYPE_BIT2 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex27 */
{ DEFTYPE_BIT2 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex28 */
{ DEFTYPE_BIT2 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex29 */
{ DEFTYPE_BIT2 , 0x2 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex30 */
#endif
	{ DEFTYPE_BIT4 , 0x4 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }}; /* Subindex31 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x8000_5[] = "SafetyParams\000"
		"Safe Input 1 Mode\000"
		"Safe Input 2 Mode\000"
		"Safe Input 3 Mode\000"
		"Safe Input 4 Mode\000"
		"Safe Input 5 Mode\000"
		"Safe Input 6 Mode\000"
		"Dummy Parameter 1\000"
		"Safe Output 1 Mode\000"
		"Safe Output 2 Mode\000"
		"Safe Output 3 Mode\000"
		"Safe Output 4 Mode\000"
		"Dummy Parameter 2\000"
		"Check Input 1 Enable\000"
		"Check Input 2 Enable\000"
		"Check Input 3 Enable\000"
		"Check Input 4 Enable\000"
		"Check Input 5 Enable\000"
		"Check Input 6 Enable\000"
		"Dummy Parameter 3\000"
		"Check Output 1 Enable\000"
		"Check Output 2 Enable\000"
		"Check Output 3 Enable\000"
		"Check Output 4 Enable\000"
		"Dummy Parameter 4\000"
		"Safe Input 1 Filter Adj.\000"
		"Safe Input 2 Filter Adj.\000"
		"Safe Input 3 Filter Adj.\000"
		"Safe Input 4 Filter Adj.\000"
		"Safe Input 5 Filter Adj.\000"
		"Safe Input 6 Filter Adj.\000"
		"Dummy Parameter 5\000\377";
#endif //#ifdef _OBJD_


/** @}*/


/******************************************************************************
*                    Object 0x8001 : SAFE I/O Error 8001
******************************************************************************/
/**
* \addtogroup 0x8001 0x8001 | SafeIOerrors_8001
* @{
* \brief Object 0x8001 (SafeIOerrors_8001) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - SafeIOerror<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x8001[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_NOPDOMAPPING }}; /* Subindex1 - Safe I/O Error */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x8001[] = "Safe I/O Errors\000"
		"Safe I/O Error\000\377";
#endif //#ifdef _OBJD_


/******************************************************************************
*                    Object 0x8010 : Configuration_8010
******************************************************************************/
/**
* \addtogroup 0x8010 0x8010 | Configuration_8010
* @{
* \brief Object 0x8010 (Configuration_8010) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Register_Area_Selection_8010<br>
* SubIndex 2 - Number_of_Registers_8010<br>
* SubIndex 3 - Register_Area_Offset_8010<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x8010[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex1 - Register_Area_Selection_8010 */
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex2 - Number_of_Registers_8010 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }}; /* Subindex3 - Register_Area_Offset_8010 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x8010[] = "Configuration 1\000"
		"Register Area Selection\000"
		"Number of Registers\000"
		"Register Area Offset\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT8 Register_Area_Selection_8010; /* Subindex1 - Register_Area_Selection_8010 */
UINT8 Number_of_Registers_8010; /* Subindex2 - Number_of_Registers_8010 */
UINT32 Register_Area_Offset_8010; /* Subindex3 - Register_Area_Offset_8010 */
} OBJ_STRUCT_PACKED_END
TOBJ8010;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ8010 Configuration_80100x8010
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={3,11,0,0x00000000}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x8020 : Configuration_8020
******************************************************************************/
/**
* \addtogroup 0x8020 0x8020 | Configuration_8020
* @{
* \brief Object 0x8020 (Configuration_8020) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Register_Area_Selection_8020<br>
* SubIndex 2 - Number_of_Registers_8020<br>
* SubIndex 3 - Register_Area_Offset_8020<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x8020[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex1 - Register_Area_Selection_8020 */
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex2 - Number_of_Registers_8020 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }}; /* Subindex3 - Register_Area_Offset_8020 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x8020[] = "Configuration 2\000"
		"Register Area Selection\000"
		"Number of Registers\000"
		"Register Area Offset\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT8 Register_Area_Selection_8020; /* Subindex1 - Register_Area_Selection_8020 */
UINT8 Number_of_Registers_8020; /* Subindex2 - Number_of_Registers_8020 */
UINT32 Register_Area_Offset_8020; /* Subindex3 - Register_Area_Offset_8020 */
} OBJ_STRUCT_PACKED_END
TOBJ8020;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ8020 Configuration_80200x8020
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={3,11,0,0x00000000}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x8030 : Configuration_8030
******************************************************************************/
/**
* \addtogroup 0x8030 0x8030 | Configuration_8030
* @{
* \brief Object 0x8030 (Configuration_8030) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Register_Area_Selection_8030<br>
* SubIndex 2 - Number_of_Registers_8030<br>
* SubIndex 3 - Register_Area_Offset_8030<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x8030[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex1 - Register_Area_Selection_8030 */
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex2 - Number_of_Registers_8030 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }}; /* Subindex3 - Register_Area_Offset_8030 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x8030[] = "Configuration 3\000"
		"Register Area Selection\000"
		"Number of Registers\000"
		"Register Area Offset\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT8 Register_Area_Selection_8030; /* Subindex1 - Register_Area_Selection_8030 */
UINT8 Number_of_Registers_8030; /* Subindex2 - Number_of_Registers_8030 */
UINT32 Register_Area_Offset_8030; /* Subindex3 - Register_Area_Offset_8030 */
} OBJ_STRUCT_PACKED_END
TOBJ8030;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ8030 Configuration_80300x8030
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={3,11,0,0x00000000}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x8040 : Configuration_8040
******************************************************************************/
/**
* \addtogroup 0x8040 0x8040 | Configuration_8040
* @{
* \brief Object 0x8040 (Configuration_8040) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Register_Area_Selection_8040<br>
* SubIndex 2 - Number_of_Registers_8040<br>
* SubIndex 3 - Register_Area_Offset_8040<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x8040[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex1 - Register_Area_Selection_8040 */
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex2 - Number_of_Registers_8040 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }}; /* Subindex3 - Register_Area_Offset_8040 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x8040[] = "Configuration 4\000"
		"Register Area Selection\000"
		"Number of Registers\000"
		"Register Area Offset\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT8 Register_Area_Selection_8040; /* Subindex1 - Register_Area_Selection_8040 */
UINT8 Number_of_Registers_8040; /* Subindex2 - Number_of_Registers_8040 */
UINT32 Register_Area_Offset_8040; /* Subindex3 - Register_Area_Offset_8040 */
} OBJ_STRUCT_PACKED_END
TOBJ8040;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ8040 Configuration_80400x8040
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={3,11,0,0x00000000}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x8050 : Configuration_8050
******************************************************************************/
/**
* \addtogroup 0x8050 0x8050 | Configuration_8050
* @{
* \brief Object 0x8050 (Configuration_8050) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Register_Area_Selection_8050<br>
* SubIndex 2 - Number_of_Registers_8050<br>
* SubIndex 3 - Register_Area_Offset_8050<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x8050[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex1 - Register_Area_Selection_8050 */
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex2 - Number_of_Registers_8050 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }}; /* Subindex3 - Register_Area_Offset_8050 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x8050[] = "Configuration 5\000"
		"Register Area Selection\000"
		"Number of Registers\000"
		"Register Area Offset\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT8 Register_Area_Selection_8050; /* Subindex1 - Register_Area_Selection_8050 */
UINT8 Number_of_Registers_8050; /* Subindex2 - Number_of_Registers_8050 */
UINT32 Register_Area_Offset_8050; /* Subindex3 - Register_Area_Offset_8050 */
} OBJ_STRUCT_PACKED_END
TOBJ8050;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ8050 Configuration_80500x8050
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={3,11,0,0x00000000}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x8060 : Configuration_8060
******************************************************************************/
/**
* \addtogroup 0x8060 0x8060 | Configuration_8060
* @{
* \brief Object 0x8060 (Configuration_8060) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Register_Area_Selection_8060<br>
* SubIndex 2 - Number_of_Registers_8060<br>
* SubIndex 3 - Register_Area_Offset_8060<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x8060[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex1 - Register_Area_Selection_8060 */
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex2 - Number_of_Registers_8060 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }}; /* Subindex3 - Register_Area_Offset_8060 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x8060[] = "Configuration 6\000"
		"Register Area Selection\000"
		"Number of Registers\000"
		"Register Area Offset\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT8 Register_Area_Selection_8060; /* Subindex1 - Register_Area_Selection_8060 */
UINT8 Number_of_Registers_8060; /* Subindex2 - Number_of_Registers_8060 */
UINT32 Register_Area_Offset_8060; /* Subindex3 - Register_Area_Offset_8060 */
} OBJ_STRUCT_PACKED_END
TOBJ8060;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ8060 Configuration_80600x8060
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={3,11,0,0x00000000}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x8070 : Configuration_8070
******************************************************************************/
/**
* \addtogroup 0x8070 0x8070 | Configuration_8070
* @{
* \brief Object 0x8070 (Configuration_8070) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Register_Area_Selection_8070<br>
* SubIndex 2 - Number_of_Registers_8070<br>
* SubIndex 3 - Register_Area_Offset_8070<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x8070[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex1 - Register_Area_Selection_8070 */
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex2 - Number_of_Registers_8070 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }}; /* Subindex3 - Register_Area_Offset_8070 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x8070[] = "Configuration 7\000"
		"Register Area Selection\000"
		"Number of Registers\000"
		"Register Area Offset\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT8 Register_Area_Selection_8070; /* Subindex1 - Register_Area_Selection_8070 */
UINT8 Number_of_Registers_8070; /* Subindex2 - Number_of_Registers_8070 */
UINT32 Register_Area_Offset_8070; /* Subindex3 - Register_Area_Offset_8070 */
} OBJ_STRUCT_PACKED_END
TOBJ8070;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ8070 Configuration_80700x8070
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={3,11,0,0x00000000}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x8080 : Configuration_8080
******************************************************************************/
/**
* \addtogroup 0x8080 0x8080 | Configuration_8080
* @{
* \brief Object 0x8080 (Configuration_8080) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Register_Area_Selection_8080<br>
* SubIndex 2 - Number_of_Registers_8080<br>
* SubIndex 3 - Register_Area_Offset_8080<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x8080[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex1 - Register_Area_Selection_8080 */
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex2 - Number_of_Registers_8080 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }}; /* Subindex3 - Register_Area_Offset_8080 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x8080[] = "Configuration 8\000"
		"Register Area Selection\000"
		"Number of Registers\000"
		"Register Area Offset\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT8 Register_Area_Selection_8080; /* Subindex1 - Register_Area_Selection_8080 */
UINT8 Number_of_Registers_8080; /* Subindex2 - Number_of_Registers_8080 */
UINT32 Register_Area_Offset_8080; /* Subindex3 - Register_Area_Offset_8080 */
} OBJ_STRUCT_PACKED_END
TOBJ8080;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ8080 Configuration_80800x8080
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={3,11,0,0x00000000}
#endif
;
/** @}*/


/******************************************************************************
*                    Object 0x9001 : FSOE Communication Parameter
******************************************************************************/
/**
* \addtogroup 0x9001 0x9001 | FSOE Communication Parameter
* @{
* \brief Object 0x9001 (FSOE Communication Parameter) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Index_distance <br>
* SubIndex 2 - Maximum_number_of_modules <br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x9001[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_VISIBLESTRING , 0x10 , ACCESS_READ }, /* Subindex1 - Version  */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex2 - Safe Address  */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex3 - Connection ID  */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex4 - Watchdog Time  */
{ DEFTYPE_OCTETSTRING , 0x30 , ACCESS_READ }, /* Subindex5 - Unique ID  */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex6 - Connection Type  */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex7 - Communication Parameter Length  */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }}; /* Subindex8 - Application Parameter Length  */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x9001[] = "FSOE Communication Parameters\000"
"Version\000"
"Safe Address\000"
"Connection ID\000"
"Watchdog Time\000"
"Unique ID\000"
"Connection Type\000"
"Communication Parameter Length\000"
"Application Parameter Length\000\377";

#endif //#ifdef _OBJD_

/** @}*/

/******************************************************************************
*                    Object 0xF000 : Modular_Device_Profile
******************************************************************************/
/**
* \addtogroup 0xF000 0xF000 | Modular_Device_Profile
* @{
* \brief Object 0xF000 (Modular_Device_Profile) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Index_distance <br>
* SubIndex 2 - Maximum_number_of_modules <br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF000[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex1 - Index_distance  */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }}; /* Subindex2 - Maximum_number_of_modules  */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0xF000[] = "Modular Device Profile\000"
"Module index distance\000"
"Maximum number of modules\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT16 Index_distance; /* Subindex1 - Index_distance  */
UINT16 Maximum_number_of_modules; /* Subindex2 - Maximum_number_of_modules  */
} OBJ_STRUCT_PACKED_END
TOBJF000;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJF000 Modular_Device_Profile0xF000
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={2,0x0010,0x0008}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0xF030 : Slot_Configuration
******************************************************************************/
/**
* \addtogroup 0xF030 0xF030 | Slot_Configuration
* @{
* \brief Object 0xF030 (Slot_Configuration) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF030[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0xF030[] = "Slot_Configuration\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT32 aEntries[8];  /**< \brief Subindex 1 - 8 */
} OBJ_STRUCT_PACKED_END
TOBJF030;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJF030 Slot_Configuration0xF030
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={8,{0,0,0,0,0,0,0,0}}
#endif
;
/** @}*/

/******************************************************************************
*                    Object 0xF980 : FSOE Safe Address
******************************************************************************/
/**
* \addtogroup 0xF980 0xF980 | FSOE Safe Address
* @{
* \brief Object 0xF980 (FSOE Safe Address) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Index_distance <br>
* SubIndex 2 - Maximum_number_of_modules <br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF980[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex1 - FS0E Address  */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }}; /* Subindex2 - Serial Number  */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0xF980[] = "Safe Address\000"
"FSoE Address\000"
"Serial Number\000\377";

#endif //#ifdef _OBJD_

/** @}*/





PROTO UINT8 plc_read_2000(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess);
PROTO UINT8 plc_read_3000(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess);
PROTO UINT8 plc_read_4000(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess);
PROTO UINT8 plc_write_2000(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess);
PROTO UINT8 plc_write_3000(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess);
PROTO UINT8 plc_write_4000(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess);
PROTO UINT8 plc_read_600x(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM* pData, UINT8 bCompleteAccess);
PROTO UINT8 plc_read_700x(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM* pData, UINT8 bCompleteAccess);


#ifdef _OBJD_
#ifdef FSOE_OBJ_DYNAMIC_ASSIGN
TOBJECT OBJ1600_5 = {NULL , NULL ,  0x1600 , {DEFTYPE_PDOMAPPING , 14 | (OBJCODE_REC << 8)} , asEntryDesc0x1600_5 , aName0x1600_5 , &RxPDO_mapping_16000x1600_5 , NULL , NULL , 0x0000 };
TOBJECT OBJ1A00_5 = {NULL , NULL ,  0x1A00 , {DEFTYPE_PDOMAPPING , 18 | (OBJCODE_REC << 8)} , asEntryDesc0x1A00_5 , aName0x1A00_5 , &TxPDO_mapping_1A000x1A00_5 , NULL , NULL , 0x0000 };
TOBJECT OBJ6001_5 = {NULL , NULL ,  0x6001 , {DEFTYPE_RECORD , 12 | (OBJCODE_REC << 8)} , asEntryDesc0x6001_5 , aName0x6001_5 , &Obj.FSOE_Slave_Safety_Inputs_NrofIndexes , plc_read_600x , NULL , 0x0000 };
TOBJECT OBJ7001_5 = {NULL , NULL ,  0x7001 , {DEFTYPE_RECORD , 8 | (OBJCODE_REC << 8)} , asEntryDesc0x7001_5 , aName0x7001_5 , &Obj.FSOE_Master_Safety_Outputs_NrofIndexes , plc_read_700x , NULL , 0x0000 };
TOBJECT OBJ8000_5 = {NULL , NULL ,  0x8000 , {DEFTYPE_RECORD , 31 | (OBJCODE_REC << 8)} , asEntryDesc0x8000_5 , aName0x8000_5 , &Obj.Safe_Params_NrofIndexes , NULL , NULL , 0x0000 };
#endif

TOBJECT    OBJMEM ApplicationObjDic[] = {
/* Object 0x1600 */
#ifdef FSOE_OBJ_DYNAMIC_ASSIGN
{NULL , NULL ,  0x1600 , {DEFTYPE_PDOMAPPING , 18 | (OBJCODE_REC << 8)} , asEntryDesc0x1600 , aName0x1600 , &RxPDO_mapping_16000x1600 , NULL , NULL , 0x0000 },
#else
{NULL , NULL ,  0x1600 , {DEFTYPE_PDOMAPPING , 14 | (OBJCODE_REC << 8)} , asEntryDesc0x1600_5 , aName0x1600_5 , &RxPDO_mapping_16000x1600_5 , NULL , NULL , 0x0000 },
#endif
/* Object 0x1601 */
{NULL , NULL ,  0x1601 , {DEFTYPE_PDOMAPPING , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x1601 , aName0x1601 , &RxPDO_mapping_16010x1601 , NULL , NULL , 0x0000 },
/* Object 0x1602 */
{NULL , NULL ,  0x1602 , {DEFTYPE_PDOMAPPING , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x1602 , aName0x1602 , &RxPDO_mapping_16020x1602 , NULL , NULL , 0x0000 },
/* Object 0x1603 */
{NULL , NULL ,  0x1603 , {DEFTYPE_PDOMAPPING , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x1603 , aName0x1603 , &RxPDO_mapping_16030x1603 , NULL , NULL , 0x0000 },
/* Object 0x1604 */
{NULL , NULL ,  0x1604 , {DEFTYPE_PDOMAPPING , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x1604 , aName0x1604 , &RxPDO_mapping_16040x1604 , NULL , NULL , 0x0000 },
/* Object 0x1605 */
{NULL , NULL ,  0x1605 , {DEFTYPE_PDOMAPPING , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x1605 , aName0x1605 , &RxPDO_mapping_16050x1605 , NULL , NULL , 0x0000 },
/* Object 0x1606 */
{NULL , NULL ,  0x1606 , {DEFTYPE_PDOMAPPING , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x1606 , aName0x1606 , &RxPDO_mapping_16060x1606 , NULL , NULL , 0x0000 },
/* Object 0x1607 */
{NULL , NULL ,  0x1607 , {DEFTYPE_PDOMAPPING , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x1607 , aName0x1607 , &RxPDO_mapping_16070x1607 , NULL , NULL , 0x0000 },
/* Object 0x1608 */
{NULL , NULL ,  0x1608 , {DEFTYPE_PDOMAPPING , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x1608 , aName0x1608 , &RxPDO_mapping_16080x1608 , NULL , NULL , 0x0000 },
/* Object 0x1A00 */
#ifdef FSOE_OBJ_DYNAMIC_ASSIGN
{NULL , NULL ,  0x1A00 , {DEFTYPE_PDOMAPPING , 18 | (OBJCODE_REC << 8)} , asEntryDesc0x1A00 , aName0x1A00 , &TxPDO_mapping_1A000x1A00 , NULL , NULL , 0x0000 },
#else
{NULL , NULL ,  0x1A00 , {DEFTYPE_PDOMAPPING , 18 | (OBJCODE_REC << 8)} , asEntryDesc0x1A00_5 , aName0x1A00_5 , &TxPDO_mapping_1A000x1A00_5 , NULL , NULL , 0x0000 },
#endif
/* Object 0x1A01 */
{NULL , NULL ,  0x1A01 , {DEFTYPE_PDOMAPPING , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x1A01 , aName0x1A01 , &TxPDO_mapping_1A010x1A01 , NULL , NULL , 0x0000 },
/* Object 0x1A02 */
{NULL , NULL ,  0x1A02 , {DEFTYPE_PDOMAPPING , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x1A02 , aName0x1A02 , &TxPDO_mapping_1A020x1A02 , NULL , NULL , 0x0000 },
/* Object 0x1A03 */
{NULL , NULL ,  0x1A03 , {DEFTYPE_PDOMAPPING , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x1A03 , aName0x1A03 , &TxPDO_mapping_1A030x1A03 , NULL , NULL , 0x0000 },
/* Object 0x1A04 */
{NULL , NULL ,  0x1A04 , {DEFTYPE_PDOMAPPING , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x1A04 , aName0x1A04 , &TxPDO_mapping_1A040x1A04 , NULL , NULL , 0x0000 },
/* Object 0x1A05 */
{NULL , NULL ,  0x1A05 , {DEFTYPE_PDOMAPPING , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x1A05 , aName0x1A05 , &TxPDO_mapping_1A050x1A05 , NULL , NULL , 0x0000 },
/* Object 0x1A06 */
{NULL , NULL ,  0x1A06 , {DEFTYPE_PDOMAPPING , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x1A06 , aName0x1A06 , &TxPDO_mapping_1A060x1A06 , NULL , NULL , 0x0000 },
/* Object 0x1A07 */
{NULL , NULL ,  0x1A07 , {DEFTYPE_PDOMAPPING , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x1A07 , aName0x1A07 , &TxPDO_mapping_1A070x1A07 , NULL , NULL , 0x0000 },
/* Object 0x1A08 */
{NULL , NULL ,  0x1A08 , {DEFTYPE_PDOMAPPING , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x1A08 , aName0x1A08 , &TxPDO_mapping_1A080x1A08 , NULL , NULL , 0x0000 },
/* Object 0x1C12 */
{NULL , NULL ,  0x1C12 , {DEFTYPE_UNSIGNED16 , 9 | (OBJCODE_ARR << 8)} , asEntryDesc0x1C12 , aName0x1C12 , &sRxPDOassign , NULL , NULL , 0x0000 },
/* Object 0x1C13 */
{NULL , NULL ,  0x1C13 , {DEFTYPE_UNSIGNED16 , 9 | (OBJCODE_ARR << 8)} , asEntryDesc0x1C13 , aName0x1C13 , &sTxPDOassign , NULL , NULL , 0x0000 },
/* Object 0x2001 */
{NULL , NULL ,  0x2001 , {DEFTYPE_VISIBLESTRING , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x2001 , aName0x2001 , &Component_Name0x2001 , plc_read_2000 , plc_write_2000 , 0x0000 },
/* Object 0x2002 */
{NULL , NULL ,  0x2002 , {DEFTYPE_VISIBLESTRING , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x2002 , aName0x2002 , &Vendor_Name0x2002 , plc_read_2000 , plc_write_2000 , 0x0000 },
/* Object 0x2003 */
{NULL , NULL ,  0x2003 , {DEFTYPE_VISIBLESTRING , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x2003 , aName0x2003 , &Vendor_URL0x2003 , plc_read_2000 , plc_write_2000 , 0x0000 },
/* Object 0x2004 */
{NULL , NULL ,  0x2004 , {DEFTYPE_VISIBLESTRING , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x2004 , aName0x2004 , &Order_Number0x2004 , plc_read_2000 , plc_write_2000 , 0x0000 },
/* Object 0x2005 */
{NULL , NULL ,  0x2005 , {DEFTYPE_VISIBLESTRING , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x2005 , aName0x2005 , &Manufacturing_Date0x2005 , plc_read_2000 , plc_write_2000 , 0x0000 },
/* Object 0x2006 */
{NULL , NULL ,  0x2006 , {DEFTYPE_VISIBLESTRING , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x2006 , aName0x2006 , &QS_Date0x2006 , plc_read_2000 , plc_write_2000 , 0x0000 },
/* Object 0x2007 */
{NULL , NULL ,  0x2007 , {DEFTYPE_VISIBLESTRING , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x2007 , aName0x2007 , &Installation_Location0x2007 , plc_read_2000 , plc_write_2000 , 0x0000 },
/* Object 0x2008 */
{NULL , NULL ,  0x2008 , {DEFTYPE_UNSIGNED32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x2008 , aName0x2008 , &Operational_Hours0x2008 , plc_read_2000 , plc_write_2000 , 0x0000 },
/* Object 0x2009 */
{NULL , NULL ,  0x2009 , {DEFTYPE_VISIBLESTRING , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x2009 , aName0x2009 , &Service_Date0x2009 , plc_read_2000 , plc_write_2000 , 0x0000 },
/* Object 0x200A */
{NULL , NULL ,  0x200A , {DEFTYPE_VISIBLESTRING , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x200A , aName0x200A , &Equipment_Ident0x200A , plc_read_2000 , plc_write_2000 , 0x0000 },
/* Object 0x3740 */
{NULL , NULL ,  0x3740 , {DEFTYPE_INTEGER16 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x3740 , aName0x3740 , &Station_Alias_Setup_High0x3740 , plc_read_3000 , plc_write_3000 , 0x0000 },
/* Object 0x3741 */
{NULL , NULL ,  0x3741 , {DEFTYPE_INTEGER16 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x3741 , aName0x3741 , &Station_Alias_Selection0x3741 , plc_read_3000 , plc_write_3000 , 0x0000 },
/* Object 0x4000 */
{NULL , NULL ,  0x4000 , {DEFTYPE_RECORD , 69 | (OBJCODE_REC << 8)} , asEntryDesc0x4000 , aName0x4000 , &Service_Data_Request0x4000 , NULL , NULL , 0x0000 },
/* Object 0x4001 */
{NULL , NULL ,  0x4001 , {DEFTYPE_RECORD , 69 | (OBJCODE_REC << 8)} , asEntryDesc0x4001 , aName0x4001 , &Service_Data_Response0x4001 , NULL , NULL , 0x0000 },
/* Object 0x6000 */
{NULL , NULL ,  0x6000 , {DEFTYPE_RECORD , 4 | (OBJCODE_REC << 8)} , asEntryDesc0x6000 , aName0x6000 , &Obj.FSOE_Slave_Frame_Elements_NrofIndexes , plc_read_600x , NULL , 0x0000 },
/* Object 0x6001 */
#ifdef FSOE_OBJ_DYNAMIC_ASSIGN
{NULL , NULL ,  0x6001 , {DEFTYPE_RECORD , 13 | (OBJCODE_REC << 8)} , asEntryDesc0x6001 , aName0x6001 , &Obj.FSOE_Slave_Safety_Inputs_NrofIndexes , plc_read_600x , NULL , 0x0000 },
#else
{NULL , NULL ,  0x6001 , {DEFTYPE_RECORD , 12 | (OBJCODE_REC << 8)} , asEntryDesc0x6001_5 , aName0x6001_5 , &Obj.FSOE_Slave_Safety_Inputs_NrofIndexes , plc_read_600x , NULL , 0x0000 },
#endif
/* Object 0x6010 */
{NULL , NULL ,  0x6010 , {DEFTYPE_RECORD , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x6010 , aName0x6010 , &Input_Data_60100x6010 , NULL , NULL , 0x0000 },
/* Object 0x6020 */
{NULL , NULL ,  0x6020 , {DEFTYPE_RECORD , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x6020 , aName0x6020 , &Input_Data_60200x6020 , NULL , NULL , 0x0000 },
/* Object 0x6030 */
{NULL , NULL ,  0x6030 , {DEFTYPE_RECORD , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x6030 , aName0x6030 , &Input_Data_60300x6030 , NULL , NULL , 0x0000 },
/* Object 0x6040 */
{NULL , NULL ,  0x6040 , {DEFTYPE_RECORD , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x6040 , aName0x6040 , &Input_Data_60400x6040 , NULL , NULL , 0x0000 },
/* Object 0x6050 */
{NULL , NULL ,  0x6050 , {DEFTYPE_RECORD , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x6050 , aName0x6050 , &Input_Data_60500x6050 , NULL , NULL , 0x0000 },
/* Object 0x6060 */
{NULL , NULL ,  0x6060 , {DEFTYPE_RECORD , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x6060 , aName0x6060 , &Input_Data_60600x6060 , NULL , NULL , 0x0000 },
/* Object 0x6070 */
{NULL , NULL ,  0x6070 , {DEFTYPE_RECORD , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x6070 , aName0x6070 , &Input_Data_60700x6070 , NULL , NULL , 0x0000 },
/* Object 0x6080 */
{NULL , NULL ,  0x6080 , {DEFTYPE_RECORD , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x6080 , aName0x6080 , &Input_Data_60800x6080 , NULL , NULL , 0x0000 },
/* Object 0x7000 */
{NULL , NULL ,  0x7000 , {DEFTYPE_RECORD , 4 | (OBJCODE_REC << 8)} , asEntryDesc0x7000 , aName0x7000 , &Obj.FSOE_Master_Frame_Elements_NrofIndexes , plc_read_700x , NULL , 0x0000 },
/* Object 0x7001 */
#ifdef FSOE_OBJ_DYNAMIC_ASSIGN
{NULL , NULL ,  0x7001 , {DEFTYPE_RECORD , 13 | (OBJCODE_REC << 8)} , asEntryDesc0x7001 , aName0x7001 , &Obj.FSOE_Master_Safety_Outputs_NrofIndexes , plc_read_700x , NULL , 0x0000 },
#else
{NULL , NULL ,  0x7001 , {DEFTYPE_RECORD , 8 | (OBJCODE_REC << 8)} , asEntryDesc0x7001_5 , aName0x7001_5 , &Obj.FSOE_Master_Safety_Outputs_NrofIndexes , plc_read_700x , NULL , 0x0000 },
#endif
/* Object 0x7010 */
{NULL , NULL ,  0x7010 , {DEFTYPE_RECORD , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x7010 , aName0x7010 , &Output_Data_70100x7010 , NULL , NULL , 0x0000 },
/* Object 0x7020 */
{NULL , NULL ,  0x7020 , {DEFTYPE_RECORD , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x7020 , aName0x7020 , &Output_Data_70200x7020 , NULL , NULL , 0x0000 },
/* Object 0x7030 */
{NULL , NULL ,  0x7030 , {DEFTYPE_RECORD , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x7030 , aName0x7030 , &Output_Data_70300x7030 , NULL , NULL , 0x0000 },
/* Object 0x7040 */
{NULL , NULL ,  0x7040 , {DEFTYPE_RECORD , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x7040 , aName0x7040 , &Output_Data_70400x7040 , NULL , NULL , 0x0000 },
/* Object 0x7050 */
{NULL , NULL ,  0x7050 , {DEFTYPE_RECORD , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x7050 , aName0x7050 , &Output_Data_70500x7050 , NULL , NULL , 0x0000 },
/* Object 0x7060 */
{NULL , NULL ,  0x7060 , {DEFTYPE_RECORD , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x7060 , aName0x7060 , &Output_Data_70600x7060 , NULL , NULL , 0x0000 },
/* Object 0x7070 */
{NULL , NULL ,  0x7070 , {DEFTYPE_RECORD , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x7070 , aName0x7070 , &Output_Data_70700x7070 , NULL , NULL , 0x0000 },
/* Object 0x7080 */
{NULL , NULL ,  0x7080 , {DEFTYPE_RECORD , 64 | (OBJCODE_REC << 8)} , asEntryDesc0x7080 , aName0x7080 , &Output_Data_70800x7080 , NULL , NULL , 0x0000 },
/* Object 0x8000 */
#ifdef FSOE_OBJ_DYNAMIC_ASSIGN
{NULL , NULL ,  0x8000 , {DEFTYPE_RECORD , 2 | (OBJCODE_REC << 8)} , asEntryDesc0x8000 , aName0x8000 , &Obj.Safe_Params_NrofIndexes , NULL , NULL , 0x0000 },
#else
{NULL , NULL ,  0x8000 , {DEFTYPE_RECORD , 31 | (OBJCODE_REC << 8)} , asEntryDesc0x8000_5 , aName0x8000_5 , &Obj.Safe_Params_NrofIndexes , NULL , NULL , 0x0000 },
#endif
/* Object 0x8001 */
{NULL , NULL ,  0x8001 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x8001 , aName0x8001 , &Obj.SafeIOerror_NrofIndexes , NULL , NULL , 0x0000 },
/* Object 0x8010 */
{NULL , NULL ,  0x8010 , {DEFTYPE_RECORD , 3 | (OBJCODE_REC << 8)} , asEntryDesc0x8010 , aName0x8010 , &Configuration_80100x8010 , NULL , NULL , 0x0000 },
/* Object 0x8020 */
{NULL , NULL ,  0x8020 , {DEFTYPE_RECORD , 3 | (OBJCODE_REC << 8)} , asEntryDesc0x8020 , aName0x8020 , &Configuration_80200x8020 , NULL , NULL , 0x0000 },
/* Object 0x8030 */
{NULL , NULL ,  0x8030 , {DEFTYPE_RECORD , 3 | (OBJCODE_REC << 8)} , asEntryDesc0x8030 , aName0x8030 , &Configuration_80300x8030 , NULL , NULL , 0x0000 },
/* Object 0x8040 */
{NULL , NULL ,  0x8040 , {DEFTYPE_RECORD , 3 | (OBJCODE_REC << 8)} , asEntryDesc0x8040 , aName0x8040 , &Configuration_80400x8040 , NULL , NULL , 0x0000 },
/* Object 0x8050 */
{NULL , NULL ,  0x8050 , {DEFTYPE_RECORD , 3 | (OBJCODE_REC << 8)} , asEntryDesc0x8050 , aName0x8050 , &Configuration_80500x8050 , NULL , NULL , 0x0000 },
/* Object 0x8060 */
{NULL , NULL ,  0x8060 , {DEFTYPE_RECORD , 3 | (OBJCODE_REC << 8)} , asEntryDesc0x8060 , aName0x8060 , &Configuration_80600x8060 , NULL , NULL , 0x0000 },
/* Object 0x8070 */
{NULL , NULL ,  0x8070 , {DEFTYPE_RECORD , 3 | (OBJCODE_REC << 8)} , asEntryDesc0x8070 , aName0x8070 , &Configuration_80700x8070 , NULL , NULL , 0x0000 },
/* Object 0x8080 */
{NULL , NULL ,  0x8080 , {DEFTYPE_RECORD , 3 | (OBJCODE_REC << 8)} , asEntryDesc0x8080 , aName0x8080 , &Configuration_80800x8080 , NULL , NULL , 0x0000 },
/* Object 0x9001 */
{NULL , NULL ,  0x9001 , {DEFTYPE_RECORD , 8 | (OBJCODE_REC << 8)} , asEntryDesc0x9001 , aName0x9001 , &Obj.FSOE_Communication_Parameter_NrofIndexes , NULL , NULL , 0x0000 },
/* Object 0xF000 */
{NULL , NULL ,  0xF000 , {DEFTYPE_RECORD , 2 | (OBJCODE_REC << 8)} , asEntryDesc0xF000 , aName0xF000 , &Modular_Device_Profile0xF000 , NULL , NULL , 0x0000 },
/* Object 0xF030 */
{NULL , NULL ,  0xF030 , {DEFTYPE_UNSIGNED32 , 8 | (OBJCODE_ARR << 8)} , asEntryDesc0xF030 , aName0xF030 , &Slot_Configuration0xF030 , NULL , NULL , 0x0000 },
/* Object 0xF980 */
{NULL , NULL ,  0xF980 , {DEFTYPE_RECORD , 2 | (OBJCODE_REC << 8)} , asEntryDesc0xF980 , aName0xF980 , &Obj.Safe_Address_NrofIndexes , NULL , NULL , 0x0000 },
{NULL,NULL, 0xFFFF, {0, 0}, NULL, NULL, NULL, NULL}};
#endif    //#ifdef _OBJD_
#undef PROTO

/** @}*/
#define _SSC_DEVICE_OBJECTS_H_

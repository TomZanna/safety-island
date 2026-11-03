#include <stdio.h>
#include <memory.h>
#include <string.h>

#include "ecat_def.h"
#include "sdoserv.h"
#include "mcihw.h"

#define MIN_SERVICE_DATA_ITEMS  1
#define MAX_SERVICE_DATA_ITEMS  64

#define MIN_REGISTER_OFFSET 0
#define MAX_REGISTER_OFFSET 999999

#define MIN_VALUE_SUBINDEX  6   // starting subindex for data values
#define MAX_VALUE_SUBINDEX  69  // ending subindex
#define NUMBER_OF_ENTRIES   69  // total count of subindices used

#define REQUEST_OPERATION_NO_CONTROL    0
#define REQUEST_OPERATION_READ  1
#define REQUEST_OPERATION_WRITE 2

#define RESPONSE_STATUS_NO_RESPONSE_DATA    0
#define RESPONSE_STATUS_SUCCESS 1
#define RESPONSE_STATUS_FAILED  2
#define RESPONSE_STATUS_PROCESSING  3

#define RESPONSE_ERROR_CODE_NO_ERROR    0
#define RESPONSE_ERROR_CODE_INVALID_DATA_TYPE   1
#define RESPONSE_ERROR_CODE_WRITE_PROTECTED 2
#define RESPONSE_ERROR_CODE_INVALID_ADDRESS 3
#define RESPONSE_ERROR_CODE_COMMUNICATION_ERROR 4
#define RESPONSE_ERROR_CODE_EMPTY_DATA_SIZE 5
#define RESPONSE_ERROR_CODE_OTHER_ERROR 255



extern int gi_queued_command;

void plc_commit_4000()
{
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
UINT8 plc_read_4000(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM* pData, UINT8 bCompleteAccess)
{
    return 0;
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
//UINT8 _plc_write_4000(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM* pData, UINT8 bCompleteAccess)
UINT8 plc_write_4000(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM* pData, UINT8 bCompleteAccess)
{
    if (gi_debug & DBG_DEFAULT)
		printf(DBGFMT "index:%d, sub:%d, C:%d L:%d\n", DBGARGS, index, subindex, bCompleteAccess, dataSize);

	switch (index)
    {
    default:
        return ABORTIDX_OBJECT_NOT_EXISTING;
    }

    return 0;
}

#if 0
UINT8 plc_write_4000(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM* pData, UINT8 bCompleteAccess)
{
	UINT8 res;
	if (bCompleteAccess) printf("index=%d sub=%d\n", index, subindex);
	res = _plc_write_4000(index, subindex, dataSize, pData, bCompleteAccess);
	if (bCompleteAccess) printf("res=%d\n", res);
}
#endif

#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ecat_def.h"
#include "sdoserv.h"
#include "ini_file.h"
#include "mcihw.h"

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
UINT8 plc_read_2000(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM* pData, UINT8 bCompleteAccess)
{
    if (gi_debug & DBG_DEFAULT)
        printf(DBGFMT "%x.%x %x\n", DBGARGS, index, subindex, dataSize);

	if (bCompleteAccess)
	{
		 /* Complete Access is not supported for object 0x10F8 */
		return ABORTIDX_UNSUPPORTED_ACCESS;
	}
	if (subindex == 0)
    {
        switch (index)
        {
        case 0x2001:    // Component Name
        case 0x2002:    // Vendor Name
        case 0x2003:    // Vendor URL
        case 0x2004:    // Order Number
        case 0x2005:    // Manufacturing Date
        case 0x2006:    // QS Date
        case 0x2007:    // Installation Location
        case 0x2009:    // Service Date
        case 0x200A:    // Equipment Ident
        {
            char str_key[8] = { '\0' };
            char* str_value = NULL;

            sprintf(str_key, "0x%04x", index);
            str_value = ini_get_value(str_key);
            if (str_value)
            {
                strncpy((char*)pData, str_value, dataSize);
            }
            else
            {
                return ABORTIDX_GENERAL_ERROR;
            }
        }
        break;

        case 0x2008:    // Operational Hours
        {
            UINT32 n_hours = 0;
            int i_file = open("/sys/devices/platform/working_hours/working_hours/sys_hours", O_RDONLY);
            if (i_file != -1)
            {
                char str_value[16] = { 0 };
                if (read(i_file, str_value, 16) > 0)
                {
                    n_hours = strtoul(str_value, NULL, 0);
                }
                else
                {
                    // failed to read from file
                }

                close(i_file);
                i_file = -1;
            }
            else
            {
                // failed to open file
            }

            memcpy((void*)pData, &n_hours, sizeof(n_hours));
        }
        break;

        default:
            return ABORTIDX_OBJECT_NOT_EXISTING;
        }
    }
    else
    {
        return ABORTIDX_SUBINDEX_NOT_EXISTING;
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
UINT8 plc_write_2000(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM* pData, UINT8 bCompleteAccess)
{
    if (gi_debug & DBG_DEFAULT)
        printf(DBGFMT "\n", DBGARGS);

	if (bCompleteAccess)
	{
		 /* Complete Access is not supported for object 0x10F8 */
		return ABORTIDX_UNSUPPORTED_ACCESS;
	}
	if (subindex == 0)
    {
        switch (index)
        {
        case 0x2001:    // Component Name
        case 0x2002:    // Vendor Name
        case 0x2003:    // Vendor URL
        case 0x2004:    // Order Number
        case 0x2005:    // Manufacturing Date
        case 0x2006:    // QS Date
        case 0x2008:    // Operational Hours
        case 0x2009:    // Service Date
            return ABORTIDX_READ_ONLY_ENTRY;

        case 0x2007:    // Installation Location
        case 0x200A:    // Equipment Ident
        {
            char str_key[8] = { '\0' };
			char tmpStr[65];
			UINT32 len = (dataSize < sizeof(tmpStr)-1)?dataSize : sizeof(tmpStr)-1;
			strncpy(tmpStr, (const char*)pData, len);
			tmpStr[len] = '\0';
			sprintf(str_key, "0x%04x", index);
			ini_set_value(str_key, tmpStr);
            ini_write();
        }
        break;

        default:
            return ABORTIDX_OBJECT_NOT_EXISTING;
        }
    }
    else
    {
        return ABORTIDX_SUBINDEX_NOT_EXISTING;
    }

    return 0;
}

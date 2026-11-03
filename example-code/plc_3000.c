#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "ecat_def.h"
#include "sdoserv.h"
#include "ini_file.h"
#include "mcihw.h"

#define STATION_ALIAS_HIGH_0    0
#define STATION_ALIAS_SII   1

struct S_station_alias
{
    INT16 m_setup;
    INT16 m_selection;
} station_alias =
{
    0,
    STATION_ALIAS_SII
};

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
UINT8 plc_read_3000(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM* pData, UINT8 bCompleteAccess)
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
        case 0x3740:
            *(INT16*)pData = station_alias.m_setup;
            break;

        case 0x3741:
            *(INT16*)pData = station_alias.m_selection;
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
UINT8 plc_write_3000(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM* pData, UINT8 bCompleteAccess)
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
        case 0x3740:
        {
            INT16 i_value = *(INT16*)pData;
            if (i_value > 255)
            {
                return ABORTIDX_VALUE_TOO_GREAT;
            }
            else
            {
                char str_key[8] = { '\0' };
                char str_value[8] = { '\0' };

                station_alias.m_setup = i_value;

                sprintf(str_key, "0x%04x", index);
				sprintf(str_value, "%d", i_value);
                ini_set_value(str_key, (const char*)str_value);
                ini_write();
            }
        }
            break;

        case 0x3741:
        {
            INT16 i_value = *(INT16*)pData;
            switch (i_value)
            {
            case STATION_ALIAS_HIGH_0:
            case STATION_ALIAS_SII:
            {
                char str_key[8] = { '\0' };
                char str_value[8] = { '\0' };

                station_alias.m_selection = i_value;

                sprintf(str_key, "0x%04x", index);
				sprintf(str_value, "%d", i_value);
                ini_set_value(str_key, (const char*)str_value);
                ini_write();
            }
                break;

            default:
                return ABORTIDX_PARAM_IS_INCOMPATIBLE;
            }
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

UINT16 initialize_station_alias()
{
    char* str_value = NULL;
    int i_selection = STATION_ALIAS_SII;    // this is default
    int i_alias_high = 0;
    UINT16 i_alias = 0;

    str_value = ini_get_value("0x3741");
    if (str_value)
    {
        sscanf(str_value, "%d", &i_selection);

		switch (i_selection)
        {
        case STATION_ALIAS_HIGH_0:
        case STATION_ALIAS_SII:
            // ok
			station_alias.m_selection = i_selection;
			break;

        default:
            if (gi_debug & DBG_DEFAULT)
                printf(DBGFMT "Bad station alias selection %d read from file, using default %d instead.\n", DBGARGS, i_selection, STATION_ALIAS_SII);
            i_selection = STATION_ALIAS_SII;
            break;
        }
	}
    else
    {
        if (gi_debug & DBG_DEFAULT)
            printf(DBGFMT "Unable to read station alias selection from file, using default %d.\n", DBGARGS, STATION_ALIAS_SII);
    }

	str_value = ini_get_value("0x3740");
	if (str_value)
	{
		i_alias_high = strtol(str_value, NULL, 0);
		//sscanf(str_value, "%d", &i_alias_high);
		if (i_alias_high < 256)
			station_alias.m_setup = i_alias_high;
	}
	switch (i_selection)
    {
    case STATION_ALIAS_HIGH_0:
		i_alias = (station_alias.m_setup << 8) | 0x00;
        break;

    case STATION_ALIAS_SII:
    {
        UINT16* p16 = (UINT16*)eeprom_buffer;
        i_alias = p16[0x4];
	}
    break;

    default:
        // should never get here
        break;
    }

    return i_alias;
}

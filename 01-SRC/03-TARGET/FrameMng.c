#include <stdint.h>
#include "../01-MAIN/BootloaderTypes.h"
#include "FrameMng.h"

static tsFrameSize tsFrameLength[MAX_FRAMES] = 
{
    {1,     0,      1},
    {2,     3,      67},
    {3,     2,      2},
    {4,     0,      1},
    {5,     7,      262},
    {6,     0,      1}
};

teOperationRetVal RxFrameHandler(tsUartFrm * UARTFrm, tsBootMsg * BootMsg)
{
    uint16_t u16i = 0;
    teOperationRetVal RetVal = eOperationSuccess;
    BootMsg->u8ID = *(UARTFrm->pu8RawData + 1);
    BootMsg->u16Length = ((uint16_t)*(UARTFrm->pu8RawData + 2) << 8) & 0xFF00;
    BootMsg->u16Length |= (uint16_t)*(UARTFrm->pu8RawData + 3) & 0xFF;
    for (u16i = 0; u16i < BootMsg->u16Length; u16i++)
    {
        BootMsg->u8Data[u16i] = *(UARTFrm->pu8RawData + u16i);
    }
    
    while ((u16i < MAX_FRAMES) && (BootMsg->u8ID != tsFrameLength[u16i].u8ID))
    {
        u16i++;
    }
    
    if (BootMsg->u8ID == tsFrameLength[u16i].u8ID)
    {
        if (BootMsg->u16Length > tsFrameLength[u16i].u16MaxLen)
        {
            RetVal = eBadFrameLength;
        }
    }
    else
    {
        RetVal = eUnknownFrameID;
    }
    
    return RetVal;
}
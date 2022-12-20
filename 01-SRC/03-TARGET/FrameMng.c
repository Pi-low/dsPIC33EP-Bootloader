#include <stdint.h>
#include "../01-MAIN/BootloaderTypes.h"
#include "FrameMng.h"

static tsFrameSize tsFrameLength[REGISTERED_FRAMES] = 
{
    {1,     0,      1},
    {2,     3,      67},
    {3,     2,      2},
    {4,     0,      1},
    {5,     7,      262},
    {6,     0,      1}
};

teOperationRetVal RxFrameHandler(tsUartFrm * UARTFrm, tsGenericMsg * FpGenMsg)
{
    uint16_t u16i = 0;
    uint16_t u16j = 0;
    teOperationRetVal RetVal = eOperationSuccess;
    uint8_t u8Checksum = 0;

    if (UARTFrm->u16Index < 3)
    {
        /* Waiting for more data */
        RetVal = eOperationNotAvailable;
    }
    
    if (RetVal == eOperationSuccess)
    {
        /* Check frame length */
        FpGenMsg->u8ID = UARTFrm->pu8Data[1];
        FpGenMsg->u16Length = ((uint16_t) UARTFrm->pu8Data[2] << 8) & 0xFF00;
        FpGenMsg->u16Length |= (uint16_t) UARTFrm->pu8Data[3] & 0xFF;
        if (FpGenMsg->u16Length > MAX_FRM_LEN)
        {
            /* Payload will be too long */
            RetVal = eBadFrameLength;
        }
        else
        {
            u16i = 0;
            /* Check ID and payload length */
            while ((u16i < REGISTERED_FRAMES) && (FpGenMsg->u8ID != tsFrameLength[u16i].u8ID))
            {
                /* Look for registered ID list */
                u16i++;
            }

            if (FpGenMsg->u8ID == tsFrameLength[u16i].u8ID)
            {
                if (FpGenMsg->u16Length > tsFrameLength[u16i].u16MaxLen)
                {
                    RetVal = eBadFrameLength;
                }
            }
            else
            {
                RetVal = eUnknownFrameID;
            }
        }
    }
    
    if (RetVal == eOperationSuccess)
    {
        if (UARTFrm->u16Index == (FpGenMsg->u16Length + 3))
        {
            /* Data fully received */
            u16j = 3;
            for (u16i = 0; u16i < FpGenMsg->u16Length; u16i++) /* copy payload */
            {
                FpGenMsg->pu8Data[u16i] = UARTFrm->pu8Data[u16j];
                u8Checksum += FpGenMsg->pu8Data[u16i];
                u16j++;
            }
            if (u8Checksum != 0xFF)
            {
                RetVal = eBadChecksum;
            }
        }
        else if(UARTFrm->u16Index > (FpGenMsg->u16Length + 3))
        {
            RetVal = eOperationFail;
        }
        else
        {
            /* Continue to receive data  */
            RetVal = eOperationNotAvailable;
        }
    }
    return RetVal;
}

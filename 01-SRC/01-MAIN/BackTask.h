/* 
 * File:   BackTask.h
 * Author: Nello
 *
 * Created on 12 juillet 2022, 21:22
 */

#ifndef BACKTASK_H
#define	BACKTASK_H

#define MAX_FRM_LEN 261u

enum
{
    eBackTask_Idle = 0,
    eBackTask_StartOfFrame = 1,
    eBackTask_Data = 2
};

typedef struct
{
    uint8_t ID;
    uint16_t Length;
    uint8_t *Data;
    uint16_t CRC;
}UARTmsg_t;



void InitBackTask(void);
void ManageBackTask(void);
extern UARTmsg_t RxMsg, TxMsg;

#endif	/* BACKTASK_H */


#ifndef TARGET_H
#define TARGET_H

#define UARTTxBufferSize 256

typedef struct
{
    int bufferTail;
    int bufferQueue;
}CircularBuffer;

#endif
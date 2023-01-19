/* 
 * File:   Misc.h
 * Author: Nello
 *
 * Created on January 15, 2023, 2:27 PM
 */

#ifndef MISC_H
#define	MISC_H

#include <xc.h>
#include <stdint.h>

void BlockingDelay(uint16_t u16Timeout);
void WatchdogEnable(void);
void WatchdogDisable(void);

#endif	/* MISC_H */


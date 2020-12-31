#ifndef __PL_WIRELESS_H
#define __PL_WIRELESS_H

#include "headfile.h"
#include "pl_headfile.h"

void wireless_init(void); //包括无线传输方面的和PIT方面的
void send_ad_and_duty(void); //传输AD和舵机转角

#endif
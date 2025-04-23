#include "target_test.h"

#define MAIN_PRIORITY 5

#define HIGH_PRIORITY 9
#define MID_PRIORITY 10
#define LOW_PRIORITY 11

#ifndef STACK_SIZE
#define	STACK_SIZE 4096
#endif

#ifndef LOOP_REF
#define LOOP_REF ULONG_C(1000000)
#endif

#ifndef TOPPERS_MACRO_ONLY

// app.c
extern void	main_task(intptr_t exinf);
void init_ports();
void straight_deg(int deg);
void straight_mm(int mm);
void turn(int deg);
void turn_gyro(int deg);
int abs(int val);
void raise_percent(int per);
void raise_percent_no_sync(int per);
void lock_crane();
void unlock_crane();
void grab();
void release();

#endif
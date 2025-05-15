#pragma once

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

#include <stdio.h>

// rect_control.c
void straight_deg(int deg);
void straightInfinity();
void stop_both();
void straight_mm(int mm);

// app.c
extern void	main_task(intptr_t exinf);
void init_ports();
void turn(int deg);
void turn_gyro(int deg);
int abs(int val);
int get_ms();
void straight_until_black(int d, int white, int brake);

// arm.c
void grab();
void release();
void raise_percent(int per);
void raise_percent_no_sync(int per);

// cos_control.c
float cos_control_func(float s_min, float s_max, float x, float t);
int cos_speeding_straight(int s_min, int s_max, int d);
void cos_turn(int s_max, int d);
void cos_speeding_turn_clockwise(int s_max, unsigned int d);
void cos_speeding_turn_anticlockwise(int s_max, unsigned int d);

#endif
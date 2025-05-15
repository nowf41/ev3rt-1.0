#include "app.h"
#include "ports.h"

#include "ev3api.h"

// MUST BE FULLY OPENED BEFORE CALLING THIS
void grab() {
    // -100 1s
    ev3_motor_set_power(FINGER, -100);
    tslp_tsk(1000 * 1000);
}

void release() {
    ev3_motor_stop(FINGER, true);
    ev3_motor_set_power(FINGER, 100);
    tslp_tsk(1000 * 1000);
    ev3_motor_stop(FINGER, true);
}

void raise_percent(int per) {
    ev3_motor_rotate(CRANE, -3 * per, 100, true);
    ev3_motor_stop(CRANE, true);
}

void raise_percent_no_sync(int per) {
    ev3_motor_rotate(CRANE, -3 * per, 100, false);
}

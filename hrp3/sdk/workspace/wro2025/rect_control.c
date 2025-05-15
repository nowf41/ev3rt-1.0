#include "app.h"
#include "ports.h"

#include "ev3api.h"

void straight_deg(int deg) {
    ev3_motor_rotate(LEFT_MOTOR, deg, BASE_SPEED, false);
    ev3_motor_rotate(RIGHT_MOTOR, deg, BASE_SPEED, true);
    ev3_motor_stop(LEFT_MOTOR, true);
    ev3_motor_stop(RIGHT_MOTOR, true);
}

void straightInfinity() {
    ev3_motor_set_power(LEFT_MOTOR, 20);
    ev3_motor_set_power(RIGHT_MOTOR, 20);
}

void stop_both() {
    ev3_motor_stop(LEFT_MOTOR, true);
    ev3_motor_stop(RIGHT_MOTOR, true);
}

void straight_mm(int mm) {
    straight_deg((int)(((float)mm) / (TIRE_RADIUS * PI) * 360.0f));
}

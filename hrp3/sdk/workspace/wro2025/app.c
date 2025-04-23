#include "ev3api.h"
#include "app.h"
#include "ports.h"

void main_task(intptr_t unused) {
    init_ports();
    lock_crane();
    
    turn(-45);
    straight_mm(400);
    turn(-45);
    straight_mm(450);
    turn(-90);
    raise_percent(-100); // 0
    straight_mm(150);

    raise_percent(50); // 50
    straight_mm(100);
    // the case has been opened.
    straight_mm(-170);
    raise_percent_no_sync(0); // 55
    turn(-90);
    straight_mm(420);
    turn(90);
    straight_mm(100);
    // now: tank
    grab();
    raise_percent_no_sync(45); // 100
    straight_mm(-100);
    turn(100);
    straight_mm(400);
    turn(-90);
    straight_mm(200);
    raise_percent_no_sync(-30); // 70
    tslp_tsk(500 * 1000);
    release();
    
    raise_percent_no_sync(30); // 100
    tslp_tsk(1000 * 1000);
    straight_mm(-100);

    release();

    raise_percent(-100); // 0
    ev3_motor_rotate(FINGER, -180, 100, true);
    straight_mm(80);

    raise_percent(100);
}

void init_ports() {
    ev3_motor_config(LEFT_MOTOR, LEFT_MOTOR_TYPE);
    ev3_motor_config(RIGHT_MOTOR, RIGHT_MOTOR_TYPE);
    ev3_motor_config(CRANE, CRANE_TYPE);
    ev3_motor_config(FINGER, FINGER_TYPE);
    ev3_sensor_config(GYRO, GYRO_TYPE);
    ev3_sensor_config(GATE_COLOR, GATE_COLOR_TYPE);
    ev3_sensor_config(LEFT_COLOR, LEFT_COLOR_TYPE);
    ev3_sensor_config(RIGHT_COLOR, RIGHT_COLOR_TYPE);
    ev3_sensor_config(GATE_COLOR, GATE_COLOR_TYPE);
}

void straight_deg(int deg) {
    ev3_motor_rotate(LEFT_MOTOR, deg, BASE_SPEED, false);
    ev3_motor_rotate(RIGHT_MOTOR, deg, BASE_SPEED, true);
    ev3_motor_stop(LEFT_MOTOR, true);
    ev3_motor_stop(RIGHT_MOTOR, true);
    
}

void straightInfinity() {
    ev3_motor_set_power(LEFT_MOTOR, 50);
    ev3_motor_set_power(RIGHT_MOTOR, 50);
}

void stop_both() {
    ev3_motor_stop(LEFT_MOTOR, true);
    ev3_motor_stop(RIGHT_MOTOR, true);
}

void straight_mm(int mm) {
    straight_deg((int)(((float)mm) / (TIRE_RADIUS * PI) * 360.0f));
}

// rotates-control-based turn
void turn(int deg) {
    // pi * R * (deg / 360)dake mawasu
    int d = (int)((PI * TIRE_DISTANCE * (((float)deg) / 360.0f)) / (TIRE_RADIUS * PI) * 360.0f);
    ev3_motor_rotate(LEFT_MOTOR, d, TURN_BASE_SPEED, false);
    ev3_motor_rotate(RIGHT_MOTOR, -d, TURN_BASE_SPEED, true);
} 

// gyro-based turn
// NOT RECOMMENDED DUE TO OVERRUNNING
void turn_gyro(int deg) {
    int first_gyro = ev3_gyro_sensor_get_angle(GYRO);
    if (deg > 0) {
        ev3_motor_set_power(LEFT_MOTOR, BASE_SPEED / 3);
        ev3_motor_set_power(RIGHT_MOTOR, -BASE_SPEED / 3);
        while (ev3_gyro_sensor_get_angle(GYRO) - first_gyro < deg) {}
    } else {
        ev3_motor_set_power(LEFT_MOTOR, -BASE_SPEED / 3);
        ev3_motor_set_power(RIGHT_MOTOR, BASE_SPEED / 3);
        while (ev3_gyro_sensor_get_angle(GYRO) - first_gyro > deg) {}
    }
    ev3_motor_stop(LEFT_MOTOR, true);
    ev3_motor_stop(RIGHT_MOTOR, true);
}

int abs(int val) {
    if (val > 0) {
        return val;
    } else {
        return -val;
    }
}

void raise_percent(int per) {
    ev3_motor_rotate(CRANE, -3 * per, 100, true);
    ev3_motor_stop(CRANE, true);
}

void raise_percent_no_sync(int per) {
    ev3_motor_rotate(CRANE, -3 * per, 100, false);
}

void lock_crane() {
    ev3_motor_stop(CRANE, true);
}

void unlock_crane() {
    ev3_motor_stop(CRANE, false);
}

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
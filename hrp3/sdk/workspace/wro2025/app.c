#include "ev3api.h"

#include "app.h"
#include "ports.h"

#include <stdio.h>
#include <stdbool.h>

void main_task(intptr_t unused) { 
    init_ports();

    cos_speeding_straight(0, 50, 3000);
    // from
    cos_speeding_straight(50, 0, 3000);
    // fin
    // 0xdeefbeef will begin in this.

    return;

    straight_until_black(50, false, true);
    turn(-90);
    straight_until_black(50, true, false);
    tslp_tsk(500 * 1000);
    straight_until_black(50, true, true);
    turn(-90);
    straight_until_black(50, false, true);
    raise_percent_no_sync(50);
    straightInfinity();
    tslp_tsk(500 * 1000);
    stop_both();

}

void straight_until_black(int d, int white, int brake) {
    int deg = d * 360 / (TIRE_RADIUS * PI);

    cos_speeding_straight(0, 40, deg);
    if (white) while (ev3_color_sensor_get_reflect(GROUND_COLOR) < 70); // detect white
    while (ev3_color_sensor_get_reflect(GROUND_COLOR) > 40); // detect black
    if (brake) cos_speeding_straight(40, 0, deg);
}

void init_ports() {
    ev3_motor_config(LEFT_MOTOR, LEFT_MOTOR_TYPE);
    ev3_motor_config(RIGHT_MOTOR, RIGHT_MOTOR_TYPE);
    ev3_motor_config(CRANE, CRANE_TYPE);
    ev3_motor_config(FINGER, FINGER_TYPE);
    ev3_sensor_config(GYRO, GYRO_TYPE);
    ev3_sensor_config(GROUND_COLOR, GATE_COLOR_TYPE);
    //ev3_sensor_config(LEFT_COLOR, LEFT_COLOR_TYPE);
    //ev3_sensor_config(RIGHT_COLOR, RIGHT_COLOR_TYPE);
    ev3_sensor_config(GROUND_COLOR, GATE_COLOR_TYPE);
    ev3_lcd_set_font(EV3_FONT_MEDIUM);
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

int get_ms() {
    SYSTIM tmp;
    get_tim(&tmp);
    return (int)(tmp/1000);
}

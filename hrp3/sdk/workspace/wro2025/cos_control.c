#include "app.h"
#include "ports.h"

#include "ev3api.h"

#include <math.h>

// UNTESTED START

float cos_control_func(float s_min, float s_max, float x, float t) {
    return (s_max - s_min) * (cos(x * PI / t - PI) + 1) / 2 + s_min;
}

/// this function requires each motor's speed is already set to s_min
/// @param s_min 開始速度
/// @param s_max 終了速度
/// @param d 加減速time[ms]
/// @return moved
int cos_speeding_straight(int s_min, int s_max, int d) {
    int left_begin = ev3_motor_get_counts(LEFT_MOTOR), begin_tim = get_ms(), now_tim;

    while (true) {
        now_tim = get_ms();
        if (now_tim - begin_tim >= d) {
            break;
        }
        int now = cos_control_func(s_min, s_max, now_tim - begin_tim, d);
        ev3_motor_set_power(LEFT_MOTOR, now);
        ev3_motor_set_power(RIGHT_MOTOR, now);
    }
    ev3_motor_set_power(LEFT_MOTOR, s_max);
    ev3_motor_set_power(RIGHT_MOTOR, s_max);

    return ev3_motor_get_counts(LEFT_MOTOR) - left_begin;
}

/// this function requires the machine has stopped.
/// @param s_max 最大速度
/// @param d 回転角度(deg, ベクトル値)
void cos_turn(int s_max, int d) {
    if (d > 0) {
        cos_speeding_turn_clockwise(s_max, d);
    } else if (d < 0) {
        cos_speeding_turn_anticlockwise(s_max, -d);
    }
}

/// FIX FATAL x=0->y=0 this causes no move
/// this function requires the machine has stopped.
/// @param s_max 最大速度
/// @param d 回転角度(deg, スカラー値)
void cos_speeding_turn_clockwise(int s_max, unsigned int d) {
    int start_deg = ev3_gyro_sensor_get_angle(GYRO_SENSOR);
    int split_deg = start_deg + d / 2, finish_deg = start_deg + d;
    int now_deg;

    // increase speed
    while (true) {
        now_deg = ev3_gyro_sensor_get_angle(GYRO_SENSOR);
        if (now_deg >= split_deg) {
            break;
        }
        int now = cos_control_func(0, s_max, now_deg - start_deg, d / 2);
        ev3_motor_set_power(LEFT_MOTOR, now);
        ev3_motor_set_power(RIGHT_MOTOR, -now);
    }
    
    // decrease speed
    while (true) {
        now_deg = ev3_gyro_sensor_get_angle(GYRO_SENSOR);
        if (now_deg >= finish_deg) {
            break;
        }
        int now = cos_control_func(s_max, 0, now_deg - split_deg, d / 2);
        ev3_motor_set_power(LEFT_MOTOR, now);
        ev3_motor_set_power(RIGHT_MOTOR, -now);
    }

    // stop
    ev3_motor_stop(LEFT_MOTOR, true);
    ev3_motor_stop(RIGHT_MOTOR, true);
}

/// this function requires the machine has stopped.
/// @param s_max 最大速度
/// @param d 回転角度(deg, スカラー値)
void cos_speeding_turn_anticlockwise(int s_max, unsigned int d) {
int start_deg = ev3_gyro_sensor_get_angle(GYRO_SENSOR);
    int split_deg = start_deg - d / 2, finish_deg = start_deg - d;
    int now_deg;

    // increase speed
    while (true) {
        now_deg = ev3_gyro_sensor_get_angle(GYRO_SENSOR);
        if (now_deg <= split_deg) {
            break;
        }
        int now = cos_control_func(0, s_max, start_deg - now_deg, d / 2);
        ev3_motor_set_power(LEFT_MOTOR, -now);
        ev3_motor_set_power(RIGHT_MOTOR, now);
    }
    
    // decrease speed
    while (true) {
        now_deg = ev3_gyro_sensor_get_angle(GYRO_SENSOR);
        if (now_deg <= finish_deg) {
            break;
        }
        int now = cos_control_func(s_max, 0, split_deg - now_deg, d / 2);
        ev3_motor_set_power(LEFT_MOTOR, -now);
        ev3_motor_set_power(RIGHT_MOTOR, now);
    }

    // stop
    ev3_motor_stop(LEFT_MOTOR, true);
    ev3_motor_stop(RIGHT_MOTOR, true);
}

// UNTESTED END

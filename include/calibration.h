/**
 * @file calibration.h
 * @brief 摇杆校准
 * 每个轴校准中心点、最小值、最大值
 */

#pragma once
#include <Arduino.h>
#include "config.h"

class Calibration {
public:
    void begin();
    void start_calibration();
    bool is_calibrating() { return calibrating; }
    void process_calibration();
    void process(int16_t raw[5], int16_t out[5]);
    uint8_t read_sensitivity();

private:
    int16_t center[5];
    int16_t min_val[5];
    int16_t max_val[5];
    
    bool calibrating = false;
    int calib_counter = 0;
    
    int16_t apply_deadzone(int16_t value);
};

#endif // CALIBRATION_H

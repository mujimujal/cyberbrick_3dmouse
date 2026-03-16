/**
 * @file adc.h
 * @brief ADC采集与中值滤波
 */

#pragma once
#include <Arduino.h>
#include "config.h"

class ADCReader {
public:
    void begin();
    int16_t read(int ch);
    void read_all(int16_t out[5]);

private:
    int pins[5] = {
        JOY1_X_PIN,
        JOY1_Y_PIN,
        JOY2_X_PIN,
        JOY2_Y_PIN,
        JOY3_Z_PIN
    };
};

#endif // ADC_H

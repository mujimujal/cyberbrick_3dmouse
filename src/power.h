/**
 * @file power.h
 * @brief 电源管理，电池检测，低功耗
 */

#pragma once
#include <Arduino.h>

class PowerManager {
public:
    void begin();
    float read_battery_voltage();
    uint8_t get_battery_percent();
    bool is_usb_connected();
    void enter_low_power();

private:
    #ifdef ENABLE_BATTERY_CHECK
    float voltage;
    #endif
};

#endif // POWER_H

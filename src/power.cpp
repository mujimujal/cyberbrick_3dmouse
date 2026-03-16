/**
 * @file power.cpp
 * @brief 电源管理实现
 */

#include "power.h"
#include "config.h"

void PowerManager::begin() {
    pinMode(STATUS_LED, OUTPUT);
    #ifdef ENABLE_BATTERY_CHECK
    analogSetAttenuation(ADC_11db);
    #endif
}

float PowerManager::read_battery_voltage() {
    #ifdef ENABLE_BATTERY_CHECK
    // 分压计算：Vbat = adc * (R1+R2)/R2 / 4095 * 3.3V
    int raw = analogRead(BATTERY_PIN);
    float voltage = (float)raw * 3.3f / 4095.0f * 
                   (float)(BATTERY_DIVIDER_R1 + BATTERY_DIVIDER_R2) / 
                   (float)BATTERY_DIVIDER_R2;
    return voltage;
    #else
    return 0.0f;
    #endif
}

uint8_t PowerManager::get_battery_percent() {
    float v = read_battery_voltage();
    // 锂电池 3.0V ~ 4.2V
    if (v <= 3.0f) return 0;
    if (v >= 4.2f) return 100;
    return (uint8_t)((v - 3.0f) * 100 / 1.2f);
}

bool PowerManager::is_usb_connected() {
    // ESP32-C3 判断USB连接
    // USB供电时VDD稳定，可以通过这个判断
    // 简化实现：如果USB枚举完成就是有线模式
    return USBReady();
}

void PowerManager::enter_low_power() {
    // 闲置时进入轻度睡眠
    esp_light_sleep_start();
}

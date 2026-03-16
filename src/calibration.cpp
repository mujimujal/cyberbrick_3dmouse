/**
 * @file calibration.cpp
 * @brief 摇杆校准实现
 */

#include "calibration.h"

void Calibration::begin() {
    // 初始默认值，后续会自动校准
    for (int i = 0; i < 5; i++) {
        center[i] = 2048;  // 12位ADC中间值
        min_val[i] = 0;
        max_val[i] = 4095;
    }
    
    // 开机自动开始校准
    start_calibration();
    
    // 初始化按键
    #ifdef CALIB_BUTTON_PIN
    pinMode(CALIB_BUTTON_PIN, INPUT_PULLUP);
    #endif
}

void Calibration::start_calibration() {
    calibrating = true;
    calib_counter = 0;
    
    for (int i = 0; i < 5; i++) {
        min_val[i] = 4095;
        max_val[i] = 0;
        center[i] = 0;
    }
    
    Serial.println("=== 开始校准，请保持摇杆中立 ===");
}

void Calibration::process_calibration() {
    if (!calibrating) return;
    
    static int16_t buffer[5];
    static ADCReader adc;
    
    adc.read_all(buffer);
    
    for (int i = 0; i < 5; i++) {
        center[i] += buffer[i];
        if (buffer[i] < min_val[i]) min_val[i] = buffer[i];
        if (buffer[i] > max_val[i]) max_val[i] = buffer[i];
    }
    
    calib_counter++;
    
    if (calib_counter >= CALIBRATION_SAMPLES) {
        // 计算平均中心点
        for (int i = 0; i < 5; i++) {
            center[i] /= CALIBRATION_SAMPLES;
        }
        calibrating = false;
        Serial.println("=== 校准完成 ===");
        Serial.printf("中心点: (%d, %d, %d, %d, %d)\n", 
            center[0], center[1], center[2], center[3], center[4]);
    }
}

int16_t Calibration::apply_deadzone(int16_t value) {
    if (abs(value) < DEADZONE) {
        return 0;
    }
    return value;
}

void Calibration::process(int16_t raw[5], int16_t out[5]) {
    for (int i = 0; i < 5; i++) {
        // 减去中心偏移
        int32_t val = raw[i] - center[i];
        
        // 缩放到 -4095 ~ 4095 范围
        if (val > 0) {
            val = val * 4095 / (max_val[i] - center[i]);
        } else if (val < 0) {
            val = val * 4095 / (center[i] - min_val[i]);
        }
        
        // 应用死区
        out[i] = apply_deadzone((int16_t)val);
    }
}

uint8_t Calibration::read_sensitivity() {
    // 读取三档开关状态
    pinMode(SW_1_PIN, INPUT_PULLUP);
    pinMode(SW_2_PIN, INPUT_PULLUP);
    
    int s1 = digitalRead(SW_1_PIN);
    int s2 = digitalRead(SW_2_PIN);
    
    // 三个档位：0 = 低，1 = 中，2 = 高
    if (!s1 && s2) return 0;
    if (s1 && !s2) return 1;
    return 2;  // s1 && s2
}

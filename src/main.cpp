/**
 * @file main.cpp
 * @brief Cyberbrick 改装 3D鼠标 主程序
 * @author 基于开源项目改造
 * @date 2026-03-16
 * 
 * 功能：
 * - 2个双轴摇杆 + 1个单轴摇杆 = 5自由度
 * - 三档开关切换灵敏度
 * - 有线USB + 无线BLE自动切换
 * - 模拟3Dconnexion设备，兼容官方驱动
 */

#include <Arduino.h>
#include "config.h"
#include "adc.h"
#include "calibration.h"
#include "hid.h"
#include "ble_hid.h"
#include "power.h"

// 模块实例
ADCReader adc;
Calibration calib;
SpaceMouseHID usb_hid;
BLESpaceMouse ble_hid;
PowerManager power;

// 工作模式
typedef enum {
    MODE_USB,    // 有线USB模式
    MODE_BLE     // 无线蓝牙模式
} work_mode_t;

work_mode_t current_mode;

// 灵敏度系数数组
const float sensitivity_table[] = {
    SENSITIVITY_LOW,
    SENSITIVITY_MID,
    SENSITIVITY_HIGH
};

void apply_sensitivity(int16_t processed[5], float sensitivity) {
    // 将处理后的值缩放到适合HID的范围 (-32767 ~ 32767)
    for (int i = 0; i < 5; i++) {
        processed[i] = (int16_t)(processed[i] * sensitivity);
    }
}

void setup() {
    Serial.begin(115200);
    delay(100);
    
    Serial.println("");
    Serial.println("=== Cyberbrick 3D Mouse 启动 ===");
    
    // 初始化各模块
    adc.begin();
    calib.begin();
    power.begin();
    
    // 自动检测模式：USB连接则用有线，否则用蓝牙
    #if ENABLE_USB_AUTOSWITCH
    if (power.is_usb_connected()) {
        Serial.println("检测到USB连接，启用有线模式");
        usb_hid.begin();
        current_mode = MODE_USB;
    } else {
        Serial.println("未检测到USB，启用蓝牙无线模式");
        ble_hid.begin("Cyberbrick 3D Mouse");
        current_mode = MODE_BLE;
    }
    #else
    // 强制蓝牙模式
    ble_hid.begin("Cyberbrick 3D Mouse");
    current_mode = MODE_BLE;
    #endif
    
    digitalWrite(STATUS_LED, HIGH);
    Serial.println("=== 初始化完成 ===");
}

void loop() {
    static unsigned long last_sample = 0;
    unsigned long now = millis();
    
    // 按固定采样率运行
    if (now - last_sample >= (1000 / SAMPLING_RATE_HZ)) {
        last_sample = now;
        
        // 如果正在校准，处理校准
        if (calib.is_calibrating()) {
            calib.process_calibration();
            return;
        }
        
        // 读取ADC原始值
        int16_t raw[5];
        adc.read_all(raw);
        
        // 校准处理和死区
        int16_t processed[5];
        calib.process(raw, processed);
        
        // 读取灵敏度档位并应用
        uint8_t sens_level = calib.read_sensitivity();
        apply_sensitivity(processed, sensitivity_table[sens_level]);
        
        // 构建HID报告
        hid_report_t report;
        report.x  = processed[0];   // 摇杆1 X -> 平移X
        report.y  = processed[1];   // 摇杆1 Y -> 平移Y
        report.rx = processed[2];   // 摇杆2 X -> 旋转X
        report.ry = processed[3];   // 摇杆2 Y -> 旋转Y
        report.z  = processed[4];   // 摇杆3 Z -> 平移Z/缩放
        report.rz = 0;              // 未使用
        report.buttons = 0;         // 暂不支持按键
        
        // 发送报告
        if (current_mode == MODE_USB) {
            usb_hid.send_report(&report);
        } else {
            if (ble_hid.is_connected()) {
                ble_hid.send_report(&report);
            }
        }
        
        // LED状态指示
        static int blink_counter = 0;
        blink_counter++;
        if (blink_counter >= 1000) {
            blink_counter = 0;
            // 闪烁表示正在工作，不同模式不同频率
            digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
        }
        
        // 低功耗空闲等待
        #if ENABLE_LOW_POWER
        if (current_mode == MODE_BLE && !ble_hid.is_connected()) {
            delay(100);
        }
        #endif
    }
    
    // 检查校准按键
    #ifdef CALIB_BUTTON_PIN
    if (digitalRead(CALIB_BUTTON_PIN) == LOW) {
        delay(50);
        if (digitalRead(CALIB_BUTTON_PIN) == LOW) {
            calib.start_calibration();
        }
        while (digitalRead(CALIB_BUTTON_PIN) == LOW) delay(10);
    }
    #endif
}

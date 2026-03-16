/**
 * @file config.h
 * @brief 3D鼠标配置文件
 * 基于Cyberbrick ESP32-C3
 */

#pragma once

// ========== 引脚配置 ==========
// ADC 摇杆输入 (5通道)
// ESP32-C3 ADC1_CH0 ~ ADC1_CH4
#define JOY1_X_PIN     2    // 摇杆1 X - 平移X
#define JOY1_Y_PIN     3    // 摇杆1 Y - 平移Y
#define JOY2_X_PIN     4    // 摇杆2 X - 旋转X
#define JOY2_Y_PIN     5    // 摇杆2 Y - 旋转Y
#define JOY3_Z_PIN     6    // 摇杆3 Z - 平移Z/缩放

// 三档开关 (两个GPIO判断三个位置)
#define SW_1_PIN       7    // 开关引脚1
#define SW_2_PIN       8    // 开关引脚2

// 状态LED
#define STATUS_LED     9

// 电池电压检测 (可选)
#define ENABLE_BATTERY_CHECK 1
#define BATTERY_PIN     10   // ADC1_CH5
#define BATTERY_DIVIDER_R1  100000
#define BATTERY_DIVIDER_R2  100000

// 校准按键 (可选)
#define CALIB_BUTTON_PIN  0

// ========== 采样配置 ==========
#define ADC_SAMPLE_COUNT 8    // 采样次数 (中值滤波)
#define SAMPLING_RATE_HZ 1000 // 采样率 (1kHz)

// ========== 灵敏度配置 ==========
// 三档灵敏度
#define SENSITIVITY_LOW  0.5f
#define SENSITIVITY_MID  1.0f
#define SENSITIVITY_HIGH 2.0f

// 死区配置（摇杆中立点附近的噪声归零）
#define DEADZONE  8

// ========== 校准 ==========
#define CALIBRATION_SAMPLES 100  // 校准时采样次数

// ========== 功能开关 ==========
#define ENABLE_USB_AUTOSWITCH 1  // 自动切换有线/无线
#define ENABLE_LOW_POWER 1        // 无线模式低功耗优化

// ========== 设备标识 ==========
// 3Dconnexion 官方VID/PID，用于驱动识别
#define USB_VID 0x046D
#define USB_PID 0xC626  // SpaceMouse Compact

#endif // CONFIG_H

/**
 * @file hid.h
 * @brief USB HID 模拟 3Dconnexion 空间鼠标
 * 兼容官方驱动
 */

#pragma once
#include <Arduino.h>
#include <USB.h>
#include <USBDevice.h>

// HID报告结构
typedef struct {
    int16_t x;      // 平移X
    int16_t y;      // 平移Y
    int16_t z;      // 平移Z
    int16_t rx;     // 旋转X
    int16_t ry;     // 旋转Y
    int16_t rz;     // 旋转Z (未使用，置0)
    uint16_t buttons;  // 按键状态
} hid_report_t;

class SpaceMouseHID {
public:
    void begin();
    void send_report(hid_report_t *report);
    bool is_connected();

private:
    USBHID hid;
};

#endif // HID_H

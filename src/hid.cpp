/**
 * @file hid.cpp
 * @brief USB HID 实现 - 模拟3Dconnexion SpaceMouse
 */

#include "hid.h"
#include "config.h"

// 3Dconnexion SpaceMouse HID报告描述符
const uint8_t spacemouse_hid_report[] = {
    0x05, 0x01,        // Usage Page (Generic Desktop)
    0x09, 0x08,        // Usage (Multi-axis Controller)
    0xA1, 0x01,        // Collection (Application)
    0x15, 0x81,        // Logical Minimum (-127)
    0x25, 0x7F,        // Logical Maximum (127)
    0x75, 0x10,        // Report Size (16 bits)
    0x95, 0x06,        // Report Count (6 axes)
    0x81, 0x02,        // Input (Data,Var,Abs)
    0x75, 0x01,        // Report Size (1 bit)
    0x95, 0x10,        // Report Count (16 buttons)
    0x81, 0x02,        // Input (Data,Var,Abs)
    0xC0               // End Collection
};

void SpaceMouseHID::begin() {
    // 设置VID/PID匹配3Dconnexion，让官方驱动识别
    USBDevice.setID(USB_VID, USB_PID);
    USBDevice.setManufacturerDescriptor("3Dconnexion");
    USBDevice.setProductDescriptor("SpaceMouse Compact");
    
    hid.addReportDescriptor(spacemouse_hid_report, sizeof(spacemouse_hid_report));
    hid.begin();
    USB.begin();
}

void SpaceMouseHID::send_report(hid_report_t *report) {
    // 发送HID报告，数据顺序和报告描述符匹配
    uint8_t buffer[sizeof(hid_report_t)];
    memcpy(buffer, report, sizeof(hid_report_t));
    hid.sendReport(0, buffer, sizeof(hid_report_t));
}

bool SpaceMouseHID::is_connected() {
    return USBReady();
}

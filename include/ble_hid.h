/**
 * @file ble_hid.h
 * @brief BLE HID 蓝牙无线3D鼠标
 */

#pragma once
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEHIDDevice.h>
#include "hid.h"

class BLESpaceMouse {
public:
    void begin(const char *device_name = "Cyberbrick 3D Mouse");
    void send_report(hid_report_t *report);
    bool is_connected() { return connected; }
    void disconnect();

private:
    BLEDevice *ble_device;
    BLEHIDDevice *hid;
    BLECharacteristic *input_report;
    bool connected = false;
    
    static void callback(BLEServer *pServer, esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, ble_gatts_cb_param_t *param);
};

#endif // BLE_HID_H

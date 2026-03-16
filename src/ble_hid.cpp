/**
 * @file ble_hid.cpp
 * @brief BLE HID 实现
 */

#include "ble_hid.h"
#include "config.h"

// HID报告描述符和USB一样
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
    0x15, 0x00,        // Logical Minimum (0)
    0x25, 0x01,        // Logical Maximum (1)
    0x95, 0x10,        // Report Count (16 buttons)
    0x81, 0x02,        // Input (Data,Var,Abs)
    0xC0               // End Collection
};

static bool ble_connected = false;

void BLESpaceMouse::callback(BLEServer *pServer, esp_gatts_cb_event_t event, 
                            esp_gatt_if_t gatts_if, ble_gatts_cb_param_t *param) {
    if (event == ESP_GATTS_CONNECT_EVT) {
        ble_connected = true;
    } else if (event == ESP_GATTS_DISCONNECT_EVT) {
        ble_connected = false;
        // 断开后重新开始广播
        pServer->startAdvertising();
    }
}

void BLESpaceMouse::begin(const char *device_name) {
    BLEDevice::init(device_name);
    
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setGATTServerCallback(callback);
    pServer->getAdvertising()->setScanResponse(true);
    
    hid = new BLEHIDDevice(pServer);
    input_report = hid->inputReport(1);
    
    hid->hidService()->setAttributePermissions(
        ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE);
    
    // 设置HID报告描述符
    hid->pnp->setManufacturerData("Cyberbrick");
    hid->pnp->setModelNumber("3DMouse");
    hid->pnp->setSerialNumber("1.0");
    
    hid->hidInfo->setHIDInfo(0x01, 0x0210, 0x01, 0);
    hid->reportMap((uint8_t *)spacemouse_hid_report, sizeof(spacemouse_hid_report));
    
    hid->startServices();

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->setAppearance(HID_GEN_HID_POINTER);
    pAdvertising->addServiceUUID(hid->hidService()->getUUID());
    pAdvertising->start();
    
    connected = false;
    Serial.println("BLE 3D鼠标准备就绪，等待连接...");
}

void BLESpaceMouse::send_report(hid_report_t *report) {
    if (!connected) return;
    
    uint8_t buffer[sizeof(hid_report_t)];
    memcpy(buffer, report, sizeof(hid_report_t));
    input_report->setValue(buffer, sizeof(hid_report_t));
    input_report->notify();
}

void BLESpaceMouse::disconnect() {
    BLEDevice::deinit();
    connected = false;
}

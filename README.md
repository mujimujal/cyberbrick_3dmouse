# Cyberbrick 3D Mouse

基于拓竹 **Cyberbrick** 遥控器组件改装的开源3D空间鼠标，兼容3Dconnexion官方驱动，支持Blender、FreeCAD、SolidWorks等常用3D软件。

## 功能特性

- ✅ **硬件**：使用现成Cyberbrick组件（ESP32-C3主控 + 2个双轴摇杆 + 1个单轴摇杆 + 1个三档开关）
- ✅ **5自由度**：X/Y平移 + RX/RY旋转 + Z缩放，满足大部分3D导航需求
- ✅ **双模式**：有线USB + 无线蓝牙BLE自动切换
- ✅ **驱动兼容**：模拟官方3Dconnexion设备，直接使用官方驱动
- ✅ **开发平台**：PlatformIO + Arduino框架，易于修改编译
- ✅ **三档灵敏度**：通过三档开关快速切换
- ✅ **自动校准**：开机自动校准摇杆中心点
- ✅ **电池管理**：支持锂电池电压检测，低功耗优化

## 硬件接线

| 功能 | Cyberbrick ADC引脚 | ESP32-C3 GPIO |
|------|-------------------|---------------|
| 摇杆1 X | ADC1_CH0 | GP2 |
| 摇杆1 Y | ADC1_CH1 | GP3 |
| 摇杆2 X | ADC1_CH2 | GP4 |
| 摇杆2 Y | ADC1_CH3 | GP5 |
| 摇杆3 Z | ADC1_CH4 | GP6 |
| 三档开关 1 | - | GP7 |
| 三档开关 2 | - | GP8 |
| 状态LED | - | GP9 |
| 电池检测 | ADC1_CH5 | GP10 |

## 轴映射

| 轴 | 功能 |
|----|------|
| 摇杆1 X | 平移 X |
| 摇杆1 Y | 平移 Y |
| 摇杆2 X | 旋转 X |
| 摇杆2 Y | 旋转 Y |
| 摇杆3 Z | 平移 Z / 缩放 |

## 三档开关灵敏度

| 档位 | 灵敏度系数 |
|------|-----------|
| 位置1 | 0.5x (低) |
| 位置2 | 1.0x (中) |
| 位置3 | 2.0x (高) |

## 编译下载

### 环境要求
- VS Code + PlatformIO 插件
- Cyberbrick 核心板（ESP32-C3）

### 步骤

1. Clone 这个项目到本地
```bash
cd ~/projects
git clone <this repo>
cd 3dmouse
```

2. 用VSCode打开，PlatformIO会自动安装依赖

3. 编译
```bash
pio run
```

4. 下载（按住BOOT键插入USB，然后执行）
```bash
pio run --target upload
```

5. 查看串口日志
```bash
pio device monitor
```

## 校准

- 开机自动校准，请保持摇杆在中立位置不要动，校准完成后即可使用
- 可以外接一个按键到 `CALIB_BUTTON_PIN`，按下重新触发校准

## 系统驱动安装

Windows/macOS：下载安装 [3Dconnexion官方驱动](https://3dconnexion.com/cn/support/drivers/)，会自动识别你的设备。

Linux：使用 [FreeSpacenav](https://github.com/FreeSpacenav/spacenav) 开源驱动。

## 支持软件

- Blender ✅
- FreeCAD ✅
- SolidWorks ✅
- SketchUp ✅
- Fusion 360 ✅
- 几乎所有支持3Dconnexion的软件都原生支持 ✅

## 项目结构

```
.
├── platformio.ini          # PlatformIO 配置
├── README.md               # 说明文档
└── src/
    ├── main.cpp            # 主程序
    ├── config.h            # 配置文件（引脚、灵敏度等）
    ├── adc.h/cpp           # ADC采集与滤波
    ├── calibration.h/cpp   # 摇杆校准
    ├── hid.h/cpp           # USB HID 实现
    ├── ble_hid.h/cpp       # BLE HID 实现
    └── power.h/cpp         # 电源管理
```

## 感谢参考项目

- [josefadamcik/DIY-SpaceMouse](https://github.com/josefadamcik/DIY-SpaceMouse)
- [gvzabre/esp32-3d-mouse](https://github.com/gvzabre/esp32-3d-mouse)
- [rkaneth/SpaceMouse-ESP32-BLE](https://github.com/rkaneth/SpaceMouse-ESP32-BLE)
- [OpenCyberBrick](https://icanbe.xyz/post/opencyberbrick/)

## 许可证

MIT

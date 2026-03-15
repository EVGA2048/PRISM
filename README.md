# PRISM

[→ 中文说明](#中文说明)

A portable ambient computing device that adapts to your context.

Plug in headphones — it becomes a DAC.  
Plug in USB — it becomes a soundcard.  
Walk away from your phone — it becomes your notification center.  
No network — send a message over LoRa.

## Hardware

**Always-on core**
- ESP32-WROOM-32E — Bluetooth Classic A2DP, BLE ANCS/AMS, sensors, status bar

**High-performance core (on-demand)**
- ESP32-P4 — USB Audio UAC, MIPI-CSI camera, main UI, Lua scripting

**Peripherals**
- PCM5102A I2S DAC — 3.5mm audio output
- SSD1351 128×128 RGB OLED — main display (SPI)
- SSD1306 0.91" OLED — status bar (I2C)
- WS2812B — ambient lighting & notifications
- LoRa SX1276 — off-grid messaging
- NFC PN532 — card read/write
- MPU6050 — gesture wake, orientation
- BME280 — temperature, humidity, pressure
- SCD40 — CO₂
- ENS160 — TVOC
- IR TX/RX — universal remote learning

## Software

- UI: [AXIS UI](https://github.com/EVGA2048/AXIS-UI)
- Notifications: [NodeLib_ANCS](https://github.com/EVGA2048/NodeLib_ANCS)
- Scripting: Lua on ESP32-P4

## Power Modes

| Mode | Active | Draw |
|------|--------|------|
| Standby | 32E only | ~80mA |
| Audio | 32E + PCM5102A | ~120mA |
| Active | 32E + P4 | ~350mA |

## Status

🚧 Under active development — Phase 1 (audio + dual display)

## License

MIT

---

## 中文说明

[→ Back to English](#prism)

一个随身携带的小设备，想解决的问题很简单：

耳机一插就能听歌，不用掏手机；上课静音的时候有消息，灯光提醒一下；没有网络的地方，用 LoRa 给朋友发条消息；接上电脑，变成外置声卡。

不想做成那种功能很多但平时根本不用的东西，每个功能都是真实场景里会用到的。

## 硬件设计

两块芯片分工明确。ESP32-32E 始终在线，负责蓝牙音频、BLE 通知、传感器采集和状态栏显示，功耗低，平时就它一个人工作。ESP32-P4 按需唤醒，负责 USB 声卡、摄像头、主界面渲染和 Lua 脚本，需要处理大任务的时候才启动。

音频路由通过一颗 74HC157 多路复用器切换，蓝牙音频和 USB 音频共用同一个 PCM5102A DAC，自动根据当前模式选择信号源。

传感器方面，BME280 覆盖温湿度和气压，SCD40 测 CO₂，ENS160 测 TVOC，MPU6050 做姿态感知和抬腕唤醒，和设备名 PRISM 的「轴」含义呼应。

## 交互设计

物理按键 + 五维摇杆，不用触屏。肌肉记忆，盲操，高效。WS2812B 灯光做环境提示，收到 LoRa 消息或手机通知时静默亮起，不打扰当前状态。

## 软件架构

UI 层独立成 [AXIS UI](https://github.com/EVGA2048/AXIS-UI) 框架，可以用在其他带小屏幕的设备上。通知和媒体控制通过 [NodeLib_ANCS](https://github.com/EVGA2048/NodeLib_ANCS) 对接 iOS 和 Android。P4 端支持 Lua 脚本，可以自定义工具和自动化流程。

## 开发进度

- [x] AXIS UI 框架 v0.1
- [ ] 硬件验证：双屏 + 蓝牙音频
- [ ] NodeLib_ANCS 重构
- [ ] PRISM 32E 主程序
- [ ] P4 端 USB Audio + 主界面
- [ ] LoRa 通信
- [ ] 传感器接入
- [ ] Lua 脚本引擎

项目还在做，慢慢来。

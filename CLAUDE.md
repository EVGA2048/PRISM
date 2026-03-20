# PRISM — Claude Code 上下文

## 项目简介
PRISM 是一个随身极客设备，双芯片方案。开发者是 node0，北京联合大学学生。
UI 框架叫 AXIS UI，库体系叫 NodeLib，终端橙黄主题 #FFA500。

## 双芯片架构

### ESP32-WROOM-32E（YD-ESP32）— 永远在线
- A2DP 蓝牙音频接收
- BLE ANCS/AMS（iOS 通知 + 媒体控制）
- SSD1306 0.91" 状态栏（常亮）
- DS3231 实时时钟（I2C 0x68，与 SSD1306 共用总线 SDA=21 SCL=33）
- MPU6050 姿态感知（抬腕亮屏、翻转静音、轻摇切歌）
- WiFi AP + HTTP 服务器（LoRa WebUI）
- 唤醒 / 中断 P4

### ESP32-P4-Pico（微雪 KIT-A）— 按需唤醒
- USB Audio UAC（外置声卡，接电脑）
- SSD1351 128×128 主彩屏
- 环境传感器：BME280（温湿度/气压）、SCD40（CO₂）、ENS160（TVOC）
- LoRa SX1276 离线通信
- NFC PN532
- 红外收发（学习 + 发射，控制家电）
- W25Q128 外置 SPI Flash（16MB，存红外码库/LoRa历史/Lua脚本）
- Lua 脚本引擎
- 摄像头（MIPI-CSI，待定）

### 数据流
- P4 每 30s 采集环境数据 → UART2 推给 32E 缓存 → 32E 状态栏显示
- MPU6050 触发手势 → 32E 直接响应，或拉高 GPIO 唤醒 P4
- LoRa 收到消息 → P4 通过 UART 推给 32E → 状态栏滚动提示
- 用户发 LoRa：手机连 32E WiFi → WebUI 输入 → 32E 通过 UART 转发 P4 发射

## 仓库结构
- `PRISM/`        主工程（本仓库）
- `AXIS-UI/`      UI 框架 https://github.com/EVGA2048/AXIS-UI
- `NodeLib_ANCS/` iOS 通知库 https://github.com/EVGA2048/NodeLib_ANCS

## 开发环境
- 32E 端：Arduino IDE，ESP32 Core 3.2.0，ESP_I2S + A2DP
- P4 端：ESP-IDF v5.5.3，VS Code + ESP-IDF 插件

## 已验证硬件（Phase 1）
- SSD1351 128×128 主彩屏：SCK=18 MOSI=23 CS=2 DC=17 RST=15
- SSD1306 0.91" 状态栏：SDA=21 SCL=33
- PCM5102A DAC：BCK=26 LRCK=25 DIN=22
- 摇杆：UP=19 DOWN=32 LEFT=13 RIGHT=27 OK=4
- 按键：A=14 B=12
- WS2812：GPIO=16（板载）
- P4 开发板串口通信验证完成（360MHz，32MB Flash，576KB heap）

## 芯片间通信
- UART2（460800）传数据，GPIO 传唤醒信号
- 详见 docs/uart-protocol.md

## AXIS UI 设计语言
- 终端橙黄主题：背景纯黑，主色 #FFA500，报警红 #FF3300
- 硬直角，无圆角，极简工业风（参考 Teenage Engineering）
- U8g2_for_Adafruit_GFX 负责文字，Adafruit GFX 负责图形
- 内置屏幕：SCR_HOME（桌面）SCR_PLAYER（播放器）
- 支持屏幕旋转（MPU6050 联动），WS2812 灯光回调
- 形态：横版掌机（类 GBA），可横持/竖持，UI 自动旋转
- 灯光是信息通道，不是装饰（详见 docs/design-vision.md）
- 姿态交互：翻转静音，抬起亮屏，轻摇切歌
- 动效有重量感：easeOutElastic，120~200ms，不轻浮
- 主屏信息密度原则：只显示当前最重要的一件事
- 状态栏静止时：时间 + BT状态 + 播放状态 + 音量；有事件时滚动播报
- 温度显示加 ~ 前缀（近似值，受 MCU 发热影响）

## 调试系统
- 两块 MCU 均有串口菜单，串口输入 `m` 进入
- debug mode 开启后屏幕角落显示 `DBG`
- 各模块日志独立开关，重启后恢复默认关闭

```
=== PRISM DEBUG MENU ===
[1] Toggle DEBUG mode     (current: OFF)
[2] Toggle UART log       (current: OFF)
[3] Toggle sensor log     (current: OFF)  <- P4 only
[4] Toggle BT/BLE log     (current: OFF)  <- 32E only
[5] Toggle heartbeat log  (current: OFF)
[6] Print system info
[q] Exit menu
```

## 关键技术决策
- 音频：32E 跑 A2DP（接手机），P4 跑 USB UAC（接电脑），场景互斥，74HC157 SEL 由 P4 启动状态决定
- 姿态：MPU6050 接 32E，常驻响应，不依赖 P4
- 传感器：BME280/SCD40/ENS160 接 P4，按需采集推给 32E
- 时钟：DS3231 接 32E，I2C 与 SSD1306 共用总线
- 红外：接 P4，码库存 W25Q128，用于控制家电
- 存储：W25Q128 接 P4，存红外码库、LoRa 历史、Lua 脚本
- P4 离线：32E 心跳超时后状态栏滚动显示 ⚠ P4 OFFLINE

## 待办（按优先级）
1. AXIS-UI 在 Arduino IDE 编译验证
2. NodeLib_ANCS 重构（AMS 控制、Android GMCS、断线重连）
3. PRISM_32E 正式版（接入 AXIS UI + NodeLib_ANCS + DS3231）
4. P4 端 ESP-IDF 环境搭建完成后：USB Audio UAC
5. 32E ↔ P4 串口通信测试（见 docs/uart-protocol.md）

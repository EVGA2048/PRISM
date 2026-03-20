# PRISM 项目上下文

> 每次找 Claude 协作时，把这个文件内容发过去

## 项目概览
PRISM 是一个随身极客设备，双芯片方案（ESP32-32E + ESP32-P4）。
UI 框架叫 AXIS UI，库体系叫 NodeLib，终端橙黄主题 #FFA500。

## 仓库索引

### PRISM 主工程
| 文件 | raw 链接 |
|------|---------|
| PRISM_32E.ino | https://raw.githubusercontent.com/EVGA2048/PRISM/main/32e/PRISM_32E/PRISM_32E.ino |
| uart-protocol.md | https://raw.githubusercontent.com/EVGA2048/PRISM/main/docs/uart-protocol.md |

### AXIS-UI
| 文件 | raw 链接 |
|------|---------|
| AXIS_UI.h | https://raw.githubusercontent.com/EVGA2048/AXIS-UI/main/src/AXIS_UI.h |
| AXIS_UI.cpp | https://raw.githubusercontent.com/EVGA2048/AXIS-UI/main/src/AXIS_UI.cpp |
| axis_types.h | https://raw.githubusercontent.com/EVGA2048/AXIS-UI/main/src/axis_types.h |
| axis_anim.h | https://raw.githubusercontent.com/EVGA2048/AXIS-UI/main/src/axis_anim.h |

### NodeLib_ANCS
| 文件 | raw 链接 |
|------|---------|
| 待重构 | https://github.com/EVGA2048/NodeLib_ESP32_ANCS |

## 硬件
见 docs/wiring.md

## 当前进度
- [x] Phase 1 硬件验证：双屏 + A2DP 蓝牙音频 + 摇杆按键
- [x] AXIS UI v0.1 框架（终端橙黄主题，屏幕旋转，WS2812，U8g2）
- [x] P4 开发板验证（360MHz，32MB Flash，ESP-IDF v5.4.1）
- [x] 芯片间通信协议设计（docs/uart-protocol.md）
- [ ] ESP-IDF v5.5.3 环境配置（进行中）
- [ ] AXIS UI 安装进 Arduino 并编译验证
- [ ] NodeLib_ANCS 重构（AMS 远程控制、Android GMCS）
- [ ] PRISM_32E 正式版（AXIS UI + NodeLib_ANCS + DS3231）
- [ ] 32E ↔ P4 串口通信测试
- [ ] P4 端 USB Audio UAC

## 开发环境
- 32E：Arduino IDE，ESP32 Core 3.2.0，MacBook
- P4：ESP-IDF v5.5.3，VS Code + ESP-IDF 插件（安装中）
- 芯片间通信：UART2 460800 + GPIO 唤醒/中断

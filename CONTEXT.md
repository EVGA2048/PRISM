# PRISM 项目上下文

> 每次找 Claude 协作，先把这个文件发过去

## 项目概览
PRISM 是一个随身极客设备，双芯片方案（ESP32-32E + ESP32-P4）。
UI 框架叫 AXIS UI，库体系叫 NodeLib。

## 仓库索引

### AXIS-UI
| 文件 | raw 链接 |
|------|---------|
| AXIS_UI.h | https://raw.githubusercontent.com/EVGA2048/AXIS-UI/main/src/AXIS_UI.h |
| AXIS_UI.cpp | https://raw.githubusercontent.com/EVGA2048/AXIS-UI/main/src/AXIS_UI.cpp |
| axis_types.h | https://raw.githubusercontent.com/EVGA2048/AXIS-UI/main/src/axis_types.h |
| axis_anim.h | https://raw.githubusercontent.com/EVGA2048/AXIS-UI/main/src/axis_anim.h |
| BasicMenu.ino | https://raw.githubusercontent.com/EVGA2048/AXIS-UI/main/examples/BasicMenu/BasicMenu.ino |

### PRISM-32E（Arduino）
| 文件 | raw 链接 |
|------|---------|
| （待添加） | |

### NodeLib_ANCS
| 文件 | raw 链接 |
|------|---------|
| （待添加） | |

## 硬件速查
- ESP32-32E (YD-ESP32)：音频核心，BLE，传感器
- ESP32-P4-Pico：高性能核心，USB Audio，摄像头
- SSD1351 128×128：主彩屏，接 32E（SPI）
- SSD1306 0.91"：状态栏，接 32E（I2C）
- PCM5102A：DAC，接 32E（I2S）

## 关键引脚
见 docs/wiring.md

## 当前进度
- [x] AXIS-UI v0.1 基础框架
- [ ] 硬件验证（双屏 + 音频）
- [ ] NodeLib_ANCS 重构
- [ ] PRISM 32E 主程序
```

---

## 新建文件时怎么更新

raw 链接的规律是固定的：
```
https://raw.githubusercontent.com/EVGA2048/仓库名/main/文件路径

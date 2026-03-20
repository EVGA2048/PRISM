# PRISM 接线速查

## ESP32-32E (YD-ESP32)

### SSD1351 主彩屏（VSPI）
| 模块引脚 | GPIO | 备注 |
|---------|------|------|
| SCK     | 18   | VSPI 硬件固定 |
| MOSI    | 23   | VSPI 硬件固定 |
| CS      | 2    | 避开 GPIO5 启动 PWM |
| DC      | 17   |  |
| RST     | 15   | GPIO16 被板载 WS2812 占用 |
| VCC     | 3.3V | 板载升压，仅需 3.3V |
| GND     | GND  |  |

### SSD1306 状态栏（I2C）
| 模块引脚 | GPIO | 备注 |
|---------|------|------|
| SDA     | 21   |  |
| SCL     | 33   | GPIO22 被 I2S 占用 |
| VCC     | 3.3V |  |
| GND     | GND  |  |

### PCM5102A DAC（I2S）
| 模块引脚 | GPIO  | 备注 |
|---------|-------|------|
| BCK     | 26    |  |
| LRCK    | 25    |  |
| DIN     | 22    |  |
| VCC     | 3.3V  |  |
| GND     | GND   |  |
| XMT     | 3.3V  | 必须接高，否则静音 |
| FMT     | GND   | I2S 标准格式 |
| FLT     | GND   |  |
| SCK     | GND   | 内部 PLL |

### 五维摇杆 + 按键
| 引脚    | GPIO | 备注 |
|--------|------|------|
| UP     | 19   | INPUT_PULLUP |
| DOWN   | 32   | INPUT_PULLUP |
| LEFT   | 13   | INPUT_PULLUP |
| RIGHT  | 27   | INPUT_PULLUP |
| MID/OK | 4    | INPUT_PULLUP |
| SET    | 14   | BTN_A，INPUT_PULLUP |
| RST    | 12   | BTN_B，INPUT_PULLUP |
| COM    | GND  |  |

### P4 通信预留
| 信号      | GPIO | 方向 | 备注 |
|----------|------|------|------|
| UART2 TX | 12   | 输出 | 32E→P4 |
| UART2 RX | 35   | 输入 | P4→32E，input-only |
| P4 WAKE  | 待定 | 输出 | 拉高唤醒 P4 |
| P4 IRQ   | 36   | 输入 | P4 通知 32E，input-only |

### 74HC157 I2S 多路复用（待接）
| 信号    | GPIO | 备注 |
|--------|------|------|
| SEL    | 32   | LOW=蓝牙 HIGH=USB |

> ⚠️ 注意：BTN_B 和 74HC157 SEL 都规划在 GPIO32，
> 待 P4 到货后重新分配，当前 Phase 1 暂用 GPIO32 作 BTN_B

## 已验证工作的软件组合
- ESP32 Arduino Core: 3.2.0
- ESP32-A2DP: 1.8.6
- Adafruit SSD1351: 最新
- Adafruit SSD1306: 最新
- Adafruit GFX: 最新
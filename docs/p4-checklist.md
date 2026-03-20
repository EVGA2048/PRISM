# PRISM P4 开发 Checklist

## ESP-IDF 环境激活

每次开新终端需要执行：
```bash
. ~/esp/esp-idf/export.sh
```

或者加到 `~/.zshrc` 自动激活：
```bash
echo '. ~/esp/esp-idf/export.sh' >> ~/.zshrc
```

## Phase 1 — 环境验证

- [ ] Arduino IDE 安装 ESP32-P4 支持（esp32 core 3.x）
- [ ] 最小 blink 程序编译烧录成功
- [ ] 串口输出正常

## Phase 2 — 显示验证

- [ ] 面包板接 SSD1351（SCK=18 MOSI=23 CS=2 DC=17 RST=15）
- [ ] Adafruit SSD1351 库跑通示例
- [ ] 确认颜色、方向、刷新率正常

## Phase 3 — 输入验证

- [ ] 摇杆五向读取（UP/DOWN/LEFT/RIGHT/OK），串口打印
- [ ] A/B 按键读取，去抖处理
- [ ] MPU6050 I2C 读取，输出姿态角

## Phase 4 — WS2812 验证

- [ ] 单条 WS2812 点亮，逐颗确认索引顺序
- [ ] 确认摇杆灯索引位置
- [ ] ledSetStrip / ledSetJoystick / ledCommit 接口验证

## Phase 5 — AXIS-UI 骨架

- [ ] 屏幕管理器（SCR_HOME / SCR_PLAYER / SCR_CLOCK 切换）
- [ ] 网络节点菜单渲染（静态）
- [ ] 五维摇杆驱动节点焦点切换
- [ ] 节点跳转动效（连线小点滑动）
- [ ] 消息气泡覆盖层（静态弹出）
- [ ] 气泡动效（easeOutElastic 弹出 + 滑出）

## Phase 6 — 32E 通信

- [ ] UART2 460800 收发测试
- [ ] 定义消息协议格式
- [ ] P4 接收 32E 推送的通知数据并触发气泡
- [ ] P4 接收媒体状态更新

## Phase 7 — 各屏幕实现

- [ ] SCR_PLAYER：播放器界面
- [ ] SCR_CLOCK：时钟 + 秒表 + 倒计时 + 番茄钟
- [ ] SCR_CLOCK：沙粒倒计时（粒子系统）
- [ ] SCR_LORA：LoRa 短信界面
- [ ] SCR_REMOTE：红外遥控界面

## Phase 8 — 后台服务

- [ ] 定时器后台运行，退出屏幕不中断
- [ ] 节点角标（CLOCK 节点运行中显示橙点）
- [ ] USB Audio UAC 服务

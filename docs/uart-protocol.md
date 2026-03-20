# PRISM 芯片间通信协议

## 物理层

- 接口：UART2，460800 baud，8N1
- 32E TX → P4 RX
- P4 TX → 32E RX
- 唤醒信号：32E GPIO → P4 WAKE（独立 GPIO，拉高唤醒）
- 中断信号：P4 IRQ → 32E GPIO36（input-only）

> ⚠️ 两板必须共 GND，否则通信必然失败

## 帧格式

```
[0xAA] [TYPE] [FLAGS] [SEQ] [LEN] [PAYLOAD...] [CRC8]
  1B     1B     1B     1B    1B     0~28B         1B
```

| 字段 | 长度 | 说明 |
|------|------|------|
| 帧头 | 1B | 固定 0xAA |
| TYPE | 1B | 消息类型 |
| FLAGS | 1B | bit0=分包标志(FRAG)，bit1=末包标志(LAST)，其余保留 |
| SEQ | 1B | 分包序号，单包消息固定 0x00 |
| LEN | 1B | Payload 字节数（0~28）|
| PAYLOAD | 0~28B | 消息内容 |
| CRC8 | 1B | 对 TYPE+FLAGS+SEQ+LEN+PAYLOAD 做 CRC8 |

### FLAGS 说明

| FLAGS | 含义 |
|-------|------|
| `0x00` | 单包完整消息 |
| `0x01` | 分包，非末包（FRAG=1, LAST=0）|
| `0x03` | 分包，末包（FRAG=1, LAST=1）|

### 分包示例

发送 60 字节的 LoRa 消息：
```
帧1: AA 30 01 00 1C [28字节] CRC   ← FRAG=1 LAST=0 SEQ=0
帧2: AA 30 01 01 1C [28字节] CRC   ← FRAG=1 LAST=0 SEQ=1
帧3: AA 30 03 02 04 [4字节]  CRC   ← FRAG=1 LAST=1 SEQ=2
```
接收方按 SEQ 顺序拼接，收到 LAST=1 后组装完整消息。

## 消息类型

### P4 → 32E

| TYPE | 含义 | Payload | 分包 |
|------|------|---------|------|
| `0x01` | 环境数据 | temp(f32) humi(f32) co2(u16) tvoc(u16) pres(f32) — 共 18B | 否 |
| `0x02` | P4 上线通知 | p4_state(u8): 0=idle 1=active 2=sleeping | 否 |
| `0x14` | 歌词行 | text(str, 最长 28B，超出截断) | 否 |
| `0x20` | 心跳 | uptime(u32) 单位 ms | 否 |
| `0x30` | LoRa 收到消息 | sender(u8) text(str) | 支持分包 |

### 32E → P4

| TYPE | 含义 | Payload | 分包 |
|------|------|---------|------|
| `0x10` | 唤醒确认 | 无 | 否 |
| `0x11` | 媒体状态 | playing(u8) volume(u8) title(str, 最长 26B) | 否 |
| `0x12` | ANCS 通知 | priority(u8) app_id(u8) text(str, 最长 26B，超出截断) | 否 |
| `0x20` | 心跳 | uptime(u32) 单位 ms | 否 |
| `0x31` | LoRa 发送请求 | target(u8) text(str) | 支持分包 |

### 双向

| TYPE | 含义 | Payload |
|------|------|---------|
| `0xFF` | ACK | echo_type(u8) echo_seq(u8) |
| `0xFE` | NAK（CRC错误/丢包）| echo_type(u8) echo_seq(u8) |

## 通知优先级

状态栏滚动条同时只显示一条，高优先级打断低优先级：

```
电话来电 > LoRa 消息 > ANCS 通知 > 歌词 > 环境数据播报
```

## 音频切换逻辑

74HC157 SEL 引脚由 32E 控制（GPIO32），使用场景互斥无需主动切换命令：

| SEL | 音频源 | 场景 |
|-----|--------|------|
| LOW | 32E I2S（蓝牙 A2DP）| 默认，P4 未启动 |
| HIGH | P4 I2S（USB Audio）| P4 激活且 USB 已连接电脑 |

P4 启动时自行拉高 SEL，关闭前拉低。

## 通信节奏

| 消息 | 触发方式 | 频率 |
|------|---------|------|
| 环境数据 `0x01` | P4 定时 | 每 30s |
| 心跳 `0x20` | 双向定时 | 每 5s |
| 心跳超时 | — | 15s 无心跳视为对端异常，状态栏滚动 ⚠ P4 OFFLINE |
| 媒体状态 `0x11` | 事件触发 | 状态变化时立即推 |
| ANCS 通知 `0x12` | 事件触发 | 收到通知时立即推 |
| 歌词 `0x14` | 事件触发 | 逐行推送 |
| LoRa 消息 `0x30` | 事件触发 | 收到时立即推 |

## 稳定性设计

- **接收状态机**：WAIT_HEADER → WAIT_TYPE → WAIT_FLAGS → WAIT_SEQ → WAIT_LEN → WAIT_PAYLOAD → WAIT_CRC，非阻塞，每次 loop() 处理一个字节
- **CRC 校验**：多项式 0x07（CRC-8），错误帧直接丢弃，发送方收到 NAK 后重传
- **分包超时**：开始接收分包后，5s 内未收到末包视为传输失败，丢弃已收数据
- **长文本截断**：不支持分包的消息类型，发送前强制截断到最大长度，不分包
- **波特率降级**：如测试中 CRC 错误率高，降至 230400 或 115200

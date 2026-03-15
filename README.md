# PRISM

A portable ambient computing device that adapts to your context.
Plug in headphones — it becomes a DAC.
Plug in USB — it becomes a soundcard.  
Walk away from your phone — it becomes your notification center.

## Hardware
- ESP32-WROOM-32E — always-on core (BT audio, BLE, sensors)
- ESP32-P4 — high-performance core (USB audio, camera, UI)
- PCM5102A DAC, SSD1351 128×128, SSD1306 0.91"
- LoRa SX1276, NFC PN532, MPU6050, BME280, WS2812

## Software
- UI: [AXIS UI](https://github.com/EVGA2048/AXIS-UI)
- Notifications: [NodeLib_ANCS](https://github.com/EVGA2048/NodeLib_ANCS)

## Status
🚧 Under active development

## License
MIT

---

一个随身携带的小设备，想解决的问题很简单：

耳机一插就能听歌，不用掏手机；
上课静音的时候有消息，灯光提醒一下；
没有网络的地方，用 LoRa 给朋友发条消息；
接上电脑，变成外置声卡。

不想做成那种功能很多但平时根本不用的东西，
每个功能都是真实场景里会用到的。

硬件用的是 ESP32-32E 加 ESP32-P4 双芯片方案，
平时只有 32E 在工作，功耗低，
需要处理大任务的时候才唤醒 P4。

项目还在做，慢慢来。

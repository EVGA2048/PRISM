// PRISM P4 开发板可用性测试
// 目标：ESP32-P4-Pico（微雪 KIT-A）
// 无需任何外设，仅通过串口验证芯片基本功能
// 开发环境：Arduino IDE + ESP32 Arduino Core（ESP32-P4 支持版本）

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("=============================");
  Serial.println("  PRISM P4 Board Test v0.1   ");
  Serial.println("=============================");

  // 芯片信息
  Serial.printf("Chip model   : %s\n", ESP.getChipModel());
  Serial.printf("Chip revision: %d\n", ESP.getChipRevision());
  Serial.printf("CPU freq     : %d MHz\n", ESP.getCpuFreqMHz());
  Serial.printf("Flash size   : %d KB\n", ESP.getFlashChipSize() / 1024);
  Serial.printf("Free heap    : %d bytes\n", ESP.getFreeHeap());
  Serial.printf("SDK version  : %s\n", ESP.getSdkVersion());

  Serial.println("-----------------------------");
  Serial.println("Setup done. Looping...");
}

unsigned long lastPrint = 0;
int count = 0;

void loop() {
  if (millis() - lastPrint >= 2000) {
    lastPrint = millis();
    count++;
    Serial.printf("[%4d] uptime=%lus  heap=%d bytes\n",
                  count, millis() / 1000, ESP.getFreeHeap());
  }
}

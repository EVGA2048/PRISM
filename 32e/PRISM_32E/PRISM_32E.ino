#include "ESP_I2S.h"
#include "BluetoothA2DPSink.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

// ── 引脚 ─────────────────────────────────────────
#define OLED_CS   2
#define OLED_DC   17
#define OLED_RST  15
#define JOY_UP    19
#define JOY_DWN   32
#define JOY_LFT   13
#define JOY_RHT   27
#define JOY_MID   4
#define BTN_SET   14
#define BTN_RST   12

// ── 显示对象 ─────────────────────────────────────
Adafruit_SSD1351 mainDisp(128, 128, &SPI, OLED_CS, OLED_DC, OLED_RST);
Adafruit_SSD1306 statusDisp(128, 32, &Wire, -1);

// ── 音频 ─────────────────────────────────────────
I2SClass i2s;
A2DPSimpleExponentialVolumeControl vc;
BluetoothA2DPSink a2dp_sink(i2s);

// ── 媒体信息 ─────────────────────────────────────
String trackTitle  = "Waiting...";
String trackArtist = "";
bool   isPlaying   = false;
bool   btConnected = false;
int    volume      = 80;

// ── 按键去抖 ─────────────────────────────────────
#define DB_MS 180
unsigned long _db[7] = {};

bool pressed(int pin, int idx) {
  if (digitalRead(pin) == LOW && millis() - _db[idx] > DB_MS) {
    _db[idx] = millis();
    return true;
  }
  return false;
}

// ── 回调 ─────────────────────────────────────────
void avrc_metadata_callback(uint8_t id, const uint8_t* text) {
  if (id == 0x01) trackTitle  = (char*)text;
  if (id == 0x02) trackArtist = (char*)text;
}

void connection_state_callback(esp_a2d_connection_state_t state, void*) {
  btConnected = (state == ESP_A2D_CONNECTION_STATE_CONNECTED);
  if (!btConnected) {
    trackTitle = "Waiting...";
    trackArtist = "";
    isPlaying = false;
  }
}

void playback_state_callback(esp_avrc_playback_stat_t stat) {
  isPlaying = (stat == ESP_AVRC_PLAYBACK_PLAYING);
}

// ── 音量控制 ──────────────────────────────────────
void setVolume(int v) {
  volume = constrain(v, 0, 100);
  a2dp_sink.set_volume((uint8_t)(volume * 128 / 100));
  Serial.printf("[VOL] %d%%\n", volume);
}

// ── 工具 ─────────────────────────────────────────
String truncate(String s, int maxChars) {
  if ((int)s.length() <= maxChars) return s;
  return s.substring(0, maxChars - 1) + "~";
}

// ── 主屏刷新 ──────────────────────────────────────
void refreshMain() {
  mainDisp.fillScreen(0x0841);

  // 顶栏
  mainDisp.fillRect(0, 0, 128, 16, 0x0862);
  mainDisp.fillCircle(6, 8, 4, btConnected ? 0x07FF : 0x39E7);
  mainDisp.setTextSize(1);
  mainDisp.setTextColor(0xFFFF);
  mainDisp.setCursor(14, 4);
  mainDisp.print(btConnected ? "BT Connected" : "Searching...");

  // 播放图标
  uint16_t stCol = isPlaying ? 0x07E0 : 0xFD20;
  mainDisp.fillRoundRect(2, 20, 36, 36, 4, 0x1083);
  mainDisp.drawRoundRect(2, 20, 36, 36, 4, stCol);
  if (isPlaying) {
    mainDisp.fillRect(11, 29, 5, 18, stCol);
    mainDisp.fillRect(22, 29, 5, 18, stCol);
  } else {
    for (int i = 0; i < 10; i++)
      mainDisp.drawFastVLine(13+i, 29+i/2, 18-i, stCol);
  }

  // 歌曲信息
  mainDisp.setTextColor(0xFFFF);
  mainDisp.setCursor(42, 22);
  mainDisp.print(truncate(trackTitle, 11));
  mainDisp.setTextColor(0xFEA0);
  mainDisp.setCursor(42, 34);
  mainDisp.print(truncate(trackArtist, 11));

  // 分割线
  mainDisp.drawFastHLine(0, 62, 128, 0x2104);

  // 音量文字
  mainDisp.setTextColor(0x7BEF);
  mainDisp.setCursor(4, 68);
  mainDisp.print("VOL");
  mainDisp.setTextColor(0xFFFF);
  mainDisp.setCursor(28, 68);
  mainDisp.print(volume);
  mainDisp.print("%");

  // 音量条
  mainDisp.fillRoundRect(4, 78, 100, 5, 2, 0x1083);
  int volW = (int)(volume / 100.0f * 100);
  for (int i = 0; i < volW; i++) {
    uint16_t c = ((float)i / 100.0f) < 0.6f ? 0x901E : 0xF81F;
    mainDisp.drawFastVLine(4+i, 78, 5, c);
  }

  // 提示
  mainDisp.drawFastHLine(0, 115, 128, 0x2104);
  mainDisp.setTextColor(0x39E7);
  mainDisp.setCursor(2, 118);
  mainDisp.print("SET:Vol+ RST:Vol- MID:Play");
}

// ── 状态栏刷新 ────────────────────────────────────
void refreshStatus() {
  statusDisp.clearDisplay();
  statusDisp.setTextSize(1);
  statusDisp.setTextColor(SSD1306_WHITE);

  if (btConnected) statusDisp.fillCircle(4, 4, 3, SSD1306_WHITE);
  else             statusDisp.drawCircle(4, 4, 3, SSD1306_WHITE);

  statusDisp.setCursor(11, 0);
  statusDisp.print(isPlaying ? ">" : "||");
  statusDisp.setCursor(26, 0);
  statusDisp.print(truncate(trackTitle, 10));
  statusDisp.setCursor(98, 0);
  statusDisp.print(volume);
  statusDisp.print("%");

  statusDisp.drawFastHLine(0, 9, 128, SSD1306_WHITE);

  statusDisp.setCursor(0, 12);
  statusDisp.print(truncate(trackArtist, 21));

  int volW = (int)(volume / 100.0f * 128);
  if (volW > 0) statusDisp.fillRect(0, 28, volW, 4, SSD1306_WHITE);
  statusDisp.drawRect(0, 28, 128, 4, SSD1306_WHITE);

  statusDisp.display();
}

// ── setup ─────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  // 输入引脚
  int pullupPins[] = {JOY_UP, JOY_LFT, JOY_RHT, JOY_MID, BTN_SET, BTN_RST};
  for (int p : pullupPins) pinMode(p, INPUT_PULLUP);

  // 主屏
  SPI.begin(18, -1, 23, OLED_CS);
  mainDisp.begin();
  mainDisp.fillScreen(0x0841);
  mainDisp.setTextColor(0x07FF);
  mainDisp.setTextSize(2);
  mainDisp.setCursor(22, 44);
  mainDisp.print("PRISM");
  mainDisp.setTextSize(1);
  mainDisp.setTextColor(0xFEA0);
  mainDisp.setCursor(20, 68);
  mainDisp.print("Starting BT...");

  // 状态栏
  Wire.begin(21, 33);
  if (statusDisp.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    statusDisp.clearDisplay();
    statusDisp.setTextColor(SSD1306_WHITE);
    statusDisp.setCursor(0, 0);
    statusDisp.print("PRISM  Booting...");
    statusDisp.display();
  }

  // I2S
  i2s.setPins(26, 25, 22);
  if (!i2s.begin(I2S_MODE_STD, 44100, I2S_DATA_BIT_WIDTH_16BIT,
                 I2S_SLOT_MODE_STEREO, I2S_STD_SLOT_BOTH)) {
    Serial.println("[I2S] Init failed!");
  }

  // A2DP
  a2dp_sink.set_volume_control(&vc);
  a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
  a2dp_sink.set_on_connection_state_changed(connection_state_callback);
  a2dp_sink.set_avrc_rn_playstatus_callback(playback_state_callback);
  a2dp_sink.start("PRISM");

  setVolume(80);
  Serial.println("[PRISM] Ready");
}

// ── loop ──────────────────────────────────────────
unsigned long lastRefresh = 0;

void loop() {
  if (pressed(BTN_SET, 0)) setVolume(volume + 5);
  if (pressed(BTN_RST, 1)) setVolume(volume - 5);
  if (pressed(JOY_LFT, 2)) a2dp_sink.previous();
  if (pressed(JOY_RHT, 3)) a2dp_sink.next();
  if (pressed(JOY_MID, 4)) {
    if (isPlaying) a2dp_sink.pause();
    else           a2dp_sink.play();
  }

  if (millis() - lastRefresh >= 200) {
    lastRefresh = millis();
    refreshMain();
    refreshStatus();
  }
}
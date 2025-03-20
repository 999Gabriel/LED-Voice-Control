#include <Adafruit_NeoPixel.h>
#include <Dabble.h>
#include <SoftwareSerial.h>

#define LED_PIN     5
#define NUM_LEDS    30
#define BRIGHTNESS  64

// Bluetooth Pins
#define BT_TX 2
#define BT_RX 3

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
String command;

void setup() {
  Serial.begin(9600);
  Dabble.begin(9600, BT_TX, BT_RX);
  
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();
  clearLEDs();
}

void loop() {
  Dabble.processInput();
  
  // Check USB Serial
  if (Serial.available() > 0) {
    command = Serial.readStringUntil('\n');
    Serial.println("USB Serial Befehl empfangen: " + command);
    processCommand(command);
  }
  
  // Check Voice Commands
  if (Terminal.available()) {
    String voiceCommand = Terminal.readString();
    voiceCommand.toUpperCase();
    Serial.println("Bluetooth Terminal Befehl empfangen: " + voiceCommand);
    processCommand(voiceCommand);
  }
}

void processCommand(String cmd) {
  cmd.trim();
  Serial.println("Verarbeite Befehl: " + cmd);
  
  // German commands
  if (cmd == "EINSCHALTEN" || cmd == "ON") {
    Serial.println("-> Führe aus: Alle LEDs einschalten (weiß)");
    setAllLEDs(strip.Color(255, 255, 255));
  }
  else if (cmd == "AUSSCHALTEN" || cmd == "OFF") {
    Serial.println("-> Führe aus: Alle LEDs ausschalten");
    clearLEDs();
  }
  else if (cmd == "ROT" || cmd == "RED") {
    Serial.println("-> Führe aus: Setze Farbe auf Rot");
    setAllLEDs(strip.Color(255, 0, 0));
  }
  else if (cmd == "GRUEN" || cmd == "GREEN") {
    Serial.println("-> Führe aus: Setze Farbe auf Grün");
    setAllLEDs(strip.Color(0, 255, 0));
  }
  else if (cmd == "BLAU" || cmd == "BLUE") {
    Serial.println("-> Führe aus: Setze Farbe auf Blau");
    setAllLEDs(strip.Color(0, 0, 255));
  }
  else if (cmd == "WEISS" || cmd == "WHITE") {
    Serial.println("-> Führe aus: Setze Farbe auf Weiß");
    setAllLEDs(strip.Color(255, 255, 255));
  }
  else if (cmd == "REGENBOGEN" || cmd == "RAINBOW") {
    Serial.println("-> Führe aus: Starte Regenbogen-Effekt");
    rainbow();
  }
  // New brightness commands
  else if (cmd == "HELL" || cmd == "BRIGHT") {
    Serial.println("-> Führe aus: Erhöhe Helligkeit");
    setBrightness(255);
  }
  else if (cmd == "DUNKEL" || cmd == "DIM") {
    Serial.println("-> Führe aus: Reduziere Helligkeit");
    setBrightness(64);
  }
  else if (cmd == "BLINKEN" || cmd == "BLINK") {
    Serial.println("-> Führe aus: Blink-Effekt");
    blinkEffect();
  }
  else if (cmd == "FADE" || cmd == "ÜBERGANG") {
    Serial.println("-> Führe aus: Fade-Effekt");
    fadeEffect();
  }
  else if (cmd == "WARM" || cmd == "WARM WHITE") {
    Serial.println("-> Führe aus: Warmes Weiß");
    setAllLEDs(strip.Color(255, 147, 41));
  }
  else if (cmd == "KALT" || cmd == "COOL WHITE") {
    Serial.println("-> Führe aus: Kaltes Weiß");
    setAllLEDs(strip.Color(255, 255, 255));
  }
  else if (cmd == "VORNE" || cmd == "FRONT") {
    setZone(0, NUM_LEDS/2, strip.Color(255, 255, 255));
  }
  else if (cmd == "HINTEN" || cmd == "BACK") {
    setZone(NUM_LEDS/2, NUM_LEDS, strip.Color(255, 255, 255));
  }
}

void setZone(int start, int end, uint32_t color) {
  for(int i = start; i < end; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void blinkEffect() {
  for(int i = 0; i < 5; i++) {
    setAllLEDs(strip.Color(255, 255, 255));
    delay(500);
    clearLEDs();
    delay(500);
  }
}

void fadeEffect() {
  for(int brightness = 0; brightness < 255; brightness++) {
    strip.setBrightness(brightness);
    strip.show();
    delay(10);
  }
  for(int brightness = 255; brightness >= 0; brightness--) {
    strip.setBrightness(brightness);
    strip.show();
    delay(10);
  }
}

void setAllLEDs(uint32_t color) {
  for(int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void clearLEDs() {
  setAllLEDs(strip.Color(0, 0, 0));
}

void rainbow() {
  static uint16_t hue = 0;
  for(int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(hue + (i * 65536L / strip.numPixels()))));
  }
  strip.show();
  hue += 256;
  delay(20);
}

void setBrightness(int bright) {
  strip.setBrightness(bright);
  strip.show();
}

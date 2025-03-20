# ArduVoice LED Control System

A voice-controlled LED system using Arduino and web interface, created by GAWI.

![ArduVoice Interface](/Users/gabriel/Developer/voice_control_spl/arduvoice.png)

## Features

- 🎤 Voice Control via Web Interface
- 📱 Bluetooth Control using Dabble App
- 🌈 Multiple LED Effects:
  - Basic colors (Red, Green, Blue, White)
  - Rainbow effect
  - Fade transitions
  - Blink patterns
  - Warm/Cool white modes
- 💡 Brightness Control
- 🌐 Bilingual Support (English/German)
- 🎯 Zone Control (Front/Back)

## Hardware Requirements

- Arduino Board
- WS2812B LED Strip
- HC-05/HC-06 Bluetooth Module
- Power Supply (5V)

## Pin Configuration

- LED Data Pin: 5
- Bluetooth TX: 2
- Bluetooth RX: 3

## Usage

1. Upload the Arduino code to your board
2. Connect the hardware according to the pin configuration
3. Start the web server:
   ```bash
   node server.js

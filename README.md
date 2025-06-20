# Digital Clock with Arduino Nano

A comprehensive digital clock project built with Arduino Nano, featuring a 6-digit 7-segment display, real-time clock, temperature/humidity sensor, alarm functionality, and timer.

## Features

### Display Mode
- **Time Display**: Shows current time in HH:MM:SS format
- **Date Display**: Shows current date in DD.MM.YY format (Button 2)
- **Temperature/Humidity**: Alternates between temperature and humidity every second (Button 3)
- **Alarm Display**: Shows current alarm time (Button 4)

### Settings Mode
- **Time Setting**: Adjust hours, minutes, and seconds
- **Date Setting**: Adjust day, month, and year
- **Alarm Setting**: Set alarm time and enable/disable
- **Timer Setting**: Set countdown timer

### Hardware Features
- 6-digit 7-segment display with BCD decoder and shift register
- DS1307 Real-Time Clock module
- DHT11 temperature and humidity sensor
- 4 control buttons
- Buzzer for alarm functionality

## Hardware Requirements

### Components
- Arduino Nano
- 6-digit 7-segment display
- 74HC595 shift register
- BCD to 7-segment decoder (74LS47 or similar)
- DS1307 RTC module
- DHT11 sensor
- 4 push buttons
- Buzzer
- Resistors (220Ω for display segments, 10kΩ for buttons)
- Breadboard and jumper wires

### Pin Connections

| Component | Arduino Pin | Description |
|-----------|-------------|-------------|
| Button 1 | D2 | Settings mode entry |
| Button 2 | D3 | Display mode / Settings adjustment |
| Button 3 | D4 | Display mode / Settings adjustment |
| Button 4 | D5 | Display mode / Settings adjustment |
| Buzzer | D6 | Alarm output |
| DHT11 | D7 | Temperature/humidity sensor |
| Shift Register Data | D8 | 74HC595 data pin |
| Shift Register Clock | D9 | 74HC595 clock pin |
| Shift Register Latch | D10 | 74HC595 latch pin |
| RTC SDA | A4 | I2C data line |
| RTC SCL | A5 | I2C clock line |

## Software Architecture

The project is organized into modular classes:

### Core Classes
- **Clock**: Main clock logic, time management, alarm, and timer
- **Display**: 7-segment display control via shift register
- **RTC**: DS1307 real-time clock interface
- **Button**: Debounced button input handling
- **Buzzer**: Alarm tone generation
- **DHT11**: Temperature and humidity sensor interface
- **EEPROM**: Settings storage and retrieval

### File Structure
```
arduino-clock/
├── src/
│   └── main.cpp              # Main Arduino program
├── include/
│   ├── Clock.h              # Clock class header
│   ├── Display.h            # Display class header
│   ├── RTC.h               # RTC class header
│   ├── Button.h            # Button class header
│   ├── Buzzer.h            # Buzzer class header
│   ├── DHT11.h             # DHT11 class header
│   └── EEPROM.h            # EEPROM class header
├── lib/
│   ├── Clock.cpp            # Clock class implementation
│   ├── Display.cpp          # Display class implementation
│   ├── RTC.cpp             # RTC class implementation
│   ├── Button.cpp          # Button class implementation
│   ├── Buzzer.cpp          # Buzzer class implementation
│   ├── DHT11.cpp           # DHT11 class implementation
│   └── EEPROM.cpp          # EEPROM class implementation
├── platformio.ini          # PlatformIO configuration
└── README.md               # This file
```

## Usage Instructions

### Normal Operation
1. **Time Display**: Default mode shows current time
2. **Date View**: Press Button 2 to view current date
3. **Temperature/Humidity**: Press Button 3 to view sensor data
4. **Alarm View**: Press Button 4 to view alarm time
5. **Auto-return**: Display returns to time after 5 seconds of inactivity

### Settings Mode
1. **Enter Settings**: Hold Button 1 for 3 seconds
2. **Navigate Settings**: Press Button 1 to cycle through settings
3. **Adjust Values**: Use Buttons 2, 3, and 4 to adjust different parts of each setting
4. **Exit Settings**: Settings mode automatically exits after 30 seconds or when settings are saved

### Settings Navigation
- **Time Setting**: Button 2 (hour), Button 3 (minute), Button 4 (second)
- **Date Setting**: Button 2 (day), Button 3 (month), Button 4 (year)
- **Alarm Setting**: Button 2 (hour), Button 3 (minute), Button 4 (enable/disable)
- **Timer Setting**: Button 2 (hour), Button 3 (minute), Button 4 (second)

## Building and Uploading

### Prerequisites
- PlatformIO IDE or PlatformIO CLI
- Arduino Nano board
- Required libraries (automatically installed via platformio.ini)

### Build Commands
```bash
# Build the project
pio run

# Upload to Arduino Nano
pio run --target upload

# Monitor serial output
pio device monitor
```

### Required Libraries
- DHT sensor library (Adafruit)
- Adafruit Unified Sensor
- RTClib
- Arduino EEPROM

## Configuration

### Pin Configuration
Edit the pin definitions in `src/main.cpp` to match your wiring:

```cpp
const int BUTTON_1_PIN = 2;
const int BUTTON_2_PIN = 3;
const int BUTTON_3_PIN = 4;
const int BUTTON_4_PIN = 5;
const int BUZZER_PIN = 6;
const int DHT_PIN = 7;
const int SHIFT_DATA_PIN = 8;
const int SHIFT_CLOCK_PIN = 9;
const int SHIFT_LATCH_PIN = 10;
const int RTC_SDA_PIN = A4;
const int RTC_SCL_PIN = A5;
```

### Timing Configuration
Adjust timing parameters in the code:
- Settings mode timeout: 30 seconds
- Display auto-return: 5 seconds
- Temperature update interval: 2 seconds
- Alarm duration: 10 seconds

## Troubleshooting

### Common Issues
1. **Display not working**: Check shift register connections and BCD decoder
2. **RTC not responding**: Verify I2C connections and DS1307 battery
3. **Buttons not responding**: Check pull-up resistors and button connections
4. **DHT11 readings invalid**: Ensure proper wiring and power supply
5. **Alarm not working**: Check buzzer connections and volume

### Debug Information
The program outputs debug information via Serial at 9600 baud:
- Initialization status
- Settings mode entry/exit
- Error messages for hardware issues

## License

This project is open source and available under the MIT License.

## Contributing

Feel free to submit issues, feature requests, or pull requests to improve the project. 
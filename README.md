# Digital Clock with Arduino Nano

A comprehensive digital clock project built with Arduino Nano, featuring a 4-digit 7-segment display with BCD multiplexing, real-time clock, temperature/humidity sensor, alarm functionality, timer, and serial command interface.

## Features

### Display Mode

- **Time Display**: Shows current time in HH:MM:SS format with blinking colon
- **Date Display**: Shows current date in DD.MM.YYYY format (Button 1)
- **Temperature/Humidity**: Alternates between temperature and humidity every 3 seconds (Button 2)
- **Alarm Display**: Shows current alarm time (Button 3)
- **Timer Display**: Shows current timer countdown (Button 4)

### Settings Mode

- **Time Setting**: Adjust hours, minutes, and seconds
- **Date Setting**: Adjust day, month, and year
- **Alarm Setting**: Set alarm time and enable/disable
- **Timer Setting**: Set countdown timer duration

### Serial Command Interface

- **Real-time Control**: Control all clock functions via serial commands
- **Status Monitoring**: Get current time, date, temperature, humidity, alarm, and timer status
- **Remote Configuration**: Set time, date, alarm, and timer remotely

### Hardware Features

- 4-digit 7-segment display with BCD multiplexing
- DS1307 Real-Time Clock module
- DHT11 temperature and humidity sensor
- 4 control buttons with debouncing
- Buzzer for alarm functionality
- EEPROM storage for settings persistence

## Hardware Requirements

### Components

- Arduino Nano
- 4-digit 7-segment display with BCD decoder
- 74HC595 shift register
- DS1307 RTC module
- DHT11 sensor
- 4 push buttons
- Buzzer
- Resistors (220Ω for display segments, 10kΩ for buttons)
- Breadboard and jumper wires

### Pin Connections

| Component            | Arduino Pin | Description                        |
| -------------------- | ----------- | ---------------------------------- |
| DHT11                | A0          | Temperature/humidity sensor        |
| Button 1             | A1          | Display mode / Settings navigation |
| Button 2             | A2          | Display mode / Settings adjustment |
| Button 3             | A3          | Display mode / Settings adjustment |
| Button 4             | D12         | Display mode / Settings adjustment |
| Buzzer               | D9          | Alarm output                       |
| BCD Digit 1          | D3          | Display multiplexing               |
| BCD Digit 2          | D4          | Display multiplexing               |
| BCD Digit 3          | D5          | Display multiplexing               |
| BCD Digit 4          | D6          | Display multiplexing               |
| Shift Register Data  | D8          | 74HC595 data pin                   |
| Shift Register Clock | D7          | 74HC595 clock pin                  |
| RTC SDA              | A4          | I2C data line                      |
| RTC SCL              | A5          | I2C clock line                     |

## Software Architecture

The project is organized into modular classes:

### Core Classes

- **Clock**: Main clock logic, time management, alarm, and timer coordination
- **Display**: 4-digit 7-segment display control with BCD multiplexing
- **RTClock**: DS1307 real-time clock interface
- **Button**: Debounced button input handling with long press detection
- **Buzzer**: Alarm tone generation
- **HTSensor**: DHT11 temperature and humidity sensor interface
- **EEPROMStorage**: Settings storage and retrieval
- **Alarm**: Alarm functionality and management
- **Timer**: Countdown timer functionality
- **SerialCommandHandler**: Serial communication and command processing

### File Structure

```
arduino-clock/
├── src/
│   ├── main.cpp                    # Main Arduino program
│   ├── Clock.cpp                   # Clock class implementation
│   ├── Display.cpp                 # Display class implementation
│   ├── RTClock.cpp                 # RTC class implementation
│   ├── Button.cpp                  # Button class implementation
│   ├── Buzzer.cpp                  # Buzzer class implementation
│   ├── HTSensor.cpp                # DHT11 class implementation
│   ├── EEPROMStorage.cpp           # EEPROM class implementation
│   ├── Alarm.cpp                   # Alarm class implementation
│   ├── Timer.cpp                   # Timer class implementation
│   └── SerialCommandHandler.cpp    # Serial command handler implementation
├── include/
│   ├── Clock.h                     # Clock class header
│   ├── Display.h                   # Display class header
│   ├── RTClock.h                   # RTC class header
│   ├── Button.h                    # Button class header
│   ├── Buzzer.h                    # Buzzer class header
│   ├── HTSensor.h                  # DHT11 class header
│   ├── EEPROMStorage.h             # EEPROM class header
│   ├── Alarm.h                     # Alarm class header
│   ├── Timer.h                     # Timer class header
│   └── SerialCommandHandler.h      # Serial command handler header
├── platformio.ini                  # PlatformIO configuration
├── README.md                       # This file
├── REQUIREMENTS.md                 # Original project requirements
└── SERIAL_COMMANDS.md              # Serial commands documentation
```

## Usage Instructions

### Normal Operation

1. **Time Display**: Default mode shows current time with blinking colon
2. **Date View**: Press Button 1 to view current date
3. **Temperature/Humidity**: Press Button 2 to view sensor data (alternates every 3 seconds)
4. **Alarm View**: Press Button 3 to view alarm time
5. **Timer View**: Press Button 4 to view timer countdown
6. **Auto-return**: Display returns to time after button release

### Settings Mode

1. **Enter Settings**: Long press Button 1 (3 seconds)
2. **Navigate Settings**: Press Button 1 to cycle through settings (Time → Date → Alarm → Timer)
3. **Adjust Values**: Use Buttons 2, 3, and 4 to adjust different parts of each setting
4. **Exit Settings**: Settings mode automatically exits after 30 seconds or when Button 1 is long pressed again

### Settings Navigation

- **Time Setting**: Button 2 (hour), Button 3 (minute), Button 4 (second)
- **Date Setting**: Button 2 (day), Button 3 (month), Button 4 (year)
- **Alarm Setting**: Button 2 (hour), Button 3 (minute), Button 4 (enable/disable)
- **Timer Setting**: Button 2 (hour), Button 3 (minute), Button 4 (second)

### Serial Commands

Connect via serial monitor at 115200 baud to use commands:

- `help` - Show available commands
- `status` - Display current status
- `time HHMMSS` - Set time
- `date DDMMYYYY` - Set date
- `alarm set HHMM` - Set alarm time
- `alarm on/off` - Enable/disable alarm
- `timer set HHMMSS` - Set timer duration
- `timer start/stop/reset` - Control timer

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

- DHT sensor library (Adafruit) v1.4.4
- Adafruit Unified Sensor v1.1.9
- RTClib v2.1.1
- Arduino EEPROM (built-in)

## Configuration

### Pin Configuration

Edit the pin definitions in `src/main.cpp` to match your wiring:

```cpp
#define DHT_PIN A0
#define BUZZER_PIN 9
#define BUTTON_1_PIN A1
#define BUTTON_2_PIN A2
#define BUTTON_3_PIN A3
#define BUTTON_4_PIN 12
#define BCD_1_PIN 3
#define BCD_2_PIN 4
#define BCD_3_PIN 5
#define BCD_4_PIN 6
#define CLOCK_PIN 7
#define SHIFT_PIN 8
#define RTC_SDA_PIN A4
#define RTC_SCL_PIN A5
```

### Timing Configuration

Adjust timing parameters in the code:

- Settings mode timeout: 30 seconds
- Temperature/humidity toggle: 3 seconds
- Dot blink interval: 500ms
- Settings exit cooldown: 2 seconds
- Button long press: 3 seconds

## Troubleshooting

### Common Issues

1. **Display not working**: Check BCD decoder connections and shift register wiring
2. **RTC not responding**: Verify I2C connections and DS1307 battery
3. **Buttons not responding**: Check pull-up resistors and button connections
4. **DHT11 readings invalid**: Ensure proper wiring and power supply
5. **Alarm not working**: Check buzzer connections and volume
6. **Serial commands not working**: Verify baud rate is set to 115200

### Debug Information

The program outputs debug information via Serial at 115200 baud:

- Initialization status
- Settings mode entry/exit
- Serial command processing
- Error messages for hardware issues

## License

This project is open source and available under the MIT License.

## Contributing

Feel free to submit issues, feature requests, or pull requests to improve the project.

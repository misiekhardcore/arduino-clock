# Serial Commands Documentation

The clock project includes a dedicated `SerialCommandHandler` class that provides a comprehensive interface for controlling the clock via serial communication. The code has been refactored to follow DRY (Don't Repeat Yourself) principles for better maintainability.

## Setup

The SerialCommandHandler is automatically initialized when the clock starts up. It communicates via Serial at **115200 baud rate**.

## Available Commands

### Basic Commands

- `help` or `h` - Show available commands
- `status` or `s` - Display current time, date, temperature, humidity, alarm status, and timer status

### Time and Date Commands

- `time HHMMSS` - Set the RTC time directly

  - Format: 24-hour time (HHMMSS)
  - Example: `time 143045`

- `date DDMMYYYY` - Set the RTC date directly
  - Format: DDMMYYYY
  - Example: `date 25122024`

### Alarm Commands

- `alarm` or `a` - Show alarm command help

- `alarm on` - Enable the alarm

- `alarm off` - Disable the alarm

- `alarm set HHMM` - Set alarm time
  - Format: 24-hour time (HHMM)
  - Example: `alarm set 0730`

### Timer Commands

- `timer` or `tr` - Show timer command help

- `timer start` - Start the timer countdown

- `timer stop` - Stop the timer countdown

- `timer reset` - Reset the timer to 00:00:00

- `timer set HHMMSS` - Set timer duration
  - Format: HHMMSS
  - Example: `timer set 000530` (5 minutes 30 seconds)

## Usage Examples

### Setting the time

```
> time 143000
Time set to: 14:30:00
```

### Setting an alarm

```
> alarm set 0700
Alarm set to: 07:00
> alarm on
Alarm enabled
```

### Setting a timer

```
> timer set 001000
Timer set to: 00:10:00
> timer start
Timer started
```

### Checking status

```
> status
=== Clock Status ===
Time: 14:30:25
Date: 25.12.2024
Temperature: 22°C
Humidity: 45%
Alarm: 07:00 (Enabled)
Timer: 00:09:45 (Running)
==================
```

## Command Reference

| Command            | Description         | Example                      |
| ------------------ | ------------------- | ---------------------------- |
| `help`             | Show all commands   | `help`                       |
| `status`           | Show current status | `status`                     |
| `time HHMM(SS)`    | Set time            | `time 1430` or `time 143020` |
| `date DDMMYYYY`    | Set date            | `date 25122024`              |
| `alarm set HHMM`   | Set alarm time      | `alarm set 0730`             |
| `alarm on`         | Enable alarm        | `alarm on`                   |
| `alarm off`        | Disable alarm       | `alarm off`                  |
| `timer set HHMMSS` | Set timer duration  | `timer set 000530`           |
| `timer start`      | Start timer         | `timer start`                |
| `timer stop`       | Stop timer          | `timer stop`                 |
| `timer reset`      | Reset timer         | `timer reset`                |

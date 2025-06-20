#include <Arduino.h>
#include "Clock.h"
#include "Display.h"
#include "RTClock.h"
#include "Button.h"
#include "Buzzer.h"
#include "HTSensor.h"
#include "EEPROMStorage.h"

// Pin definitions
#define BUTTON_1_PIN A1
#define BUTTON_2_PIN A2
#define BUTTON_3_PIN A3
#define BUTTON_4_PIN A6
#define BUZZER_PIN 9
#define DHT_PIN A0

// BCD selector pins for display multiplexing
#define BCD_1_PIN 3
#define BCD_2_PIN 4
#define BCD_3_PIN 5
#define BCD_4_PIN 6

// Shift register pins
#define SHIFT_PIN 8
#define CLOCK_PIN 7

#define RTC_SDA_PIN A4
#define RTC_SCL_PIN A5

// Global objects
Clock clock;
Display display;
RTClock rtc;
Button button1(BUTTON_1_PIN);
Button button2(BUTTON_2_PIN);
Button button3(BUTTON_3_PIN);
Button button4(BUTTON_4_PIN);
Buzzer buzzer(BUZZER_PIN);
HTSensor dht11(DHT_PIN);

// Mode variables
bool isSettingsMode = false;
unsigned long settingsModeStartTime = 0;
const unsigned long SETTINGS_MODE_TIMEOUT = 30000; // 30 seconds timeout

// Display state variables
unsigned long lastDisplayUpdate = 0;
unsigned long lastTempHumidityToggle = 0;
unsigned long lastDotToggle = 0;
bool showTemperature = true;
bool dotState = false;
int currentDisplayMode = 0; // 0: time, 1: date, 2: temp/humidity, 3: alarm

// Settings variables
int currentSetting = 0; // 0: time, 1: date, 2: alarm, 3: timer
int settingBlinkState = 0;
unsigned long lastBlinkTime = 0;
const unsigned long BLINK_INTERVAL = 100; // Match old program's 100ms blink

// Add this variable with the other global variables
unsigned long lastSettingsExitTime = 0;
const unsigned long SETTINGS_EXIT_COOLDOWN = 2000; // 2 second cooldown

// Function declarations
void enterSettingsMode();
void exitSettingsMode();
void handleDisplayMode();
void handleSettingsMode();
void handleSerialCommands();
void setRTCTime();
void setRTCDate();
void showHelp();

void setup()
{
  Serial.begin(115200);

  // Initialize components with BCD multiplexed display
  display.begin(BCD_1_PIN, BCD_2_PIN, BCD_3_PIN, BCD_4_PIN, SHIFT_PIN, CLOCK_PIN);
  rtc.begin(RTC_SDA_PIN, RTC_SCL_PIN);
  buzzer.begin();
  dht11.begin();

  // Initialize buttons
  button1.begin();
  button2.begin();
  button3.begin();
  button4.begin();

  // Initialize clock
  clock.begin(&rtc, &dht11);

  // Load settings from EEPROM
  clock.loadSettings();

  Serial.println("Digital Clock initialized");
  Serial.println("Type 'help' for available commands");
  Serial.println("Type 'status' to see current time and sensor data");
}

void loop()
{
  // Handle serial commands
  handleSerialCommands();

  // Update button states
  button1.update();
  button2.update();
  button3.update();
  button4.update();

  // Update buzzer
  buzzer.update();

  // Check for settings mode entry (hold button 1 for 3 seconds)
  // Add cooldown period to prevent immediate re-entry
  // if (button1.isPressed() && button1.getPressDuration() >= 3000 && !isSettingsMode &&
  //     (millis() - lastSettingsExitTime) > SETTINGS_EXIT_COOLDOWN)
  // {
  //   enterSettingsMode();
  // }

  // Check for settings mode exit (timeout only)
  if (isSettingsMode)
  {
    if (millis() - settingsModeStartTime > SETTINGS_MODE_TIMEOUT)
    {
      exitSettingsMode();
    }
  }

  // Handle mode-specific logic
  if (isSettingsMode)
  {
    handleSettingsMode();
  }
  else
  {
    handleDisplayMode();
  }

  // Update clock
  clock.update();

  // Handle dot blinking
  if (millis() - lastDotToggle > 500)
  {
    lastDotToggle = millis();
    dotState = !dotState;

    // Set dot bit based on state and current mode
    if (currentDisplayMode == 0 && dotState)
    {
      display.setDotState(true);
    }
    else
    {
      display.setDotState(false);
    }
  }

  // Check alarm
  if (clock.isAlarmTriggered())
  {
    buzzer.playAlarm();
  }

  // Continuous display refresh for BCD multiplexed display
  // The display.update() method handles the multiplexing internally
  display.update();
}

void enterSettingsMode()
{
  isSettingsMode = true;
  settingsModeStartTime = millis();
  currentSetting = 0;
  settingBlinkState = 0;
  lastBlinkTime = millis();
  Serial.println("Entered settings mode");
}

void exitSettingsMode()
{
  isSettingsMode = false;
  lastSettingsExitTime = millis(); // Set the exit time
  clock.saveSettings();
  Serial.println("Exited settings mode");
}

void handleDisplayMode()
{
  // Handle button presses for display mode - show content only while button is pressed
  if (button1.isPressed() && !button2.isPressed())
  {
    // Button 1 shows date while pressed
    currentDisplayMode = 1;
  }
  else if (button2.isPressed())
  {
    // Button 2 shows temperature/humidity while pressed
    currentDisplayMode = 2;
  }
  else if (button3.isPressed())
  {
    // Button 3 shows alarm while pressed
    currentDisplayMode = 3;
  }
  // else if (button4.isPressed())
  // {
  //   // Button 4 shows timer while pressed
  //   currentDisplayMode = 4;
  // }
  else
  {
    // No buttons pressed - return to time display
    currentDisplayMode = 0;
  }

  // Set display digits based on current mode (display.update() called in main loop)
  switch (currentDisplayMode)
  {
  case 0: // Time
  {
    display.print(clock.getTimeString());
  }
  break;
  case 1: // Date
  {
    display.print(clock.getDateString());
  }
  break;
  case 2: // Temperature/Humidity
    if (millis() - lastTempHumidityToggle > 3000)
    {
      showTemperature = !showTemperature;
      lastTempHumidityToggle = millis();
    }
    if (showTemperature)
    {
      display.print(clock.getTemperatureString());
    }
    else
    {
      display.print(clock.getHumidityString());
    }
    break;
  case 3: // Alarm
  {
    display.print(clock.getAlarmTimeString());
  }
  break;
  }

  lastDisplayUpdate = millis();
}

void handleSettingsMode()
{
  // Handle button presses for settings mode
  if (button1.wasPressed())
  {
    currentSetting = (currentSetting + 1) % 3; // Cycle through 3 settings: time, date, alarm (matching old program)
    settingBlinkState = 0;
    lastBlinkTime = millis();
  }

  if (button2.wasPressed())
  {
    clock.adjustSetting(currentSetting, 0); // Adjust first part of setting
  }

  if (button3.wasPressed())
  {
    clock.adjustSetting(currentSetting, 1); // Adjust second part of setting
  }

  // if (button4.wasPressed())
  // {
  //   clock.adjustSetting(currentSetting, 2); // Adjust third part of setting
  // }

  // Blink the current setting (matching old Arduino program's 100ms interval)
  if (millis() - lastBlinkTime > BLINK_INTERVAL)
  {
    settingBlinkState = !settingBlinkState;
    lastBlinkTime = millis();
  }

  // Display current setting with blinking (display.update() called in main loop)
  if (settingBlinkState)
  {
    switch (currentSetting)
    {
    case 0: // Time
    {
      display.print(clock.getTimeString());
    }
    break;
    case 1: // Date
    {
      display.print(clock.getDateString());
    }
    break;
    case 2: // Alarm
    {
      display.print(clock.getAlarmTimeString());
    }
    break;
    }
  }
  else
  {
    // Blink off - show blank display
    display.clear();
  }
}

void handleSerialCommands()
{
  if (Serial.available())
  {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toUpperCase();

    if (command == "SETTIME" || command == "TIME")
    {
      setRTCTime();
    }
    else if (command == "SETDATE" || command == "DATE")
    {
      setRTCDate();
    }
    else if (command == "NOW" || command == "CURRENT")
    {
      setRTCTime();
      setRTCDate();
    }
    else if (command == "HELP" || command == "?")
    {
      showHelp();
    }
    else if (command == "STATUS" || command == "INFO")
    {
      Time time = clock.getTime();
      Date date = clock.getDate();
      Serial.print("Current RTC Time: ");
      if (time.hour < 10)
        Serial.print("0");
      Serial.print(time.hour);
      Serial.print(":");
      if (time.minute < 10)
        Serial.print("0");
      Serial.print(time.minute);
      Serial.print(":");
      if (time.second < 10)
        Serial.print("0");
      Serial.println(time.second);

      Serial.print("Current RTC Date: ");
      if (date.day < 10)
        Serial.print("0");
      Serial.print(date.day);
      Serial.print("/");
      if (date.month < 10)
        Serial.print("0");
      Serial.print(date.month);
      Serial.print("/");
      Serial.println(date.year);

      Serial.print("Temperature: ");
      Serial.print(clock.getTemperature());
      Serial.println("°C");
      Serial.print("Humidity: ");
      Serial.print(clock.getHumidity());
      Serial.println("%");
    }
    else if (command.length() > 0)
    {
      Serial.println("Unknown command. Type 'help' for available commands.");
    }
  }
}

void setRTCTime()
{
  Serial.println("Enter current time (HH:MM:SS format):");
  Serial.println("Example: 1430");

  while (!Serial.available())
  {
    delay(10);
  }

  String timeStr = Serial.readStringUntil('\n');
  timeStr.trim();

  // Parse HHMM format
  int hour = timeStr.substring(0, 2).toInt();
  int minute = timeStr.substring(2, 4).toInt();

  if (hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59)
  {
    Time newTime = {hour, minute, 0};
    rtc.setTime(newTime);
    Serial.print("RTC time set to: ");
    if (hour < 10)
      Serial.print("0");
    Serial.print(hour);
    Serial.print(":");
    if (minute < 10)
      Serial.print("0");
    Serial.print(minute);
    Serial.print(":");
    Serial.println("00");
  }
  else
  {
    Serial.println("Invalid time format. Use HHMM (e.g., 1430)");
  }
}

void setRTCDate()
{
  Serial.println("Enter current date (DDMMYYYY format):");
  Serial.println("Example: 25122024");

  while (!Serial.available())
  {
    delay(10);
  }

  String dateStr = Serial.readStringUntil('\n');
  dateStr.trim();

  // Parse DDMMYYYY format
  int day = dateStr.substring(0, 2).toInt();
  int month = dateStr.substring(2, 4).toInt();
  int year = dateStr.substring(4, 8).toInt();

  if (day >= 1 && day <= 31 && month >= 1 && month <= 12 && year >= 2020 && year <= 2100)
  {
    Date newDate = {day, month, year};
    rtc.setDate(newDate);
    Serial.print("RTC date set to: ");
    if (day < 10)
      Serial.print("0");
    Serial.print(day);
    Serial.print("/");
    if (month < 10)
      Serial.print("0");
    Serial.print(month);
    Serial.print("/");
    Serial.println(year);
  }
  else
  {
    Serial.println("Invalid date format. Use DDMMYYYY (e.g., 25122024)");
  }
}

void showHelp()
{
  Serial.println("Available commands:");
  Serial.println("  settime  - Set RTC time (HHMM format)");
  Serial.println("  setdate  - Set RTC date (DDMMYYYY format)");
  Serial.println("  now      - Set RTC to current time and date");
  Serial.println("  status   - Show current RTC time, date, temp, humidity");
  Serial.println("  help     - Show this help message");
  Serial.println("");
  Serial.println("Examples:");
  Serial.println("  settime  -> Enter: 1430");
  Serial.println("  setdate  -> Enter: 25122024");
}
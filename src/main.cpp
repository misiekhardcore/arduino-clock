#include <Arduino.h>
#include "Clock.h"
#include "Display.h"
#include "RTClock.h"
#include "Button.h"
#include "Buzzer.h"
#include "HTSensor.h"
#include "EEPROMStorage.h"
#include "Timer.h"
#include "Alarm.h"

// DHT pin
#define DHT_PIN A0

// Buzzer pin
#define BUZZER_PIN 9

// Button pins
#define BUTTON_1_PIN A1
#define BUTTON_2_PIN A2
#define BUTTON_3_PIN A3
#define BUTTON_4_PIN 12

// BCD selector pins for display multiplexing
#define BCD_1_PIN 3
#define BCD_2_PIN 4
#define BCD_3_PIN 5
#define BCD_4_PIN 6

// Shift register pins
#define CLOCK_PIN 7
#define SHIFT_PIN 8

// RTC pins
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
uint16_t SETTINGS_MODE_TIMEOUT = 30000;

// Display state variables
unsigned long lastDisplayUpdate = 0;
unsigned long lastTempHumidityToggle = 0;
unsigned long lastDotToggle = 0;
bool showTemperature = true;
bool dotState = false;
uint8_t currentDisplayMode = 0; // 0: time, 1: date, 2: temp/humidity, 3: alarm, 4: timer

// Settings variables
uint8_t currentSetting = 0;    // 0: time, 1: date, 2: alarm, 3: timer
uint8_t settingBlinkState = 0; // 0: off, 1: on
unsigned long lastBlinkTime = 0;
uint8_t BLINK_INTERVAL = 50; // 50ms

// Add this variable with the other global variables
unsigned long lastSettingsExitTime = 0;
uint16_t SETTINGS_EXIT_COOLDOWN = 2000; // 2 seconds

// Serial input state variables
// DISABLED: Serial input features to save memory
// bool waitingForTimeInput = false;
// bool waitingForDateInput = false;
// String timeInputBuffer = "";
// String dateInputBuffer = "";

// Function declarations
void enterSettingsMode();
void exitSettingsMode();
void handleDisplayMode();
void handleSettingsMode();
// DISABLED: Serial command functions to save memory
// void handleSerialCommands();
// void setRTCTime();
// void setRTCDate();
// void showHelp();

void setup()
{
  Serial.begin(115200);

  // Initialize components with BCD multiplexed display
  display.begin(BCD_1_PIN, BCD_2_PIN, BCD_3_PIN, BCD_4_PIN, SHIFT_PIN, CLOCK_PIN);

  // Initialize RTC
  rtc.begin(RTC_SDA_PIN, RTC_SCL_PIN);

  // Initialize buzzer
  buzzer.begin();

  // Initialize DHT11
  dht11.begin();

  // Initialize buttons
  button1.begin();
  button2.begin();
  button3.begin();
  button4.begin();

  // Initialize clock
  clock.begin(&rtc, &dht11, &buzzer);

  // Load settings from EEPROM
  clock.loadSettings();

  Serial.println("Type 'help' for available commands");
  Serial.println("Type 'status' to see current time and sensor data");
}

void loop()
{
  // Handle serial commands
  // handleSerialCommands();

  // Update button states
  button1.update();
  button2.update();
  button3.update();
  button4.update();

  // Toggle settings mode on long press of button 1
  if (button1.isLongPressed())
  {
    button1.reset();
    if (isSettingsMode)
    {
      exitSettingsMode();
    }
    else
    {
      enterSettingsMode();
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

  // Continuous display refresh for BCD multiplexed display
  display.update();
}

void enterSettingsMode()
{
  isSettingsMode = true;
  settingsModeStartTime = millis();
  currentSetting = 0;
  settingBlinkState = 0;
  lastBlinkTime = millis();
}

void exitSettingsMode()
{
  isSettingsMode = false;
  lastSettingsExitTime = millis(); // Set the exit time
  clock.saveSettings();
}

void handleDisplayMode()
{
  if (button1.isPressed()) // Date
  {
    currentDisplayMode = 1;
  }
  else if (button2.isPressed()) // Temperature/Humidity
  {
    currentDisplayMode = 2;
  }
  else if (button3.isPressed()) // Alarm
  {
    currentDisplayMode = 3;
  }
  else if (button4.isPressed()) // Timer
  {
    currentDisplayMode = 4;
  }
  else
  {
    currentDisplayMode = 0;
  }

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
  case 4: // Timer
  {
    display.print(clock.getTimerString());
  }
  break;
  }

  lastDisplayUpdate = millis();
}

void handleSettingsMode()
{
  if (button1.wasSinglePressed())
  {
    currentSetting = (currentSetting + 1) % 4;
    settingBlinkState = 0;
    lastBlinkTime = millis();
  }

  if (button2.wasSinglePressed())
  {
    clock.adjustSetting(currentSetting, 0); // Adjust first part of setting
  }

  if (button3.wasSinglePressed())
  {
    clock.adjustSetting(currentSetting, 1); // Adjust second part of setting
  }

  if (button4.wasSinglePressed())
  {
    clock.adjustSetting(currentSetting, 2); // Adjust third part of setting
  }

  // Blink the current setting
  if (millis() - lastBlinkTime > BLINK_INTERVAL)
  {
    settingBlinkState = !settingBlinkState;
    lastBlinkTime = millis();
  }

  // Display current setting
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
    case 3: // Timer
    {
      display.print(clock.getTimerString());
    }
    break;
    }
  }
  else
  {
    display.clear();
  }
}
I have here an empty platformio project. I want to build a program to control a digital clock.
I am using Arduino Nano as the microcontroller. The clock has a 6 digit 7 segment display which is controlled by BCD decoder and shift register. It also has a DHT11 sensor to measure the temperature and humidity. As an RTC I am using the DS1307 module. It also has 4 buttons to control the clock and a buzzer for the alarm.

The clock should have the following features:

- There have to be two modes:
  - Display mode
  - Settings mode
    To switch between the modes we hold the First button for 3 seconds.
- In the display mode:
  - The clock should show the current time when no button is pressed.
  - When the Second button is pressed the clock should show the date.
  - When the Third button is pressed the clock should show the temperature and humidity.
  - When the Fourth button is pressed the clock should show the alarm.
- In the settings mode:
  - Settings should be displayed blinking to indicate that we are in the settings mode.
  - When the First button is pressed we go through the settings.
  - There are the following settings:
    - Time (HH:MM:SS)
    - Date (DD.MM.YYYY)
    - Alarm (HH:MM)
    - Timer (HH:MM:SS)
  - When the Second button it changes the first setting.
  - When the Third button it changes the second setting.
  - When the Fourth button it changes the third setting.


#include "DHT.h"
#include <Wire.h>
#include "DS3231.h"
#include "EEPROM.h"

#define DHTPIN A0

#define BCD_1 3
#define BCD_2 4
#define BCD_3 5
#define BCD_4 6

#define CLOCK_PIN 7
#define SHIFT_PIN 8
#define ALARM_PIN 9

#define BUTTON_1_PIN A1
#define BUTTON_2_PIN A2
#define BUTTON_3_PIN A3
#define BUTTON_4_PIN A6

#define DHTTYPE DHT11

int DOT_B = 0;

int DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4, DIGIT_5, DIGIT_6, sethour = 17, setmin = 54, menuItem = 0, alarmHourAdress = 0, alarmMinuteAdress = 4, clockVar = 0;

unsigned long milis = 0, prevmilis1 = 0, prevmilis2 = 0, prevMillisMenu = 0, prevMillisAllarmBuzzer = 0;

bool dot = 0, temphum = 0, isMenu = 0, blinkSwitch = 0, Button1PrevState = 0, Button2PrevState = 0, Button3PrevState = 0, Button4PrevState = 0;
long buttonTimer = 0;
long longPressTime = 5000;

boolean buttonActive = false;
boolean longPressActive = false;

unsigned char table[] =
    {B00111111, B00000110, B01011011, B01001111, B01100110,
     B01101101, B01111101, B00100111, B01111111, B01101111,
     B01100011, B00111001, B00000000, B01110110};

DHT dht(DHTPIN, DHTTYPE);

DS3231 clock;
RTCDateTime dt;

bool Alarm(int hour, int min, int sec, int sethour, int setmin)
{
  if ((hour == sethour) && (min == setmin) && (sec < 30))
  {
    return true;
  }
  else
  {
    return false;
  }
}

void Display(unsigned char sign)
{
  shiftOut(SHIFT_PIN, CLOCK_PIN, MSBFIRST, sign);
}

bool Button(int pin)
{
  bool state = digitalRead(pin);
  if (state == 1)
  {
    return true;
  }
  else
    return false;
}

bool ButtonSinglePress(int pin, bool *prev)
{
  bool state = digitalRead(pin);
  bool output;

  if ((state != *prev) && (state == 1))
  {
    output = true;
  }
  else
    output = false;
  *prev = state;
  return output;
}

void AlarmBuzzer(int alarmpin, unsigned long *prev)
{
  unsigned long t = millis();
  if ((t - *prev < 200) || ((t - *prev > 300) && (t - *prev < 500)) || ((t - *prev > 600) && (t - *prev < 800)))
  {
    tone(alarmpin, 1000);
  }
  else
  {
    noTone(alarmpin);
  }
  if (t - *prev > 1200)
  {
    *prev = t;
  }
}

void setup()
{
  Serial.begin(9600);
  clock.begin();
  dht.begin();

  // clock.setDateTime(__DATE__, __TIME__);

  // EEPROM.write(alarmHourAdress, 0);
  // EEPROM.write(alarmMinuteAdress, 0);

  sethour = EEPROM.read(alarmHourAdress);
  setmin = EEPROM.read(alarmMinuteAdress);

  pinMode(BCD_1, OUTPUT);
  pinMode(BCD_2, OUTPUT);
  pinMode(BCD_3, OUTPUT);
  pinMode(BCD_4, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(SHIFT_PIN, OUTPUT);
  pinMode(ALARM_PIN, OUTPUT);

  pinMode(BUTTON_1_PIN, INPUT);
  digitalWrite(BUTTON_1_PIN, LOW);

  pinMode(BUTTON_2_PIN, INPUT);
  digitalWrite(BUTTON_2_PIN, LOW);

  pinMode(BUTTON_3_PIN, INPUT);
  digitalWrite(BUTTON_3_PIN, LOW);

  pinMode(BUTTON_4_PIN, INPUT);
  digitalWrite(BUTTON_4_PIN, LOW);
}

void loop()
{

  milis = millis();

  // WCZYTYWANIE AKTUALNEGO CZASU I DATY
  dt = clock.getDateTime();

  // WEJŚCIE DO MENU
  if (Button(BUTTON_1_PIN))
  {
    if (buttonActive == false)
    {
      buttonActive = true;
      buttonTimer = millis();
    }
    if ((millis() - buttonTimer > longPressTime) && (longPressActive == false))
    {
      longPressActive = true;
      isMenu = !isMenu;
      menuItem = 0;
    }
  }
  else
  {
    if (buttonActive == true)
    {
      if (longPressActive == true)
      {
        longPressActive = false;
      }
      buttonActive = false;
    }
  }

  // NORMALNE WYŚWIETLANIE
  if (!isMenu)
  {

    // WYSWIETLANIE KROPKI
    if (milis - prevmilis1 > 500)
    {
      prevmilis1 = milis;
      if (dot)
      {
        DOT_B = B10000000;
        dot = false;
      }
      else
      {
        DOT_B = 0;
        dot = true;
      }
    }

    // WYSWIETLANIE DATY
    if (Button(BUTTON_1_PIN) && !Button(BUTTON_2_PIN))
    {

      DOT_B = B10000000;

      DIGIT_1 = dt.day / 10;
      DIGIT_2 = dt.day % 10;
      DIGIT_3 = dt.month / 10;
      DIGIT_4 = dt.month % 10;
      DIGIT_5 = dt.year / 10 - 200;
      DIGIT_6 = dt.year % 10;
    }

    // WYSWIETLANIE TEMP I HUM
    else if (Button(BUTTON_2_PIN) && !Button(BUTTON_1_PIN))
    {

      DOT_B = 0;

      if (milis - prevmilis2 > 1000)
      {
        prevmilis2 = milis;

        if (!temphum)
        {
          DIGIT_1 = 12;
          DIGIT_2 = 12;
          DIGIT_3 = (int)dht.readTemperature() / 10;
          DIGIT_4 = (int)dht.readTemperature() % 10;
          DIGIT_5 = 10;
          DIGIT_6 = 11;
        }
        else
        {
          DIGIT_1 = 12;
          DIGIT_2 = 12;
          DIGIT_3 = (int)dht.readHumidity() / 10;
          DIGIT_4 = (int)dht.readHumidity() % 10;
          DIGIT_5 = 10;
          DIGIT_6 = 13;
        }
        temphum = !temphum;
      }
    }

    // WYSWIETLANIE KIEDY NIE MA INNYCH AKCJI
    else
    {
      DIGIT_1 = dt.hour / 10;
      DIGIT_2 = dt.hour % 10;
      DIGIT_3 = dt.minute / 10;
      DIGIT_4 = dt.minute % 10;
      DIGIT_5 = dt.second / 10;
      DIGIT_6 = dt.second % 10;
    }
  }

  // OBSŁUGA MENU
  if (isMenu)
  {

    // PORUSZANIE PO MENU
    if (ButtonSinglePress(BUTTON_1_PIN, &Button1PrevState))
    {
      menuItem++;
      if (menuItem > 3)
        menuItem = 1;
    }

    // MENU
    switch (menuItem)
    {

    // USTAWIANIE GODZINY
    case 1:
      if (ButtonSinglePress(BUTTON_2_PIN, &Button2PrevState))
      {
        if (dt.hour >= 23)
        {
          clockVar = 0;
        }
        else
          clockVar = dt.hour + 1;
        clock.setDateTime(dt.year, dt.month, dt.day, clockVar, dt.minute, dt.second);
      }

      if (ButtonSinglePress(BUTTON_3_PIN, &Button3PrevState))
      {
        if (dt.minute >= 59)
        {
          clockVar = 0;
        }
        else
          clockVar = dt.minute + 1;
        clock.setDateTime(dt.year, dt.month, dt.day, dt.hour, clockVar, dt.second);
      }

      if (ButtonSinglePress(BUTTON_4_PIN, &Button4PrevState))
      {
        if (dt.second >= 59)
        {
          clockVar = 0;
        }
        else
          clockVar = dt.second + 1;
        clock.setDateTime(dt.year, dt.month, dt.day, dt.hour, dt.minute, clockVar);
      }

      DIGIT_1 = dt.hour / 10;
      DIGIT_2 = dt.hour % 10;
      DIGIT_3 = dt.minute / 10;
      DIGIT_4 = dt.minute % 10;
      DIGIT_5 = dt.second / 10;
      DIGIT_6 = dt.second % 10;
      break;

    // USTAWIANIE DATY
    case 2:
      if (ButtonSinglePress(BUTTON_4_PIN, &Button2PrevState))
      {
        if (dt.year >= 2050)
        {
          clockVar = 2019;
        }
        else
          clockVar = dt.year + 1;
        clock.setDateTime(clockVar, dt.month, dt.day, dt.hour, dt.minute, dt.second);
      }

      if (ButtonSinglePress(BUTTON_3_PIN, &Button3PrevState))
      {
        if (dt.month >= 12)
        {
          clockVar = 1;
        }
        else
          clockVar = dt.month + 1;
        clock.setDateTime(dt.year, clockVar, dt.day, dt.hour, dt.minute, dt.second);
      }

      if (ButtonSinglePress(BUTTON_2_PIN, &Button4PrevState))
      {
        if (dt.day >= 31)
        {
          clockVar = 1;
        }
        else
          clockVar = dt.day + 1;
        clock.setDateTime(dt.year, dt.month, clockVar, dt.hour, dt.minute, dt.second);
      }

      DIGIT_1 = dt.day / 10;
      DIGIT_2 = dt.day % 10;
      DIGIT_3 = dt.month / 10;
      DIGIT_4 = dt.month % 10;
      DIGIT_5 = dt.year / 10 - 200;
      DIGIT_6 = dt.year % 10;
      break;

    // USATWIANIE ALARMU
    case 3:
      if (ButtonSinglePress(BUTTON_2_PIN, &Button2PrevState))
      {
        if (sethour >= 23)
        {
          sethour = 0;
        }
        else
          sethour++;
        EEPROM.write(alarmHourAdress, sethour);
      }

      if (ButtonSinglePress(BUTTON_3_PIN, &Button3PrevState))
      {
        if (setmin >= 59)
        {
          setmin = 0;
        }
        else
          setmin++;
        EEPROM.write(alarmMinuteAdress, setmin);
      }

      DIGIT_1 = sethour / 10;
      DIGIT_2 = sethour % 10;
      DIGIT_3 = setmin / 10;
      DIGIT_4 = setmin % 10;
      DIGIT_5 = 0;
      DIGIT_6 = 0;
      break;
    }

    // MIGANIE W MENU
    if (milis - prevmilis1 > 100)
    {
      prevmilis1 = milis;
      blinkSwitch = !blinkSwitch;
    }
    if (blinkSwitch)
    {
      DIGIT_1 = 12;
      DIGIT_2 = 12;
      DIGIT_3 = 12;
      DIGIT_4 = 12;
      DIGIT_5 = 12;
      DIGIT_6 = 12;
    }
  }

  // PRZELACZANIE I WYSWIETLANIE
  digitalWrite(BCD_1, HIGH);
  digitalWrite(BCD_2, LOW);
  digitalWrite(BCD_3, LOW);
  digitalWrite(BCD_4, LOW);
  Display(table[DIGIT_6]);
  delay(2);
  digitalWrite(BCD_1, LOW);
  digitalWrite(BCD_2, HIGH);
  digitalWrite(BCD_3, LOW);
  digitalWrite(BCD_4, LOW);
  Display(table[DIGIT_5]);
  delay(2);
  digitalWrite(BCD_1, HIGH);
  digitalWrite(BCD_2, HIGH);
  digitalWrite(BCD_3, LOW);
  digitalWrite(BCD_4, LOW);
  Display(table[DIGIT_4] + DOT_B);
  delay(2);
  digitalWrite(BCD_1, LOW);
  digitalWrite(BCD_2, LOW);
  digitalWrite(BCD_3, HIGH);
  digitalWrite(BCD_4, LOW);
  Display(table[DIGIT_3]);
  delay(2);
  digitalWrite(BCD_1, HIGH);
  digitalWrite(BCD_2, LOW);
  digitalWrite(BCD_3, HIGH);
  digitalWrite(BCD_4, LOW);
  Display(table[DIGIT_2] + DOT_B);
  delay(2);
  digitalWrite(BCD_1, LOW);
  digitalWrite(BCD_2, HIGH);
  digitalWrite(BCD_3, HIGH);
  digitalWrite(BCD_4, LOW);
  Display(table[DIGIT_1]);
  delay(2);

  // ALARM
  if (Alarm(dt.hour, dt.minute, dt.second, sethour, setmin))
  {
    AlarmBuzzer(ALARM_PIN, &prevMillisAllarmBuzzer);
  }
  else
    noTone(ALARM_PIN);
}
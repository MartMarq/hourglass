#include "Uhr.h"


RTC_DATA_ATTR int Welche = 1;

void Uhr::drawWatchFace() {

  // reset step counter at midnight
  if (currentTime.Hour == 00 && currentTime.Minute == 00) {
    sensor.resetStepCounter();
    //turn off radios
    WiFi.mode(WIFI_OFF);
    btStop();
  }

  //time adjustment start
  //if you don't need a time adjustment, just delete all lines between time adjustment start and end
  //my watchy runs too fast (15 seconds and 500 ms a day), this code set back time each day by 15 seconds and 500 ms

  if (currentTime.Hour == 00 && currentTime.Minute == 30) {

    RTC.read(currentTime);
    int8_t sekunde = currentTime.Second;
    int8_t minute = currentTime.Minute;
    int8_t hour = currentTime.Hour;
    int8_t day = currentTime.Day;
    int8_t month = currentTime.Month;
    int8_t year = tmYearToY2k(currentTime.Year);

    delay(15500);

    tmElements_t tm;
    tm.Month = month;
    tm.Day = day;
    tm.Year = y2kYearToTm(year);
    tm.Hour = hour;
    tm.Minute = minute;
    tm.Second = sekunde;

    RTC.set(tm);
  }
  // time adjustment end

  int stundeA = currentTime.Hour;
  int minuteA = currentTime.Minute;
  float VBAT = getBatteryVoltage();
  int n = 0;
  int sxu[59] = { 50, 58, 42, 54, 46, 50, 66, 34, 38, 62, 42, 58, 54, 46, 50, 26, 74, 70, 30, 34, 66, 62, 38, 42, 58, 54, 46, 50, 18, 82, 22, 78, 26, 74, 30, 70, 34, 66, 38, 62, 42, 58, 46, 54, 18, 82, 22, 78, 26, 74, 70, 30, 34, 66, 38, 62, 18, 82, 50 };
  int syu[59] = { 184, 184, 184, 176, 176, 168, 184, 184, 176, 176, 168, 168, 160, 160, 152, 184, 184, 176, 176, 168, 168, 160, 160, 152, 152, 144, 144, 136, 184, 184, 176, 176, 168, 168, 160, 160, 152, 152, 144, 144, 136, 136, 128, 128, 168, 168, 160, 160, 152, 152, 144, 144, 136, 136, 128, 128, 152, 152, 120 };
  int sxo[59] = { 50, 54, 46, 38, 62, 32, 42, 50, 58, 68, 25, 74, 54, 46, 35, 66, 29, 72, 62, 39, 50, 20, 80, 24, 76, 32, 68, 40, 60, 50, 82, 18, 26, 74, 34, 66, 42, 58, 50, 18, 26, 74, 34, 66, 42, 58, 50, 82, 82, 18, 26, 74, 34, 66, 42, 58, 50, 22, 78 };
  int syo[59] = { 95, 87, 87, 87, 87, 82, 79, 79, 79, 82, 76, 76, 71, 71, 75, 75, 68, 68, 67, 67, 63, 68, 68, 61, 61, 60, 60, 58, 58, 55, 55, 55, 53, 53, 51, 51, 49, 49, 47, 47, 45, 45, 43, 43, 41, 41, 39, 47, 39, 39, 37, 37, 35, 35, 33, 33, 31, 31, 31 };

  uint32_t Schritt = sensor.getCounter();
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  int16_t xc, yc, bk;
  uint16_t wc, hc;

  if (Welche == 2) {
    display.setFont(&FreeSans40pt7b);

    display.setCursor(0, 60);

    if (stundeA < 10) {
      display.print("0");
    }
    display.print(stundeA);
    display.print(":");
    if (minuteA < 10) {
      display.print("0");
    }
    display.println(minuteA);

    display.setFont(&FreeSans16pt7b);

    int16_t x1, y1;
    uint16_t w, h;

    String dayOfWeek = dayShortStr(currentTime.Wday);
    display.getTextBounds(dayOfWeek, 5, 85, &x1, &y1, &w, &h);

    display.setCursor(50, 125);
    display.println(dayOfWeek);

    String month = monthShortStr(currentTime.Month);
    display.getTextBounds(month, 60, 110, &x1, &y1, &w, &h);
    display.setCursor(50, 95);
    display.println(month);

    display.setCursor(5, 95);
    if (currentTime.Day < 10) {
      display.print("0");
    }
    display.println(currentTime.Day);

    display.drawBitmap(5, 175, steps, 19, 23, GxEPD_BLACK);
    display.drawBitmap(5, 145, akku, 30, 20, GxEPD_BLACK);
    display.setCursor(50, 195);
    display.println(Schritt);

    display.setCursor(50, 165);
    uint8_t Protz = min<int>(100 - ((4.20 - VBAT) * 100), 100);
    Protz = max<int>(Protz, 0);
    display.print(Protz);
    display.print(" %");

  } else {

    display.drawBitmap(0, 0, sanduhr, 100, 200, GxEPD_BLACK);
    display.drawBitmap(100, 0, sanduhr, 100, 200, GxEPD_BLACK);
    n=1;
    for (n; n <= 23; n++) {

      if (n <= stundeA) {
        display.fillCircle(sxu[n - 1], syu[n - 1], 4, GxEPD_BLACK);
      }
      if (n > stundeA) {
        display.fillCircle(sxo[n - 1 - stundeA], syo[n - 1 - stundeA], 4, GxEPD_BLACK);
      }
    }

    n = 1;
    for (n; n <= 59; n++) {
      if (n <= minuteA) {
        display.fillCircle(sxu[n - 1] + 100, syu[n - 1], 4, GxEPD_BLACK);
      }
      if (n > minuteA) {
        display.fillCircle(sxo[n - 1 - minuteA] + 100, syo[n - 1 - minuteA], 4, GxEPD_BLACK);
      }
    }
  }
}


void Uhr::handleButtonPress() {
  uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
  if (IS_DOUBLE_TAP) {
    while (!sensor.getINT()) {
    }
  }

if (IS_BTN_RIGHT_UP) {
    vibrate();
    Welche = Welche + 1;
    if (Welche > 2) {
      Welche = 1;
    }
    RTC.read(currentTime);
    drawWatchFace();
    showWatchFace(true);
  } else if (IS_BTN_RIGHT_DOWN) {
    vibrate();
    Welche = Welche - 1;
    if (Welche < 1) {
      Welche = 2;
    }
    RTC.read(currentTime);
    drawWatchFace();
    showWatchFace(true);
  } else {
    Watchy::handleButtonPress();
  }
}

void Uhr::vibrate() {

  sensor.enableFeature(BMA423_WAKEUP, false);
  pinMode(VIB_MOTOR_PIN, OUTPUT);

  delay(50);
  digitalWrite(VIB_MOTOR_PIN, true);
  delay(50);
  digitalWrite(VIB_MOTOR_PIN, false);

  sensor.enableFeature(BMA423_WAKEUP, true);
}

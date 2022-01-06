#include "Watchy999.h"
#include "borders.h"
#include "slides999.h"
#include "dktime999.h"
#include "watchytris.h"
#include "synth999.h"
#include "pxl999.h"
#include "crushem999.h"
#include "lowBatt999.h"
#include "G5600.h"
#include "pebbleText.h"
#include "doom999.h"
#include "qlocky.h"

RTC_DATA_ATTR bool charging = false;
RTC_DATA_ATTR bool chargeSync = false;
RTC_DATA_ATTR bool showCached = true;
RTC_DATA_ATTR float oldVoltage;
RTC_DATA_ATTR float maxVoltage = 4.2;
RTC_DATA_ATTR float battCalculator;
RTC_DATA_ATTR String tempCondition;
RTC_DATA_ATTR int rawTemperature;
RTC_DATA_ATTR int rtcTemperature;
RTC_DATA_ATTR int oldSteps = 0;
RTC_DATA_ATTR int updateHour;
RTC_DATA_ATTR int lowBattHour;
RTC_DATA_ATTR int lowBattMin;
int syncCount = 0;

RTC_DATA_ATTR timeData latestTime;
RTC_DATA_ATTR bool lowBattFace = false;

//Word Time Strings
const char *ONES[] = {"", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve",  "thirteen"};
const char *TENS[] = {"o'", nullptr, "twenty", "thirty", "forty", "fifty"};
const char* TEENS_SPLIT[][2] = { {"", ""}, {"eleven", ""}, {"twelve", ""}, {"thirteen", ""}, {"four", "teen"}, {"fifteen", ""},
  {"sixteen", ""}, {"seven", "teen"}, {"eight", "teen"}, {"nine", "teen"}
};

//Watch Face Settings - Boolean 0 False, 1 True - { Show Weather, Show Border, Toggle Border, Show Steps, hasDarkMode }
uint8_t dktime[5] { 0, 1, 0, 1, 0 };
uint8_t pxl999[5] { 1, 1, 0, 0, 0 };
uint8_t slides[5] { 0, 1, 1, 1, 1 };
uint8_t synth[5] { 1, 1, 1, 0, 1 };
uint8_t crushem[5] { 1, 0, 0, 1, 0 };
uint8_t lowbatt[5] { 0, 1, 1, 0, 1 };
uint8_t watchytris[5] { 1, 1, 0, 1, 0 };
uint8_t G5600[5] { 0, 1, 1, 0, 1 };
uint8_t pebbletext[5] { 0, 1, 1, 0, 1 };
uint8_t doom[5] { 0, 0, 0, 1, 0 };
uint8_t qlocky[5] { 0, 1, 1, 0, 1 };
uint8_t* watchFaces[] = { dktime, pxl999, slides, synth, crushem, lowbatt, watchytris, G5600, pebbletext, doom, qlocky };

void Watchy999::watchFaceSettings() {
  showWeather = watchFaces[watchFace][0];
  showBorder = watchFaces[watchFace][1];
  toggleBorder = watchFaces[watchFace][2];
  showSteps = watchFaces[watchFace][3];
  hasDarkMode = watchFaces[watchFace][4];
}

void Watchy999::displayWatchFace() {
  if (lowBattFace) { //Low Power Face
    drawLowBattWatchFace();
  } else if (sleep_mode) {
    gotoSleep();
  } else {
    switch (watchFace) {
      case 0:
        drawDkWatchFace();
        break;
      case 1:
        drawPxlWatchFace();
        break;
      case 2:
        drawSlidesWatchFace();
        break;
      case 3:
        drawSynthWatchFace();
        break;
      case 4:
        drawCeWatchFace();
        break;
      case 5:
        drawLowBattWatchFace();
        break;
      case 6:
        drawWatchytrisWatchFace();
        break;
      case 7:
        drawG5600WatchFace();
        break;
      case 8:
        drawPebbleTextFace();
        break;
      case 9:
        drawDoomWatchFace();
        break;
      case 10:
        drawQlockyWatchFace();
        break;
      default:
        drawDkWatchFace();
    }

  }
}

//pxl Time Font
uint8_t pxlLength[10] = { 4, 3, 5, 5, 3, 5, 5, 4, 7, 5 };
uint8_t pxlZero[16] = { 11, 0, 31, 11, 0, 11, 11, 31, 42, 11, 11, 31, 11, 42, 31, 11 };
uint8_t pxlOne[12] = { 11, 10, 10, 11, 21, 0, 11, 42, 0, 42, 53, 11 };
uint8_t pxlTwo[20] = { 0, 0, 42, 10, 42, 10, 11, 11, 11, 21, 31, 11, 0, 32, 11, 10, 0, 42, 53, 11 };
uint8_t pxlThree[20] = { 0, 0, 42, 10, 42, 10, 11, 11, 0, 21, 42, 11, 42, 32, 11, 10, 0, 42, 42, 11 };
uint8_t pxlFour[12] = { 0, 0, 11, 21, 11, 21, 31, 11, 42, 0, 11, 53 };
uint8_t pxlFive[20] = { 0, 0, 53, 11, 0, 11, 11, 10, 0, 21, 42, 11, 42, 32, 11, 10, 0, 42, 42, 11 };
uint8_t pxlSix[20] = { 11, 0, 42, 11, 0, 11, 11, 31, 11, 21, 31, 11, 42, 32, 11, 10, 11, 42, 31, 11 };
uint8_t pxlSeven[16] = { 0, 0, 53, 11, 42, 11, 11, 10, 31, 21, 11, 11, 20, 32, 11, 21 };
uint8_t pxlEight[28] = { 11, 0, 31, 11, 0, 11, 11, 10, 42, 11, 11, 10, 11, 21, 31, 11, 0, 32, 11, 10, 42, 32, 11, 10, 11, 42, 31, 11 };
uint8_t pxlNine[20] = { 11, 0, 31, 10, 0, 10, 11, 11, 42, 10, 11, 32, 11, 21, 31, 11, 11, 42, 31, 11 };
uint8_t* pxlDigits[] = { pxlZero, pxlOne, pxlTwo, pxlThree, pxlFour, pxlFive, pxlSix, pxlSeven, pxlEight, pxlNine };

//LECO Font
uint8_t lecoLength[10] = { 4, 3, 6, 4, 3, 6, 5, 3, 5, 5 };
uint8_t lecoZero[16] = { 0, 0, 21, 6, 0, 6, 6, 18, 15, 6, 6, 18, 0, 24, 21, 6 };
uint8_t lecoOne[12] = { 2, 0, 12, 6, 8, 6, 6, 18, 0, 24, 21, 6 };
uint8_t lecoTwo[24] = { 0, 0, 6, 9, 6, 0, 9, 6, 15, 0, 6, 18, 0, 12, 6, 18, 6, 12, 9, 6, 6, 24, 15, 6 };
uint8_t lecoThree[16] = { 0, 0, 15, 6, 15, 0, 6, 30, 3, 12, 12, 6, 0, 24, 15, 6 };
uint8_t lecoFour[12] = { 0, 0, 6, 18, 6, 12, 9, 6, 15, 0, 6, 30 };
uint8_t lecoFive[24] = { 0, 0, 21, 6, 0, 6, 6, 12, 6, 12, 15, 6, 0, 21, 6, 9, 6, 24, 9, 6, 15, 18, 6, 12 };
uint8_t lecoSix[20] = { 0, 0, 6, 30, 6, 0, 15, 6, 6, 12, 15, 6, 15, 18, 6, 6, 6, 24, 15, 6 };
uint8_t lecoSeven[12] = { 0, 0, 6, 9, 6, 0, 15, 6, 15, 6, 6, 24 };
uint8_t lecoEight[20] = { 0, 0, 6, 30, 6, 0, 9, 6, 15, 0, 6, 30, 6, 12, 9, 6, 6, 24, 9, 6 };
uint8_t lecoNine[20] = { 0, 0, 6, 18, 6, 0, 9, 6, 15, 0, 6, 30, 6, 12, 9, 6, 0, 24, 15, 6 };
uint8_t* lecoDigits[] = { lecoZero, lecoOne, lecoTwo, lecoThree, lecoFour, lecoFive, lecoSix, lecoSeven, lecoEight, lecoNine };

Watchy999::Watchy999() {}

void Watchy999::drawPxlNum(int pDigit, int pX, int pY, bool colorChange) {
  for (uint8_t i = 0; i < pxlLength[pDigit]; i++) {
    display.fillRect((pxlDigits[pDigit][i * 4] + pX), pxlDigits[pDigit][(i * 4) + 1] + pY, pxlDigits[pDigit][(i * 4) + 2], pxlDigits[pDigit][(i * 4) + 3], (colorChange) ? GxEPD_WHITE : GxEPD_BLACK);
  }
}

void Watchy999::drawLecoNum(int pDigit, int pX, int pY, bool colorChange) {
  for (uint8_t i = 0; i < lecoLength[pDigit]; i++) {
    display.fillRect((lecoDigits[pDigit][i * 4] + pX), lecoDigits[pDigit][(i * 4) + 1] + pY, lecoDigits[pDigit][(i * 4) + 2], lecoDigits[pDigit][(i * 4) + 3], (colorChange) ? GxEPD_WHITE : GxEPD_BLACK);
  }
}

void Watchy999::centerJustify(const String txt, uint16_t xPos, uint16_t yPos) {
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(txt, 0, 0, &x1, &y1, &w, &h);
  display.setCursor(xPos - w / 2, yPos);
  display.print(txt);
}

timeData Watchy999::getTimeDate() {

  int Hour = watchyTime.Hour;

  if (twelveMode == 0) {
    if (Hour > 12) {
      Hour = Hour - 12;
    } else if (Hour == 0) {
      Hour = 12;
    }
  }

  char hour1 = String(Hour / 10).charAt(0);
  char hour2 = String(Hour % 10).charAt(0);

  char min1 = String(watchyTime.Minute / 10).charAt(0);
  char min2 = String(watchyTime.Minute % 10).charAt(0);

  String hourStr = String(watchyTime.Hour);
  String minStr = String(watchyTime.Minute);

  hourStr = (!twelveMode && watchyTime.Hour > 12 &&  watchyTime.Hour <= 21) ? "0" + String(watchyTime.Hour - 12) : (!twelveMode && watchyTime.Hour > 12) ? String(watchyTime.Hour - 12) :
            (!twelveMode && watchyTime.Hour == 0) ? "12" : watchyTime.Hour < 10 ? "0" + hourStr : hourStr;
  minStr = watchyTime.Minute < 10 ? "0" + minStr : minStr;

  String monthStr;
  String dateStr;
  String longYearStr;
  String shortYearStr;
  char month1;
  char month2;
  char date1;
  char date2;

  monthStr = watchyTime.Month < 10 ? "0" + String(watchyTime.Month) : String(watchyTime.Month);
  dateStr = watchyTime.Day < 10 ? "0" + String(watchyTime.Day) : String(watchyTime.Day);
  longYearStr = String(2000 + tmYearToY2k(watchyTime.Year));
  //  longYearStr = String(watchyTime.Year);
  shortYearStr = String(tmYearToY2k(watchyTime.Year));
  //  shortYearStr = String(watchyTime.Year).charAt(3) + String(watchyTime.Year).charAt(4);
  month1 = (watchyTime.Month / 10 != 0) ? String(watchyTime.Month / 10).charAt(0) : '0';
  month2 = String(watchyTime.Month % 10).charAt(0);
  date1 = (watchyTime.Day / 10 != 0) ? String(watchyTime.Day / 10).charAt(0) : '0';
  date2 = String(watchyTime.Day % 10).charAt(0);

  const char* dayNames[7] = { "SUN" , "MON" , "TUE" , "WED" , "THU" , "FRI" , "SAT" };
  const char* monthNames[12] = { "JAN" , "FEB" , "MAR" , "APR" , "MAY" , "JUN" , "JUL" , "AUG" , "SEP" , "OCT" , "NOV" , "DEC" };

  const char* dayAbbrev = dayNames[watchyTime.Wday - 1];
  const char * monthAbbrev = monthNames[watchyTime.Month - 1];

  isNight = (watchyTime.Hour >= 18 || watchyTime.Hour <= 5) ? true : false;

  latestTime.hour1 = hour1;
  latestTime.hour2 = hour2;
  latestTime.min1 = min1;
  latestTime.min2 = min2;
  latestTime.hourStr = hourStr;
  latestTime.minStr = minStr;
  latestTime.monthStr = monthStr;
  latestTime.dateStr = dateStr;
  latestTime.month1 = month1;
  latestTime.month2 = month2;
  latestTime.date1 = date1;
  latestTime.date2 = date2;
  latestTime.dayAbbrev = const_cast<char*>(dayAbbrev);
  latestTime.monthAbbrev = const_cast<char*>(monthAbbrev);
  latestTime.longYearStr = longYearStr;
  latestTime.shortYearStr = shortYearStr;

  return latestTime;
}

void Watchy999::drawWeather() {
  if (showCached == false && weatherMode != 2) {
    weatherData latestWeather = weather999();
    rawTemperature = latestWeather.temperature;
    weatherConditionCode = latestWeather.weatherConditionCode;
    updateHour = watchyTime.Hour;
  }

  if (weatherMode == 2) {
    rtcTemperature = rtcTemp() - ambientOffset;
    weatherConditionCode = 999;
    cityNameID = 999;
    latestWeather.weatherConditionCode = weatherConditionCode;
    temperature = (weatherFormat) ? (int)(rtcTemperature * 9. / 5. + 32.) : rtcTemperature;
  } else {
    temperature = (weatherFormat) ? (int)(rawTemperature * 9. / 5. + 32.) : rawTemperature;
  }

  //https://openweathermap.org/weather-conditions
  if (weatherConditionCode == 999) { //RTC
    tempCondition = "AMBIENT";
  } else if (weatherConditionCode > 801 && weatherConditionCode < 805) { //Cloudy
    tempCondition = "CLOUDY";
  } else if (weatherConditionCode == 801) { //Few Clouds
    tempCondition = "CLOUDS";
  } else if (weatherConditionCode == 800) { //Clear
    tempCondition = "CLEAR SKY";
  } else if (weatherConditionCode >= 700) { //Atmosphere
    tempCondition = "MISTY";
  } else if (weatherConditionCode >= 600) { //Snow
    tempCondition = "SNOW";
  } else if (weatherConditionCode >= 500) { //Rain
    tempCondition = "RAIN";
  } else if (weatherConditionCode >= 300) { //Drizzle
    tempCondition = "DRIZZLE";
  } else if (weatherConditionCode >= 200) { //Thunderstorm
    tempCondition = "THUNDER";
  }

  if (debugger) {
    Serial.println("weatherConditionCode: " + String(weatherConditionCode));
    Serial.println("tempCondition: " + tempCondition);
  }
}

//void Watchy999::handleButtonPress(){
//    WatchyBase::handleButtonPress();
//
//    uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
//    if(IS_DOUBLE_TAP){
//        RTC.clearAlarm();
//        RTC.read(watchyTime);
//        darkMode = true;
//        showWatchFace(true);
//        if(debugger)
//          Serial.println("Double Tapped");
//        return;
//    }
//}

void Watchy999::checkBattery() {
  //Sync NTP when charging, this is very very amateur, but it works and will only sync once per usb connection
  //  float battery =  analogReadMilliVolts(ADC_PIN) / 1000.0f * 2.0f;
  float battery = getBatteryVoltage();
  charging = (battery > oldVoltage) ? true : false;
  oldVoltage = (oldVoltage == 0) ? battery : (battery > oldVoltage) ? battery : oldVoltage;

  if (battery < 3.6 && lowBattFace == false) {
    lowBattFace = true;
    lowBattHour = (latestTime.hourStr).toInt();
    lowBattMin = (latestTime.minStr).toInt();
  } else if (battery > 3.6 && !charging) {
    lowBattFace = false;
  }

  if (ntpMode == 0 && !runOnce) {
    if (!chargeSync && charging || battery >= maxVoltage && !chargeSync) {
      if (debugger)
        Serial.println("Charging NTP Sync");
      chargeSync = true;

      if (syncNtpTime()) {
        if (debugger)
          Serial.println("Charge Sync Success");
      } else {
        if (debugger)
          Serial.println("Charge Sync Failed");
      }

    } else if (oldVoltage > (battery + .02) & chargeSync && !charging) {
      chargeSync = false;
      oldVoltage = 0;
    }
  }

}

void Watchy999::checkSteps() {

  //  if (watchyTime.Day != sensorDayStamp) {
  if (watchyTime.Hour == 0 && watchyTime.Minute == 0) {
    sensor.resetStepCounter();
    stepGoal = 500;
    oldSteps = 0;
    //    sensorDayStamp = watchyTime.Day;
  }

  int stepNumber = sensor.getCounter();
  if (oldSteps < stepNumber) {
    if (animMode == 3 && watchFace != 3 && watchFace != 7 && watchFace != 8 && watchFace != 9) { //check for watch faces that use watchAction to toggle design changes
      watchAction = true;
    }
    oldSteps = stepNumber;
  }
}

void Watchy999::gotoSleep() {
#ifdef ENABLEBORDERS
  display.epd2.setDarkBorder(true);
#endif
  display.fillScreen(GxEPD_BLACK);
  display.drawBitmap(113, 56, sleep1, 63, 14, GxEPD_WHITE);
  display.drawBitmap(65, 68, sleep2, 31, 31, GxEPD_WHITE);
  display.drawBitmap(0, 99, sleep3, 143, 101, GxEPD_WHITE);
  display.drawBitmap(143, 138, sleep4, 57, 62, GxEPD_WHITE);
  display.display(false);
  interruptAlarm(false);
  display.hibernate();
  // Set pins 0-39 to input to avoid power leaking out
  for (int i = 0; i < 40; i++) {
    pinMode(i, INPUT);
  }
  esp_sleep_enable_ext1_wakeup(BTN_PIN_MASK, ESP_EXT1_WAKEUP_ANY_HIGH); //enable deep sleep wake on button press
  RTC.clearAlarm(); //resets the alarm flag in the RTC
  esp_deep_sleep_start();
}
void Watchy999::dznIntro() {
  display.fillScreen(GxEPD_BLACK);
  drawLecoNum(9, 61, 80, GxEPD_WHITE);
  drawLecoNum(9, 89, 80, GxEPD_WHITE);
  drawLecoNum(9, 117, 80, GxEPD_WHITE);
  display.setFont(&SMALL_TEXT);
  centerJustify("SYNCING", 100, 125);

#ifdef ENABLEBORDERS
  display.epd2.setDarkBorder(true);
#endif
  display.display(false);
}

void Watchy999::drawWatchFace() {

  if (runOnce || switchFace)
    dznIntro();

  watchFaceSettings();

  if (ntpMode == 1 && watchyTime.Hour == SYNC_HOUR && watchyTime.Minute == SYNC_MINUTE && !lowBattFace || runOnce) { //Sync NTP at specified time (3am default)
    if (syncNtpTime()) {
      if (debugger)
        Serial.println("AutoNTP Sync Success");
    } else {
      if (debugger)
        Serial.println("AutoNTP Sync Failed");
    }
  }

  RTC.read(watchyTime);
  timeData latestTime = getTimeDate();


  checkBattery();

  if (showSteps)
    checkSteps();

  if (animMode == 0 && watchyTime.Second == 0 || animMode == 1 && watchyTime.Minute % 30 == 0 || animMode == 2 && watchyTime.Minute == 0) {
    if (watchFace != 3 && watchFace != 7 && watchFace != 8 && watchFace != 9 && !sleep_mode)
      watchAction = true;
  }

  if (showWeather && !lowBattFace && !sleep_mode) {

    if (weatherMode == 0 && watchyTime.Minute % 30 == 0 && weatherMode != 2 || weatherMode == 1 && watchyTime.Minute == 0 && weatherMode != 2 || runOnce || switchFace && updateHour != watchyTime.Hour) {
      showCached = false;
    } else {
      showCached = true;
    }

    drawWeather();
  }



#ifdef ENABLEBORDERS
  if (showBorder) {
    if (toggleBorder) {
      display.epd2.setDarkBorder((darkMode) ? true : false);
    } else {
      display.epd2.setDarkBorder(true);
    }
  } else {
    display.epd2.setDarkBorder(false);
  }
#endif

  displayWatchFace();
  display.display(true);

  if (switchFace)
    switchFace = false;

  if (runOnce)
    runOnce = false;

  if (WiFi.status() == WL_CONNECTED)
    disableWiFi();

}

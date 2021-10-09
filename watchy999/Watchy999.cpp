#include "Watchy999.h"
#include "slides999.h"
#include "dktime999.h"
#include "tetris.h"
#include "synth999.h"
#include "pxl999.h"
#include "crushem999.h"
#include "lowBatt999.h"


RTC_DATA_ATTR bool charging = false;
RTC_DATA_ATTR bool chargeSync = false;
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

RTC_DATA_ATTR timeData latestTime;
RTC_DATA_ATTR bool lowBattFace = false;

Watchy999::Watchy999() {}

timeData Watchy999::getTimeDate() {

  int Hour = currentTime.Hour;

  if (twelveMode == 0) {
    if (Hour > 12) {
      Hour = Hour - 12;
    } else if (Hour == 0) {
      Hour = 12;
    }
  }

  char hour1 = String(Hour / 10).charAt(0);
  char hour2 = String(Hour % 10).charAt(0);

  char min1 = String(currentTime.Minute / 10).charAt(0);
  char min2 = String(currentTime.Minute % 10).charAt(0);

  String hourStr = String(currentTime.Hour);
  String minStr = String(currentTime.Minute);

  hourStr = (!twelveMode && currentTime.Hour > 12 &&  currentTime.Hour <= 21) ? "0" + String(currentTime.Hour - 12) : (!twelveMode && currentTime.Hour > 12) ? String(currentTime.Hour - 12) :
            (!twelveMode && currentTime.Hour == 0) ? "12" : currentTime.Hour < 10 ? "0" + hourStr : hourStr;
  minStr = currentTime.Minute < 10 ? "0" + minStr : minStr;

  String monthStr;
  String dateStr;
  char month1;
  char month2;
  char date1;
  char date2;

  if (dateMode == 0) {
    monthStr = currentTime.Month < 10 ? "0" + String(currentTime.Month) : String(currentTime.Month);
    dateStr = currentTime.Day < 10 ? "0" + String(currentTime.Day) : String(currentTime.Day);
    month1 = (currentTime.Month / 10 != 0) ? String(currentTime.Month / 10).charAt(0) : '0';
    month2 = String(currentTime.Month % 10).charAt(0);
    date1 = (currentTime.Day / 10 != 0) ? String(currentTime.Day / 10).charAt(0) : '0';
    date2 = String(currentTime.Day % 10).charAt(0);
  } else {
    monthStr = currentTime.Day < 10 ? "0" + String(currentTime.Day) : String(currentTime.Day);
    dateStr = currentTime.Month < 10 ? "0" + String(currentTime.Month) : String(currentTime.Month);
    date1 = (currentTime.Month / 10 != 0) ? String(currentTime.Month / 10).charAt(0) : '0';
    date2 = String(currentTime.Month % 10).charAt(0);
    month1 = (currentTime.Day / 10 != 0) ? String(currentTime.Day / 10).charAt(0) : '0';
    month2 = String(currentTime.Day % 10).charAt(0);
  }

  char *dayNames[7] = { "SUN" , "MON" , "TUE" , "WED" , "THU" , "FRI" , "SAT" };
  char *monthNames[12] = { "JAN" , "FEB" , "MAR" , "APR" , "MAY" , "JUN" , "JUL" , "AUG" , "SEP" , "OCT" , "NOV" , "DEC" };

  char *dayAbbrev = dayNames[currentTime.Wday - 1];
  char *monthAbbrev = monthNames[currentTime.Month - 1];

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
  latestTime.dayAbbrev = dayAbbrev;
  latestTime.monthAbbrev = monthAbbrev;

  return latestTime;
}

void Watchy999::drawWeather() {
  if (showCached == false && weatherMode != 2) {
    weatherData latestWeather = weather999();
    rawTemperature = latestWeather.temperature;
    weatherConditionCode = latestWeather.weatherConditionCode;
    updateHour = currentTime.Hour;
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

void Watchy999::checkBattery() {
  //Sync NTP when charging, this is very very amateur, but it works and will only sync once per usb connection
  float battery =  analogReadMilliVolts(ADC_PIN) / 1000.0f * 2.0f;
  charging = (currentTime.Minute % 2 == 0) && (battery > oldVoltage) ? true : (battery < oldVoltage) ? false : charging;
  oldVoltage = (oldVoltage == 0) ? battery : (battery > oldVoltage) ? battery : oldVoltage;

  if (battery < 3.6 && lowBattFace == false) {
    lowBattFace = true;
    lowBattHour = currentTime.Hour;
    lowBattMin = latestTime.minStr.toInt();
  } else if (battery > 3.6 && charging) {
    lowBattFace = false;
  }

  if (syncNTP == 0 && !runOnce) {
    if (!chargeSync && charging || battery >= maxVoltage && !chargeSync) {
      if (debugger)
        Serial.println("Charging NTP Sync");
      chargeSync = true;
      syncNtpTime();
    } else if (oldVoltage > (battery + .02) & chargeSync && !charging) {
      chargeSync = false;
      oldVoltage = 0;
    }
  }

}

void Watchy999::checkSteps() {
  if (currentTime.Hour == 0 && currentTime.Minute == 0) {
    sensor.resetStepCounter();
    stepGoal = 500;
    oldSteps = 0;
  }
  int stepNumber = sensor.getCounter();
  if (oldSteps < stepNumber) {
    if (animMode == 3) {
      watchAction = true;
    }
    oldSteps = stepNumber;
  }
}

void Watchy999::drawWatchFace() {

  timeData latestTime = getTimeDate();

  checkBattery();

  if (watchFace != 5 && !lowBattFace)
    checkSteps();

  if (watchFace == 1 || watchFace == 3 || watchFace == 4) {
    showWeather = true;
  } else {
    showWeather = false;
  }

  if (animMode == 0 && currentTime.Second == 0 || animMode == 1 && currentTime.Minute % 30 == 0 || animMode == 2 && currentTime.Minute == 0) {
    if (watchFace != 3)
      watchAction = true;
  }

  if (showWeather && !lowBattFace) {

    if (weatherMode == 0 && currentTime.Minute % 2 == 0 && weatherMode != 2 || weatherMode == 1 && currentTime.Minute == 0 && weatherMode != 2 || runOnce || switchFace && updateHour != currentTime.Hour) {
      showCached = false;
    } else {
      showCached = true;
    }

    drawWeather();
  }

  if (syncNTP == 1 && currentTime.Hour == ntpSyncHour && currentTime.Minute == 0 && !lowBattFace) { //Sync NTP at specified time (3am default)
    syncNtpTime();
  }

  if (lowBattFace) { //Low Power Face
    drawLowBattWatchFace();
  } else if (watchFace == 0) { //DKtime
    drawDkWatchFace();
  } else if (watchFace == 1) {//pxl999
    drawPxlWatchFace();
  } else if (watchFace == 2) { //Slides999
    drawSlidesWatchFace();
  } else if (watchFace == 3) { //Synth999
    drawSynthWatchFace();
  } else if (watchFace == 4) { //Crushem
    drawCeWatchFace();
  } else if (watchFace == 5) { //lowBatt
    drawLowBattWatchFace();
  } else if (watchFace == 6) { //Tetris
    drawTetrisWatchFace();
  }

  if(debugger)
    Serial.println("runOnce: " + String(runOnce));
  if(runOnce)
    runOnce = false;

  if(switchFace)
    switchFace = false;

}

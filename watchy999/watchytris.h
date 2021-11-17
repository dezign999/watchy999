const unsigned char *tris_nums[10] = { t0, t1, t2, t3, t4, t5, t6, t7, t8, t9 };
const unsigned char *trisDayNames[7] = { tSun, tMon, tTue, tWed, tThu, tFri, tSat };
const unsigned char *trisMonthNames[12] = { tJan , tFeb, tMar, tApr, tMay, tJun, tJul, tAug, tSep, tOct, tNov, tDec };
const unsigned char *trisSmallNums[10] = { ts0, ts1, ts2, ts3, ts4, ts5, ts6, ts7, ts8, ts9 };


void Watchy999::drawWatchytrisWatchFace() {

  //BG
  display.fillScreen(GxEPD_WHITE);
//  display.drawBitmap(0, 0, tbg, DISPLAY_WIDTH, DISPLAY_HEIGHT, GxEPD_WHITE);
  display.drawBitmap(0, 0, tBricks, 16, 200, GxEPD_BLACK);
  display.drawBitmap(120, 0, tBricks, 16, 200, GxEPD_BLACK);
  display.fillRect(138, 0, 62, 200, GxEPD_BLACK);
  display.drawBitmap(138, 18, tDateBg, 62, 44, GxEPD_WHITE);
  display.drawBitmap(141, 69, tStepsbg, 56, 33, GxEPD_WHITE);
  display.drawBitmap(141, 105, tTempbg, 56, 33, GxEPD_WHITE);
  display.drawBitmap(141, 145, tBglg, 56, 49, GxEPD_WHITE);

  //Hour
  display.drawBitmap(24, 17, tris_nums[(latestTime.hour1 - '0')], 39, 78, GxEPD_BLACK); //first digit
  display.drawBitmap(73, 17, tris_nums[(latestTime.hour2 - '0')], 39, 78, GxEPD_BLACK); //second digit

  //Minute
  display.drawBitmap(24, 106, tris_nums[(latestTime.min1 - '0')], 39, 78, GxEPD_BLACK); //first digit
  display.drawBitmap(73, 106, tris_nums[(latestTime.min2 - '0')], 39, 78, GxEPD_BLACK); //second digit

  //Day
  display.drawBitmap(156, 25, trisDayNames[(currentTime.Wday - 1)], 25, 6, GxEPD_BLACK); //day abbrev name

  //Month
  display.drawBitmap((dateMode) ? 167 : 144, 49, trisMonthNames[(currentTime.Month - 1)], 25, 6, GxEPD_BLACK); //month abbrev name

  //Date
  display.drawBitmap((dateMode) ? 144 : 176, 49, trisSmallNums[(latestTime.date1 - '0')], 7, 6, GxEPD_BLACK); //date first digit
  display.drawBitmap((dateMode) ? 153 : 185, 49, trisSmallNums[(latestTime.date2 - '0')], 7, 6, GxEPD_BLACK); //date second digit

  //Steps
  int trisSteps = sensor.getCounter();
//  int trisSteps = 12345;

  if(String(trisSteps).length() == 1) {
    display.drawBitmap(165, 87, trisSmallNums[trisSteps], 7, 6, GxEPD_BLACK);
  } else if (String(trisSteps).length() == 2) {
    display.drawBitmap(160, 87, trisSmallNums[String(trisSteps).charAt(0) - '0'], 7, 6, GxEPD_BLACK);
    display.drawBitmap(169, 87, trisSmallNums[String(trisSteps).charAt(1) - '0'], 7, 6, GxEPD_BLACK);
  } else if (String(trisSteps).length() == 3) {
    display.drawBitmap(156, 87, trisSmallNums[String(trisSteps).charAt(0) - '0'], 7, 6, GxEPD_BLACK);
    display.drawBitmap(165, 87, trisSmallNums[String(trisSteps).charAt(1) - '0'], 7, 6, GxEPD_BLACK);
    display.drawBitmap(174, 87, trisSmallNums[String(trisSteps).charAt(2) - '0'], 7, 6, GxEPD_BLACK);
  } else if (String(trisSteps).length() == 4) {
    display.drawBitmap(151, 87, trisSmallNums[String(trisSteps).charAt(0) - '0'], 7, 6, GxEPD_BLACK);
    display.drawBitmap(160, 87, trisSmallNums[String(trisSteps).charAt(1) - '0'], 7, 6, GxEPD_BLACK);
    display.drawBitmap(169, 87, trisSmallNums[String(trisSteps).charAt(2) - '0'], 7, 6, GxEPD_BLACK);
    display.drawBitmap(178, 87, trisSmallNums[String(trisSteps).charAt(3) - '0'], 7, 6, GxEPD_BLACK);
  } else if (String(trisSteps).length() == 5 && !watchAction){
    display.drawBitmap(147, 87, trisSmallNums[String(trisSteps).charAt(0) - '0'], 7, 6, GxEPD_BLACK);
    display.drawBitmap(156, 87, trisSmallNums[String(trisSteps).charAt(1) - '0'], 7, 6, GxEPD_BLACK);
    display.drawBitmap(165, 87, tsDot, 7, 6, GxEPD_BLACK);
    display.drawBitmap(174, 87, trisSmallNums[String(trisSteps).charAt(2) - '0'], 7, 6, GxEPD_BLACK);
    display.drawBitmap(183, 87, tsK, 7, 6, GxEPD_BLACK);
  } else if (String(trisSteps).length() == 5 && watchAction) {
    display.drawBitmap(147, 87, trisSmallNums[String(trisSteps).charAt(0) - '0'], 7, 6, GxEPD_BLACK);
    display.drawBitmap(156, 87, trisSmallNums[String(trisSteps).charAt(1) - '0'], 7, 6, GxEPD_BLACK);
    display.drawBitmap(165, 87, trisSmallNums[String(trisSteps).charAt(2) - '0'], 7, 6, GxEPD_BLACK);
    display.drawBitmap(174, 87, trisSmallNums[String(trisSteps).charAt(3) - '0'], 7, 6, GxEPD_BLACK);
    display.drawBitmap(183, 87, trisSmallNums[String(trisSteps).charAt(4) - '0'], 7, 6, GxEPD_BLACK);
  }

  //Temp
//  int trisTemp = temperature;
  
  if(String(temperature).length() == 1) {
    display.drawBitmap(164, 123, trisSmallNums[temperature], 7, 6, GxEPD_BLACK);
    display.drawBitmap(173, 123, tsDeg, 7, 6, GxEPD_BLACK);
  } else if (String(temperature).length() == 2) {
    display.drawBitmap(160, 123, trisSmallNums[String(temperature).charAt(0) - '0'], 7, 6, GxEPD_BLACK);
    display.drawBitmap(169, 123, trisSmallNums[String(temperature).charAt(1) - '0'], 7, 6, GxEPD_BLACK);
    display.drawBitmap(178, 123, tsDeg, 7, 6, GxEPD_BLACK);
  } else if (String(temperature).length() == 3) {
    display.drawBitmap(155, 123, trisSmallNums[String(temperature).charAt(0) - '0'], 7, 6, GxEPD_BLACK);
    display.drawBitmap(164, 123, trisSmallNums[String(temperature).charAt(1) - '0'], 7, 6, GxEPD_BLACK);
    display.drawBitmap(173, 123, trisSmallNums[String(temperature).charAt(2) - '0'], 7, 6, GxEPD_BLACK);
    display.drawBitmap(182, 123, tsDeg, 7, 6, GxEPD_BLACK);
  }

  //Weather Icon
  const unsigned char* weatherIcon;
  
  if (weatherConditionCode == 999 || 0) { //RTC
    weatherIcon = trtc;
  } else if (weatherConditionCode > 801 && weatherConditionCode < 805) { //Cloudy
    weatherIcon = tscatteredclouds;
  } else if (weatherConditionCode == 801) { //Few Clouds
    weatherIcon = (isNight) ? tfewcloudsnight : tfewclouds;
  } else if (weatherConditionCode == 800) { //Clear
    weatherIcon = (isNight) ? tclearskynight : tclearsky;
  } else if (weatherConditionCode >= 700) { //Atmosphere
    weatherIcon = tmist;
  } else if (weatherConditionCode >= 600) { //Snow
    weatherIcon = tsnow;
  } else if (weatherConditionCode >= 500) { //Rain
    weatherIcon = train;
  } else if (weatherConditionCode >= 300) { //Drizzle
    weatherIcon = tdrizzle;
  } else if (weatherConditionCode >= 200) { //Thunderstorm
    weatherIcon = tthunderstorm;
  }

  display.drawBitmap(149, 153, weatherIcon, 40, 33, GxEPD_BLACK);
   
}

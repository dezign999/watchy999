const unsigned char* timeDigit[10] = { gt0, gt1, gt2, gt3, gt4, gt5, gt6, gt7, gt8, gt9 };
const unsigned char* altTimeDigit[10] = { gtd0, gtd1, gtd2, gtd3, gtd4, gtd5, gtd6, gtd7, gtd8, gtd9 };
const unsigned char* dateDigit[10] = { gd0, gd1, gd2, gd3, gd4, gd5, gd6, gd7, gd8, gd9 };
const unsigned char* gDateNames[7] = { gSu, gMo, gTu, gWe, gTh, gFr, gSa };

void Watchy999::drawG5600WatchFace() {

  display.fillScreen((!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  //BG
  display.drawBitmap(72, 3, gSQFMI, 56, 12, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.drawBitmap(31, 19, gESP, 138, 8, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.drawBitmap(22, 151, gBt, 152, 8, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.drawBitmap(22, 174, gAlarm, 44, 8, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.drawBitmap(70, 165, gShock, 60, 34, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.drawBitmap(134, 174, gChrono, 53, 8, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.fillRect(7, 31, 186, 116, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.fillRect(17, 33, 163, 3, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.fillRect(12, 132, 176, 3, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.fillRect(17, 142, 166, 3, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.fillRect(9, 41, 3, 96, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.fillRect(99, 135, 2, 7, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.fillRect(188, 44, 3, 93, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.fillRect(111, 39, 69, 2, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.fillRect(111, 68, 69, 2, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.fillRect(106, 44, 2, 21, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.fillRect(183, 44, 2, 21, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.fillRect(144, 57, 3, 3, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.fillRect(144, 61, 3, 3, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.drawBitmap(7, 31, gC1, 10, 10, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.drawBitmap(180, 31, gC2, 13, 13, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.drawBitmap(7, 137, gC3, 10, 10, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.drawBitmap(183, 137, gC4, 10, 10, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.drawBitmap(106, 39, gsC1, 5, 5, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.drawBitmap(106, 65, gsC3, 5, 5, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.drawBitmap(180, 65, gsC4, 5, 5, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  if(watchAction){
  display.fillRect(97, 86, 10, 9, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.fillRect(96, 89, 10, 7, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.fillRect(94, 108, 10, 8, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.fillRect(93, 110, 10, 7, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  } else {
  display.fillRect(96, 88, 8, 8, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.fillRect(96, 108, 8, 8, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  }

  //DayName
  display.drawBitmap(60, 45, gDateNames[(currentTime.Wday - 1)], 36, 20, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //day abbrev name

  //Month
  display.drawBitmap((dateMode) ? 151 : 114, 45, dateDigit[latestTime.month1 - '0'], 11, 19, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //first digit
  display.drawBitmap((dateMode) ? 166 : 129, 45, dateDigit[latestTime.month2 - '0'], 11, 19, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //second digit

  //Day
  display.drawBitmap((dateMode) ? 114 : 151, 45, dateDigit[latestTime.date1 - '0'], 11, 19, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //first digit
  display.drawBitmap((dateMode) ? 129 : 166, 45, dateDigit[latestTime.date2 - '0'], 11, 19, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //second digit

  if (watchAction) {
    //Hour
    display.drawBitmap(20, 76, timeDigit[latestTime.hour1 - '0'], 34, 52, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //first digit
    display.drawBitmap(56, 76, timeDigit[latestTime.hour2 - '0'], 34, 52, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //second digit

    //Minute
    display.drawBitmap(110, 76, timeDigit[latestTime.min1 - '0'], 34, 52, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //first digit
    display.drawBitmap(146, 76, timeDigit[latestTime.min2 - '0'], 34, 52, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //second digit
  } else {
    //Hour
    display.drawBitmap(23, 78, altTimeDigit[latestTime.hour1 - '0'], 27, 48, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //first digit
    display.drawBitmap(58, 78, altTimeDigit[latestTime.hour2 - '0'], 27, 48, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //second digit

    //Minute
    display.drawBitmap(115, 78, altTimeDigit[latestTime.min1 - '0'], 27, 48, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //first digit
    display.drawBitmap(150, 78, altTimeDigit[latestTime.min2 - '0'], 27, 48, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //second digit
    
  }

//  float batteryVal =  analogReadMilliVolts(ADC_PIN) / 1000.0f * 2.0f;
  float batteryVal = getBatteryVoltage();

  if (batteryVal < 3.7) {
    // LOW
    display.fillRect(103, 137, 22, 3, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  }
  else if (batteryVal < 3.9) {
    // MEDIUM
    display.fillRect(131, 137, 22, 3, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  }
  else {
    // HIGH
    display.fillRect(159, 137, 22, 3, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  }

}

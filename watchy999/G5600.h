const unsigned char* timeDigit[10] = { gt0, gt1, gt2, gt3, gt4, gt5, gt6, gt7, gt8, gt9 };
const unsigned char* altTimeDigit[10] = { gtd0, gtd1, gtd2, gtd3, gtd4, gtd5, gtd6, gtd7, gtd8, gtd9 };
const unsigned char* dateDigit[10] = { gd0, gd1, gd2, gd3, gd4, gd5, gd6, gd7, gd8, gd9 };
const unsigned char* gDateNames[7] = { gSu, gMo, gTu, gWe, gTh, gFr, gSa };

void Watchy999::drawG5600WatchFace() {

  display.fillScreen((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);

  display.drawBitmap(0, 0, gBg, DISPLAY_WIDTH, DISPLAY_HEIGHT, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);

  //DayName
  display.drawBitmap(60, 45, gDateNames[(currentTime.Wday - 1)], 36, 20, (!darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //day abbrev name

  //Month
  display.drawBitmap(114, 45, dateDigit[latestTime.month1 - '0'], 11, 19, (!darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //first digit
  display.drawBitmap(129, 45, dateDigit[latestTime.month2 - '0'], 11, 19, (!darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //second digit

  //Day
  display.drawBitmap(151, 45, dateDigit[latestTime.date1 - '0'], 11, 19, (!darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //first digit
  display.drawBitmap(166, 45, dateDigit[latestTime.date2 - '0'], 11, 19, (!darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //second digit

  if (watchAction) {
    //Hour
    display.drawBitmap(20, 76, timeDigit[latestTime.hour1 - '0'], 34, 52, (!darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //first digit
    display.drawBitmap(56, 76, timeDigit[latestTime.hour2 - '0'], 34, 52, (!darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //second digit

    //Minute
    display.drawBitmap(110, 76, timeDigit[latestTime.min1 - '0'], 34, 52, (!darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //first digit
    display.drawBitmap(146, 76, timeDigit[latestTime.min2 - '0'], 34, 52, (!darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //second digit
  } else {
    //Hour
    display.drawBitmap(23, 78, altTimeDigit[latestTime.hour1 - '0'], 27, 48, (!darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //first digit
    display.drawBitmap(58, 78, altTimeDigit[latestTime.hour2 - '0'], 27, 48, (!darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //second digit

    //Minute
    display.drawBitmap(115, 78, altTimeDigit[latestTime.min1 - '0'], 27, 48, (!darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //first digit
    display.drawBitmap(150, 78, altTimeDigit[latestTime.min2 - '0'], 27, 48, (!darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //second digit
    
  }

  float batteryVal =  analogReadMilliVolts(ADC_PIN) / 1000.0f * 2.0f;
  Serial.println("Batt: " + String(batteryVal));

  if (batteryVal < 3.7) {
    // LOW
    display.fillRect(103, 137, 22, 3, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  }
  else if (batteryVal < 3.9) {
    // MEDIUM
    display.fillRect(131, 137, 22, 3, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  }
  else {
    // HIGH
    display.fillRect(159, 137, 22, 3, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  }

}

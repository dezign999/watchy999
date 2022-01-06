#define bithamHour Gotham_Bold_TD22pt7b // hours
#define bithamMin Gotham_Light_TD22pt7b // minutes

void Watchy999::drawPebbleTextFace() {

  int xPos = 6;
  int yPosOne = (twelveMode) ? 54 : 70;
  int yPosTwo = 92;
  int yPosThree = (twelveMode) ? 132 : 114;
  int yPosFour = (twelveMode) ? 173 : 156;

  display.fillScreen((darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
  display.setTextColor((!darkMode) ? GxEPD_BLACK : GxEPD_WHITE);

  // hours
  display.setFont(&bithamHour);
  display.setCursor(xPos, yPosOne);
  if (twelveMode) {
    if (watchyTime.Hour == 0) {
      display.setCursor(xPos, yPosOne);
      display.print("zero");
      display.setCursor(xPos, yPosTwo);
      display.print("dark");
    } else if (10 <= watchyTime.Hour && watchyTime.Hour < 14) {
      if (watchyTime.Hour >= 10 && watchyTime.Hour <= 13) {
        display.setCursor(xPos, yPosOne);
        display.print("zero");
        display.setCursor(xPos, yPosTwo);
      }
      display.print(ONES[watchyTime.Hour]);
    } else if (watchyTime.Hour > 13 && watchyTime.Hour < 20) {
      if (watchyTime.Hour >= 15 && watchyTime.Hour <= 16)
        display.setCursor(xPos, yPosTwo);
      display.print(TEENS_SPLIT[(watchyTime.Hour - 10) % 10][0]);
      display.setCursor(xPos, yPosTwo);
      display.print(TEENS_SPLIT[(watchyTime.Hour - 10) % 10][1]);
    } else if (watchyTime.Hour <= 24) {
      if (twelveMode && watchyTime.Hour / 10 == 0) {
        display.print("zero");
      } else {
        if (watchyTime.Hour == 20)
          display.setCursor(xPos, yPosTwo);
        display.print(TENS[watchyTime.Hour / 10]);
      }
      display.setCursor(xPos, yPosTwo);
      display.print(ONES[watchyTime.Hour % 10]);
    }
  } else {
    display.print(ONES[(watchyTime.Hour + 11) % 12 + 1]);
  }

  // minutes
  display.setFont(&bithamMin);
  display.setCursor(xPos, yPosThree);
  if (watchyTime.Minute == 0) {
    if (twelveMode) {
      display.print("hundred");
    } else {
      display.print((watchyTime.Hour == 0) ? "midnight" : (watchyTime.Hour == 12) ? "noon" : "o'clock");
    }
  } else if (10 <= watchyTime.Minute && watchyTime.Minute < 14) {
    display.print(ONES[watchyTime.Minute]);
  } else if (watchyTime.Minute > 13 && watchyTime.Minute < 20) {
    display.print(TEENS_SPLIT[(watchyTime.Minute - 10) % 10][0]);
    display.setCursor(xPos, yPosFour);
    display.print(TEENS_SPLIT[(watchyTime.Minute - 10) % 10][1]);
  } else if (watchyTime.Minute <= 59) {
//    if (twelveMode && watchyTime.Minute / 10 == 0) {
//      display.print("zero");
//    } else {
      display.print(TENS[watchyTime.Minute / 10]);
//    }
    display.setCursor(xPos, yPosFour);
    display.print(ONES[watchyTime.Minute % 10]);
  }

}

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
    if (currentTime.Hour == 0) {
      display.setCursor(xPos, yPosOne);
      display.print("zero");
      display.setCursor(xPos, yPosTwo);
      display.print("dark");
    } else if (10 <= currentTime.Hour && currentTime.Hour < 14) {
      if (currentTime.Hour >= 10 && currentTime.Hour <= 13) {
        display.setCursor(xPos, yPosOne);
        display.print("zero");
        display.setCursor(xPos, yPosTwo);
      }
      display.print(ONES[currentTime.Hour]);
    } else if (currentTime.Hour > 13 && currentTime.Hour < 20) {
      if (currentTime.Hour >= 15 && currentTime.Hour <= 16)
        display.setCursor(xPos, yPosTwo);
      display.print(TEENS_SPLIT[(currentTime.Hour - 10) % 10][0]);
      display.setCursor(xPos, yPosTwo);
      display.print(TEENS_SPLIT[(currentTime.Hour - 10) % 10][1]);
    } else if (currentTime.Hour <= 24) {
      if (twelveMode && currentTime.Hour / 10 == 0) {
        display.print("zero");
      } else {
        if (currentTime.Hour == 20)
          display.setCursor(xPos, yPosTwo);
        display.print(TENS[currentTime.Hour / 10]);
      }
      display.setCursor(xPos, yPosTwo);
      display.print(ONES[currentTime.Hour % 10]);
    }
  } else {
    display.print(ONES[(currentTime.Hour + 11) % 12 + 1]);
  }

  // minutes
  display.setFont(&bithamMin);
  display.setCursor(xPos, yPosThree);
  if (currentTime.Minute == 0) {
    if (twelveMode) {
      display.print("hundred");
    } else {
      display.print((currentTime.Hour == 0) ? "midnight" : (currentTime.Hour == 12) ? "noon" : "o'clock");
    }
  } else if (10 <= currentTime.Minute && currentTime.Minute < 14) {
    display.print(ONES[currentTime.Minute]);
  } else if (currentTime.Minute > 13 && currentTime.Minute < 20) {
    display.print(TEENS_SPLIT[(currentTime.Minute - 10) % 10][0]);
    display.setCursor(xPos, yPosFour);
    display.print(TEENS_SPLIT[(currentTime.Minute - 10) % 10][1]);
  } else if (currentTime.Minute <= 59) {
//    if (twelveMode && currentTime.Minute / 10 == 0) {
//      display.print("zero");
//    } else {
      display.print(TENS[currentTime.Minute / 10]);
//    }
    display.setCursor(xPos, yPosFour);
    display.print(ONES[currentTime.Minute % 10]);
  }

}

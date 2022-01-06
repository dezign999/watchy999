#define SM_FONT PebblecoNumbers_MONO7pt7b

RTC_DATA_ATTR int stepGoal = 500;
int goalIncrement = 500;
const unsigned char *ce1 [8] = {ce10, ce11, ce12, ce13, ce14, ce15, ce16, ce17};
const unsigned char *ce2 [8] = {ce20, ce21, ce22, ce23, ce24, ce25, ce26, ce27};

void Watchy999::drawCeWatchFace() {
    if(switchFace)
#ifdef ENABLEBORDERS
    display.epd2.setDarkBorder(false);
#endif

  display.setFont(&SM_FONT);
  display.setTextColor(GxEPD_WHITE);

  //BG
  display.fillScreen(GxEPD_WHITE);
  display.fillRect(0, 140, 200, 60, GxEPD_BLACK);
  //Ticks
  int lineX = 9;
  for (uint8_t i = 0; i < 33; i++) {
    display.fillRect(lineX, 147, 1, 8, GxEPD_WHITE);
    lineX += 4;
  }

  //Date Line
  display.fillRect(166, 172, 2, 6, GxEPD_WHITE);

  //Step Count
  int stepNumber = sensor.getCounter();
  int step0 = (String(stepNumber).length() == 5) ? String(stepNumber).charAt(4) - '0' : (String(stepNumber).length() == 4) ? String(stepNumber).charAt(3) - '0' :
              (String(stepNumber).length() == 3) ? String(stepNumber).charAt(2) - '0' : (String(stepNumber).length() == 2) ? String(stepNumber).charAt(1) - '0' :
              (String(stepNumber).length() == 1) ? String(stepNumber).charAt(0) - '0' : 0;
  int step1 = (String(stepNumber).length() == 5) ? String(stepNumber).charAt(3) - '0' : (String(stepNumber).length() == 4) ? String(stepNumber).charAt(2) - '0' :
              (String(stepNumber).length() == 3) ? String(stepNumber).charAt(1) - '0' : (String(stepNumber).length() == 2) ? String(stepNumber).charAt(0) - '0' : 0;
  int step2 = (String(stepNumber).length() == 5) ? String(stepNumber).charAt(2) - '0' : (String(stepNumber).length() == 4) ? String(stepNumber).charAt(1) - '0' :
              (String(stepNumber).length() == 3) ? String(stepNumber).charAt(0) - '0' : 0;
  int step3 = (String(stepNumber).length() == 5) ? String(stepNumber).charAt(1) - '0' : (String(stepNumber).length() == 4) ? String(stepNumber).charAt(0) - '0' : 0;
  int step4 = (String(stepNumber).length() == 5) ? String(stepNumber).charAt(0) - '0' : 0;

  if (dateToggle) {
    drawLecoNum(step4, 9, 162, true);
    drawLecoNum(step3, 36, 162, true);
    drawLecoNum(step2, 63, 162, true);
    drawLecoNum(step1, 90, 162, true);
    drawLecoNum(step0, 117, 162, true);

    //Time Colon
    display.fillRect(166, 160, 2, 2, GxEPD_WHITE);
    display.fillRect(166, 164, 2, 2, GxEPD_WHITE);

    //Hour
    display.setCursor(145, 167);
    display.print(latestTime.hourStr);

    //Min
    display.setCursor(172, 167);
    display.print(latestTime.minStr);
  } else {
    display.setCursor(145, 155);
    display.print((String(step4)) + (String(step3)) + (String(step2)) + (String(step1)) + (String(step0)));

    drawLecoNum(latestTime.hour1 - '0', 9, 162, true);
    drawLecoNum(latestTime.hour2 - '0', 40, 162, true);
    drawLecoNum(latestTime.min1 - '0', 86, 162, true);
    drawLecoNum(latestTime.min2 - '0', 117, 162, true);

    display.fillRect(71, 168, 6, 6, GxEPD_WHITE);
    display.fillRect(71, 186, 6, 6, GxEPD_WHITE);
  }
  //Step Meter
  if (stepNumber > 0) {
    while (stepNumber > stepGoal) {
      stepGoal += goalIncrement;
      if (stepGoal > stepNumber)
        break;
    }
    float stepTrim = goalIncrement - (stepGoal - stepNumber);
    display.fillRect(9, 147, 129 * (float)(stepTrim / goalIncrement), 8, GxEPD_WHITE);
    Serial.println("step percent: " + String((float)(stepTrim / goalIncrement)));
  }


  display.setCursor(145, (dateToggle) ? 155 : 167);
  if (String(stepGoal).length() == 5) {
    display.print(String(stepGoal));
  } else if (String(stepGoal).length() == 4) {
    display.print("0" + String(stepGoal));
  } else if (String(stepGoal).length() == 3) {
    display.print("00" + String(stepGoal));
  } else if (String(stepGoal).length() == 2) {
    display.print("000" + String(stepGoal));
  } else {
    display.print("0000" + String(stepGoal));
  }

  //Month
  display.setCursor(145, 179);
  display.print((dateMode) ? latestTime.dateStr : latestTime.monthStr);

  //Date
  display.setCursor(172, 179);
  display.print((dateMode) ? latestTime.monthStr : latestTime.dateStr);

  //Temperature
  //  drawWeather();
  display.setCursor(145, 191);
  display.println(String(temperature));
  //  display.drawBitmap((String(temperature).length() > 2) ? 176 : 167, 182, degree, 2, 2, GxEPD_WHITE); //Degree dot
  display.fillRect((String(temperature).length() > 2) ? 175 : 166, 182, 2, 2, GxEPD_WHITE);

  //Animation
  if (switchFace || watchAction) {
    drawCeAnim();
    watchAction = false;
  } else {
    //Static CE
    display.fillRect(0, 0, 200, 140, GxEPD_WHITE);;
    display.drawBitmap(50, 4, ce1[0], 89, 98, GxEPD_BLACK);
    display.drawBitmap(0, 102, ce2[0], 200, 38, GxEPD_BLACK);
    display.display(true);
  }
}

void Watchy999::drawCeAnim() {

  int animFrames = 8;
  int animCount = animFrames;
  int i = 0;
  if (watchAction) {
    animCount = 16;
    watchAction = false;
  }

  for (uint8_t i = 0; i < animCount; i++) {
    display.drawBitmap(50, 4, ce1[i % animFrames], 89, 98, GxEPD_BLACK);
    display.drawBitmap(0, 102, ce2[i % animFrames], 200, 38, GxEPD_BLACK);
    display.display(true);
    display.fillRect(0, 0, 200, 140, GxEPD_WHITE);
  }

  //Hide Ghosting
  display.drawBitmap(50, 4, ce1[0], 89, 98, GxEPD_BLACK);
  display.drawBitmap(0, 102, ce2[0], 200, 38, GxEPD_BLACK);

}

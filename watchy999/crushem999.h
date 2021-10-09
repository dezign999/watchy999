#define STEP_FONT MONO22pt7b
#define SM_FONT PebblecoNumbers_MONO7pt7b

RTC_DATA_ATTR int stepGoal = 500;
int goalIncrement = 500;
const unsigned char *ce_anim [8] = {ce0, ce1, ce2, ce3, ce4, ce5, ce6, ce7};

void Watchy999::drawCeWatchFace() {

  //BG
  display.fillScreen(GxEPD_BLACK);
  display.drawBitmap(9, 147, ticks, 129, 8, GxEPD_WHITE);
  display.drawBitmap(167, 161, colon, 2, 6, GxEPD_WHITE);
  display.drawBitmap(167, 172, divider, 2, 6, GxEPD_WHITE);

  //Step Count
  display.setFont(&STEP_FONT);
  display.setTextColor(GxEPD_WHITE);
  display.setCursor(6, 191);

  int stepNumber = sensor.getCounter();

  if (String(stepNumber).length() == 5) {
    display.print(String(stepNumber));
  } else if (String(stepNumber).length() == 4) {
    display.print("0" + String(stepNumber));
  } else if (String(stepNumber).length() == 3) {
    display.print("00" + String(stepNumber));
  } else if (String(stepNumber).length() == 2) {
    display.print("000" + String(stepNumber));
  } else {
    display.print("0000" + String(stepNumber));
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
    Serial.println("step percent: " + String((float)(stepTrim/goalIncrement)));
  }

  display.setFont(&SM_FONT);
  display.setCursor(145, 155);

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

  //Hour
  display.setCursor(145, 167);
  display.print(latestTime.hourStr);

  //Min
  display.setCursor(172, 167);
  display.print(latestTime.minStr);

  //Month
  display.setCursor(145, 179);
  display.print(latestTime.monthStr);

  //Date
  display.setCursor(172, 179);
  display.print(latestTime.dateStr);

  //Temperature
//  drawWeather();
  display.setCursor(145, 191);
  display.println(String(temperature));
  display.drawBitmap((String(temperature).length() > 2) ? 176 : 167, 182, degree, 2, 2, GxEPD_WHITE); //Degree dot

  //Animation
  if (switchFace || watchAction) {
    drawCeAnim();
    watchAction = false;
  } else {
    //Static CE
    display.fillRect(0, 0, 200, 140, GxEPD_BLACK);;
    display.drawBitmap(0, 0, ce_anim[0], 200, 140, GxEPD_WHITE);
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
    display.drawBitmap(0, 0, ce_anim[i % animFrames], 200, 140, GxEPD_WHITE);
    display.display(true);
    display.fillRect(0, 0, 200, 140, GxEPD_BLACK);
  }

  //Hide Ghosting
  display.drawBitmap(0, 0, ce_anim[0], 200, 140, GxEPD_WHITE);
  display.display(true);
}


#define TIME_FONT PebblecoNumbers_MONO32pt7b

RTC_DATA_ATTR char  step1, step2, step3, step4;
RTC_DATA_ATTR char  stepC1 = '0';
RTC_DATA_ATTR char  stepC2 = '0';
RTC_DATA_ATTR char  stepC3 = '0';
RTC_DATA_ATTR char  stepC4 = '0';
RTC_DATA_ATTR char  month1, month2, date1, date2;
RTC_DATA_ATTR char  hour1, hour2, min1, min2;
RTC_DATA_ATTR char  hourC1 = '0';
RTC_DATA_ATTR char  hourC2 = '0';
RTC_DATA_ATTR char  minC1 = '0';
RTC_DATA_ATTR char  minC2 = '0';
RTC_DATA_ATTR char  monthC1 = '0';
RTC_DATA_ATTR char  monthC2 = '0';
RTC_DATA_ATTR char  dateC1 = '0';
RTC_DATA_ATTR char  dateC2 = '0';
RTC_DATA_ATTR String stepCount;

char rNum1, rNum2, rNum3, rNum4, rNum5, rNum6, rNum7, rNum8;

void Watchy999::getSlidesRand() {
  srand(currentTime.Minute * currentTime.Second * currentTime.Wday); //seeds rand() with the current minute causing watchy to display a new random animation once per minute
  String rNumber = String(random(10000000, 99999999));
  rNum1 = rNumber.charAt(0);
  rNum2 = rNumber.charAt(1);
  rNum3 = rNumber.charAt(2);
  rNum4 = rNumber.charAt(3);
  rNum5 = rNumber.charAt(4);
  rNum6 = rNumber.charAt(5);
  rNum7 = rNumber.charAt(6);
  rNum8 = rNumber.charAt(7);
}

void Watchy999::getSlidesSteps() {

  if (debugger)
    stepCount = 10667;
  else
    stepCount = sensor.getCounter();

  if (stepCount.length() == 5) {
    step1 = stepCount.charAt(0);
    step2 = stepCount.charAt(1);
    step3 = '.';
    step4 = stepCount.charAt(2);
  } else if (stepCount.length() == 4) {
    step1 = stepCount.charAt(0);
    step2 = stepCount.charAt(1);
    step3 = stepCount.charAt(2);
    step4 = stepCount.charAt(3);
  } else if (stepCount.length() == 3) {
    step1 = '0';
    step2 = stepCount.charAt(0);
    step3 = stepCount.charAt(1);
    step4 = stepCount.charAt(2);
  } else if (stepCount.length() == 2) {
    step1 = '0';
    step2 = '0';
    step3 = stepCount.charAt(0);
    step4 = stepCount.charAt(1);
  } else {
    step1 = '0';
    step2 = '0';
    step3 = '0';
    step4 = stepCount.charAt(0);
  }

}

void Watchy999::updateSlidesCache() {
  getSlidesSteps();
  month1 = latestTime.month1;
  month2 = latestTime.month2;
  date1 = latestTime.date1;
  date2 = latestTime.date2;

  monthC1 = month1;
  monthC2 = month2;
  dateC1 = date1;
  dateC2 = date2;
  hourC1 = hour1;
  hourC2 = hour2;
  minC1 = min1;
  minC2 = min2;
  stepC1 = step1;
  stepC2 = step2;
  stepC3 = step3;
  stepC4 = step4;
}

void Watchy999::drawSlidesTime() {

  hour1 = latestTime.hour1;
  hour2 = latestTime.hour2;
  min1 = latestTime.min1;
  min2 = latestTime.min2;

  display.setFont(&TIME_FONT);

  display.setTextColor((darkMode) ? GxEPD_BLACK : GxEPD_WHITE);

  //Hour
  display.setCursor(11, 121);
  display.print(hour1);
  display.setCursor(57, 121);
  display.print(hour2);

  //Minute
  display.setCursor(103, 121);
  display.print(min1);
  display.setCursor(149, 121);
  display.print(min2);
}

void Watchy999::drawSlidesDate() {
  
  month1 = latestTime.month1;
  month2 = latestTime.month2;
  date1 = latestTime.date1;
  date2 = latestTime.date2;

  display.setFont(&TIME_FONT);

  display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);

  //Month
  display.setCursor(11, 58);
  display.print(month1);
  display.setCursor(57, 58);
  display.print(month2);

  //Date
  display.setCursor(103, 58);
  display.print(date1);
  display.setCursor(149, 58);
  display.print(date2);

}

void Watchy999::drawSlidesSteps() {

  getSlidesSteps();

  display.setFont(&TIME_FONT);

  display.fillRect(0, 135, 200, 60, (!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);

  //Steps
  display.setCursor(11, 185);
  display.print(step1);
  display.setCursor(57, 185);
  display.print(step2);
  display.setCursor(103, 185);
  display.print(step3);
  display.setCursor(149, 185);
  display.print(step4);
}

void Watchy999::drawSlidesAnim() {

  display.setFont(&TIME_FONT);

  for (uint8_t i = 0; i < 9; i++) {

    if (i == 0) {

      display.fillScreen((darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
      display.fillRect(0, 70, 200, 60, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);

      display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Month
      display.setCursor(11, 58);
      display.print(hourC1);

      display.setCursor(57, 58);
      display.print(monthC2);

      //Date
      display.setCursor(103, 58);
      display.print(dateC1);

      display.setCursor(149, 58);
      display.print(dateC2);

      display.setTextColor((!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Hour
      display.setCursor(11, 121);
      display.print(stepC1);

      display.setCursor(57, 121);
      display.print(hourC2);

      //Minute
      display.setCursor(103, 121);
      display.print(minC1);

      display.setCursor(149, 121);
      display.print(minC2);

      display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Steps
      display.setCursor(11, 185);
      display.print(rNum1);

      display.setCursor(57, 185);
      display.print(stepC2);

      display.setCursor(103, 185);
      display.print(stepC3);

      display.setCursor(149, 185);
      display.print(stepC4);

      display.display(true);

    } else if (i == 1) {

      display.fillScreen((darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
      display.fillRect(0, 70, 200, 60, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);

      display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Month
      display.setCursor(11, 58);
      display.print(stepC1);

      display.setCursor(57, 58);
      display.print(monthC2);

      //Date
      display.setCursor(103, 58);
      display.print(minC1);

      display.setCursor(149, 58);
      display.print(dateC2);

      display.setTextColor((!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Hour
      display.setCursor(11, 121);
      display.print(rNum1);

      display.setCursor(57, 121);
      display.print(hourC2);

      //Minute
      display.setCursor(103, 121);
      display.print(stepC3);

      display.setCursor(149, 121);
      display.print(minC2);

      display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Steps
      display.setCursor(11, 185);
      display.print(rNum2);

      display.setCursor(57, 185);
      display.print(stepC2);

      display.setCursor(103, 185);
      display.print(rNum5);

      display.setCursor(149, 185);
      display.print(stepC4);

      display.display(true);

    } else if (i == 2) {

      month1 = latestTime.month1;
      month2 = latestTime.month2;
      date1 = latestTime.date1;
      date2 = latestTime.date2;

      display.fillScreen((darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
      display.fillRect(0, 70, 200, 60, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);

      display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Month
      display.setCursor(11, 58);
      display.print(rNum1);

      display.setCursor(57, 58);
      display.print(monthC2);

      //Date
      display.setCursor(103, 58);
      display.print(stepC3);

      display.setCursor(149, 58);
      display.print(dateC2);

      display.setTextColor((!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Hour
      display.setCursor(11, 121);
      display.print(rNum2);

      display.setCursor(57, 121);
      display.print(hourC2);

      //Minute
      display.setCursor(103, 121);
      display.print(rNum5);

      display.setCursor(149, 121);
      display.print(minC2);

      display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Steps
      display.setCursor(11, 185);
      display.print(month1);

      display.setCursor(57, 185);
      display.print(stepC2);

      display.setCursor(103, 185);
      display.print(rNum6);

      display.setCursor(149, 185);
      display.print(stepC4);

      display.display(true);

    } else if (i == 3) {

      getSlidesSteps();

      hour1 = latestTime.hour1;
      hour2 = latestTime.hour2;
      min1 = latestTime.min1;
      min2 = latestTime.min2;

      display.fillScreen((darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
      display.fillRect(0, 70, 200, 60, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);

      display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Month
      display.setCursor(11, 58);
      display.print(rNum2);

      display.setCursor(57, 58);
      display.print(monthC2);

      //Date
      display.setCursor(103, 58); //Column 3 Start
      display.print(rNum5);

      display.setCursor(149, 58);
      display.print(dateC2);

      display.setTextColor((!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Hour
      display.setCursor(11, 121);
      display.print(month1);

      display.setCursor(57, 121);
      display.print(hourC2);

      //Minute
      display.setCursor(103, 121);
      display.print(rNum6);

      display.setCursor(149, 121);
      display.print(minC2);

      display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Steps
      display.setCursor(11, 185);
      display.print(hour1);

      display.setCursor(57, 185);
      display.print(stepC2);

      display.setCursor(103, 185);
      display.print(date1);

      display.setCursor(149, 185);
      display.print(stepC4);

      display.display(true);

    } else if (i == 4) {

      display.fillScreen((darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
      display.fillRect(0, 70, 200, 60, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);

      display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Month
      display.setCursor(11, 58);
      display.print(month1);

      display.setCursor(57, 58);
      display.print(rNum3);

      //Date
      display.setCursor(103, 58);
      display.print(rNum6);

      display.setCursor(149, 58);
      display.print(dateC2);

      display.setTextColor((!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Hour
      display.setCursor(11, 121);
      display.print(hour1);

      display.setCursor(57, 121);
      display.print(monthC2);

      //Minute
      display.setCursor(103, 121);
      display.print(date1);

      display.setCursor(149, 121);
      display.print(minC2);

      display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Steps
      display.setCursor(11, 185);
      display.print(step1);

      display.setCursor(57, 185);
      display.print(hourC2);

      display.setCursor(103, 185);
      display.print(min1);

      display.setCursor(149, 185);
      display.print(stepC4);

      display.display(true);

    } else if (i == 5) {

      display.fillScreen((darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
      display.fillRect(0, 70, 200, 60, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);

      display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Month
      display.setCursor(11, 58);
      display.print(month1);

      display.setCursor(57, 58);
      display.print(rNum4);

      //Date
      display.setCursor(103, 58);
      display.print(date1);

      display.setCursor(149, 58);
      display.print(rNum7);

      display.setTextColor((!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Hour
      display.setCursor(11, 121);
      display.print(hour1);

      display.setCursor(57, 121);
      display.print(rNum3);

      //Minute
      display.setCursor(103, 121);
      display.print(min1);

      display.setCursor(149, 121);
      display.print(dateC2);

      display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Steps
      display.setCursor(11, 185);
      display.print(step1);

      display.setCursor(57, 185);
      display.print(hourC2);

      display.setCursor(103, 185);
      display.print(step3);

      display.setCursor(149, 185);
      display.print(minC2); //Column 4 start

      display.display(true);

    } else if (i == 6) {

      display.fillScreen((darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
      display.fillRect(0, 70, 200, 60, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);

      display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Month
      display.setCursor(11, 58);
      display.print(month1);

      display.setCursor(57, 58);
      display.print(step2);

      //Date
      display.setCursor(103, 58);
      display.print(date1);

      display.setCursor(149, 58);
      display.print(rNum8);

      display.setTextColor((!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Hour
      display.setCursor(11, 121);
      display.print(hour1);

      display.setCursor(57, 121);
      display.print(rNum4);

      //Minute
      display.setCursor(103, 121);
      display.print(min1);

      display.setCursor(149, 121);
      display.print(rNum7);

      display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Steps
      display.setCursor(11, 185);
      display.print(step1);

      display.setCursor(57, 185);
      display.print(rNum3);

      display.setCursor(103, 185);
      display.print(step3);

      display.setCursor(149, 185);
      display.print(dateC2); //Column 4 start

      display.display(true);

    } else if (i == 7) {

      display.fillScreen((darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
      display.fillRect(0, 70, 200, 60, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);

      display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Month
      display.setCursor(11, 58);
      display.print(month1);

      display.setCursor(57, 58);
      display.print(hour2);

      //Date
      display.setCursor(103, 58);
      display.print(date1);

      display.setCursor(149, 58);
      display.print(step4);

      display.setTextColor((!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Hour
      display.setCursor(11, 121);
      display.print(hour1);

      display.setCursor(57, 121);
      display.print(step2);

      //Minute
      display.setCursor(103, 121);
      display.print(min1);

      display.setCursor(149, 121);
      display.print(rNum8);

      display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Steps
      display.setCursor(11, 185);
      display.print(step1);

      display.setCursor(57, 185);
      display.print(rNum4);

      display.setCursor(103, 185);
      display.print(step3);

      display.setCursor(149, 185);
      display.print(rNum7);

      display.display(true);

    } else if (i == 8) {

      display.fillScreen((darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
      display.fillRect(0, 70, 200, 60, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);

      display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Month
      display.setCursor(11, 58);
      display.print(month1);

      display.setCursor(57, 58);
      display.print(month2);

      //Date
      display.setCursor(103, 58);
      display.print(date1);

      display.setCursor(149, 58);
      display.print(min2);

      display.setTextColor((!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Hour
      display.setCursor(11, 121);
      display.print(hour1);

      display.setCursor(57, 121);
      display.print(hour2);

      //Minute
      display.setCursor(103, 121);
      display.print(min1);

      display.setCursor(149, 121);
      display.print(step4);

      display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Steps
      display.setCursor(11, 185);
      display.print(step1);

      display.setCursor(57, 185);
      display.print(step2);

      display.setCursor(103, 185);
      display.print(step3);

      display.setCursor(149, 185);
      display.print(rNum8);

      display.display(true);

    } else if (i == 9) {

      display.fillScreen((darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
      display.fillRect(0, 70, 200, 60, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);

      display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Month
      display.setCursor(11, 58);
      display.print(month1);

      display.setCursor(57, 58);
      display.print(month2);

      //Date
      display.setCursor(103, 58);
      display.print(date1);

      display.setCursor(149, 58);
      display.print(date2);

      display.setTextColor((!darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Hour
      display.setCursor(11, 121);
      display.print(hour1);

      display.setCursor(57, 121);
      display.print(hour2);

      //Minute
      display.setCursor(103, 121);
      display.print(min1);

      display.setCursor(149, 121);
      display.print(min2);

      display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      //Steps
      display.setCursor(11, 185);
      display.print(step1);

      display.setCursor(57, 185);
      display.print(step2);

      display.setCursor(103, 185);
      display.print(step3);

      display.setCursor(149, 185);
      display.print(step4);

      display.display(true);

    }

  }
  updateSlidesCache();
}

void Watchy999::drawSlidesWatchFace() {

  if (switchFace || watchAction) {
    getSlidesRand();
    drawSlidesAnim();
    watchAction = false;
    if (switchFace)
      updateSlidesCache();
  }
  display.fillScreen((darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
  display.fillRect(0, 70, 200, 60, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  drawSlidesTime();
  drawSlidesDate();
  drawSlidesSteps();
}

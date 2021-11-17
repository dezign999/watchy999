#define TIME_FONT absinth44pt7b
#define DATE_FONT synthDate14pt7b
#define SMALL_TEXT tinyPixel10pt7b
const unsigned char *abSM [10] = {ab0, ab1, ab2, ab3, ab4, ab5, ab6, ab7, ab8, ab9};

void Watchy999::drawSynthWatchFace() {

  display.fillScreen((darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
  display.setFont(&TIME_FONT);
  display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);

  if (watchAction) {
    display.fillRect(10, 10, 180, 180, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //Redraw Helper
    //Hour
    display.setCursor(18, 102);
    display.print(latestTime.hourStr);
    //Minute
    display.setCursor(18, 182);
    display.print(latestTime.minStr);
  } else {
    display.fillRect(12, 13, 130, 130, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //Redraw Helper
    display.drawBitmap(18, 19, abSM[latestTime.hour1 - '0'], 61, 61, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK); //hour digit 1
    display.drawBitmap(75, 19, abSM[latestTime.hour2 - '0'], 61, 61, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK); //hour digit 2
    display.drawBitmap(18, 76, abSM[latestTime.min1 - '0'], 61, 61, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK); //minute digit 1
    display.drawBitmap(75, 76, abSM[latestTime.min2 - '0'], 61, 61, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK); //minute digit 2
  }

  if (!watchAction) {
    //Date
    display.fillRect(142, 88, 45, 55, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //Redraw Helper
    display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
    display.setFont(&DATE_FONT);
    display.setCursor(144, (dateMode) ? 101 : 120);
    display.print(latestTime.dateStr);
    display.setFont(&SMALL_TEXT);
    display.setCursor(144, (dateMode) ? 119 : 98);
    display.print(latestTime.monthAbbrev);
    display.setCursor(144, 137);
    display.print(latestTime.dayAbbrev);

    //Battery
    int dotX = 21;
    for (uint8_t i = 0; i < 21; i++) {
      display.fillRect(dotX, 154, 2, 2, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
      dotX += 8;
    }

    float battery =  analogReadMilliVolts(ADC_PIN) / 1000.0f * 2.0f;
    float batt = (battery - (4.2 - 0.9)) / 0.9;
    if (batt > 0) {
      display.fillRect(18, 154, 165 * batt, 2, (!darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
    }

    //Temp
    display.fillRect(12, 167, 188, 21, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //Redraw Helper
    display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
    display.setFont(&SMALL_TEXT);
    display.setCursor(18, 182);
    display.println(String(temperature) + "' " + tempCondition);
  }

}

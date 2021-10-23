#define TIME_FONT absinth44pt7b
#define SM_TIME_FONT absinth31pt7b
#define DATE_FONT synthDate14pt7b
#define SMALL_TEXT tinyPixel10pt7b

RTC_DATA_ATTR bool showCached = true;

void Watchy999::drawSynthTime() {

  display.setFont((watchAction) ? &TIME_FONT : &SM_TIME_FONT);
  display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);

  //Hour
  if (watchAction) {
    display.fillRect(10, 10, 180, 180, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //Redraw Helper
    display.setCursor(18, 102);
  } else {
    display.fillRect(12, 13, 130, 130, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //Redraw Helper
    display.setCursor(18, 80);
  }
  display.print(latestTime.hourStr);

  //Minute
  if (watchAction) {
    display.setCursor(18, 182);
  } else {
    display.setCursor(18, 137);
  }
  display.print(latestTime.minStr);
}

void Watchy999::drawSynthDate() {

  display.fillRect(142, 88, 45, 55, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //Redraw Helper

  display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);

  display.setFont(&DATE_FONT);
  display.setCursor(144, 102);
  display.print(latestTime.dateStr);

  display.setFont(&SMALL_TEXT);
  display.setCursor(144, 119);
  display.print(latestTime.monthAbbrev);

  display.setCursor(144, 137);
  display.print(latestTime.dayAbbrev);
}

//Based on Braininabowl's Bahn code: https://github.com/BraininaBowl/Bahn-for-Watchy
void Watchy999::drawSynthBattery() {

  display.drawBitmap(18, 154, battery, 164, 2, (!darkMode) ? GxEPD_BLACK : GxEPD_WHITE);

  float battery =  analogReadMilliVolts(ADC_PIN) / 1000.0f * 2.0f;
  float batt = (battery - (4.2 - 0.9)) / 0.9;

  if (batt > 0) {
    display.fillRect(18, 154, 164 * batt, 2, (!darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
  }

}

void Watchy999::drawSynthWatchFace() {

  display.fillScreen((darkMode) ? GxEPD_BLACK : GxEPD_WHITE);

  display.setFont((watchAction) ? &TIME_FONT : &SM_TIME_FONT);
  display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);

  //Hour
  if (watchAction) {
    display.fillRect(10, 10, 180, 180, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //Redraw Helper
    display.setCursor(18, 102);
  } else {
    display.fillRect(12, 13, 130, 130, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //Redraw Helper
    display.setCursor(18, 80);
  }
  display.print(latestTime.hourStr);

  //Minute
  if (watchAction) {
    display.setCursor(18, 182);
  } else {
    display.setCursor(18, 137);
  }
  display.print(latestTime.minStr);

  if (!watchAction) {
    //Date
    display.fillRect(142, 88, 45, 55, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //Redraw Helper
    display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
    display.setFont(&DATE_FONT);
    display.setCursor(144, 102);
    display.print(latestTime.dateStr);
    display.setFont(&SMALL_TEXT);
    display.setCursor(144, 119);
    display.print(latestTime.monthAbbrev);
    display.setCursor(144, 137);
    display.print(latestTime.dayAbbrev);

    //Battery
    display.drawBitmap(18, 154, battery, 164, 2, (!darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
    float battery =  analogReadMilliVolts(ADC_PIN) / 1000.0f * 2.0f;
    float batt = (battery - (4.2 - 0.9)) / 0.9;
    if (batt > 0) {
      display.fillRect(18, 154, 164 * batt, 2, (!darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
    }

    //Temp
    display.fillRect(12, 167, 188, 21, (darkMode) ? GxEPD_BLACK : GxEPD_WHITE); //Redraw Helper
    display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
    display.setFont(&SMALL_TEXT);
    display.setCursor(18, 182);
    display.println(String(temperature) + "' " + tempCondition);
  }

}

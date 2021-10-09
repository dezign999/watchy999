#define LOW_FONT MONO22pt7b

void Watchy999::drawLowBattWatchFace() {

  display.fillScreen((darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
  if(!watchAction) {
    display.drawBitmap(82, 130, power, 37, 70, (!darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
  }

  //Time

  display.setFont(&LOW_FONT);
  display.setTextColor((!darkMode) ? GxEPD_BLACK : GxEPD_WHITE);

  int16_t  x1, y1;
  uint16_t w, h;
  display.getTextBounds(String(latestTime.hourStr) + ":" + String(latestTime.minStr), 0, 0, &x1, &y1, &w, &h);
  display.setCursor(98 - w / 2, 110);
  display.println(String(latestTime.hourStr) + ":" + String(latestTime.minStr));
  if(watchAction && lowBattFace) {
    display.setFont(&FreeMonoBold9pt7b);
    display.getTextBounds(String(lowBattHour) + ":" + String(lowBattMin), 0, 0, &x1, &y1, &w, &h);
    display.setCursor(98 - w / 2, 135);
    display.println(String(lowBattHour) + ":" + String(lowBattMin));
  }

  display.display(true);

}

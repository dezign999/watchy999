void Watchy999::drawLowBattWatchFace() {

  display.fillScreen((darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
  display.fillRect(97, 91, 6, 6, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.fillRect(97, 109, 6, 6, (darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  if(!watchAction) {
    display.drawBitmap(82, 130, power, 37, 70, (!darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
  }

  //Time
  drawLecoNum(latestTime.hour1 - '0', 42, 85, (darkMode) ? true : false);
  drawLecoNum(latestTime.hour2 - '0', 70, 85, (darkMode) ? true : false);
  drawLecoNum(latestTime.min1 - '0', 109, 85, (darkMode) ? true : false);
  drawLecoNum(latestTime.min2 - '0', 136, 85, (darkMode) ? true : false);

  if(watchAction && lowBattFace) {
    display.setTextColor((!darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
    display.setFont(&FreeMonoBold9pt7b);
  centerJustify("12:12", 98, 135);
  }

}

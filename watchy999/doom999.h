const unsigned char *doom_imp[2] = {doomImp0, doomImp1};
const unsigned char *doom_idle[2] = {doomImp1, doomImp2};
const unsigned char *doom_impShot[4] = {doomImpshot0, doomImpshot1, doomImpshot2, doomImpshot3};
const unsigned char *doom_gun[4] = {doomgun1, doomgun2, doomgun2, doomgun0};

#define DOOM_FONT DooM16pt7b

void Watchy999::drawDoomWatchFace() {

  if(switchFace)
    darkMode = false;

  //BG
  display.fillScreen(GxEPD_WHITE);
  display.drawBitmap(0, 0, doombg1, 50, 59, GxEPD_BLACK);
  display.drawBitmap(138, 0, doombg2, 62, 74, GxEPD_BLACK);
  display.fillRect(0, 147, 200, 53, GxEPD_BLACK);
  display.drawBitmap(0, 145, doomHline, 100, 2, GxEPD_BLACK);
  display.drawBitmap(100, 145, doomHline, 100, 2, GxEPD_BLACK);
  display.drawBitmap(66, 147, doomVline, 2, 53, GxEPD_WHITE);
  display.drawBitmap(132, 147, doomVline, 2, 53, GxEPD_WHITE);
  display.setTextColor(GxEPD_WHITE);
  display.setFont(&DOOM_FONT);

  if(darkMode) {
    if(dateMode) {
      display.drawBitmap(138, 187, doomMonth, 57, 8, GxEPD_WHITE);
      display.drawBitmap(12, 187, doomDate,  40, 8, GxEPD_WHITE);
      centerJustify(latestTime.dateStr, 31, 178);
      centerJustify(latestTime.monthStr, 165, 178);
    } else {
      display.drawBitmap(4, 187, doomMonth, 57, 8, GxEPD_WHITE);
      display.drawBitmap(146, 187, doomDate,  40, 8, GxEPD_WHITE);
      centerJustify(latestTime.monthStr, 31, 178);
      centerJustify(latestTime.dateStr, 165, 178);
    }  
  } else {
    display.drawBitmap(4, 187, doomHealth, 57, 8, GxEPD_WHITE);
    display.drawBitmap(143, 187, doomAmmo, 47, 8, GxEPD_WHITE);
    centerJustify(latestTime.hourStr, 31, 178);
    centerJustify(latestTime.minStr, 165, 178);
  }

  if (switchFace || runOnce) {
    for (uint8_t i = 0; i < 2; i++) {
      display.fillRect(50, 0, 88, 93, GxEPD_WHITE);
      display.drawBitmap(50, 0, doom_imp[i % 2], 88, 93, GxEPD_BLACK);
      display.drawBitmap(56, 93, doomgun0, 95, 52, GxEPD_BLACK);
      display.fillRect(83, 151, 36, 46, GxEPD_BLACK);
      display.drawBitmap(83, 151, doomface0, 36, 46, GxEPD_WHITE);
      if (i == 0)
        delay(500);
      display.display(true);
    }
  } else {
    display.drawBitmap(50, 0, doom_idle[currentTime.Minute % 2], 88, 93, GxEPD_BLACK);
    display.drawBitmap(56, 93, doomgun0, 95, 52, GxEPD_BLACK);
    display.fillRect(83, 151, 36, 46, GxEPD_BLACK);
    display.drawBitmap(83, 151, doomface0, 36, 46, GxEPD_WHITE);
  }

  if (watchAction) {
    display.fillRect(83, 151, 36, 46, GxEPD_BLACK);
    display.drawBitmap(83, 151, doomface1, 36, 46, GxEPD_WHITE);
    watchAction = false;
    for (uint8_t i = 0; i < 4; i++) {
      display.fillRect(56, 93, 95, 52, GxEPD_WHITE);
      display.drawBitmap(56, 93, doom_gun[i % 4], 95, 52, GxEPD_BLACK);
      display.fillRect(50, 0, 88, 93, GxEPD_WHITE);
      display.drawBitmap(50, 0, doom_impShot[i % 4], 88, 93, GxEPD_BLACK);
      if (i == 0)
        vibrate();
      display.display(true);
    }
    display.fillRect(56, 93, 95, 52, GxEPD_WHITE);
    display.drawBitmap(56, 93, doomgun0, 95, 52, GxEPD_BLACK);
    for (uint8_t i = 0; i < 2; i++) {
      display.fillRect(50, 0, 88, 93, GxEPD_WHITE);
      display.drawBitmap(50, 0, doom_imp[i % 2], 88, 93, GxEPD_BLACK);
      delay((i == 0) ? 500 : 0);
      display.display(true);
    }
    display.fillRect(83, 151, 36, 46, GxEPD_BLACK);
    display.drawBitmap(83, 151, doomface0, 36, 46, GxEPD_WHITE);
  }

  if(darkMode) {
    darkMode = false;
    display.display(true);
    delay(3000);
    showWatchFace(true);
  }
}

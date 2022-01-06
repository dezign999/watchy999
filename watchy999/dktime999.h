const unsigned char *dk_nums [10] = {dk0, dk1, dk2, dk3, dk4, dk5, dk6, dk7, dk8, dk9};
const unsigned char *dk_anim1 [2] = {dk_1, dk_2};
const unsigned char *dk_anim2 [2] = {dk_3, dk_4};
const unsigned char *dk_anim3 [4] = {dk_5, dk_6, dk_7, dk_8};

void Watchy999::drawDkWatchFace() {

  //BG
  display.fillScreen(GxEPD_BLACK);
  display.drawBitmap(0, 0, bg_top, 200, 73, GxEPD_WHITE);
  display.drawBitmap(0, 196, bg_bot, 200, 4, GxEPD_WHITE);
  if(dateToggle)
    display.fillRect(98, 33, 4, 26, GxEPD_WHITE);

  //Hour
  display.drawBitmap(35, 33, dk_nums[(dateToggle) ? (latestTime.month1 - '0') : (latestTime.hour1 - '0')], 28, 26, GxEPD_WHITE); //first digit
  display.drawBitmap(66, 33, dk_nums[(dateToggle) ? (latestTime.month2 - '0') : (latestTime.hour2 - '0')], 28, 26, GxEPD_WHITE); //second digit

  //Minute
  display.drawBitmap(105, 33, dk_nums[(dateToggle) ? (latestTime.date1 - '0') : (latestTime.min1 - '0')], 28, 26, GxEPD_WHITE); //first digit
  display.drawBitmap(137, 33, dk_nums[(dateToggle) ? (latestTime.date2 - '0') : (latestTime.min2 - '0')], 28, 26, GxEPD_WHITE); //second digit

  if (switchFace || watchAction) {
    drawDkAnim();
    watchAction = false;
  } else {
    //Static DK
    display.fillRect(0, 73, 200, 124, GxEPD_BLACK);
    display.drawBitmap(7, 76, dk_anim1[0], 182, 121, GxEPD_WHITE);
    #ifdef ENABLEBORDERS
      display.epd2.setDarkBorder(true);
    #endif
    display.display(true);
  }

  if (dateToggle) {
    dateToggle = false;
    display.display(true);
    delay(3000);
    showWatchFace(true);
  }

}

void Watchy999::drawDkAnim() {

  //Choose an Animation
  int randNum = random(-1, 3);  // (3 possibilities)
  int animFrames = 2;
  int animOneFrames = 8;
  int animTwoFrames = 6;
  int animThreeFrames = 4;

  if (randNum == 0) {
    for (uint8_t i = 0; i < animOneFrames; i++) {
      display.drawBitmap(7, 76, dk_anim1[i % animFrames], 182, 121, GxEPD_WHITE);
      #ifdef ENABLEBORDERS
        display.epd2.setDarkBorder(true);
      #endif
      display.display(true);
      display.fillRect(0, 76, 200, 121, GxEPD_BLACK);
     }
  } else if (randNum == 1) {
    for (uint8_t i = 0; i < animTwoFrames; i++) {
      display.drawBitmap(7, 76, dk_anim2[i % animFrames], 182, 121, GxEPD_WHITE);
      #ifdef ENABLEBORDERS
        display.epd2.setDarkBorder(true);
      #endif
      display.display(true);
      display.fillRect(0, 76, 200, 121, GxEPD_BLACK);
    }
  } else {
    for (uint8_t i = 0; i < animThreeFrames; i++) {
      display.drawBitmap(0, 76, dk_anim3[i % animThreeFrames], 200, 121, GxEPD_WHITE);
    #ifdef ENABLEBORDERS
      display.epd2.setDarkBorder(true);
    #endif
      display.display(true);
      display.fillRect(0, 76, 200, 121, GxEPD_BLACK);
    }
  }

  //Hide Ghosting
  display.fillRect(0, 76, 200, 121, GxEPD_BLACK);
  display.drawBitmap(7, 76, dk_anim1[0], 182, 121, GxEPD_WHITE);
  #ifdef ENABLEBORDERS
    display.epd2.setDarkBorder(true);
  #endif
}

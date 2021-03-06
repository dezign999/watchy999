const unsigned char *tetris_nums [10] = {tetris0, tetris1, tetris2, tetris3, tetris4, tetris5, tetris6, tetris7, tetris8, tetris9};

void Watchy999::drawTetrisWatchFace() {

  int Hour = currentTime.Hour;

    //Change to 12 Hour Time
    if(!twelveMode) {
        if (Hour > 12) {
          Hour = Hour - 12;
        } else if (Hour == 0) {
          Hour = 12;
        }
    }

  display.fillScreen(GxEPD_WHITE);
  display.drawBitmap(0, 0, tetrisbg, DISPLAY_WIDTH, DISPLAY_HEIGHT, GxEPD_BLACK);

  //Hour
  display.drawBitmap(25, 20, tetris_nums[Hour / 10], 40, 60, GxEPD_BLACK); //first digit
  display.drawBitmap(75, 20, tetris_nums[Hour % 10], 40, 60, GxEPD_BLACK); //second digit

  //Minute
  display.drawBitmap(25, 110, tetris_nums[currentTime.Minute / 10], 40, 60, GxEPD_BLACK); //first digit
  display.drawBitmap(75, 110, tetris_nums[currentTime.Minute % 10], 40, 60, GxEPD_BLACK); //second digit
}

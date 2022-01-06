#define qlockyBold Jost_SemiBold10pt7b // active
#define qlockyLight Jost_Light10pt7b // inactive

const short COLWIDTH = 18;
const short ROWHEIGHT = 20;

const char LETTERS[10][11] = {
  // 0    1    2    3    4    5    6    7    8    9   10
  {'I', 'T', 'L', 'I', 'S', 'S', 'A', 'T', 'I', 'M', 'E' }, // 0
  {'A', 'C', 'Q', 'U', 'A', 'R', 'T', 'E', 'R', 'D', 'C' }, // 1
  {'T', 'W', 'E', 'N', 'T', 'Y', 'F', 'I', 'V', 'E', 'X' }, // 2
  {'H', 'A', 'L', 'F', 'B', 'T', 'E', 'N', 'F', 'T', 'O' }, // 3
  {'P', 'A', 'S', 'T', 'E', 'R', 'U', 'N', 'I', 'N', 'E' }, // 4
  {'O', 'N', 'E', 'S', 'I', 'X', 'T', 'H', 'R', 'E', 'E' }, // 5
  {'F', 'O', 'U', 'R', 'F', 'I', 'V', 'E', 'T', 'W', 'O' }, // 6
  {'E', 'I', 'G', 'H', 'T', 'E', 'L', 'E', 'V', 'E', 'N' }, // 7
  {'S', 'E', 'V', 'E', 'N', 'T', 'W', 'E', 'L', 'V', 'E' }, // 8
  {'T', 'E', 'N', 'S', 'E', 'O', 'C', 'L', 'O', 'C', 'K' }, // 9
};

const Word PREFIX_IT = {0, 0, 2}; // IT
const Word PREFIX_IS = {0, 3, 2}; // IS

const Word EMPTY = { 0, 0, 0 }; // for times with less than five parts

const Word INFIX_BEFORE = {3, 9, 2}; // TO
const Word INFIX_AFTER = {4, 0, 4}; // PAST

const Word SUFFIX_OCLOCK = {9, 5, 6}; // OCLOCK

const Word MINUTE_FIVE = {2, 6, 4}; // FIVE
const Word MINUTE_TEN = { 3, 5, 3 }; // TEN
const Word INDEFINITE_ARTICLE = { 0, 6, 1 }; // A
const Word MINUTE_QUARTER = {1, 2, 7}; // QUARTER
const Word MINUTE_TWENTY = {2, 0, 6}; // TWENTY
const Word MINUTE_TWENTYFIVE = {2, 0, 10}; // TWENTYFIVE

const Word MINUTE_HALF = {3, 0, 4}; // HALF

const Word HOURS[12] = {
  {5, 0, 3}, // ONE
  {6, 8, 3}, // TWO
  {5, 6, 5}, // THREE
  {6, 0, 4}, // FOUR
  {6, 4, 4}, // FIVE
  {5, 3, 3}, // SIX
  {8, 0, 5}, // SEVEN
  {7, 0, 5}, // EIGHT
  {4, 7, 4}, // NINE
  {9, 0, 3}, // TEN
  {7, 5, 6}, // ELEVEN
  {8, 5, 6}, // TWELVE
};

TimeSayer::TimeSayer() {
  words_on[0] = &PREFIX_IT;
  words_on[1] = &PREFIX_IS;
}

void TimeSayer::setWords(int8_t hour, int8_t minute) {

  int8_t fiveMinutes = minute / 5;

  switch (fiveMinutes) {
    case 0:
      words_on[2] = &EMPTY;
      words_on[3] = &EMPTY;
      break;
    case 1:
      words_on[2] = &EMPTY;
      words_on[3] = &MINUTE_FIVE;
      break;
    case 2:
      words_on[2] = &EMPTY;
      words_on[3] = &MINUTE_TEN;
      break;
    case 3:
      words_on[2] = &INDEFINITE_ARTICLE;
      words_on[3] = &MINUTE_QUARTER;
      break;
    case 4:
      words_on[2] = &EMPTY;
      words_on[3] = &MINUTE_TWENTY;
      break;
    case 5:
      words_on[2] = &EMPTY;
      words_on[3] = &MINUTE_TWENTYFIVE;
      break;
    case 6:
      words_on[2] = &EMPTY;
      words_on[3] = &MINUTE_HALF;
      break;
    case 7:
      words_on[2] = &EMPTY;
      words_on[3] = &MINUTE_TWENTYFIVE;
      break;
    case 8:
      words_on[2] = &EMPTY;
      words_on[3] = &MINUTE_TWENTY;
      break;
    case 9:
      words_on[2] = &INDEFINITE_ARTICLE;
      words_on[3] = &MINUTE_QUARTER;
      break;
    case 10:
      words_on[2] = &EMPTY;
      words_on[3] = &MINUTE_TEN;
      break;
    case 11:
      words_on[2] = &EMPTY;
      words_on[3] = &MINUTE_FIVE;
      break;
  }

  int hourIndex = hour += 11;
  if (fiveMinutes > 6) {
    hourIndex -= 11;
    words_on[4] = &INFIX_BEFORE;
    words_on[5] = &HOURS[hourIndex % 12];
  }
  else if (fiveMinutes > 0) {
    words_on[4] = &INFIX_AFTER;
    words_on[5] = &HOURS[hourIndex % 12];
  }
  else {
    words_on[4] = &HOURS[hourIndex % 12];
    words_on[5] = &SUFFIX_OCLOCK;
  }
}

void Watchy999 :: setMinute(int8_t minute) {
  int dotW = 2;
  int dotH = 3;
  int dotX = 198;
  int dotYone = 193;
  int dotYtwo = 186;
  int dotYthree = 173;
  int dotYFour = 166;

  int8_t minutes = minute % 5;
  switch (minutes) {
    case 0:
       display.fillRect(dotX, dotYone, dotW, dotH, ((darkMode) ? GxEPD_BLACK : GxEPD_WHITE));
      break;
    case 1:
      display.fillRect(dotX, dotYone, dotW, dotH, ((!darkMode) ? GxEPD_BLACK : GxEPD_WHITE));
      break;
    case 2:
      display.fillRect(dotX, dotYone, dotW, dotH, ((!darkMode) ? GxEPD_BLACK : GxEPD_WHITE));
      display.fillRect(dotX, dotYtwo, dotW, dotH, ((!darkMode) ? GxEPD_BLACK : GxEPD_WHITE));
      break;
    case 3:
      display.fillRect(dotX, dotYone, dotW, dotH, ((!darkMode) ? GxEPD_BLACK : GxEPD_WHITE));
      display.fillRect(dotX, dotYtwo, dotW, dotH, ((!darkMode) ? GxEPD_BLACK : GxEPD_WHITE));
      display.fillRect(dotX, dotYthree, dotW, dotH, ((!darkMode) ? GxEPD_BLACK : GxEPD_WHITE));
      break;
    case 4:
      display.fillRect(dotX, dotYone, dotW, dotH, ((!darkMode) ? GxEPD_BLACK : GxEPD_WHITE));
      display.fillRect(dotX, dotYtwo, dotW, dotH, ((!darkMode) ? GxEPD_BLACK : GxEPD_WHITE));
      display.fillRect(dotX, dotYthree, dotW, dotH, ((!darkMode) ? GxEPD_BLACK : GxEPD_WHITE));
      display.fillRect(dotX, dotYFour, dotW, dotH, ((!darkMode) ? GxEPD_BLACK : GxEPD_WHITE));
      break;
  }
}

void Watchy999::drawQlockyWatchFace() {
  #ifdef ENABLEBORDERS
    display.epd2.setDarkBorder(darkMode);
  #endif
  display.fillScreen((darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
  display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.setTextWrap(false);
  if (watchAction)
    setMinute(watchyTime.Minute);
  timeSayer.setWords(watchyTime.Hour, watchyTime.Minute);
  nextWord();
  display.setFont(&Jost_Light10pt7b);
  //  display.setTextSize(1);
  display.setTextColor((darkMode) ? GxEPD_WHITE : GxEPD_BLACK);
  display.setTextWrap(false);
  for (int row = 0; row < 10; row++) {
    if (row - currentWord->row == 1) {
      display.setFont(&qlockyLight);
      nextWord();
    }
    for (int col = 0; col < 11; col++) {
      char letter = LETTERS[row][col];
      short x = col * COLWIDTH;
      short y = row * ROWHEIGHT + 17;
      if (currentWord->row == row) {
        if (currentWord->column == col) {
          display.setFont(&qlockyBold);
        }
        else if (currentWord->column + currentWord->length == col) {
          display.setFont(&Jost_Light10pt7b);
          nextWord();
        }
      }
      display.getTextBounds(String(letter), x, y, &x1, &y1, &w, &h);
      short startCursor = x + (COLWIDTH - w) / 2;
      display.setCursor(startCursor, y);
      display.print(letter);
    }
  }
};

void Watchy999::nextWord() {
  if (wordIndex < 5) {
    wordIndex++;
    currentWord = timeSayer.words_on[wordIndex];
    if (currentWord->length == 0) {
      nextWord();
    }
  }
};

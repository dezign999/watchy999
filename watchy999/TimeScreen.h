#include <stdlib.h>
#include <time.h>
//#include <Dusk2Dawn.h>

#define teko_semibold26pt7b teko_SemiBold26pt7b // hours
#define teko_regular20pt7b teko_Regular20pt7b // minutes
#define teko_regular12pt7b teko_Regular12pt7b // data line
//#include "GetLocation.h"

extern RTC_DATA_ATTR int gmtOffset;
int gmtHour;
int gmtMinute;
String gmtMinStr;

// inspired by http://rosettacode.org/wiki/Number_names#C.2B.2B
const char *smallNumbers[] = {"",        "one",       "two",      "three",
                              "four",    "five",      "six",      "seven",
                              "eight",   "nine",      "ten",      "eleven",
                              "twelve",  "thirteen",  "fourteen", "fifteen",
                              "sixteen", "seventeen", "eighteen", "nineteen"};
const char *decades[] = {"oh", nullptr, "twenty", "thirty", "forty", "fifty"};

//void Watchy999::rightJustify(const char *txt, uint16_t &yPos) {
void Watchy999::rightJustify(const String txt, uint16_t &yPos) {
  int16_t x1, y1;
  uint16_t w, h;
  const uint8_t PADDING = 5; // how much padding to leave around text
  display.getTextBounds(txt, 0, 0, &x1, &y1, &w, &h);
  // right justify with padding
  display.setCursor(200-x1-w-PADDING, yPos);
  display.print(txt);  
}

void Watchy999::drawTimeScreenFace() {
  
  display.fillScreen((darkMode) ? GxEPD_BLACK : GxEPD_WHITE);
  display.setTextColor((!darkMode) ? GxEPD_BLACK : GxEPD_WHITE);

  // hours
  const GFXfont * font = &teko_semibold26pt7b;
  display.setFont(font);
  uint16_t yPos = (font->yAdvance / 2) + 5; // assume cursor(0,0)
  rightJustify(smallNumbers[(currentTime.Hour + 11) % 12 + 1], yPos);

  // minutes
  font = &teko_regular20pt7b;
  display.setFont(font);
  yPos += (font->yAdvance / 2) + 8;
  const char *txt;
  assert(currentTime.Minute >= 0 && currentTime.Minute < 60);
  if (currentTime.Minute == 0) {
    // 0: exactly on the hour
    if (currentTime.Hour == 0) {
      txt = "midnight";
    } else if (currentTime.Hour == 12) {
      txt = "noon";
    } else {
      txt = "o'clock";
    }
  } else if (10 <= currentTime.Minute && currentTime.Minute < 20) {
    // 10-19
    txt = smallNumbers[currentTime.Minute];
  } else if (currentTime.Minute <= 59) {
    // 1-9, 20-59
    rightJustify(decades[currentTime.Minute / 10], yPos);
    yPos += (font->yAdvance / 2) + 8;
    txt = smallNumbers[currentTime.Minute % 10];
  }
  // ignore warning about txt not initialized, assert guarantees it will be
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
  rightJustify(txt, yPos);
  #pragma GCC diagnostic pop

//  Dusk2Dawn localLocation(localLat, localLon, gmtOffset/3600);
//  int localSunrise = localLocation.sunrise(currentTime.Year, currentTime.Month, currentTime.Day, false);
//  int localSunset  = localLocation.sunset(currentTime.Year, currentTime.Month, currentTime.Day, false);
//  char sunriseStr[] = "00:00";
//  Dusk2Dawn::min2str(sunriseStr, localSunrise);
//  char sunsetStr[] = "00:00";
//  Dusk2Dawn::min2str(sunsetStr, localSunset);

  // GMT Time

  // convert from GMT offset in seconds to integer hours
  gmtHour = currentTime.Hour - gmtOffset/3600;
  
  // check for 30 minute gmtOffsets
  if (fabs(gmtOffset/3600.0) > abs(gmtOffset/3600)) 
    { 
      // adjust minutes by 30 if needed
      gmtMinute = currentTime.Minute + 30;
      // if minutes go over 59, roll minutes over and add an hour to gmtHour instead
      if (gmtMinute > 59 ) {
        gmtMinute = gmtMinute - 60;
        gmtHour = gmtHour +1; 
      }
    } 
  else 
  // there are no 30 minute GMT offsets so minutes are just minutes
    { gmtMinute = currentTime.Minute; }

  // if hours go less than 0, or over 23, roll the clock back into range
  if (gmtHour < 0) { gmtHour = gmtHour +24; }
  if (gmtHour > 23) { gmtHour = gmtHour - 24; }

  // convert integers to strings
  String gmtHourStr = String(gmtHour);
  String gmtMinStr = String(gmtMinute);
  String gmtTime;

  // add leading zeroes to single-digit hours and minutes
  gmtHourStr = gmtHour < 10 ? "0" + gmtHourStr : gmtHourStr;
  gmtMinStr = gmtMinute < 10 ? "0" + gmtMinStr : gmtMinStr;

  // display date left justified at bottom
  yPos = 195;
  display.setCursor(5, yPos);
  display.setFont(&teko_regular12pt7b);
  // display.print(&t, "%F  %H:%M  %Z");
  String txtt = String(currentTime.Year + 1970) + '-' + latestTime.monthStr + '-' + String(latestTime.dateStr);
  //txtt = txtt + "   " + latestTime.hourStr + ':' + latestTime.minStr;
  //txtt = txtt + "   [" + gmtHourStr + ':' + gmtMinStr + " Z]";
  display.println(txtt);

  // display GMT time right justified at bottom
  gmtTime = "[" + gmtHourStr + ":" + gmtMinStr + " Z]";
  rightJustify(gmtTime, yPos);
}

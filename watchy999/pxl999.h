#define TIME_FONT timeLGMono42pt7b
#define DATE_FONT timeLGMono20pt7b
#define SMALL_TEXT smTextMono8pt7b

//Night weather icons ;)
RTC_DATA_ATTR bool isNight = false;

void Watchy999::drawPxlWatchFace() {

  display.fillScreen(GxEPD_BLACK);

  //Time

  display.setFont(&TIME_FONT);
  display.setTextColor(GxEPD_WHITE);

  display.fillRect(11, 27, 128, 124, GxEPD_BLACK); //Redraw Helper

  //Hour
  display.setCursor(16, 87);
  display.print(latestTime.hourStr);

  //Minute
  display.setCursor(16, 148);
  display.print(latestTime.minStr);

  //Date
  String dayName = dayStr(currentTime.Wday);
  String monthName = monthStr(currentTime.Month);
  String dateStr = String(currentTime.Day);
  dateStr = currentTime.Day < 10 ? "0" + dateStr : dateStr;

  display.fillRect(11, 153, 178, 38, GxEPD_BLACK); //Redraw Helper

  display.setFont(&DATE_FONT);
  display.setCursor(16, 184);
  display.print(dateStr);

  display.setFont(&SMALL_TEXT);
  display.setCursor(76, 169);
  display.print(monthName);

  display.setCursor(76, 184);
  display.print(dayName);

  display.setFont(&SMALL_TEXT);
  display.fillRect(142, 136, 49, 13, GxEPD_BLACK); //Redraw Helper
  int16_t  x1, y1;
  uint16_t w, h;
  display.getTextBounds(String(temperature) + ".", 45, 13, &x1, &y1, &w, &h);
  display.setCursor(166 - w / 2, 148);
  display.println(String(temperature) + ".");

  cityName = getCityAbbv();
  display.fillRect(142, 77, 49, 13, GxEPD_BLACK); //Redraw Helper
  display.getTextBounds(cityName, 45, 13, &x1, &y1, &w, &h);
  display.setCursor(165 - w / 2, 87);
  display.println(cityName);

  //Weather Icon
  isNight = (currentTime.Hour >= 18 || currentTime.Hour <= 5) ? true : false;
  const unsigned char* weatherIcon;

  if (weatherMode == 2)
    weatherConditionCode = 998;

  if (weatherConditionCode == 999 || 0) { //RTC
    weatherIcon = rtc;
  } else if (weatherConditionCode == 998) { //RTC SLEEEP
    weatherIcon = rtcsleep;
  } else if (weatherConditionCode > 801 && weatherConditionCode < 805) { //Cloudy
    weatherIcon = scatteredclouds;
  } else if (weatherConditionCode == 801) { //Few Clouds
    weatherIcon = (isNight) ? fewcloudsnight : fewclouds;
  } else if (weatherConditionCode == 800) { //Clear
    weatherIcon = (isNight) ? clearskynight : clearsky;
  } else if (weatherConditionCode >= 700) { //Atmosphere
    weatherIcon = mist;
  } else if (weatherConditionCode >= 600) { //Snow
    weatherIcon = snow;
  } else if (weatherConditionCode >= 500) { //Rain
    weatherIcon = rain;
  } else if (weatherConditionCode >= 300) { //Drizzle
    weatherIcon = drizzle;
  } else if (weatherConditionCode >= 200) { //Thunderstorm
    weatherIcon = thunderstorm;
  }

  display.fillRect(141, 91, 49, 44, GxEPD_BLACK); //Redraw Helper
  display.drawBitmap(143, 93, weatherIcon, 45, 40, GxEPD_WHITE);

  //another silly work around to help reduce ghosting
  display.display(true);

}

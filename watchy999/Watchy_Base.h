//Derived from peerdavid's source at: https://github.com/peerdavid/Watchy
#ifndef WATCHY_BASE_H
#define WATCHY_BASE_H

#include <Watchy.h>

#if __has_include("config.h") && __has_include(<stdint.h>)
# include "config.h"
#endif

extern RTC_DATA_ATTR bool runOnce;
extern RTC_DATA_ATTR int twelveMode;
extern RTC_DATA_ATTR bool darkMode;
extern RTC_DATA_ATTR bool debugger;
extern RTC_DATA_ATTR int animMode;
extern RTC_DATA_ATTR int weatherMode;
extern RTC_DATA_ATTR int dateMode;
extern RTC_DATA_ATTR int syncNTP;
extern RTC_DATA_ATTR int watchFace;
extern RTC_DATA_ATTR bool switchFace;
extern RTC_DATA_ATTR weatherData latestWeather;
extern RTC_DATA_ATTR char city;
extern RTC_DATA_ATTR String cityName;
extern RTC_DATA_ATTR int cityNameID;
extern RTC_DATA_ATTR int8_t temperature;
extern RTC_DATA_ATTR int16_t weatherConditionCode;
extern RTC_DATA_ATTR bool weatherFormat;
extern RTC_DATA_ATTR bool watchAction;
extern RTC_DATA_ATTR bool showWeather;
extern RTC_DATA_ATTR bool isNight;
extern uint16_t ambientOffset;
extern RTC_DATA_ATTR String dezign;
extern int ntpSyncHour;

// Btn definitions
//#define IS_DOUBLE_TAP       (wakeupBit & ACC_INT_MASK && guiState == WATCHFACE_STATE)
#define IS_BTN_RIGHT_UP     (wakeupBit & UP_BTN_MASK && guiState == WATCHFACE_STATE)
#define IS_BTN_LEFT_UP      (wakeupBit & BACK_BTN_MASK && guiState == WATCHFACE_STATE)
#define IS_BTN_RIGHT_DOWN   (wakeupBit & DOWN_BTN_MASK && guiState == WATCHFACE_STATE)
#define EXT_INT_MASK        MENU_BTN_MASK|BACK_BTN_MASK|UP_BTN_MASK|DOWN_BTN_MASK

class WatchyBase : public Watchy {
  public:
    WatchyBase();
    virtual void init();
    virtual void handleButtonPress();
    void vibrate(uint8_t times = 1, uint32_t delay_time = 50);
    esp_sleep_wakeup_cause_t wakeup_reason;
    void saveVars();
    void syncNtpTime();
    bool wifi999();
    void getWifi();
    void watchfaceApp();
    void disableWiFi();
    void twelveModeApp();
    void animationApp();
    void dateModeApp();
    void wifiModeApp();
    void weatherApp();
    void weatherFormatApp();
    void ntpApp();
    weatherData weather999();
    bool noAlpha(String str);
    String getCityName();
    String getCityAbbv();
    int rtcTemp();
    void showList(char *listItems[], byte itemCount, byte listIndex, bool selected, bool partialRefresh);
    void showMenu(byte menuIndex, bool partialRefresh);
    virtual void showFastMenu(byte menuIndex);

  private:
    void _rtcConfig();
    void _bmaConfig();
    static uint16_t _readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
    static uint16_t _writeRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
};

#endif

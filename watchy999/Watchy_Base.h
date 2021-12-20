
//Derived from peerdavid's source at: https://github.com/peerdavid/Watchy
#ifndef WATCHY_BASE_H
#define WATCHY_BASE_H

#include <Watchy.h>
#include <WatchyRTC.h>

#if __has_include("config.h") && __has_include(<stdint.h>)
# include "config.h"
#endif

extern RTC_DATA_ATTR bool runOnce;
extern RTC_DATA_ATTR uint8_t twelveMode;
extern RTC_DATA_ATTR uint8_t darkMode;
extern RTC_DATA_ATTR bool debugger;
extern RTC_DATA_ATTR uint8_t animMode;
extern RTC_DATA_ATTR uint8_t weatherMode;
extern RTC_DATA_ATTR uint8_t dateMode;
extern RTC_DATA_ATTR uint8_t syncNTP;
extern RTC_DATA_ATTR uint8_t watchFace;
extern RTC_DATA_ATTR bool switchFace;
extern RTC_DATA_ATTR weatherData latestWeather;
extern RTC_DATA_ATTR char city;
extern RTC_DATA_ATTR String cityName;
extern RTC_DATA_ATTR int cityNameID;
extern RTC_DATA_ATTR int8_t temperature;
extern RTC_DATA_ATTR int16_t weatherConditionCode;
extern RTC_DATA_ATTR uint8_t weatherFormat;
extern RTC_DATA_ATTR bool watchAction;
extern RTC_DATA_ATTR bool showWeather;
extern RTC_DATA_ATTR bool showBorder;
extern RTC_DATA_ATTR bool showSteps;
extern RTC_DATA_ATTR bool isNight;
extern uint16_t ambientOffset;
extern RTC_DATA_ATTR String dezign;
extern RTC_DATA_ATTR bool sleep_mode;
extern RTC_DATA_ATTR uint8_t SLEEP_HOUR;
extern RTC_DATA_ATTR uint8_t SLEEP_MINUTE;
extern RTC_DATA_ATTR uint8_t SYNC_HOUR;
extern RTC_DATA_ATTR uint8_t SYNC_MINUTE;
extern RTC_DATA_ATTR bool initialSync;

// Btn definitions
//#define IS_DOUBLE_TAP       (wakeupBit & ACC_INT_MASK && guiState == WATCHFACE_STATE)
#define IS_BTN_RIGHT_UP     (wakeupBit & UP_BTN_MASK && guiState == WATCHFACE_STATE)
#define IS_BTN_LEFT_UP      (wakeupBit & BACK_BTN_MASK && guiState == WATCHFACE_STATE)
#define IS_BTN_RIGHT_DOWN   (wakeupBit & DOWN_BTN_MASK && guiState == WATCHFACE_STATE)
#define EXT_INT_MASK        MENU_BTN_MASK|BACK_BTN_MASK|UP_BTN_MASK|DOWN_BTN_MASK

class WatchyBase : public Watchy {
  public:
    WatchyBase();
    void interruptAlarm(bool enable);
    virtual void init(String datetime = "");
    virtual void handleButtonPress();
    virtual void deepSleep();
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
    void sleepModeApp();
//    void setSleepTime();
    weatherData weather999();
    bool noAlpha(String str);
    String getCityName();
    String getCityAbbv();
    int rtcTemp();
    void showList(const char* listItems[], byte itemCount, byte listIndex, bool selected, bool partialRefresh);
    void showMenu(byte menuIndex, bool partialRefresh);
    virtual void showFastMenu(byte menuIndex);

  private:
    void _rtcConfig();
    void _bmaConfig();
    static uint16_t _readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
    static uint16_t _writeRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
};

#endif

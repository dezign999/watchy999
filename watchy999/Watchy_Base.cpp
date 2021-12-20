//Derived from peerdavid's source at: https://github.com/peerdavid/Watchy
#include "Watchy_Base.h"
#include "config999.h"
#include "ArduinoNvs.h"
#include "esp_sntp.h"


RTC_DATA_ATTR bool runOnce = true;
RTC_DATA_ATTR uint8_t twelveMode = 0;
RTC_DATA_ATTR uint8_t darkMode = 0;
RTC_DATA_ATTR uint8_t syncNTP = 0;
RTC_DATA_ATTR uint8_t animMode = 3;
RTC_DATA_ATTR uint8_t dateMode = 0;
RTC_DATA_ATTR int wifiMode = 0;
RTC_DATA_ATTR uint8_t weatherMode = 0;
RTC_DATA_ATTR int selectedItem;
RTC_DATA_ATTR int syncIndex = 0;
RTC_DATA_ATTR uint8_t watchFace = 0; //0 = Donkey Kong, 1 = pxl999, 2 = slides999, 3 = synth999, 4 = Crush Em999, 5 = lowBatt999, 5 = Tetris, 6 = G5600
RTC_DATA_ATTR bool switchFace = true; // Enable redraw when switching faces;
RTC_DATA_ATTR int cityNameID;
RTC_DATA_ATTR String cityName;
RTC_DATA_ATTR String dezign;
RTC_DATA_ATTR int8_t temperature;
RTC_DATA_ATTR int16_t weatherConditionCode = 999;
RTC_DATA_ATTR uint8_t weatherFormat = 1;
RTC_DATA_ATTR bool watchAction = false;
RTC_DATA_ATTR weatherData latestWeather;
RTC_DATA_ATTR bool showWeather = false;
RTC_DATA_ATTR bool showBorder = false;
RTC_DATA_ATTR bool showSteps = false;
RTC_DATA_ATTR bool isNight = false;
RTC_DATA_ATTR int prefAP = 999;
RTC_DATA_ATTR int oldPrefAP = 666;
RTC_DATA_ATTR bool wifiError = false;
RTC_DATA_ATTR bool sleep_mode = false;
RTC_DATA_ATTR uint8_t disableSleepMode = 0;
RTC_DATA_ATTR uint8_t SLEEP_HOUR = 0;
RTC_DATA_ATTR uint8_t SLEEP_MINUTE = 0;
RTC_DATA_ATTR uint8_t SYNC_HOUR = 23;
RTC_DATA_ATTR uint8_t SYNC_MINUTE = 59;
RTC_DATA_ATTR bool displayFullInit_ = true;
RTC_DATA_ATTR bool initialSync = false;

int i, n;
bool res;
bool saveChanges = false;
bool manualSync = false;
const char *offsetStatus = "success";

const char *menuItems[] = {"Watch Face", "Time Sync", "Weather Sync", "Weather Format", "Hour Format", "Date Format", "Animation", "WiFi Mode", "Setup WiFi", "Set Time", "Sleep Mode", "Show Battery"};
int16_t menuOptions = sizeof(menuItems) / sizeof(menuItems[0]);

WatchyBase::WatchyBase() {}

void WatchyBase::interruptAlarm(bool enable) {
  if (enable) {
    RTC.clearAlarm();
  if(RTC.rtcType == DS3231){
//      RTC.rtc_ds.squareWave(SQWAVE_NONE); //disable square wave output
//      RTC.rtc_ds.setAlarm(ALM2_EVERY_MINUTE, 0, 0, 0, 0); //alarm wakes up Watchy every minute
      RTC.rtc_ds.alarmInterrupt(ALARM_2, true); //enable alarm interrupt   
    }else{
      int nextAlarmMinute = RTC.rtc_pcf.getMinute();
      nextAlarmMinute = (nextAlarmMinute == 59) ? 0 : (nextAlarmMinute + 1);
      RTC.rtc_pcf.setAlarm(nextAlarmMinute, 99, 99, 99);  
    }
  } else {
    if (RTC.rtcType == PCF8563) 
      RTC.rtc_pcf.resetAlarm(); 
    else 
      RTC.rtc_ds.alarmInterrupt(ALARM_2,false); 
  }  
}

void WatchyBase::init(String datetime) {
  if (debugger)
    Serial.begin(115200);

  NVS.begin();

  if (runOnce) {
    size_t blobLength = NVS.getBlobSize("dezign");
    uint8_t dezign[blobLength];
    res = NVS.getBlob("dezign", dezign, sizeof(dezign));

    if (blobLength > 0) {
      watchFace = dezign[0];
      twelveMode = dezign[1];
      animMode = dezign[2];
      weatherMode = dezign[3];
      syncNTP = dezign[4];
      darkMode = dezign[5];
      weatherFormat = dezign[6];
      dateMode = dezign[7];
      disableSleepMode = dezign[8];
      SLEEP_HOUR = dezign[9];
      SLEEP_MINUTE = dezign[10];
      SYNC_HOUR = dezign[11];
      SYNC_MINUTE = dezign[12];
    } else {
      //set defaults
      //[0]watchFace, [1]twelveMode, [2]animMode, [3]weatherMode, [4]syncNTP, [5]darkMode, [6]weatherFormat, [7]dateMode
      //[8]disableSleepMode, [9]SLEEP_HOUR, [10]SLEEP_MINUTE
      uint8_t dezignString [13] = { watchFace, twelveMode, animMode, weatherMode, 1, 1, 1, 0, disableSleepMode, SLEEP_HOUR, SLEEP_MINUTE, SYNC_HOUR, SYNC_MINUTE };
      res = NVS.setBlob("dezign", dezignString, sizeof(dezignString)); // store dezign [11] to key "dezign" on NVS
    }
  }

  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause(); //get wake up reason
  Wire.begin(SDA, SCL); //init i2c
  RTC.init();

  // Init the display here for all cases, if unused, it will do nothing
    display.init(0, displayFullInit_, 10, true); // 10ms by spec, and fast pulldown reset
    display.epd2.setBusyCallback(displayBusyCallback);
    if(runOnce)
      display.display(false);

  switch (wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0: //RTC Alarm
        RTC.clearAlarm(); //resets the alarm flag in the RTC
      if (guiState == WATCHFACE_STATE) {
        RTC.read(currentTime);

        if (currentTime.Hour == SLEEP_HOUR && currentTime.Minute == SLEEP_MINUTE && !disableSleepMode) {
          sleep_mode = true;
//            RTC.alarmInterrupt(ALARM_2, false); 
          interruptAlarm(false);
        }

        showWatchFace(true); //partial updates on tick
      }
      break;

    case ESP_SLEEP_WAKEUP_EXT1: //button Press + no handling if wakeup
      if (sleep_mode) {
        sleep_mode = false;
//        RTC.alarmInterrupt(ALARM_2, true);
        interruptAlarm(true);
        runOnce = true;
        RTC.read(currentTime);
        showWatchFace(false); //full update on wakeup from sleep mode
        break;
      }

      handleButtonPress();
      break;

    default: //reset
      RTC.config(datetime);
      _bmaConfig();
      showWatchFace(true); //full update on reset
      break;
  }

  // Sometimes BMA crashes - simply try to reinitialize bma...
  if (sensor.getErrorCode() != 0) {
    sensor.shutDown();
    sensor.wakeUp();
    sensor.softReset();
    _bmaConfig();
  }
  deepSleep();
}

void WatchyBase::deepSleep(){
    display.hibernate();
    displayFullInit_ = false; // Notify not to init it again    
    RTC.clearAlarm(); //resets the alarm flag in the RTC
     // Set pins 0-39 to input to avoid power leaking out
    for(int i=0; i<40; i++) {
        pinMode(i, INPUT);
    }    
    esp_sleep_enable_ext0_wakeup(RTC_PIN, 0); //enable deep sleep wake on RTC interrupt
    esp_sleep_enable_ext1_wakeup(BTN_PIN_MASK, ESP_EXT1_WAKEUP_ANY_HIGH); //enable deep sleep wake on button press
    esp_deep_sleep_start();
}

void WatchyBase::saveVars() {
  //[0]watchFace, [1]twelveMode, [2]animMode, [3]weatherMode, [4]syncNTP, [5]darkMode, [6]weatherFormat, [7]dateMode
  //[8]disableSleepMode, [9]SLEEP_HOUR, [10]SLEEP_MINUTE, [11]SYNC_HOUR, [12]SYNC_MINUTE
  uint8_t dezignString [13] = {watchFace, twelveMode, animMode, weatherMode, syncNTP, darkMode, weatherFormat, dateMode,
                               disableSleepMode, SLEEP_HOUR, SLEEP_MINUTE, SYNC_HOUR, SYNC_MINUTE
                              };
  res = NVS.setBlob("dezign", dezignString, sizeof(dezignString)); // store dezign [13] to key "dezign" on NVS
}

void WatchyBase::handleButtonPress() {
  uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();

  //Menu Button
  if (wakeupBit & MENU_BTN_MASK) {
    if (guiState == WATCHFACE_STATE) { //enter menu state if coming from watch face
      vibrate();
      showMenu(menuIndex, false);
    } else if (guiState == MAIN_MENU_STATE) { //if already in menu, then select menu item
      switch (menuIndex)
      {
        case 0:
          watchfaceApp();
          break;
        case 1:
          ntpApp();
          break;
        case 2:
          weatherApp();
          break;
        case 3:
          weatherFormatApp();
          break;
        case 4:
          twelveModeApp();
          break;
        case 5:
          dateModeApp();
          break;
        case 6:
          animationApp();
          break;
        case 7:
          wifiModeApp();
          break;
        case 8:
          setupWifi();
          break;
        case 9:
          setTime();
          break;
        case 10:
          sleepModeApp();
          break;
        case 11:
          showBattery();
          break;
        default:
          break;
      }
    } else if (guiState == FW_UPDATE_STATE) {
      updateFWBegin();
    }
  }
  //Back Button
  else if (wakeupBit & BACK_BTN_MASK) {
    if (guiState == MAIN_MENU_STATE) { //exit to watch face if already in menu
      RTC.clearAlarm();
      RTC.read(currentTime);
      showWatchFace(true);
      menuIndex = 0;
    } else if (guiState == APP_STATE) {
      showMenu(menuIndex, false);//exit to menu if already in app
    } else if (guiState == FW_UPDATE_STATE) {
      showMenu(menuIndex, false);//exit to menu if already in app
    }
  }
  //Up Button
  else if (wakeupBit & UP_BTN_MASK) {
    if (guiState == MAIN_MENU_STATE) { //increment menu index
      menuIndex--;
      if (menuIndex < 0) {
        menuIndex = menuOptions - 1;
      }
      showMenu(menuIndex, true);
    }
  }
  //Down Button
  else if (wakeupBit & DOWN_BTN_MASK) {
    if (guiState == MAIN_MENU_STATE) { //decrement menu index
      menuIndex++;
      if (menuIndex > menuOptions - 1) {
        menuIndex = 0;
      }
      showMenu(menuIndex, true);
    }
  }

  if (IS_BTN_LEFT_UP) {
    vibrate();
    RTC.clearAlarm();
    RTC.read(currentTime);
    showWatchFace(true);
    return;
  }

  if (IS_BTN_RIGHT_UP) {
    RTC.read(currentTime);
    vibrate();
    darkMode = (!darkMode) ? true : false;
    if(watchFace != 0 && watchFace != 1 && watchFace != 6 && watchFace != 9)
//    saveVars();
    showWatchFace(true);
    return;
  }

  if (IS_BTN_RIGHT_DOWN) {
    RTC.read(currentTime);
    if (watchFace != 9)
      vibrate();
    watchAction = (!watchAction) ? true : false;
    showWatchFace(true);
    return;
  }

  /***************** fast menu *****************/
  bool timeout = false;
  long lastTimeout = millis();
  pinMode(MENU_BTN_PIN, INPUT);
  pinMode(BACK_BTN_PIN, INPUT);
  pinMode(UP_BTN_PIN, INPUT);
  pinMode(DOWN_BTN_PIN, INPUT);

  while (!timeout) {
    if (millis() - lastTimeout > 5000) {
      timeout = true;
    } else {
      if (digitalRead(MENU_BTN_PIN) == 1) {
        vibrate();
        lastTimeout = millis();
        if (guiState == MAIN_MENU_STATE) { //if already in menu, then select menu item
          switch (menuIndex)
          {
            case 0:
              watchfaceApp();
              break;
            case 1:
              ntpApp();
              break;
            case 2:
              weatherApp();
              break;
            case 3:
              weatherFormatApp();
              break;
            case 4:
              twelveModeApp();
              break;
            case 5:
              dateModeApp();
              break;
            case 6:
              animationApp();
              break;
            case 7:
              wifiModeApp();
              break;
            case 8:
              setupWifi();
              break;
            case 9:
              setTime();
              break;
            case 10:
              sleepModeApp();
              break;
            case 11:
              showBattery();
              break;
            default:
              break;
          }
        } else if (guiState == FW_UPDATE_STATE) {
          updateFWBegin();
        }
      } else if (digitalRead(BACK_BTN_PIN) == 1) {
        vibrate();
        lastTimeout = millis();
        if (guiState == MAIN_MENU_STATE) { //exit to watch face if already in menu
          RTC.clearAlarm();
          RTC.read(currentTime);
          showWatchFace(false);
          break; //leave loop
        } else if (guiState == APP_STATE) {
          showMenu(menuIndex, false);//exit to menu if already in app
        } else if (guiState == FW_UPDATE_STATE) {
          showMenu(menuIndex, false);//exit to menu if already in app
        }
      } else if (digitalRead(UP_BTN_PIN) == 1) {
        vibrate();
        lastTimeout = millis();
        if (guiState == MAIN_MENU_STATE) { //increment menu index
          menuIndex--;
          if (menuIndex < 0) {
            menuIndex = menuOptions - 1;
          }
          showFastMenu(menuIndex);
        }
      } else if (digitalRead(DOWN_BTN_PIN) == 1) {
        vibrate();
        lastTimeout = millis();
        if (guiState == MAIN_MENU_STATE) { //decrement menu index
          menuIndex++;
          if (menuIndex > menuOptions - 1) {
            menuIndex = 0;
          }
          showFastMenu(menuIndex);
        }
      }
    }
  }

  //  Watchy::handleButtonPress();
  display.hibernate();

}

void WatchyBase::vibrate(uint8_t times, uint32_t delay_time) {
  // Ensure that no false positive double tap is produced
  sensor.enableFeature(BMA423_WAKEUP, false);

  pinMode(VIB_MOTOR_PIN, OUTPUT);
  for (uint8_t i = 0; i < times; i++) {
    delay(delay_time);
    digitalWrite(VIB_MOTOR_PIN, true);
    delay(delay_time);
    digitalWrite(VIB_MOTOR_PIN, false);
  }

  sensor.enableFeature(BMA423_WAKEUP, true);
}

void WatchyBase::_bmaConfig() {

  if (sensor.begin(_readRegister, _writeRegister, delay) == false) {
    //fail to init BMA
    return;
  }

  // Accel parameter structure
  Acfg cfg;
  /*!
      Output data rate in Hz, Optional parameters:
          - BMA4_OUTPUT_DATA_RATE_0_78HZ
          - BMA4_OUTPUT_DATA_RATE_1_56HZ
          - BMA4_OUTPUT_DATA_RATE_3_12HZ
          - BMA4_OUTPUT_DATA_RATE_6_25HZ
          - BMA4_OUTPUT_DATA_RATE_12_5HZ
          - BMA4_OUTPUT_DATA_RATE_25HZ
          - BMA4_OUTPUT_DATA_RATE_50HZ
          - BMA4_OUTPUT_DATA_RATE_100HZ
          - BMA4_OUTPUT_DATA_RATE_200HZ
          - BMA4_OUTPUT_DATA_RATE_400HZ
          - BMA4_OUTPUT_DATA_RATE_800HZ
          - BMA4_OUTPUT_DATA_RATE_1600HZ
  */
  cfg.odr = BMA4_OUTPUT_DATA_RATE_100HZ;
  /*!
      G-range, Optional parameters:
          - BMA4_ACCEL_RANGE_2G
          - BMA4_ACCEL_RANGE_4G
          - BMA4_ACCEL_RANGE_8G
          - BMA4_ACCEL_RANGE_16G
  */
  cfg.range = BMA4_ACCEL_RANGE_2G;
  /*!
      Bandwidth parameter, determines filter configuration, Optional parameters:
          - BMA4_ACCEL_OSR4_AVG1
          - BMA4_ACCEL_OSR2_AVG2
          - BMA4_ACCEL_NORMAL_AVG4
          - BMA4_ACCEL_CIC_AVG8
          - BMA4_ACCEL_RES_AVG16
          - BMA4_ACCEL_RES_AVG32
          - BMA4_ACCEL_RES_AVG64
          - BMA4_ACCEL_RES_AVG128
  */
  cfg.bandwidth = BMA4_ACCEL_NORMAL_AVG4;

  /*! Filter performance mode , Optional parameters:
      - BMA4_CIC_AVG_MODE
      - BMA4_CONTINUOUS_MODE
  */
  cfg.perf_mode = BMA4_CONTINUOUS_MODE;

  // Configure the BMA423 accelerometer
  sensor.setAccelConfig(cfg);

  // Enable BMA423 accelerometer
  // Warning : Need to use feature, you must first enable the accelerometer
  sensor.enableAccel();

  struct bma4_int_pin_config config ;
  config.edge_ctrl = BMA4_LEVEL_TRIGGER;
  config.lvl = BMA4_ACTIVE_HIGH;
  config.od = BMA4_PUSH_PULL;
  config.output_en = BMA4_OUTPUT_ENABLE;
  config.input_en = BMA4_INPUT_DISABLE;
  // The correct trigger interrupt needs to be configured as needed
  sensor.setINTPinConfig(config, BMA4_INTR1_MAP);

  struct bma423_axes_remap remap_data;
  remap_data.x_axis = 1;
  remap_data.x_axis_sign = 0xFF;
  remap_data.y_axis = 0;
  remap_data.y_axis_sign = 0xFF;
  remap_data.z_axis = 2;
  remap_data.z_axis_sign = 0xFF;
  // Need to raise the wrist function, need to set the correct axis
  sensor.setRemapAxes(&remap_data);

  // Enable BMA423 isStepCounter feature
  sensor.enableFeature(BMA423_STEP_CNTR, true);
  // Enable BMA423 isTilt feature
  sensor.enableFeature(BMA423_TILT, true);
  // Enable BMA423 isDoubleClick feature
  //sensor.enableFeature(BMA423_WAKEUP, true);

  // Reset steps
  //sensor.resetStepCounter();

  // Turn on feature interrupt
  //sensor.enableStepCountInterrupt();
  //sensor.enableTiltInterrupt();
  // It corresponds to isDoubleClick interrupt
  //sensor.enableWakeupInterrupt();
}

uint16_t WatchyBase::_readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)address, (uint8_t)len);
  uint8_t i = 0;
  while (Wire.available()) {
    data[i++] = Wire.read();
  }
  return 0;
}

uint16_t WatchyBase::_writeRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(data, len);
  return (0 !=  Wire.endTransmission());
}

//scrolling menu by Alex Story
void WatchyBase::showMenu(byte menuIndex, bool partialRefresh) {
  display.init(0, false); //_initial_refresh to false to prevent full update on init
  display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b);

  int16_t  x1, y1;
  uint16_t w, h;
  int16_t yPos;
  int16_t startPos = 0;
  //Code to move the menu if current selected index out of bounds
  if (menuIndex + MENU_LENGTH > menuOptions)
  {
    startPos = (menuOptions - 1) - (MENU_LENGTH - 1);
  }
  else
  {
    startPos = menuIndex;
  }
  for (int i = startPos; i < MENU_LENGTH + startPos; i++) {
    yPos = 30 + (MENU_HEIGHT * (i - startPos));
    display.setCursor(20, yPos);
    if (i == menuIndex) {
      display.getTextBounds(menuItems[i], 0, yPos, &x1, &y1, &w, &h);
      display.fillRect(x1 - 1, y1 - 10, 200, h + 15, GxEPD_WHITE);
      display.setTextColor(GxEPD_BLACK);
      display.println(menuItems[i]);
    } else {
      display.setTextColor(GxEPD_WHITE);
      display.println(menuItems[i]);
    }
  }

  display.display(true);
  //display.hibernate();

  guiState = MAIN_MENU_STATE;
}

//Specific functions by dezign999 start here
//Sub Menu with selectable items by dezign999
void WatchyBase::showFastMenu(byte menuIndex) {
  display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b);

  int16_t  x1, y1;
  uint16_t w, h;
  int16_t yPos;
  int16_t startPos = 0;
  if (menuIndex + MENU_LENGTH > menuOptions)
  {
    startPos = (menuOptions - 1) - (MENU_LENGTH - 1);
  }
  else
  {
    startPos = menuIndex;
  }
  for (int i = startPos; i < MENU_LENGTH + startPos; i++) {
    yPos = 30 + (MENU_HEIGHT * (i - startPos));
    display.setCursor(20, yPos);
    if (i == menuIndex) {
      display.getTextBounds(menuItems[i], 0, yPos, &x1, &y1, &w, &h);
      display.fillRect(x1 - 1, y1 - 10, 200, h + 15, GxEPD_WHITE);
      display.setTextColor(GxEPD_BLACK);
      display.println(menuItems[i]);
    } else {
      display.setTextColor(GxEPD_WHITE);
      display.println(menuItems[i]);
    }
  }

  display.display(true);

  guiState = MAIN_MENU_STATE;
}

void WatchyBase::watchfaceApp() {
  guiState == APP_STATE;
  display.init(0, false); //_initial_refresh to false to prevent full update on init
  display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_WHITE);
  display.fillScreen(GxEPD_BLACK);

  const char* listItems[] = {"DKtime", "pxl", "slides", "synth", "Crush Em", "lowBatt", "Watchytris", "G5600 by NiVZ", "Pebble", "Doom999", "Qlockchy"};
  byte itemCount = sizeof(listItems) / sizeof(listItems[0]);

  uint16_t listIndex = watchFace;

  //Send the stored animMode value to set the selected item
  showList(listItems, itemCount, watchFace, true, false);

  while (1) {

    if (digitalRead(BACK_BTN_PIN) == 1) {
      vibrate();
      if(saveChanges)
        saveVars();
      saveChanges = false;
      break;
    }

    if (digitalRead(MENU_BTN_PIN) == 1) {
      vibrate();
      if(listIndex != watchFace) {
        watchFace = listIndex;
        saveChanges = true;
      }
      switchFace = true;
      showList(listItems, itemCount, listIndex, true, true);
    }

    if (digitalRead(DOWN_BTN_PIN) == 1) {
      listIndex == (itemCount - 1) ? (listIndex = 0) : listIndex++;
      vibrate();
      showList(listItems, itemCount, listIndex, false, true);
    }

    if (digitalRead(UP_BTN_PIN) == 1) {
      listIndex == 0 ? (listIndex = (itemCount - 1)) : listIndex--;
      vibrate();
      showList(listItems, itemCount, listIndex, false, true);
    }

  }
  display.hibernate();
  showMenu(menuIndex, false);
}

void WatchyBase::animationApp() {
  guiState == APP_STATE;
  display.init(0, false); //_initial_refresh to false to prevent full update on init
  display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_WHITE);
  display.fillScreen(GxEPD_BLACK);

  const char* listItems[] = {"Every Minute", "Every Half Hour", "Every Hour", "On Step Change", "Off"};
  byte itemCount = sizeof(listItems) / sizeof(listItems[0]);

  uint16_t listIndex = animMode;

  //Send the stored animMode value to set the selected item
  showList(listItems, itemCount, animMode, true, false);

  while (1) {

    if (digitalRead(BACK_BTN_PIN) == 1) {
      vibrate();
      if(saveChanges)
        saveVars();
      saveChanges = false;
      break;
    }

    if (digitalRead(MENU_BTN_PIN) == 1) {
      vibrate();
      if(listIndex != animMode) {
        animMode = listIndex;
        saveChanges = true;
      }
      showList(listItems, itemCount, listIndex, true, true);
    }

    if (digitalRead(DOWN_BTN_PIN) == 1) {
      listIndex == (itemCount - 1) ? (listIndex = 0) : listIndex++;
      vibrate();
      showList(listItems, itemCount, listIndex, false, true);
    }

    if (digitalRead(UP_BTN_PIN) == 1) {
      listIndex == 0 ? (listIndex = (itemCount - 1)) : listIndex--;
      vibrate();
      showList(listItems, itemCount, listIndex, false, true);
    }

  }
  display.hibernate();
  showMenu(menuIndex, false);
}

void WatchyBase::weatherApp() {
  guiState == APP_STATE;
  display.init(0, false); //_initial_refresh to false to prevent full update on init
  display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_WHITE);
  display.fillScreen(GxEPD_BLACK);

  const char* listItems[] = {"Every Half Hour", "Every Hour", "Off", "Now"};
  byte itemCount = sizeof(listItems) / sizeof(listItems[0]);

  uint16_t listIndex = weatherMode;

  //Send the stored animMode value to set the selected item
  showList(listItems, itemCount, weatherMode, true, false);

  while (1) {

    if (digitalRead(BACK_BTN_PIN) == 1) {
      vibrate();
      if(saveChanges)
        saveVars();
      saveChanges = false;
      break;
    }

    if (digitalRead(MENU_BTN_PIN) == 1) {
      vibrate();
      if (listIndex != 3) {
        switchFace = true;
        if(listIndex != weatherMode) {
        weatherMode = listIndex;
        saveChanges = true;
      }
        showList(listItems, itemCount, listIndex, true, true);
      } else if (listIndex == 3) {

        showList(listItems, itemCount, listIndex, false, true);

        display.setTextColor(GxEPD_WHITE);
        display.setCursor(20, 145);
        display.println("Checking WiFi");
        display.display(true);

        manualSync = true;

        //        weather999();
        weatherData latestWeather = weather999();
        temperature = latestWeather.temperature;
        weatherConditionCode = latestWeather.weatherConditionCode;

        display.setCursor(20, 160);
        display.println("Temp: " + String((weatherFormat) ? (int)(temperature * 9. / 5. + 32.) : temperature));

        String tempCondition;

        //https://openweathermap.org/weather-conditions
        if (weatherConditionCode == 999) { //RTC
          tempCondition = "AMBIENT";
        } else if (weatherConditionCode > 801 && weatherConditionCode < 805) { //Cloudy
          tempCondition = "CLOUDY";
        } else if (weatherConditionCode == 801) { //Few Clouds
          tempCondition = "CLOUDS";
        } else if (weatherConditionCode == 800) { //Clear
          tempCondition = "CLEAR SKY";
        } else if (weatherConditionCode >= 700) { //Atmosphere
          tempCondition = "MISTY";
        } else if (weatherConditionCode >= 600) { //Snow
          tempCondition = "SNOW";
        } else if (weatherConditionCode >= 500) { //Rain
          tempCondition = "RAIN";
        } else if (weatherConditionCode >= 300) { //Drizzle
          tempCondition = "DRIZZLE";
        } else if (weatherConditionCode >= 200) { //Thunderstorm
          tempCondition = "THUNDER";
        }

        display.setCursor(20, 175);
        display.println(tempCondition);

        display.display(true);

      }
    }

    if (digitalRead(DOWN_BTN_PIN) == 1) {
      listIndex == (itemCount - 1) ? (listIndex = 0) : listIndex++;
      vibrate();
      showList(listItems, itemCount, listIndex, false, true);
    }

    if (digitalRead(UP_BTN_PIN) == 1) {
      listIndex == 0 ? (listIndex = (itemCount - 1)) : listIndex--;
      vibrate();
      showList(listItems, itemCount, listIndex, false, true);
    }

  }
  display.hibernate();
  showMenu(menuIndex, false);
}

void WatchyBase::weatherFormatApp() {
  guiState == APP_STATE;
  display.init(0, false); //_initial_refresh to false to prevent full update on init

  const char* listItems[] = {"Celcius", "Farenheit"};
  byte itemCount = sizeof(listItems) / sizeof(listItems[0]);

  uint16_t listIndex = weatherFormat;

  //Send the stored value to set the selected item
  showList(listItems, itemCount, weatherFormat, true, false);

  while (1) {

    if (digitalRead(BACK_BTN_PIN) == 1) {
      vibrate();
      if(saveChanges)
        saveVars();
      saveChanges = false;
      break;
    }

    if (digitalRead(MENU_BTN_PIN) == 1) {
      if(listIndex != weatherFormat) {
        weatherFormat = (listIndex == 0) ? 0 : 1;
        saveChanges = true;
      }     
      vibrate();
      showList(listItems, itemCount, listIndex, true, true);
    }

    if (digitalRead(DOWN_BTN_PIN) == 1) {
      listIndex = (listIndex == 0) ? 1 : 0;
      vibrate();
      showList(listItems, itemCount, listIndex, false, true);
      if (debugger)
        Serial.println(String(listIndex));
      RTC.read(currentTime);
    }

    if (digitalRead(UP_BTN_PIN) == 1) {
      listIndex = (listIndex == 0) ? 1 : 0;
      vibrate();
      showList(listItems, itemCount, listIndex, false, true);
      if (debugger)
        Serial.println(String(listIndex));
      RTC.read(currentTime);
    }

  }
  display.hibernate();
  showMenu(menuIndex, false);
}

void WatchyBase::twelveModeApp() {
  guiState == APP_STATE;
  display.init(0, false); //_initial_refresh to false to prevent full update on init
  const char* listItems[] = {"12 Hour", "24 Hour"};
  byte itemCount = sizeof(listItems) / sizeof(listItems[0]);

  uint16_t listIndex = twelveMode;

  //Send the stored value to set the selected item
  showList(listItems, itemCount, twelveMode, true, false);

  while (1) {

    if (digitalRead(BACK_BTN_PIN) == 1) {
      vibrate();
      if(saveChanges)
        saveVars();
      saveChanges = false;
      break;
    }

    if (digitalRead(MENU_BTN_PIN) == 1) {     
      vibrate();
      if(listIndex != twelveMode) {
        twelveMode = (listIndex == 0) ? 0 : 1;
        saveChanges = true;
      }
      showList(listItems, itemCount, listIndex, true, true);
    }

    if (digitalRead(DOWN_BTN_PIN) == 1) {
      listIndex = (listIndex == 0) ? 1 : 0;
      vibrate();
      showList(listItems, itemCount, listIndex, false, true);
      if (debugger)
        Serial.println(String(listIndex));
      RTC.read(currentTime);
    }

    if (digitalRead(UP_BTN_PIN) == 1) {
      listIndex = (listIndex == 0) ? 1 : 0;
      vibrate();
      showList(listItems, itemCount, listIndex, false, true);
      if (debugger)
        Serial.println(String(listIndex));
      RTC.read(currentTime);
    }

  }
  display.hibernate();
  showMenu(menuIndex, false);
}

void WatchyBase::sleepModeApp() {
  guiState == APP_STATE;
  display.init(0, false); //_initial_refresh to false to prevent full update on init
  const char* listItems[] = {"Sleep Mode On", "Sleep Mode Off", "Sleep Now", "Set Sleep Time"};
  byte itemCount = sizeof(listItems) / sizeof(listItems[0]);

  uint16_t listIndex = disableSleepMode;

  //Send the stored value to set the selected item
  showList(listItems, itemCount, disableSleepMode, true, false);

  while (1) {

    if (digitalRead(BACK_BTN_PIN) == 1) {
      vibrate();
      if(saveChanges)
        saveVars();
      saveChanges = false;
      break;
    }

    if (digitalRead(MENU_BTN_PIN) == 1) {
      vibrate();
      if (listIndex < 2) {
        if(listIndex != disableSleepMode)
          saveChanges = true;
        disableSleepMode = listIndex;
        showList(listItems, itemCount, listIndex, true, true);
      } else if (listIndex == 2) {
        break;
      } else if (listIndex == 3) {

        int8_t minute = SLEEP_MINUTE;
        int8_t hour = SLEEP_HOUR;

        int8_t setIndex = SET_HOUR;

        int8_t blink = 0;

        pinMode(DOWN_BTN_PIN, INPUT);
        pinMode(UP_BTN_PIN, INPUT);
        pinMode(MENU_BTN_PIN, INPUT);
        pinMode(BACK_BTN_PIN, INPUT);

        display.init(0, true); //_initial_refresh to false to prevent full update on init

        while (1) {

          if (digitalRead(MENU_BTN_PIN) == 1) {
            setIndex++;
            if (setIndex > SET_MINUTE) {
              vibrate();
              SLEEP_HOUR = hour;
              SLEEP_MINUTE = minute;
              break;
            }
          }
          if (digitalRead(BACK_BTN_PIN) == 1) {
            if (setIndex != SET_HOUR) {
              setIndex--;
              vibrate();
            }
          }

          blink = 1 - blink;

          if (digitalRead(DOWN_BTN_PIN) == 1) {
            blink = 1;
            switch (setIndex) {
              case SET_HOUR:
                vibrate();
                hour == 23 ? (hour = 0) : hour++;
                break;
              case SET_MINUTE:
                vibrate();
                minute == 59 ? (minute = 0) : minute++;
                break;
              default:
                break;
            }
          }

          if (digitalRead(UP_BTN_PIN) == 1) {
            blink = 1;
            switch (setIndex) {
              case SET_HOUR:
                vibrate();
                hour == 0 ? (hour = 23) : hour--;
                break;
              case SET_MINUTE:
                vibrate();
                minute == 0 ? (minute = 59) : minute--;
                break;
              default:
                break;
            }
          }

          display.fillScreen(GxEPD_BLACK);
          display.setTextColor(GxEPD_WHITE);
          display.setFont(&DSEG7_Classic_Bold_53);

          display.setCursor(5, 125);
          if (setIndex == SET_HOUR) { //blink hour digits
            display.setTextColor(blink ? GxEPD_WHITE : GxEPD_BLACK);
          }
          if (hour < 10) {
            display.print("0");
          }
          display.print(hour);

          display.setTextColor(GxEPD_WHITE);
          display.print(":");

          display.setCursor(108, 125);
          if (setIndex == SET_MINUTE) { //blink minute digits
            display.setTextColor(blink ? GxEPD_WHITE : GxEPD_BLACK);
          }
          if (minute < 10) {
            display.print("0");
          }
          display.print(minute);

          display.display(true); //partial refresh
        }


      }

      if (listIndex != 2) {
        listIndex = 0;
        showList(listItems, itemCount, listIndex, false, true);
      }

    }

    if (digitalRead(DOWN_BTN_PIN) == 1) {
      listIndex == (itemCount - 1) ? (listIndex = 0) : listIndex++;
      vibrate();
      showList(listItems, itemCount, listIndex, false, true);
      if (debugger)
        Serial.println(String(listIndex));
      RTC.read(currentTime);
    }

    if (digitalRead(UP_BTN_PIN) == 1) {
      listIndex == 0 ? (listIndex = (itemCount - 1)) : listIndex--;
      vibrate();
      showList(listItems, itemCount, listIndex, false, true);
      if (debugger)
        Serial.println(String(listIndex));
      RTC.read(currentTime);
    }

  }

  if (listIndex == 2) {
    sleep_mode = true;
//    saveVars();   
    interruptAlarm(false); 
    
    RTC.read(currentTime);
    showWatchFace(true);
    menuIndex = 0;
  }

  display.hibernate();
  if (listIndex != 2)
    showMenu(menuIndex, true);
}

void WatchyBase::dateModeApp() {
  guiState == APP_STATE;
  display.init(0, false); //_initial_refresh to false to prevent full update on init
  const char* listItems[] = {"MM/DD", "DD/MM"};
  byte itemCount = sizeof(listItems) / sizeof(listItems[0]);

  uint16_t listIndex = dateMode;

  //Send the stored value to set the selected item
  showList(listItems, itemCount, dateMode, true, false);

  while (1) {

    if (digitalRead(BACK_BTN_PIN) == 1) {
      vibrate();
      if(saveChanges)
        saveVars();
      saveChanges = false;
      break;
    }

    if (digitalRead(MENU_BTN_PIN) == 1) {    
      vibrate();
      if(listIndex != dateMode) {
        dateMode = (listIndex == 0) ? 0 : 1;
        saveChanges = true;
      }
      showList(listItems, itemCount, listIndex, true, true);
    }

    if (digitalRead(DOWN_BTN_PIN) == 1) {
      listIndex = (listIndex == 0) ? 1 : 0;
      vibrate();
      showList(listItems, itemCount, listIndex, false, true);
      if (debugger)
        Serial.println(String(listIndex));
      RTC.read(currentTime);
    }

    if (digitalRead(UP_BTN_PIN) == 1) {
      listIndex = (listIndex == 0) ? 1 : 0;
      vibrate();
      showList(listItems, itemCount, listIndex, false, true);
      if (debugger)
        Serial.println(String(listIndex));
      RTC.read(currentTime);
    }

  }
  display.hibernate();
  showMenu(menuIndex, false);
}

void WatchyBase::ntpApp() {
  guiState == APP_STATE;
  display.init(0, false); //_initial_refresh to false to prevent full update on init
  const char* listItems[] = {"When Charging", "Once a Day", "Off", "Now (Press Menu)", "Set Sync Time"};
  byte itemCount = sizeof(listItems) / sizeof(listItems[0]);

  //  uint16_t listIndex = syncIndex;
  uint16_t listIndex = syncNTP;

  bool synced = false;

  //Send the stored animMode value to set the selected item
  showList(listItems, itemCount, syncNTP, true, false);

  while (1) {

    if (digitalRead(BACK_BTN_PIN) == 1) {
      vibrate();
      if(saveChanges)
        saveVars();
      saveChanges = false;
      break;
    }

    if (digitalRead(DOWN_BTN_PIN) == 1) {
      listIndex == (itemCount - 1) ? (listIndex = 0) : listIndex++;
      synced = false;
      vibrate();
      showList(listItems, itemCount, listIndex, false, true);
    }

    if (digitalRead(UP_BTN_PIN) == 1) {
      listIndex == 0 ? (listIndex = (itemCount - 1)) : listIndex--;   
      synced = false;
      vibrate();
      showList(listItems, itemCount, listIndex, false, true);
    }

    if (digitalRead(MENU_BTN_PIN) == 1) {
      vibrate();
      if (debugger)
        Serial.println("ntp listIndex: " + String(listIndex));

      if (listIndex < 3) {
//        syncNTP = listIndex;
        //        syncIndex = listIndex;
        if(listIndex != syncNTP)
          saveChanges = true;
        syncNTP = listIndex;
        showList(listItems, itemCount, listIndex, true, true);

      } else if (listIndex == 3) {

        showList(listItems, itemCount, listIndex, false, true);

        display.setTextColor(GxEPD_WHITE);
        //        display.setCursor(20, 120);
        display.fillRect(15, 138, 170, 20, GxEPD_BLACK);
        display.setCursor(20, 145);
        display.println("Checking WiFi");
        display.display(true);

        manualSync = true;

        syncNtpTime();

        synced = true;
      }  else if (listIndex == 4) {

        int8_t minute = SYNC_MINUTE;
        int8_t hour = SYNC_HOUR;

        int8_t setIndex = SET_HOUR;

        int8_t blink = 0;

        pinMode(DOWN_BTN_PIN, INPUT);
        pinMode(UP_BTN_PIN, INPUT);
        pinMode(MENU_BTN_PIN, INPUT);
        pinMode(BACK_BTN_PIN, INPUT);

        display.init(0, true); //_initial_refresh to false to prevent full update on init

        while (1) {

          if (digitalRead(MENU_BTN_PIN) == 1) {
            setIndex++;
            if (setIndex > SET_MINUTE) {
              vibrate();
              SYNC_HOUR = hour;
              SYNC_MINUTE = minute;
              break;
            }
          }
          if (digitalRead(BACK_BTN_PIN) == 1) {
            if (setIndex != SET_HOUR) {
              setIndex--;
              vibrate();
            }
          }

          blink = 1 - blink;

          if (digitalRead(DOWN_BTN_PIN) == 1) {
            blink = 1;
            switch (setIndex) {
              case SET_HOUR:
                vibrate();
                hour == 23 ? (hour = 0) : hour++;
                break;
              case SET_MINUTE:
                vibrate();
                minute == 59 ? (minute = 0) : minute++;
                break;
              default:
                break;
            }
          }

          if (digitalRead(UP_BTN_PIN) == 1) {
            blink = 1;
            switch (setIndex) {
              case SET_HOUR:
                vibrate();
                hour == 0 ? (hour = 23) : hour--;
                break;
              case SET_MINUTE:
                vibrate();
                minute == 0 ? (minute = 59) : minute--;
                break;
              default:
                break;
            }
          }

          display.fillScreen(GxEPD_BLACK);
          display.setTextColor(GxEPD_WHITE);
          display.setFont(&DSEG7_Classic_Bold_53);

          display.setCursor(5, 125);
          if (setIndex == SET_HOUR) { //blink hour digits
            display.setTextColor(blink ? GxEPD_WHITE : GxEPD_BLACK);
          }
          if (hour < 10) {
            display.print("0");
          }
          display.print(hour);

          display.setTextColor(GxEPD_WHITE);
          display.print(":");

          display.setCursor(108, 125);
          if (setIndex == SET_MINUTE) { //blink minute digits
            display.setTextColor(blink ? GxEPD_WHITE : GxEPD_BLACK);
          }
          if (minute < 10) {
            display.print("0");
          }
          display.print(minute);

          display.display(true); //partial refresh
        }


      }

      if (listIndex == 4) {
        listIndex = 0;
        showList(listItems, itemCount, listIndex, false, true);
      }
    }

  }

  //  syncNTP = (syncIndex == 1) ? true : false;
  //  if (syncIndex != 3) {
  //    syncNTP = syncIndex;
  //  }

  display.hibernate();
  showMenu(menuIndex, false);

}

void WatchyBase::wifiModeApp() {
  guiState == APP_STATE;
  display.init(0, false); //_initial_refresh to false to prevent full update on init
  const char* listItems[] = {"Multi APs", "Default WiFi"};
  byte itemCount = sizeof(listItems) / sizeof(listItems[0]);

  uint16_t listIndex = wifiMode;
  uint8_t selectCounter = 0;

  //Send the stored value to set the selected item
  showList(listItems, itemCount, wifiMode, true, false);

  while (1) {

    if (digitalRead(BACK_BTN_PIN) == 1) {
      vibrate();
      if(saveChanges)
        saveVars();
      saveChanges = false;
      break;
    }

    if (digitalRead(MENU_BTN_PIN) == 1) {
      if(listIndex != wifiMode) {
        wifiMode = (listIndex == 0) ? 0 : 1;
        saveChanges = true;
      }  
      selectCounter++;
      vibrate();
      if (selectCounter < 2)
        showList(listItems, itemCount, listIndex, true, true);
      if (wifiMode == 1) {
        display.setTextColor(GxEPD_WHITE);
        display.setCursor(20, 90);
        display.println("SSID Reset");
        display.setCursor(20, 105);
        display.println("Press Menu");
        display.setCursor(20, 120);
        display.println("to Setup WiFi");
        display.display(true);
      }
      if (selectCounter == 2) {
        setupWifi();
      }
    }

    if (digitalRead(DOWN_BTN_PIN) == 1) {
      listIndex = (listIndex == 0) ? 1 : 0;
      vibrate();
      showList(listItems, itemCount, listIndex, false, true);
      if (debugger)
        Serial.println(String(listIndex));
    }

    if (digitalRead(UP_BTN_PIN) == 1) {
      listIndex = (listIndex == 0) ? 1 : 0;
      vibrate();
      showList(listItems, itemCount, listIndex, false, true);
      if (debugger)
        Serial.println(String(listIndex));
    }

  }
  display.hibernate();
  showMenu(menuIndex, false);
}

void WatchyBase::showList(const char* listItems[], byte itemCount, byte listIndex, bool selected, bool partialRefresh) {
  display.init(0, false); //_initial_refresh to false to prevent full update on init
  display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b);

  if (selected)
    selectedItem = listIndex;

  if (debugger) {
    Serial.print("listItem: ");
    Serial.println(listItems[listIndex]);
    Serial.println("listOption length: " + String(itemCount));
    Serial.print("Selected: ");
    Serial.println((selected) ? "true" : "false");
    Serial.println("Selected Item: " + String(selectedItem));
  }

  int16_t  x1, y1;
  uint16_t w, h;
  int16_t yPos;
  int16_t startPos = 0;
  bool longList = (itemCount >= 6) ? true : false;

  if (longList) {
    if (listIndex + MENU_LENGTH > itemCount) {
      startPos = (itemCount - 1) - (MENU_LENGTH - 1);
    } else {
      startPos = listIndex;
    }
  }

  for (int i = ((longList) ? startPos : 0); i < ((longList) ? (MENU_LENGTH + startPos) : itemCount); i++) {
    yPos = 30 + (MENU_HEIGHT * ((longList) ? (i - startPos) : i));
    display.setCursor(20, yPos);
    if (i == listIndex) {
      display.getTextBounds(listItems[i], 0, yPos, &x1, &y1, &w, &h);
      display.fillRect(x1 - 1, y1 - 10, 200, h + 15, GxEPD_WHITE);
      display.setTextColor(GxEPD_BLACK);
      display.println(listItems[i]);
      if (i == selectedItem) {
        display.setCursor(5, yPos);
        display.println(">");
      }
    } else {
      display.setTextColor(GxEPD_WHITE);
      display.println(listItems[i]);
      if (i == selectedItem) {
        display.setCursor(5, yPos);
        display.println(">");
      }
    }
  }

  display.display(true);
  guiState = APP_STATE;
}

void WatchyBase::syncNtpTime() {

  if (debugger)
    Serial.println("Syncing NTP");

  if (!runOnce) {
    if (wifiMode == 0) {
      if (debugger)
        Serial.println("Trying wifi999");
      wifi999();
    } else {
      if (debugger)
        Serial.println("Trying WiFi");
      connectWiFi();
    }
  }

  if (WiFi.status() == WL_CONNECTED) {

    time_t t;
    bool syncFailed = false;

    int gatewayAddress = WiFi.gatewayIP();

    if (gmtOffset == 0) { //Get Time Offset
      HTTPClient http;
      http.setConnectTimeout(3000);//3 second max timeout

      String gmtOffsetURL = "http://ip-api.com/json/?fields=33570816";

      http.begin(gmtOffsetURL.c_str());
      int httpResponseCode = http.GET();
      if (httpResponseCode == 200) {
        String payload = http.getString();
        JSONVar responseObject = JSON.parse(payload);
        offsetStatus = responseObject["status"];
//        gmtOffset = int(responseObject["offset"]);
        if (strcmp(offsetStatus, "success") != 0) {
          if (manualSync) {
            display.setCursor(20, 160);
            display.println("Offset Failed");
            syncFailed = true;
          }
        } else {
          gmtOffset = int(responseObject["offset"]); //Good practice edit by @ekalfwonS
        }
      } else {
        //http error
        if (manualSync) {
          display.setCursor(20, 160);
          display.println("Offset Failed");
          syncFailed = true;
        }
      }
    }

    

//    time_t now = time(nullptr);


    if (debugger) {
      Serial.println("Offset: " + String(gmtOffset));
      Serial.println("GatewayIP: " + String(gatewayAddress));
    }  

  if (!syncFailed) {
    configTime(gmtOffset, 0, ntpServer);

    int i = 0;
      while (!sntp_get_sync_status() == SNTP_SYNC_STATUS_COMPLETED && i < 20) {
//      while (now < SECS_YR_2000 && i < 20) { // possible fix
//      now = time(nullptr);
      if (debugger)
        Serial.print(".");
      delay(1000);
      i++;
      if(debugger)
        Serial.println("i = " + String(i));
      if (i == 20) {
        if (manualSync) {
          display.setCursor(20, 160);
          display.println("Failed to sync");
        }
        syncFailed = true;
        break;
      }
    }
  }

    if (!syncFailed) {
      time_t tnow = time(nullptr);
      struct tm *local = localtime(&tnow);
      if (manualSync) {
        display.setCursor(20, 160);
        display.println("Offset: " + String(gmtOffset));
        display.setCursor(20, 175);
        if (local->tm_hour < 10) {
          display.print("0");
        }
        display.print(local->tm_hour);
        display.print(":");
        if (local->tm_min < 10) {
          display.print("0");
        }
        display.print(local->tm_min);
      }

      currentTime.Year = local->tm_year + YEAR_OFFSET - 2040; //This change matches watchy defaults
      currentTime.Month = local->tm_mon + 1;
      currentTime.Day = local->tm_mday;
      currentTime.Hour = local->tm_hour;
      currentTime.Minute = local->tm_min;
      currentTime.Second = local->tm_sec;
      currentTime.Wday = local->tm_wday + 1;
      RTC.set(currentTime);
      RTC.read(currentTime);

    }
  } else {
    if (debugger)
      Serial.println("No WiFi");
    if (manualSync) {
      display.setCursor(20, 160);
      display.println("No WiFi Found");
    }
  }

  disableWiFi();
  display.display(true);
  if (manualSync)
    delay(3000);
  if(debugger)
    Serial.println("NTP Sync Done");
  manualSync = false;
  initialSync = true;
}

void WatchyBase::disableWiFi() {
  WiFi.mode(WIFI_OFF);
  WIFI_CONFIGURED = false;
  btStop();
  if (debugger) {
    Serial.print("WiFi Turned Off. IP Check: ");
    Serial.println(WiFi.localIP());
  }
}

void WatchyBase::getWifi() {

  for (n = (prefAP == oldPrefAP) ? prefAP-- : 0; n < accessPointCount; n++) {

    if (debugger)
      Serial.println("Trying: " + String(accessPoints[n]));

    //Sanity Check Wifi Credentials against number of declared Access Points
    wifiError = (accessPointCount != apPasswordsCount) ? true : (accessPointCount != apTimeoutsCount) ? true : (accessPointCount != cityNamesCount) ? true
                : (accessPointCount != cityAbbvCount) ? true : false;
    if (wifiError) {
      if (debugger)
        Serial.println("AP Count & Credential Mismatch");
      break;
    }

    WiFi.begin(accessPoints[n], apPasswords[n]);
    i = 0;
    while (i < apTimeouts[n] && WiFi.status() != WL_CONNECTED) {
      i++;
      delay(500);
      if (debugger)
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
      prefAP = n;
      oldPrefAP = n;
      break;
    }
    if (debugger) {
      Serial.print(F("\nConnecting to "));
      Serial.println(accessPoints[n]);
    }
  }
}

bool WatchyBase::wifi999() {

  getWifi();

  if (WiFi.status() == WL_CONNECTED) {
    if (debugger) {
      Serial.print(F("\nWiFi connected, your IP address is "));
      Serial.println(WiFi.localIP());
      Serial.print("City Name: ");
      Serial.println(cityNames[n]);
    }
    cityNameID = n;
    WIFI_CONFIGURED = true;
  } else if (WiFi.status() != WL_CONNECTED && prefAP != 999 && !wifiError) {
    prefAP = 999;
    if (debugger)
      Serial.println("trying wifi again");
    //    wifi999();
    getWifi();
  } else {
    if (debugger)
      Serial.println("No WiFi");
    WIFI_CONFIGURED = false;
    disableWiFi();
  }

  if (debugger) {
    Serial.print("WIFI_CONFIGURED = ");
    Serial.println((WIFI_CONFIGURED == 1) ? "true" : "false");
  }
  return WIFI_CONFIGURED;
}

bool WatchyBase::noAlpha(String str) { //Check if the city name is an ID code or a text name
  for (int i = 0; i < str.length(); i++) {
    if (str[i] >= '0' && str[i] <= '9') {
      return true;
    } else {
      return false;
    }
  }
  return 0;
}

int WatchyBase::rtcTemp() {
//  temperature = (RTC.temperature() / 4) - ambientOffset; //celsius
    temperature = sensor.readTemperature() - ambientOffset; //celsius
  if (debugger)
    Serial.println("rtcTemp(): " + String(temperature));
  return temperature;
}

String WatchyBase::getCityAbbv() {
  String scity;
  if (cityNameID == 999 || wifiError) {
    scity = "RTC";
  } else {
    scity = cityAbbv[cityNameID];
  }
  return scity;
}

String WatchyBase::getCityName() {
  String scity;
  if (cityNameID == 999 || wifiError) {
    scity = "RTC";
  } else {
    scity = cityNames[cityNameID];
  }
  return scity;
}

weatherData WatchyBase::weather999() {

  if (debugger)
    Serial.println("WifiMode: " + String(wifiMode));
  if (wifiMode == 0) {
    if (debugger)
      Serial.println("Trying wifi999");
    wifi999();
  } else {
    if (debugger)
      Serial.println("Trying WiFi");
    connectWiFi();
  }

  if (WiFi.status() == WL_CONNECTED) { //Use Weather API for live data if WiFi is connected

    HTTPClient http;
    http.setConnectTimeout(3000);//3 second max timeout

    String weatherQueryURL = (noAlpha(getCityName())) ? String(URL) + String("?id=") + String(getCityName())
                             : String(URL) + String("?q=") + String(getCityName()) + String(",") + String(COUNTRY);
    weatherQueryURL = weatherQueryURL + String("&units=metric&appid=") + String(APIKEY);

    if (debugger) {
      Serial.print("CITY NAME OR ID: ");
      Serial.println(noAlpha(getCityName()) ? "ID" : "NAME");
      Serial.println("weatherQueryURL = " + weatherQueryURL);
    }

    http.begin(weatherQueryURL.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode == 200) {
      String payload = http.getString();
      JSONVar responseObject = JSON.parse(payload);
      latestWeather.temperature = int(responseObject["main"]["temp"]);
      //      latestWeather.temperature = (weatherFormat) ? ((int(responseObject["main"]["temp"])) * 9. / 5. + 32.) : int(responseObject["main"]["temp"]);
      latestWeather.weatherConditionCode = int(responseObject["weather"][0]["id"]);

    } else {
      //http error
      if (manualSync) {
        display.setCursor(20, 145);
        display.println("Failed to sync");
      }
    }

    http.end();

  } else {
    //No WiFi, use RTC Temperature
    temperature = rtcTemp() - ambientOffset;
    latestWeather.temperature = temperature;
    if (debugger) {
      Serial.println("No WiFi, getting RTC Temp");
      Serial.println("latestWeather.temperature: " + String(latestWeather.temperature));
    }
    weatherConditionCode = 999;
    cityNameID = 999;
    latestWeather.weatherConditionCode = weatherConditionCode;
  }

  if (!runOnce) {
    disableWiFi();
    if (debugger)
      Serial.println("disabling weather wifi");
  }
  return latestWeather;
  manualSync = false;

  if (runOnce) {
    display.display(false);
  } else {
    display.display(true);
  }
}

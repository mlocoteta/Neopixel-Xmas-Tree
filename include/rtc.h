#ifndef __rtc_h__
#define __rtc_h__
#include <Arduino.h>
#include <SparkFun_RV8803.h>
#include <Wire.h>

#include "pins.h"
// Define which alarm registers we want to match, make sure you only enable weekday or date alarm, enabling both will default to a date alarm
// In its current state, an alarm will be generated once an hour, when the MINUTES registers on the time and alarm match. Setting MINUTE_ALARM_ENABLE to false would trigger an alarm every minute
#define MINUTE_ALARM_ENABLE true
#define HOUR_ALARM_ENABLE true
#define WEEKDAY_ALARM_ENABLE false
#define DATE_ALARM_ENABLE false

int rtc_init();
void debugRTCStatus(int status);
bool rtcWakeup();
void clearRTC();

#endif
#include "rtc.h"

#include <EEPROM.h>

// Initialize RTC and check functionality
// -1 means we couldn't detect RTC
// -2 means we we couldn't get the time for some reason
// -3 means we RTC time at some point and require a new compiler time
// Split compiler unix time to 4 bytes
// Store to eeprom to check if we need to update RTC
// If we see the same EEPROM time but our RTC time is less than compiler time
// We know the battery was removed and this is a fault we don't want to repair

RV8803 rtc;  // declare global

int rtc_init() {
    bool compilerTimeAvailable = 0;

    uint8_t ct_B3 = (COMPILER_TIME & 0xff000000UL) >> 24;  // Break 32-bit into 4x 8-bit bytes
    uint8_t ct_B2 = (COMPILER_TIME & 0x00ff0000UL) >> 16;
    uint8_t ct_B1 = (COMPILER_TIME & 0x0000ff00UL) >> 8;
    uint8_t ct_B0 = (COMPILER_TIME & 0x000000ffUL);

    if (EEPROM.read(3) != ct_B3 || EEPROM.read(2) != ct_B2 || EEPROM.read(1) != ct_B1 || EEPROM.read(0) != ct_B0) {  // Write compiler time into EEPROM
        EEPROM.write(3, ct_B3);
        EEPROM.write(2, ct_B2);
        EEPROM.write(1, ct_B1);
        EEPROM.write(0, ct_B0);
        compilerTimeAvailable = 1;
    }

    Wire.begin();

    if (rtc.begin() == false) {  // Initialize RTC. If false we can't communicate.
        return -1;
    }

    if (rtc.updateTime() == false) {  // Check that we can talk to RTC.
        return -2;
    }

    bool rtcIsBehind = COMPILER_TIME > (rtc.getLocalEpoch(1) + 18000);  // True if compiler time is newer than current RTC time. Need to adjust for local time
                                                                        // Clear all flags in case any interrupts have occurred.
    if (rtcIsBehind && compilerTimeAvailable == 1) {                    // Check if we need to update the time. Shouldn't occur unless battery is removed or initial programming
        rtc.setToCompilerTime();
        return 8;
    } else if (rtcIsBehind) {
        return -3;
    }
    return 1;
}

bool rtcWakeup() {
    Wire.begin();

    bool intFlag = rtc.getInterruptFlag(FLAG_ALARM);

    rtc.set24Hour();
    rtc.updateTime();
    rtc.disableAllInterrupts();
    rtc.clearAllInterruptFlags();
    uint8_t setHour;
    uint8_t currentHours = rtc.getHours();
    uint8_t currentMonth = rtc.getMonth();

    if (currentHours >= 21) {
        setHour = currentHours + 1;
    }

    if (setHour == 24) {  // Not sure if rollover is smart enough to handle this
        setHour = 21;
    }
    if (currentMonth == 12 || currentMonth == 1) {
        rtc.setItemsToMatchForAlarm(MINUTE_ALARM_ENABLE, HOUR_ALARM_ENABLE, WEEKDAY_ALARM_ENABLE, DATE_ALARM_ENABLE);
    } else {
        rtc.setItemsToMatchForAlarm(MINUTE_ALARM_ENABLE, HOUR_ALARM_ENABLE, WEEKDAY_ALARM_ENABLE, !DATE_ALARM_ENABLE);
        rtc.setAlarmDate(0);
    }

    rtc.setAlarmMinutes(0);
    rtc.setAlarmHours(setHour);
    rtc.enableHardwareInterrupt(ALARM_INTERRUPT);

    Wire.end();
    return intFlag;
}

void debugRTCStatus(int status) {
#ifdef UNO
    Serial.print("RTC Status is: ");
    Serial.println(status);
    Serial.print("Compiler time: ");
    Serial.println(COMPILER_TIME);
    Serial.println();
    Serial.print("RTC Epoch Time: ");
    Serial.println(rtc.getLocalEpoch(1));
    Serial.println();
    Serial.println("EEPROM: ");
    Serial.println(EEPROM.read(0), HEX);
    Serial.println(EEPROM.read(1), HEX);
    Serial.println(EEPROM.read(2), HEX);
    Serial.println(EEPROM.read(3), HEX);
#endif
}
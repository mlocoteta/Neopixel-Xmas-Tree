#include "led.h"
#include "patterns.h"
#include "pins.h"
#include "rtc.h"
#ifdef __AVR__
#include <avr/power.h>
#include <avr/sleep.h>
#endif

#define adc_disable() (ADCSRA &= ~(1 << ADEN))  // disable ADC (before power-off)
#define adc_enable() (ADCSRA |= (1 << ADEN))    // disable ADC (before power-off)

void supersleep() {
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    // Wire.end();
    adc_disable();  // ADCSRA = 0; Use this if it doesn't work
    // power_all_disable();
    sleep_enable();
    sleep_cpu();
    // sleep_disable();
    // power_all_enable();
    // Wire.begin();
    adc_enable();  // ADCSRA = 0; Use this if it doesn't work

    analogReference(INTERNAL);
    delay(200);
}

void setup() {
#ifdef UNO
    Serial.begin(115200);
    PCICR |= 0x01;
    PCMSK0 |= 0x02;
#endif

#ifdef ATTINY85
    GIMSK |= (1 << PCIE);
    PCMSK |= (1 << PCINT1);
    sei();
#endif
    analogReference(INTERNAL);

    pinMode(LED_ENABLE, OUTPUT);
    int rtc_status = rtc_init();  // Initialize RTC
    led_init();                   // Initialize LEDs
    display_status(rtc_status);   // Print the status out. Green is good, red is bad
    rtcWakeup();
}

void loop() {
    supersleep();
    bool rtcWake = rtcWakeup();
    led_routine(rtcWake);
}
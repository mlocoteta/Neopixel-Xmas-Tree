#include "led.h"
#include "patterns.h"
#include "pins.h"
#include "rtc.h"
#ifdef __AVR__
#include <avr/power.h>
#include <avr/sleep.h>
#endif

#define adc_disable() (ADCSRA &= ~(1 << ADEN))  // disable ADC 
#define adc_enable() (ADCSRA |= (1 << ADEN))    // enable ADC 

void supersleep() {
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    adc_disable();  
    sleep_enable();
    sleep_cpu();
    adc_enable();  
    analogReference(INTERNAL);
    delay(10); // Lets things settle otherwise I2C almost always fails for reading RTC
}

void setup() {
#ifdef UNO
    Serial.begin(115200);
    PCICR |= 0x01;
    PCMSK0 |= 0x02;
#endif

#ifdef ATTINY85
    GIMSK |= (1 << PCIE);       // Setup pin change interrupts
    PCMSK |= (1 << PCINT1);
#endif
    analogReference(INTERNAL);
    pinMode(LED_ENABLE, OUTPUT);
    int rtc_status = rtc_init();  // Initialize RTC
    led_init();                   // Initialize LEDs
    display_status(rtc_status);   // Print the status out. Green is good, red is bad
    rtcWakeup();
}

void loop() {
    supersleep();               // Goto/ exit sleep
    bool rtcWake = rtcWakeup(); // Check if the RTC initialized the wake and setup next RTC wake
    delay(200);                 // Not sure why this is nedeed but led_routine doesn't work right without it
    led_routine(rtcWake);       // Run LEDs
}
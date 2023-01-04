#ifndef __pins_h__
#define __pins_h__

#ifdef ATTINY85

#include <Arduino.h>
#define SDA PB0
#define SCL PB2
#define INT PB1         // For device wakeup/ selection
#define LED_ENABLE PB4  // Enables boost to power LEDs
#define VIN 0         // Also reset function. Needs high voltage programmer. Not clear why PB5 can't be used here?
#define DOUT PB3        // Data output to LEDs
#endif

#ifdef UNO
#include <Arduino.h>
#define SDA A4
#define SCL A5
#define INT 9         // For device wakeup/ selection
#define LED_ENABLE 2  // Enables boost to power LEDs
#define VIN A1        // Also reset function. Needs high voltage programmer
#define DOUT 6        // Data output to LEDs
#endif

#endif
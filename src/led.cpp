#include "led.h"

#include "patterns.h"
bool update = false;
bool interrupted = false;
byte mode;
Adafruit_NeoPixel pixels(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

ISR(PCINT0_vect) {
    mode |= 1UL << 6;
}

void led_init() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    // clock_prescale_set(clock_div_1);
#endif
    pixels.begin();
    pixels.clear();
}

int modeUpdate(int serialError) {
    if (mode >> 6 == 1) {
        mode &= ~(1UL << 6);
        if (!serialError) {
            mode += 1;
        }
        delay(200);
        return 1;
    }
    return 0;
}
void led_routine(bool rtcWake) {
    digitalWrite(LED_ENABLE, HIGH);
    bool onBattery = true;
    int brightness = MAX_BAT;
    bool loopForever = true;
    mode = 1;
    unsigned long runtime = RUNTIME_BAT;
    randomSeed(analogRead(VIN));

    while (loopForever) {  // Higher than 3.5V
        loopForever = !rtcWake;
        if (analogRead(VIN) > 750) {
            brightness = MAX;
            runtime = RUNTIME;
            if (rtcWake) {
                mode = random(1, 4);
            }
        } else {
            mode = random(1, 4);
        }

        for (uint32_t tStart = millis(); (millis() - tStart) < runtime;) {
            if (analogRead(VIN) > 750) {  // If we're above 4V we're not on battery
                onBattery = false;
            } else {
                loopForever = false;
                if (onBattery == false) {  // If supply is low this means we're back on battery, stop running immediately
                    break;
                }
            }

            uint8_t r = 0;
            uint8_t g = 0;
            uint8_t b = 0;

            for (;;) {
                r = brightness * random(0, 255) / 255;  // pgm_read_byte(&gamma8[(random(0, 255))]) / 255;
                g = brightness * random(0, 255) / 255;  // pgm_read_byte(&gamma8[random(0, 255)]) / 255;
                b = brightness * random(0, 255) / 255;  // pgm_read_byte(&gamma8[random(0, 255)]) / 255;
                if (r > 8 || g > 8 || b > 8)
                    break;
            }
            modeUpdate(0);
            switch (mode) {
                    // case 5: {
                    // float dimness = onBattery ? 0.5 : 1;
                    // XmasColors(brightness * dimness, brightness * dimness, 0);
                    // break;
                    // }

                    // case 6: {
                    // allwhite(4, 4, 4);
                    // break;
                    // }

                case 1: {
                    TwinkleRandom(20, 100, false, brightness / 2);
                    break;
                }

                case 2: {
                    SparkleXmas(brightness, brightness, 0, 1);
                    break;
                }
                    // case 5: {
                    //     Sparkle(brightness, 0, 0, 1);
                    //     break;
                    // }
                    // case 6: {
                    //     Sparkle(0, brightness, 0, 1);
                    //     break;
                    // }
                    // case 7: {
                    //     Sparkle(r, g, b, 1);
                    //     break;
                    // }

                case 3: {
                    SnowSparkle(4, 4, 4, 20, random(100, 1000));
                    break;
                }

                    // case 9: {
                    //     RunningLights(r, g, 0, 50);
                    //     break;
                    // }

                    // case 10: {
                    //     colorWipe(0x00, brightness / 2, 0x00, 50);
                    //     colorWipe(0x00, 0x00, 0x00, 50);
                    //     colorWipe(brightness / 2, 0, 0x00, 50);
                    //     colorWipe(0x00, 0x00, 0x00, 50);
                    //     break;
                    // }

                case 4: {
                    int dimness = onBattery ? 8 : 4;
                    rainbowCycle(20, dimness);
                    break;
                }

                    // case 12: {
                    //     byte colors[2][3] = {{brightness, 0x00, 0x00},
                    //                          {0x00, brightness, 0x00}};
                    //     BouncingColoredBalls(2, colors, false);
                    //     byte color2[2][3] = {{0x00, brightness, 0x00},
                    //                          {brightness, 0x00, 0x00}};
                    //     BouncingColoredBalls(2, color2, false);
                    //     break;
                    // }

                default:
                    if (mode > 20) {
                        modeUpdate(1);
                        break;
                    } else {
                        loopForever = false;
                        runtime = 0;
                        break;
                    }
            }
        }
    }
    digitalWrite(LED_ENABLE, LOW);

    return;
}

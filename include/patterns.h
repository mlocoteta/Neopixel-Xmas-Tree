#ifndef __patterns_h__
#define __patterns_h__
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#define ADAFRUIT_NEOPIXEL_H

void RGBLoop(int max);
void showStrip();
void colorWipe(byte red, byte green, byte blue, int SpeedDelay);
void setPixel(int Pixel, byte red, byte green, byte blue);
void setAll(byte red, byte green, byte blue);
void FadeInOut(byte red, byte green, byte blue);
void Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause);
void HalloweenEyes(byte red, byte green, byte blue,
                   int EyeWidth, int EyeSpace,
                   boolean Fade, int Steps, int FadeDelay,
                   int EndPause);
void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay);
void CenterToOutside(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay);
void OutsideToCenter(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay);
void LeftToRight(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay);
void RightToLeft(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay);
void NewKITT(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay);
void Twinkle(byte red, byte green, byte blue, int Count, int SpeedDelay, boolean OnlyOne);
void TwinkleRandom(int Count, int SpeedDelay, boolean OnlyOne, int max);
void Sparkle(byte red, byte green, byte blue, int SpeedDelay);
void SnowSparkle(byte red, byte green, byte blue, int SparkleDelay, int SpeedDelay);
void RunningLights(byte red, byte green, byte blue, int WaveDelay);
byte *Wheel(byte WheelPos);
void rainbowCycle(int SpeedDelay, int brightness, bool onBattery);
void theaterChase(byte red, byte green, byte blue, int SpeedDelay);
void theaterChaseRainbow(int SpeedDelay, int dimness);
void setPixelHeatColor(int Pixel, byte temperature);
void Fire(int Cooling, int Sparking, int SpeedDelay);
void BouncingColoredBalls(int BallCount, byte colors[][3], boolean continuous);
void fadeToBlack(int ledNo, byte fadeValue);
void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay);
void display_status(int status);
void SparkleXmas(byte red, byte green, byte blue, int SpeedDelay);
void XmasColors(byte red, byte green, byte blue);
void allwhite(byte red, byte green, byte blue);
#endif

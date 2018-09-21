/*
 * stepperUnipolar lib: https://github.com/pavelmc/stepperunipolar
 *
 * A custom unipolar motro lib with some unique features, see README.md
 *
**/

#ifndef STEPPERUNIPOLAR_H
#define STEPPERUNIPOLAR_H

#include "Arduino.h"

#define UNIPOLAR_WAVE (0)
#define UNIPOLAR_HALF (1)

// def of the callback function
typedef void (*ptrcallBack)(byte);

class stepperunipolar {
public:
    // constructor, classic version with pins
    stepperunipolar(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

    // alternative, passing the bit in a byte to a callback function
    // usefull when you need to pass the bit info to a shift register
    // params is only the mode
    stepperunipolar(uint8_t);

    // procedures
    void addCallBack(void (*)(uint8_t));
    void steps(int32_t);
    void check();
    bool busy();
    void speed(uint16_t);       // max 1000, no need more in my case
    void off();                 // power off all power consumption
    void stop();                // stop movement and sign it as stale
    uint16_t stepsPerTurn;      // how many steps per turn
    bool eco = false;           // eco mode
    bool ecoDone = false;       // echo is done

private:
    // private procedure
    void setOutput();

    // steps account
    int32_t nowstep = 0;
    uint32_t stepsLeft = 0;
    bool dir = 0;
    uint16_t speedDelay = 0;    // the millis between step
    uint32_t nextMillis = 0;    // next time to check for steps

    // steps
    uint8_t lookup_wave[4] = {B0101, B0110, B1010, B1001};
    uint8_t lookup_half[8] = {B0001, B0101, B0100, B0110, B0010, B1010, B1000, B1001};

    // pins
    uint8_t pins[4];

    // mode
    uint8_t mode = 0;

    // if we use a callback or not
    bool useCallBack = 0;
};

#endif

/*
 * stepperUnipolar lib: https://github.com/pavelmc/stepperunipolar
 *
 * A custom unipolar motro lib with some unique features, see README.md
 *
**/

/*
 * One revolution with a callback
 *
**/

#include "stepperunipolar.h"

// In this case we capture the callback and print it on Serial
// you can pass the value via any means to other device
// where the motor is connected for example a shift register or
// via i2c to a i/o expander or even via wireless with a nrf24L01
//
// only parameter is mode, you can use the pre-defined values
// UNIPOLAR_WAVE
// UNIPOLAR_HALF
stepperunipolar stepper(UNIPOLAR_WAVE);

// how many steps per 1 turn ?
int oneTurn = 200;

/////////////////////////////////////////////////////////////////////////////
void printCallBack(byte data) {
    // this is the function that receive the pins values
    Serial.println(data, BIN);
}

//////////////////////////////////////////////////////////////////////////////
void setup() {
    // for debug debug
    Serial.begin(9600);

    // set speed in steps per one second (1000 Max)
    stepper.speed(100);

    // assign callback, this is a function that accept a byte
    stepper.addCallBack(printCallBack);

    // set steps per turn
    stepper.stepsPerTurn = oneTurn;

    // set eco mode, false to disable
    // check that in stop interval you can move
    // the motor freely if eco is activated
    stepper.eco = true;
}

//////////////////////////////////////////////////////////////////////////////
void loop() {
    // check if the stepper is available to move
    if (!stepper.busy()) {
        // sleep for 1 second
        delay(1000);

        // do
        stepper.steps(oneTurn);

        // debug
        Serial.print("Motor set to move ");
        Serial.print(oneTurn);
        Serial.println(" steps");
    }

    // check async to do the move
    stepper.check();
}

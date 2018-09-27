/*
 * stepperUnipolar lib: https://github.com/pavelmc/stepperunipolar
 *
 * A custom unipolar motro lib with some unique features, see README.md
 *
 * Author: Pavel Milanes <pavelmc@gmail.com> CO7WT
 *
**/

/*
 * One revolution with some options example
 *
**/

#include "stepperunipolar.h"

// Motor pins connected to P06N03 Mosfets gates pulling down motor coils
// if 5V use 1kohms, if 3.3V use ~220 ohms
//
// Parameter is 4 pins and mode, you can use the pre-defined mode values
// UNIPOLAR_WAVE, UNIPOLAR_HALF
stepperunipolar stepper(4, 5, 6, 7, UNIPOLAR_WAVE);

// how many steps per 1 turn ?
int oneTurn = 200;

//////////////////////////////////////////////////////////////////////////////
void setup() {
    // for debug debug
    Serial.begin(9600);

    // set speed in steps per one second (1000 Max)
    stepper.speed(100);

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

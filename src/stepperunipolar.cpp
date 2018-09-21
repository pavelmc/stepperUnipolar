/*
 * stepperUnipolar lib: https://github.com/pavelmc/stepperunipolar
 *
 * A custom unipolar motro lib with some unique features, see README.md
 *
**/

#include "stepperunipolar.h"
#include "Arduino.h"

// def the callback function
static ptrcallBack callBack;

// constructor procedure
stepperunipolar::stepperunipolar(uint8_t _pin1, uint8_t _pin2, uint8_t _pin3, uint8_t _pin4, uint8_t _mode) {
    // assing internal values
    this->pins[0] = _pin1;
    this->pins[1] = _pin2;
    this->pins[2] = _pin3;
    this->pins[3] = _pin4;

    // mode of the lib, 0 is wave, 1 is half step | wave is default
    this->mode = _mode;

    // set pins as output
    for (uint8_t i = 0; i < 4; i++)
        pinMode(this->pins[i], OUTPUT);

    // set that we will not use a callback
    this->useCallBack = false;
};


// alternative constructor, you must pass latter a function that take a
// byte that contains the pin info in the lower nibbles, it takes
// the mode
stepperunipolar::stepperunipolar(uint8_t _mode) {
    // set that we will use a callback
    this->useCallBack = true;

    // mode of the lib, 0 is wave, 1 is half step | wave is default
    this->mode = _mode;
}


// calternative constructor, you must pass a function that take a
// byte that contains the pin info in the lower nibbles
void stepperunipolar::addCallBack(void (*userFunc)(uint8_t)) {
    // set this func
    callBack = userFunc;
}


// set the speed, speed is turns per second
void stepperunipolar::speed(uint16_t s) {
    // calc the usec delay for the desired sped
    this->speedDelay = 1000 / s;
}


// resturn true if the motor is moving and can't move now, false otherwise
bool stepperunipolar::busy() {
    if (this->stepsLeft > 0) {
        // no, I'm busy...
        return true;
    } else {
        // ready to nex steps
        return false;
    }
}


// stop movement at once!
void stepperunipolar::stop() {
    // check if we are busy
    if (this->stepsLeft > 0) {
        // Yes, I'm busy... stop it
        this->stepsLeft = 0;

        // check eco mode
        if (this->eco) {
            // power it off
            this->off();
        }
    }
}


// move a step
void stepperunipolar::steps(int32_t count) {
    // pick direction of movement
    if (count > 0) {this->dir = 1;}
    if (count < 0) {this->dir = 0;}

    // push the steps on the wheel
    this->stepsLeft = abs(count);

    // check to wake from eco mode
    if (this->eco and this->ecoDone) {
        // enable drivers, this will prevent to miss the first step
        this->setOutput();

        // set eco mode no false
        this->ecoDone = false;
    }
};


// power off all outputs
void stepperunipolar::off() {
    // power off all outputs

    // callback or not?
    if (this->useCallBack) {
        // callback will zero
        callBack(0);
    } else {
        // Use pins
        for (uint8_t i = 0; i < 4; i++)
            digitalWrite(pins[i], 0);
    }
}


// move motor to next calculated position
void stepperunipolar::setOutput() {
    // var
    uint8_t index, out;

    // find the index of the lookup table and pick the matching byte
    if (this->mode == 0) {
        // calculate the next step to do
        index = abs(this->nowstep % 4);
        out = this->lookup_wave[index];

    } else {
        // calculate the next step to do
        index = abs(this->nowstep % 8);
        out = this->lookup_half[index];
    }

    // push the byte to the callback or the pins
    if (this->useCallBack) {
        // callback will zero
        callBack(out);
    } else {
        // set it to the pins
        for (uint8_t i = 0; i < 4; i++) {
            digitalWrite(pins[i], bitRead(out, i));
        }
    }
}


// check if steps are left and do the move
void stepperunipolar::check() {
    // check if we need to do a step
    if (this->stepsLeft > 0 and this->nextMillis < millis()) {
        // calc next step
        if (this->dir) {
            // inc steps
            this->nowstep++;
            // check to avoid turn back on movement
            if (this->nowstep > this->stepsPerTurn) {
                this->nowstep = 0;
            }
        } else {
            // dec steps
            this->nowstep--;
            // check to avoid turn back on movement
            if (this->nowstep < 0) {
                this->nowstep = this->stepsPerTurn;
            }
        }

        // move to next steps
        this->setOutput();

        // decrement the left step
        this->stepsLeft--;

        // set next time
        this->nextMillis = millis() + this->speedDelay;
    }

    // if eco is set power off the
    if (this->eco and !this->ecoDone and this->stepsLeft == 0) {
        this->ecoDone = true;
        this->off();
    }
}

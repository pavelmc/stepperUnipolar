#stepperUnipolar#

A non-blocking stepper Library to control unipolar Motors with mosFET as switching devices, also it can use a callback to return the pin values to pass it to a shift register, and has a eco mode.

#Motivation#

I was building a small car and need a small and effective library to control the unipolar stepper motor, also on the development I need to move the output of the lib to a shift register (74HC595) to command the motor from there and some other features.

#Features#

* The lib takes care of the pins mode, no need to initiate itas outputs, etc.
* Is has a eco mode, in my case the motor draw 1.2A once powered and I'm running on batteries, so if your application has a way to stay in place once moved or you don't care of it (like mine) you can activate the eco mode and the motor will draw current _only_ when in motion.
* It moves with a non-blocking procedure, in most libs out there you are stuck when you are moving the motor to a place and you must wait for it to be done (blocking). In my lib you can do another things while the motor are in motion, just need to insert a check procedure in the loop.
* You can retrieve the pin status from the lib instead of doing the real move with a callback function. This feature will help you to remote control a motor, you can transmit that code via any channel (I2C, SPI, wireless) and move the motor at a remote place. In my case I use a SPI bus to control 4 pins of a 74HC595 shift register because I'm short of pins in my project and the SPI bus was already in place.
* Two modes of motion, WAVE or HALF_STEP, you can select the modes in the lib instantiate time.

See the examples for more.

#Author#

Pavel Milanes, amateur radio operator CO7WT, SysAdmin and DIY fan. You can reach me via my email pavelmc@gmail.com

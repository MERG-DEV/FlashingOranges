# FlashingOranges
Arduino Nano v3 project to emulate flashing warning lights on model vehicles.

A colleague at the Basingstoke club is building a model of Ropley in the preservation era (Mid Hants Railway). He has a couple of road vehicles in the loco shed area, a coal loader and a fork lift truck, and said he would like to fit flashing orange lights. I gave him a couple of 3mm orange LEDs for him to whittle down to size to fit the vehicles, and I promised to provide electronics to make them flash.
I could have simply opted for Merg PMP 8 or PMP21 kits, but with Arduino Nano modules available so cheaply from China, I thought I would attempt something a little more sophisticated. I wanted to avoid having the lights on the two vehicles flashing in unison, which would look very un-prototypical. I also wanted to avoid having the lights flashing continuously. I thought it would look better if they flashed for a while, then turned off for a while, again not in unison with each other.
The sketch as written will handle two LEDs, attached to I/O pins 5 amd 6 of the Arduino Nano. However, it could easily be adapted to handle LEDs attached to all the available I/O pins. See comments in the code.
The LEDs are multiplexed, so only one attached LED is alight at any given moment, keeping the maximum current draw low.

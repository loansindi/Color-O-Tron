Color-O-Tron
============
**Check the wiki for the latest hardware news**

A small, inexpensive electronics kit intended for distribution at Maker Faires and other eductational events.

This kit requires no soldering - all electrical connections are made simply by twisting component leads together. This means the kit is readily assembled by anyone old enough to listen to verbal instructions and able to perform the required physical manipulations.

The basic premise is to emulate Simon - the electronic pattern-matching game - in the simplest way possible. This version will have only 3 colors (red, amber and green old-school indicator LEDs) and will use the simplest possible user input (hopefully a piece of bare wire will act as a switch, either by polling ADC inputs or using Atmel's QTouch libray. Experimentation is needed).

![Board Art](https://raw.githubusercontent.com/loansindi/Color-O-Tron/master/laser_cut_board.png)

---

Current tasks

1. Investigate input options
    * ADC as input seems to be functional enough. Use a ~Mohm resistor as a pulldown
2. Design board art
3. Work on game code. Right now it's mostly functional but has some issues - readily visible if you build the hardware

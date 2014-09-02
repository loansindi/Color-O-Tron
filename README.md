Color-O-Tron
============
**Check the wiki for the latest hardware news**

A small, inexpensive electronics kit intended for distribution at Maker Faires and other eductational events.

This kit requires no soldering - all electrical connections are made simply by twisting component leads together. This means the kit is readily assembled by anyone old enough to listen to verbal instructions and able to perform the required physical manipulations.

The basic premise is to emulate Simon - the electronic pattern-matching game - in the simplest way possible. This version will have only 3 colors (red, amber and green old-school indicator LEDs) and will use the simplest possible user input (hopefully a piece of bare wire will act as a switch, either by polling ADC inputs or using Atmel's QTouch libray. Experimentation is needed).

![Board Art](https://raw.githubusercontent.com/loansindi/Color-O-Tron/master/laser_cut_board.png)

---

How can I help?
-----

The final board is probably going to be based on an ATTiny85 - the project will use all available GPIO (including the reset pin). Low-voltage indicator LEDs (rather than modern high-brightness units, driving the LED from the 3v battery is our constraint).

Current tasks

1. Investigate input options
    * ADC as input seems to be functional enough. Use a ~Mohm resistor as a pulldown
2. Design board art
    * There's a rough draft of this right now. Circuit needs finished, layout may need optimization. Add OSHW logo
3. Work on game code. Right now it's mostly functional but has some issues - readily visible if you build the hardware
4. Draw a schematic. This is pretty straightforward - some of the needed info is in the draft of the board art. Basically a 1Mohm pull-down resistor on each of the 3 ADC inputs, Vcc/GND to battery, the other three pins to the LEDs > ground.
5. BOM
    * Need to locate appropriate indicator LEDs available in single-unit quantities
    * Battery/holder can come from ps1_noise
    * whatever I'm forgetting at the moment
6. This project also needs an info card - something that dsecribes 'how to' do the project for someone who wants to recreate it

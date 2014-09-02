// An arduino-based, simple implementation of a pattern-memory game
#include <avr/wdt.h>
// Declare pin assignments
#define ledRed 3
#define ledGreen 5
#define ledBlue 6
// constants to make referring to colors easier
#define red 0
#define green 1
#define blue 2
// Our input pins are pulled down to ground - the plan is to use the player as a switch
#define inputRed A0
#define inputGreen A1
#define inputBlue A2
// pattern will be the sequence the player needs to remember
// This implementation is deterministic, it will generate the same sequence of numbers every time you restart the uC. Normally you would seed the RNG with a floating analog input pin, but my target microcontroller only has 3 ADCs so they'll all be tied to ground.
byte pattern[20];
// i'm not sure if player input needs to be stored as an array - still working on this bit
byte playerInput[20];
// tracking what 'round' the game on is the big problem I'm thinking about at the moment
int roundNum = 0;

void setup()
{
    pinMode(ledRed, OUTPUT);
    pinMode(ledBlue, OUTPUT);
    pinMode(ledGreen, OUTPUT);

    pinMode(inputRed, INPUT);
    pinMode(inputGreen, INPUT);
    pinMode(inputBlue, INPUT);
    // One proposed solution is using the time a player takes to make his choices as a seed - this would require building the array as the game progressed
    pattern[0] = 0;
    // make sure the LED's turning on as expected
    initialize();
}

void loop()
{
    // loop() is likely to stay pretty sparse as most of the logic is happening elsewhere
    play();    
    input();
    delay(500);
    roundNum++;
    pattern[roundNum] = random(3); // add another step to the game pattern

}
void input()
{
    unsigned long time = micros(); // if you aren't familiar, micros() returns the number of microseconds since power on. this is probably the most common way to time things and schedule tasks without using a delay()

    // Here we're waiting for the user to do something, this loop will continue forever until voltage is present on one of our input pins
    for(int i=0; i<=roundNum; i++){
        while(analogRead(inputRed) == 0 && analogRead(inputGreen) == 0 && analogRead(inputBlue) == 0) {
        ;
        }
        // we're using the time it takes for the player to make a choice for our random seed - this function would work just as well if it was only called once per game but this placement is easy enough
        int seedVal = micros() - time;
        randomSeed(seedVal);
        // once the player has toucched a button, we use this loop to figure out which input they're touching - it's set up as a loop to prevent some weird fallthrough errors I was having pretty consistently
        int inputReceived = 0;        
        while(inputReceived == 0) {
            if(analogRead(inputRed)){
                playerInput[i] = red;
                blinkLed(ledRed, 300, 200);
                inputReceived = 1;
            }
            else if(analogRead(inputGreen)){
                playerInput[i] = green;
                blinkLed(ledGreen, 300, 200);
                inputReceived = 1;
            }
            else if(analogRead(inputBlue)){
                playerInput[i] = blue;
                blinkLed(ledBlue, 300, 200);
                inputReceived = 1;
            }
        }
        checkInput(i);
            
        }   
}
void play() {
    // pretty straightforward here - just play back the pattern so far
    int pins[] = {ledRed, ledGreen, ledBlue};
    
    for(int i=0; i<=roundNum; i++) {
        blinkLed(pins[pattern[i]], 300, 300);
    }
}

void blinkLed(int pin, int duration, int interval) {
    // this function takes three arguments - it might be overkill for this simple task but it's a good example of how to generalize functions. We can turn on any LED for an arbitrary amount of time, and also set how long it stays off before the next line of code executes.
    digitalWrite(pin, HIGH);
    delay(duration);
    digitalWrite(pin, LOW);
    delay(interval);
}

void initialize() // some kind of pre-game animation
{
    blinkLed(ledRed, 100, 100);
    blinkLed(ledGreen, 100, 100);
    blinkLed(ledBlue, 100, 500);
}

void lose(){
    // the goal of lose() is to notify the player of a lose condition
    // and to reset game state so they can play again
    // right now it just turns on multiple LEDs and resets everything. Maybe this could be an animation
    digitalWrite(ledGreen, HIGH);
    delay(100);
    digitalWrite(ledBlue, HIGH);
    delay(500); 
    wdt_enable(WDTO_15MS); // initialize a watchdog timer - this will reset the microcontroller after 15 milliseconds
    while(1){} // kill some time so the watchdog timer kicks in
}

void checkInput(int rounds){ // straightfoward - if the array the player builds doesn't match the existing array, they lose.
    for(int i=0; i<=rounds; i++){
        if(playerInput[i] != pattern[i]) {
            lose();
        }
    }
}


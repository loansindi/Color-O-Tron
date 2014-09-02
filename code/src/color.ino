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
//A timeout value for fail condition
#define INPUT_TIMEOUT 1000000  //1 second in micros
// pattern will be the sequence the player needs to remember
byte pattern[20];
// this array tracks player input
byte playerInput[20];
// stores which 'round' of the game we're on - used primarily to set limits for array indexing
int roundNum = 0;

void setup()
{
    pinMode(ledRed, OUTPUT);
    pinMode(ledBlue, OUTPUT);
    pinMode(ledGreen, OUTPUT);

    pinMode(inputRed, INPUT);
    pinMode(inputGreen, INPUT);
    pinMode(inputBlue, INPUT);
    pattern[0] = 0;
    // check LEDs and inform player of game start
    initialize();
}

void loop()
{
    // loop() is likely to stay pretty sparse as most of the logic is happening elsewhere
    play();
    input();
    delay(500); // it's confusing when the next round starts too soon
    roundNum++;
    pattern[roundNum] = random(3); // add another step to the game pattern

}
void input()
{
    unsigned long time = micros(); // if you aren't familiar, micros() returns the number of microseconds since power on. this is probably the most common way to time things and schedule tasks without using a delay()

    // Here we're waiting for the user to do something, this loop will continue forever until voltage is present on one of our input pins
    for(int i=0; i<=roundNum; i++) {
        while(analogRead(inputRed) == 0 && analogRead(inputGreen) == 0 && analogRead(inputBlue) == 0 ) {
            if((micros() - time) > INPUT_TIMEOUT) {  //Timeout waiting for user input
                lose();
            }
        }
        // we're using the time it takes for the player to make a choice for our random seed - this function would work just as well if it was only called once per game but this placement is easy enough
        int seedVal = micros() - time;
        randomSeed(seedVal);
        // once the player has toucched a button, we use this loop to figure out which input they're touching - it's set up as a loop to prevent some weird fallthrough errors I was having pretty consistently
        int inputReceived = 0;
        while(inputReceived == 0) {
            if(analogRead(inputRed)) {
                playerInput[i] = red;
                blinkLed(ledRed, 300, 200);
                inputReceived = 1;
            }
            else if(analogRead(inputGreen)) {
                playerInput[i] = green;
                blinkLed(ledGreen, 300, 200);
                inputReceived = 1;
            }
            else if(analogRead(inputBlue)) {
                playerInput[i] = blue;
                blinkLed(ledBlue, 300, 200);
                inputReceived = 1;
            }
        }
        checkCurrentInput(i);

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

void lose() {
    // the goal of lose() is to notify the player of a lose condition
    // and to reset game state so they can play again
    // right now it just turns on multiple LEDs and resets everything. Maybe this could be an animation
    digitalWrite(ledGreen, HIGH);
    delay(100);
    digitalWrite(ledBlue, HIGH);
    delay(500);
    wdt_enable(WDTO_15MS); // initialize a watchdog timer - this will reset the microcontroller after 15 milliseconds
    while(1) {} // kill some time so the watchdog timer kicks in
}

void checkInput(int rounds) { // straightfoward - if the array the player builds doesn't match the existing array, they lose.
    for(int i=0; i<=rounds; i++) {
        checkCurrInput(i)
    }
}

void checkCurrentInput(int round) { //Function to only check the current round, bit more efficient than checkInput running everytime
    if(playerInput[round] != pattern[i] {
        lose();
    } 
}



/* Set platform as needed.
   Valid options are: 
   * Uno = 1
   * ATTiny84 = 2
*/

#define platform 2


/* #if platform == 1
// Declare pin assignments
    #define ledRed 3
    #define ledGreen 5
    #define ledBlue 6

// Our input pins are pulled down to ground - the plan is to use the player as a switch
    #define inputRed A0
    #define inputGreen A1
    #define inputBlue A2

#elif platform == 2
*/
// Declare output pin assignments
    #define ledRed 9
    #define ledGreen 8
    #define ledBlue 6
// Declare inputs - these will be pulled down to ground with a 1Mohm resistor
    #define inputRed 1
    #define inputGreen 3
    #define inputBlue 5

/* #else 
    #error What the fuuuuuuuuuu
    
#endif
*/
// An arduino-based, simple implementation of a pattern-memory game
#include <avr/wdt.h>

int pins[] = {ledRed, ledGreen, ledBlue};
// constants to make referring to colors easier
#define red 0
#define green 1
#define blue 2
//A timeout value for fail condition
#define INPUT_TIMEOUT 5000000  //1 second in micros
//Definitions to get rid of "magic numbers", will help in tweaking game behavior
#define INPUT_TIME_ON 300
#define INPUT_TIME_OFF 200
#define PLAY_TIME_ON 300
#define PLAY_TIME_OFF 200
#define MAX_ROUNDS 20
// pattern will be the sequence the player needs to remember
byte pattern[MAX_ROUNDS];
// this array tracks player input
byte playerInput[MAX_ROUNDS];
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
    for(int i = 0; i < MAX_ROUNDS; i++) {
        play();
        input();
        delay(500); // it's confusing when the next round starts too soon
        roundNum++;
        pattern[roundNum] = random(3); // add another step to the game pattern
    }
    win();

}
void input()
{
    unsigned long time = micros(); // if you aren't familiar, micros() returns the number of microseconds since power on. this is probably the most common way to time things and schedule tasks without using a delay()

    // Here we're waiting for the user to do something, this loop will continue forever until voltage is present on one of our input pins
    for(int i=0; i<=roundNum; i++) {
        unsigned long roundTime = micros();
        while(analogRead(inputRed) == 0 && analogRead(inputGreen) == 0 && analogRead(inputBlue) == 0 ) {
            if((micros() - roundTime) > INPUT_TIMEOUT) {  //Timeout waiting for user input
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
                blinkLed(ledRed, INPUT_TIME_ON, INPUT_TIME_OFF);
                inputReceived = 1;
            }
            else if(analogRead(inputGreen)) {
                playerInput[i] = green;
                blinkLed(ledGreen, INPUT_TIME_ON, INPUT_TIME_OFF);
                inputReceived = 1;
            }
            else if(analogRead(inputBlue)) {
                playerInput[i] = blue;
                blinkLed(ledBlue, INPUT_TIME_ON, INPUT_TIME_OFF);
                inputReceived = 1;
            }
        }
        checkCurrentInput(i);

    }
}
void play() {
    // pretty straightforward here - just play back the pattern so far

    for(int i=0; i<=roundNum; i++) {
        blinkLed(pins[pattern[i]], PLAY_TIME_ON, PLAY_TIME_OFF);
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
    restart();
}

void win() {
    //Notify player that they beat the game   
    
    for(int i = 0; i < 10; i ++) {
        for (int j = 0; j < 3; j++) {
            blinkLed(pins[j], 100, 50);
        }    
    }
    restart();
}

void restart() {
  wdt_enable(WDTO_15MS); // initialize a watchdog timer - this will reset the microcontroller after 15 milliseconds
  while(1) {} // kill some time so the watchdog timer kicks in
}

void checkAllInputs(int rounds) { // straightfoward - if the array the player builds doesn't match the existing array, they lose.
    for(int i=0; i<=rounds; i++) {
        checkCurrentInput(i);
    }
}

void checkCurrentInput(int round) { //Function to only check the current round, bit more efficient than checkInput running everytime
    if(playerInput[round] != pattern[round]) {
        lose();
    } 
}



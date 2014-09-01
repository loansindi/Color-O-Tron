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
// it's seven steps long at the moment - this will need to be dynamic or large enough to beat most players (or i guess there could be a win condition)
int pattern[7];
// i'm not sure if player input needs to be stored as an array - still working on this bit
int playerInput[7];
// tracking what 'round' the game on is the big problem I'm thinking about at the moment
int roundNum = 0;

void setup()
{
    Serial.begin(9600);
    pinMode(ledRed, OUTPUT);
    pinMode(ledBlue, OUTPUT);
    pinMode(ledGreen, OUTPUT);

    pinMode(inputRed, INPUT);
    pinMode(inputGreen, INPUT);
    pinMode(inputBlue, INPUT);
    // This implementation is deterministic, it will generate the same sequence of numbers every time you restart the uC. Normally you would seed the RNG with a floating analog input pin, but my target microcontroller only has 3 ADCs so they'll all be tied to ground.
    // One proposed solution is using the time a player takes to make his choices as a seed - this would require building the array as the game progressed
    for(int i=0; i<=6; i++) {
       pattern[i] = random(3);
       Serial.println(pattern[i]);
  }
    // make sure the LED's turning on as expected
    // initialize();
}

void loop()
{
    // loop() is likely to stay pretty sparse as most of the logic is happening elsewhere
    play();    
    input();
    roundNum++;

}
void input()
{
    // Here we're waiting for the user to do something, this loop will continue forever until voltage is present on one of our input pins
    for(int i=0; i<=roundNum; i++){
        while(analogRead(inputRed) == 0 && analogRead(inputGreen) == 0 && analogRead(inputBlue) == 0) {
        ;
        }
        if(analogRead(inputRed)){
            playerInput[i] = red;
            blinkLed(red, 200, 100);
        }
        else if(analogRead(inputGreen)){
            playerInput[i] = green;
            blinkLed(green, 200, 100);
        }
        else{
            playerInput[i] = blue;
            blinkLed(blue, 200, 100);
        }
        Serial.println(playerInput[i]);
        checkInput(i);
            
        }   
}
void play() {
    int pins[] = {ledRed, ledGreen, ledBlue};
    
    for(int i=0; i<=roundNum; i++) {
        blinkLed(pins[pattern[i]], 300, 300);
    }
}

void blinkLed(int pin, int duration, int interval) {
    digitalWrite(pin, HIGH);
    delay(duration);
    digitalWrite(pin, LOW);
    delay(interval);
}

void initialize() // some kind of pre-game animation
{
    blinkLed(ledRed, 100, 0);
    blinkLed(ledGreen, 100, 0);
    blinkLed(ledBlue, 100, 0);
}

void lose(){
    // the goal of lose() is to notify the player of a lose condition
    // and to reset game state so they can play again
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledBlue, HIGH);
    delay(500); // right now this is only lighting the red LED - look into
    wdt_enable(WDTO_15MS); // initialize a watchdog timer - this will reset the microcontroller after 15 seconds
    while(1){} // kill some time so the watchdog timer kicks in
}

void checkInput(int rounds){ // straightfoward - if the array the player builds doesn't match the existing array, they lose.
    for(int i=0; i<=rounds; i++){
        if(playerInput[i] != pattern[i]) {
            lose();
        }
    }
}

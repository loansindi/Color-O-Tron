// An arduino-based, simple implementation of a pattern-memory game
#include <avr/wdt.h>
// Declare output pin assignments
#define LEDRED 9
#define LEDGREEN 8
#define LEDBLUE 6

// Declare inputs - these will be pulled down to ground with a 1Mohm resistor
#define INPUTRED 1
#define INPUTGREEN 3
#define INPUTBLUE 5

#define randomInput 4

#define SENSITIVITY 10 // 10 is a totally abritrary number but it works

// Using a struct allows to have a single object with 
// corresponding leds and inputs

struct pins_s {
  byte led;
  byte input;
};

pins_s pins[] = {{LEDRED, INPUTRED},
                 {LEDGREEN, INPUTGREEN},
                 {LEDBLUE, INPUTBLUE}};
                 
// So now we can access pin[number].led and pin[number].input for respectively the 'number' led and its input

//A timeout value for fail condition
#define INPUT_TIMEOUT 1000  //1 second in millis
//Definitions to get rid of "magic numbers", will help in tweaking game behavior
#define INPUT_TIME_ON 300
#define INPUT_TIME_OFF 200
#define PLAY_TIME_ON 300
#define PLAY_TIME_OFF 200
#define MAX_ROUNDS 10
// pattern will be the sequence the player needs to remember
byte pattern[MAX_ROUNDS];
// this array tracks player input
byte playerInput[MAX_ROUNDS];
// stores which 'round' of the game we're on - used primarily to set limits for array indexing
signed short int roundNum = 0;

void setup()
{
  randomSeed(analogRead(randomInput));
  
  for (byte i=0; i<3;i++) {
    pinMode(pins[i].led, OUTPUT); // Init leds
    pinMode(pins[i].input, INPUT);  // Init inputs
  }
    
  // check LEDs and inform player of game start
  initialize();
}

void loop()
{
  // loop() is likely to stay pretty sparse as most of the logic is happening elsewhere
  while(roundNum < MAX_ROUNDS) {
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
  unsigned long time = millis(); // if you aren't familiar, millis() returns the number of milliseconds since power on. this is probably the most common way to time things and schedule tasks without using a delay()

  // Here we're waiting for the user to do something, this loop will continue forever until voltage is present on one of our input pins
  for(int i=0; i<=roundNum; i++) {
    // we're using the time it takes for the player to make a choice for our random seed - this function would work just as well if it was only called once per game but this placement is easy enough
    int seedVal = millis() - time;
    randomSeed(seedVal);
    // once the player has touched a button, we use this loop to figure out which input they're touching - it's set up as a loop to prevent some weird fallthrough errors I was having pretty consistently
    byte inputReceived = 0;
    while(inputReceived <5) { // We are waiting for a real touch
      for (byte couple=0; couple<3; couple++) { // We loop over each input
        inputReceived = 0;
        if(analogRead(pins[couple].input)>SENSITIVITY) { // 10 is a totally abritrary number
          for (byte wt=0; wt < 10; wt++) {
            if (analogRead(pins[couple].input)>SENSITIVITY) {
              inputReceived++;
            }
          }
          if (inputReceived >= 5) {
            playerInput[i] = couple;
            blinkLed(pins[couple].led, INPUT_TIME_ON, INPUT_TIME_OFF);
            break; // If we have a real touch, lets break the loop and check if the combination is right
          }   
        }
      }
    }
    checkCurrentInput(i);
  }
}

void play() {
  // pretty straightforward here - just play back the pattern so far

  for(byte i=0; i<=roundNum; i++) {
    blinkLed(pins[pattern[i]].led, PLAY_TIME_ON, PLAY_TIME_OFF);
  }
}

void blinkLed(byte pin, int duration, int interval) {
  // this function takes three arguments - it might be overkill for this simple task but it's a good example of how to generalize functions. We can turn on any LED for an arbitrary amount of time, and also set how long it stays off before the next line of code executes.
  digitalWrite(pin, HIGH);
  delay(duration);
  digitalWrite(pin, LOW);
  delay(interval);
}

void initialize() // some kind of pre-game animation and reset arrays
{
  blinkLed(pins[0].led, 100, 100);
  blinkLed(pins[1].led, 100, 100);
  blinkLed(pins[2].led, 100, 500);

  for (byte i=0; i<=MAX_ROUNDS; i++) {
    pattern[i] = 0;
    playerInput[i] = 0;
  }
  
  roundNum = 0; // if you set roundNum to 0, you end up on round 2 of the game, because the next 
                 // step in loop() increments roundNum. 
}

void lose() {
  // the goal of lose() is to notify the player of a lose condition
  // and to reset game state so they can play again
  // right now it just turns on multiple LEDs and resets everything. Maybe this could be an animation
  for (byte i=0;i<10;i++) {
    blinkLed(pins[1].led, 50, 50);
    blinkLed(pins[0].led, 50, 50);
    blinkLed(pins[2].led, 50, 50);
  }
  initialize();
  roundNum = -1; // We are still in the input loop so it will be incremented next time
}

void win() {
  //Notify player that they beat the game   
  for(byte i = 0; i < 10; i ++) {
    for (byte j = 0; j < 3; j++) {
      blinkLed(pins[j].led, 100, 50);
    }    
  }
  initialize();
}

void checkAllInputs(byte rounds) { // straightfoward - if the array the player builds doesn't match the existing array, they lose.
  for(byte i=0; i<=rounds; i++) {
    checkCurrentInput(i);
  }
}

void checkCurrentInput(byte round) { //Function to only check the current round, bit more efficient than checkInput running everytime
  if(playerInput[round] != pattern[round]) {
     lose();
  } 
}

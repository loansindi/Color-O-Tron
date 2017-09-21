// An arduino-based, simple implementation of a pattern-memory game
#include <avr/wdt.h>
// Declare output pin assignments
#define ledRed 9
#define ledGreen 8
#define ledBlue 6
// Declare inputs - these will be pulled down to ground with a 1Mohm resistor
#define inputRed 1
#define inputGreen 3
#define inputBlue 5

#define randomInput 4


int pins[] = {ledRed, ledGreen, ledBlue};
// constants to make referring to colors easier
#define red 0
#define green 1
#define blue 2
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
int roundNum = 0;

void setup()
{
  randomSeed(analogRead(randomInput));
  pinMode(ledRed, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(ledGreen, OUTPUT);

  pinMode(inputRed, INPUT);
  pinMode(inputGreen, INPUT);
  pinMode(inputBlue, INPUT);
  for (int i=0; i<=MAX_ROUNDS; i++) {
    pattern[i] = 0;
    playerInput[i] = 0;
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
  unsigned long time = millis(); // if you aren't familiar, millis() returns the number of milliseconds since power on. 
                         //this is probably the most common way to time things and schedule tasks without using a delay()

  // Here we're waiting for the user to do something, this loop will continue forever until voltage is 
  //present on one of our input pins
  for(int i=0; i<=roundNum; i++) {
    unsigned long roundTime = millis();

    // we're using the time it takes for the player to make a choice for our random seed
    // this function would work just as well if it was only called once per game but this placement is easy enough
    int seedVal = millis() - time;
    randomSeed(seedVal);
    // once the player has touched a button, we use this loop to figure out which input they're touching
    // it's set up as a loop to prevent some weird fallthrough errors I was having pretty consistently
    int inputReceived = 0;
    while(inputReceived <5) {
      inputReceived = 0;
      if(analogRead(inputRed)>10) {
        for (int i=0; i < 10; i++) {
          if (analogRead(inputRed)>10) {
            inputReceived++;
          }
        }
        if (inputReceived >= 5) {
          playerInput[i] = red;
          blinkLed(ledRed, INPUT_TIME_ON, INPUT_TIME_OFF);
        }
         
      }
      else if(analogRead(inputGreen) > 10) {
        for (int i=0; i < 10; i++) {
          if (analogRead(inputGreen)>10) {
            inputReceived++;
          }
        }
        if (inputReceived >= 5) {
          playerInput[i] = green;
          blinkLed(ledGreen, INPUT_TIME_ON, INPUT_TIME_OFF);
        }     
      }
      else if(analogRead(inputBlue) > 10) {
        for (int i=0; i < 10; i++) {
          if (analogRead(inputBlue)>10) {
            inputReceived++;
          }
        }
        if (inputReceived >= 5) {
          playerInput[i] = blue;
          blinkLed(ledBlue, INPUT_TIME_ON, INPUT_TIME_OFF);
        }   
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
  // this function takes three arguments - it might be overkill for this simple task but 
  // it's a good example of how to generalize functions. We can turn on any LED for an arbitrary 
  // amount of time, and also set how long it stays off before the next line of code executes.
  digitalWrite(pin, HIGH);
  delay(duration);
  digitalWrite(pin, LOW);
  delay(interval);
}

void initialize() // some kind of pre-game animation
{
  blinkLed(ledGreen, 100, 100);
  blinkLed(ledRed, 100, 100);
  blinkLed(ledBlue, 100, 500);
}

void lose() {
  // the goal of lose() is to notify the player of a lose condition
  // and to reset game state so they can play again
  // right now it just turns on multiple LEDs and resets everything. Maybe this could be an animation
  for (int i=0;i<10;i++) {
    blinkLed(ledGreen, 50, 50);
    blinkLed(ledRed, 50, 50);
    blinkLed(ledBlue, 50, 50);
  }
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

void restart() { // we have a state machine thing going on here, reset things so we can start over
  for(int i = 0; i <= roundNum; i++) {
    pattern[i] = 0;
    playerInput[i] = 0;
  }
  roundNum = -1; // if you set roundNum to 0, you end up on round 2 of the game, because the next 
  initialize();  // step in loop() increments roundNum. 
}

void checkAllInputs(int rounds) { // straightfoward - if the array the player builds doesn't
                                  // match the existing array, they lose.
  for(int i=0; i<=rounds; i++) {
    checkCurrentInput(i);
  }
}

void checkCurrentInput(int round) { // Function to only check the current round, 
                                    // bit more efficient than checkInput running everytime
  if(playerInput[round] != pattern[round]) {
     lose();
  } 
}




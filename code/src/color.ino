// An arduino-based, simple implementation of a pattern-memory game

// Declare pin assignments
#define ledRed 3
#define ledGreen 5
#define ledBlue 6
// Our input pins are pulled down to ground - the plan is to use the player as a switch
#define inputRed A0
#define inputGreen A1
#define inputBlue A2
// pattern will be the sequence the player needs to remember
int pattern[7];
// i'm not sure if player input needs to be stored as an array - still working on this bit
int playerInput[7];
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
    // This implementation is deterministic, it will generate the same sequence of numbers every time you restart the uC. Normally you would seed the RNG with a floating analog input pin, but my target microcontroller only has 3 ADCs so they'll all be tied to ground.
    // One proposed solution is using the time a player takes to make his choices as a seed - this would require building the array as the game progressed
    for(int i=0; i<=6; i++) {
       pattern[i] = random(3);
  }
    // make sure the LED's turning on as expected
    initialize();
    delay(500);
}

void loop()
{
    // loop() is likely to stay pretty sparse as most of the logic is happening elsewhere
    play();    
    input();

}

void input()
{
    // Here we're waiting for the user to do something, this loop will continue forever until voltage is present on one of our input pins
    while(analogRead(inputRed) == 0 && analogRead(inputGreen) == 0 && analogRead(inputBlue) == 0) {
        ;
    }   
    // at this point we'll need to figure out which pin was activated - i'm hoping an if/else/then statement will be sufficient - with only three inputs it should be quick enough to catch the input before the finger goes away. This will get stored - probably as an array so we can compare the two 
}
void play() {
    int pins[] = {ledRed, ledGreen, ledBlue};
    
    for(int i=0; i<=roundNum; i++) {
        blinkLed(pins[pattern[i]]);
    }
}

void blinkLed(int pin) {
    digitalWrite(pin, HIGH);
    delay(200);
    digitalWrite(pin, LOW);
    delay(200);
}

void initialize()
{
    blinkLed(ledRed);
    blinkLed(ledGreen);
    blinkLed(ledBlue);
}


//Copyright 2022 Radoslaw Jaros

// cppcheck-suppress missingIncludeSystem
#include <Arduino.h>

const int analogReadShortEventPin = 1;
const int digitalSignalPinToRelayPin = 0;
const int ledPin = LED_BUILTIN;
const int analogReadThreshold = 700; //max 1023 = 5V
const int analogReadDivider = 100;

const unsigned long hikvisionPowerLossDelay = 3000; //README.md 1)
const unsigned long acceptNextCommandTime = 8000;
const unsigned long shortRelayTime = 500; // time range for Nice Roubus600 0.4-0.8s
const unsigned long debounceDelay = 500;  // the debounce time; increase if the output flickers
const unsigned long blinkRelayTriggered = 250;
const unsigned long blinkErrorToFastTime = 600;
const unsigned long initialBlinkTime = 100;


boolean buttonState = false;     // the current reading from the input pin
boolean lastButtonState = false; // the previous reading from the input pin

unsigned long lastCommandTime = 0;  // the last time the relay was triggered
unsigned long lastDebounceTime = 0; // the last time the output pin was toggled

/**
* Used for blinking built-in LED
* n - number of blinking
* delayTime - wait time after each ledPin change state
*/
void blink(int n, unsigned long delayTime) {
  for (int i = 0; i < n; i++) {
    digitalWrite(ledPin, HIGH);
    delay(delayTime);
    digitalWrite(ledPin, LOW);
    delay(delayTime);
  }
}

// cppcheck-suppress unusedFunction
void setup() {
  //initialize pins
  pinMode(ledPin, OUTPUT);
  pinMode(digitalSignalPinToRelayPin, OUTPUT);
  digitalWrite(digitalSignalPinToRelayPin, HIGH);

  // power loss (of everything)
  delay(hikvisionPowerLossDelay);
  blink(3, initialBlinkTime);
}

// cppcheck-suppress unusedFunction
void loop() {
  int analogReadShortEventPinValue = analogRead(analogReadShortEventPin);

  boolean reading = analogReadShortEventPinValue > analogReadThreshold;

  unsigned long currentTime = millis();

  // if the switch changed, due to noise or pressing
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = currentTime;
  }

  if ((currentTime - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state

    // if the button state has changed
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == true) {
        // Do not accept next command until acceptNextCommandTime
        if((currentTime - lastCommandTime) > acceptNextCommandTime) {
          // turn the NO on Relay to short Nice Robus600
          digitalWrite(digitalSignalPinToRelayPin, LOW);
          //wait
          delay(shortRelayTime);
          // turn the NC on Relay
          digitalWrite(digitalSignalPinToRelayPin, HIGH);
          // blink n-times depends on analogRead value and divided by 100
          blink(analogReadShortEventPinValue / analogReadDivider, blinkRelayTriggered);
          lastCommandTime = currentTime;
        } else {
          //Not enough time between commands
          blink(1, blinkErrorToFastTime);
        }
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState
  lastButtonState = reading;
}

#include "Button.h"
#include <Arduino.h>
#include <cstdint>

Button::Button(uint8_t bttnPin, unsigned long debounceThreshold, bool initToggleState)
:  bttnPin(bttnPin), debounceThreshold(debounceThreshold), toggleState(initToggleState), lastDebounceTime(0), lastButtonState(HIGH)
{}

Button::Button(uint8_t bttnPin, unsigned long debounceThreshold)
:  bttnPin(bttnPin), debounceThreshold(debounceThreshold), toggleState(false), lastDebounceTime(0), lastButtonState(HIGH)
{}

void Button::tick() {
  bool reading = digitalRead(bttnPin);

  if ((millis() - lastDebounceTime) > debounceThreshold) {
    if (reading == LOW && lastButtonState == HIGH) {
      toggleState = !toggleState;
      pressEvent = true;
      lastDebounceTime = millis();
    }
  } else {
    pressEvent = false;
  }
  lastButtonState = reading;
}

bool Button::getToggleState() {
  return toggleState;
}

void Button::setToggleState(bool val) {
  toggleState = val;
}

bool Button::getPressState() {
  return pressEvent;
}
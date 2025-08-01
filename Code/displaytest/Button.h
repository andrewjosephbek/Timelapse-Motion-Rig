#ifndef BUTTON
#define BUTTON

#include <cstdint>

class Button {

public:
  Button(uint8_t bttnPin, unsigned long debounceThreshold, bool initToggleState);
  void tick();
  bool getToggleState();
  void setToggleState(bool val);
  bool getPressState();

private:
  uint8_t bttnPin;
  bool lastButtonState;
  unsigned long lastDebounceTime;
  unsigned long debounceThreshold;
  bool toggleState;
  bool pressEvent;

};

#endif
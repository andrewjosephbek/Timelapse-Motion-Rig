#ifndef BUTTON
#define BUTTON

#include <cstdint>

class Button {

public:
  Button(uint8_t bttnPin, unsigned long debounceThreshold, bool initToggleState);
  Button(uint8_t bttnPin, unsigned long debounceThreshold);

  void tick();
  inline bool getToggleState() { return toggleState; }
  inline void setToggleState(bool val) { toggleState = val; }
  inline bool getPressState() { return pressEvent; }

private:
  uint8_t bttnPin;
  bool lastButtonState;
  unsigned long lastDebounceTime;
  unsigned long debounceThreshold;
  bool toggleState;
  bool pressEvent;

};

#endif
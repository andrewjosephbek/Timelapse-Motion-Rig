
bool handleToggleButton(uint8_t pin, bool &toggle) {
  bool reading = digitalRead(pin);

  if ((millis() - lastDebounceTime) > BUTTON_DEBOUNCE_MILLIS) {
    if (reading == LOW && lastButtonState == HIGH) {
      toggle = !toggle;
      lastDebounceTime = millis();
    }
  }
  lastButtonState = reading;
}

int proccessEncoderPosition(unsigned int rawEncoderPos) {
  static unsigned int prevEncoderPos = ::rawEncoderPos >> 2;

  unsigned int encoderPos = rawEncoderPos >> 2;
  encoderChange = encoderPos - prevEncoderPos;
  prevEncoderPos = encoderPos;

return encoderChange;

}

void IRAM_ATTR handleEncoder() {
  static uint8_t lastState = 0;
  uint8_t currentState = (digitalRead(ENCODER_CLK) << 1) | digitalRead(ENCODER_DT);

  // Form a 4 bit value: prev_state << 2 | curr_state
  uint8_t stateTransition = (lastState << 2) | currentState;

  // Use a lookup table to determine direction
  // Valid transitions (CW or CCW)
  switch (stateTransition) {
    case 0b0001:
    case 0b0111:
    case 0b1110:
    case 0b1000:
      rawEncoderPos++;
      break;

    case 0b0010:
    case 0b0100:
    case 0b1101:
    case 0b1011:
      rawEncoderPos--;
      break;

    default:
      // ignore invalid transitions (could be bounce)
      break;
  }

  lastState = currentState;
}
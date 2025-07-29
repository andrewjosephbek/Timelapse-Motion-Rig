
void drawUI(int encoderChange) {

  static unsigned int uiState = 1 << 31;

  // draw suptitle
  u8g2.setFont(u8g2_font_tinytim_tf);
  u8g2.drawStr(0, 5, "CONFIGURE TIMELAPSE");
  u8g2.drawLine(0, 6, 125, 6);

  char buffer[5];

  if(uiScroll) {
    uiState += encoderChange;
  }

  for (int i = 0; i < N_DISPLAY_ELEMENTS; i++) {
    int j = ((uiState - i) % N_DISPLAY_ELEMENTS + N_DISPLAY_ELEMENTS) % N_DISPLAY_ELEMENTS;
    int y_pos = menuLineYPos[i];

    if(!uiScroll && i == 1)
      *timelapseParams[j] += 10 * encoderChange;

    // draw line titles
    u8g2.setFont(menuLineFonts[i]);
    u8g2.drawStr(TITLE_X_POS, y_pos, menuLinesTitles[j]);

    // draw data vals
    snprintf(buffer, sizeof(buffer), "%d", *timelapseParams[j]);
    u8g2.drawStr(DATA_X_POS, y_pos, buffer);
  }
}



void handleButton() {
  bool reading = digitalRead(BUTTON_PIN);

  if ((millis() - lastDebounceTime) > BUTTON_DEBOUNCE_MILLIS) {
    if (reading == LOW && lastButtonState == HIGH) {
      uiScroll = !uiScroll;
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
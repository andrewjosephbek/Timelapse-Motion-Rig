
void drawTimelapseUI(int encoderChange) {

  static unsigned int uiState = 1 << 31;

  // draw suptitle
  u8g2.setFont(u8g2_font_tinytim_tf);
  u8g2.drawStr(0, 5, "CONFIGURE TIMELAPSE");
  u8g2.drawLine(0, 6, 125, 6);

  char buffer[5];

  if(uiScroll)
    uiState += encoderChange;

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
#include "globals.h"
#include "ListInterface.h"
#include "Button.h"
#include <U8g2lib.h>

void setup() {
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(BACK_PIN, INPUT);
  pinMode(CONFIRM_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), handleEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_DT), handleEncoder, CHANGE);

  // Start up the display
  u8g2.begin();

}

void loop() {

  // Copy the absolute encoder position
  noInterrupts();
  safeEncoderPos = rawEncoderPos;
  interrupts();

  // Find the change in encoder position
  encChange = proccessEncoderPosition(safeEncoderPos);

  // Update button states
  EncButton.tick();
  ConfirmButton.tick();
  BackButton.tick();

  // UI logic
  switch(menuState) {
    case MAIN:
      MainMenu.drawList(encChange, true);

      if(ConfirmButton.getPressState() == false && EncButton.getPressState() == false)
        break;

      EncButton.setToggleState(true);

      if(MainMenu.getListIndex() == 0) 
        menuState = TIMELAPSE;
      else if(MainMenu.getListIndex() == 1)
        menuState = AXES;
      break;

    case TIMELAPSE:
      TimelapseMenu.drawList(encChange, EncButton.getToggleState());

      if(BackButton.getPressState() == true)
        menuState = MAIN;
      break;

    case AXES:
      AxesMovementMenu.drawList(encChange, EncButton.getToggleState());

      if(BackButton.getPressState() == true)
        menuState = MAIN;
      break;

    default:
      menuState = MAIN;
  }
}
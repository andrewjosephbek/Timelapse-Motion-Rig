#include "globals.h"
#include "ListInterface.h"
#include "Button.h"
#include <U8g2lib.h>

void setup() {
  u8g2.begin();

  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(BACK_PIN, INPUT);
  pinMode(CONFIRM_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), handleEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_DT), handleEncoder, CHANGE);
}

void loop() {

  noInterrupts();
  safeEncoderPos = rawEncoderPos;
  interrupts();

  encoderChange = proccessEncoderPosition(safeEncoderPos);

  EncoderButton.tick();
  ConfirmButton.tick();
  BackButton.tick();

  switch(menuState) {
    case MAIN:
      MainMenu.drawList(encoderChange, true);

      if(ConfirmButton.getPressState() == false && EncoderButton.getPressState() == false)
        break;

      EncoderButton.setToggleState(true);

      if(MainMenu.getListIndex() == 0) 
        menuState = TIMELAPSE;
      else if(MainMenu.getListIndex() == 1)
        menuState = AXES;
      break;

    case TIMELAPSE:
      TimelapseMenu.drawList(encoderChange, EncoderButton.getToggleState());

      if(BackButton.getPressState() == true)
        menuState = MAIN;
      break;
    case AXES:
      AxesMovementMenu.drawList(encoderChange, EncoderButton.getToggleState());

      if(BackButton.getPressState() == true)
        menuState = MAIN;
      break;
    default:
      menuState = MAIN;
  }
}
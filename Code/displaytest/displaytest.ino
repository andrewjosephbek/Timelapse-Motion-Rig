#include <U8g2lib.h>
#include "ListInterface.h"
#include "Button.h"

#define SDA_PIN 21
#define SCL_PIN 22
#define ENCODER_CLK 34
#define ENCODER_DT 35
#define BUTTON_PIN 23
#define BACK_PIN 32
#define CONFIRM_PIN 33

#define BUTTON_DEBOUNCE_MILLIS 50  // milliseconds
#define ENC_DEBOUNCE_MICRO 100
#define N_TL_SCREEN_ITEMS 4
#define N_TL_TOTAL_ITEMS 4
#define N_MM_SCREEN_ITEMS 2
#define N_MM_TOTAL_ITEMS 2
#define N_AM_SCREEN_ITEMS 2
#define N_AM_TOTAL_ITEMS 2

// U8G2 constructor for SH1106 128x64 I2C display
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE, /* clock=*/SCL_PIN, /* data=*/SDA_PIN);

// rotary encoder handling
volatile unsigned int rawEncoderPos = 1 << 31;
volatile unsigned long lastInterruptTime = 0;
int8_t encoderChange = 0;
unsigned int safeEncoderPos = 0;

void IRAM_ATTR handleEncoder();


// UI
enum menus { MAIN, TIMELAPSE, AXES };
menus menuState = MAIN;

// Main Menu (MM)
// Main Menu UI elements
char* MM_supTitle = "MAIN MENU";
char* MM_menuLinesTitles[] = { "Conf Timelapse", "Move Axes"};

OLED_128x64_ListInterface MainMenu(&u8g2, N_MM_TOTAL_ITEMS, N_MM_SCREEN_ITEMS, MM_menuLinesTitles, nullptr, MM_supTitle);

// Timelapse Menu (TL)
// Timelapse UI elements
char* TL_supTitle = "CONFIGURE TIMELAPSE";
char* TL_menuLinesTitles[] = { "Move A1", "Move A2", "Duration", "Shots"};

// dynamic timelapse parameters (TL)
struct TLParams {
  int axis1deg = 120;
  int axis2deg = 90;
  int nShots = 1000;
  int nDurationMins = 300;
};

TLParams defaultTL;
int* TL_dynamicListParams[] = { &defaultTL.axis1deg, &defaultTL.axis2deg, &defaultTL.nDurationMins, &defaultTL.nShots };

OLED_128x64_ListInterface TimelapseMenu(&u8g2, N_TL_TOTAL_ITEMS, N_TL_SCREEN_ITEMS, TL_menuLinesTitles, TL_dynamicListParams, TL_supTitle);

// Axes Movement Menu (AM)
// Axes Movement UI elements
char* AM_supTitle = "MOVE AXES";
char* AM_menuLinesTitles[] = { "Move A1", "Move A2"};
int moveAxis1Deg = 0;
int moveAxis2Deg = 0;

int* AM_dynamicListParams[] = { &moveAxis1Deg, &moveAxis2Deg };

OLED_128x64_ListInterface AxesMovementMenu(&u8g2, N_AM_TOTAL_ITEMS, N_AM_SCREEN_ITEMS, AM_menuLinesTitles, AM_dynamicListParams, AM_supTitle);


Button EncoderButton(BUTTON_PIN, BUTTON_DEBOUNCE_MILLIS, false);
Button BackButton(BACK_PIN, BUTTON_DEBOUNCE_MILLIS);
Button ConfirmButton(CONFIRM_PIN, BUTTON_DEBOUNCE_MILLIS);

void setup() {
  // Initialize display
  u8g2.begin();

  // Encoder setup
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
#include <U8g2lib.h>
#include "ListInterface.h"

#define SDA_PIN 21
#define SCL_PIN 22
#define ENCODER_CLK 34
#define ENCODER_DT 35
#define BUTTON_PIN 23
#define BUTTON_DEBOUNCE_MILLIS 50  // milliseconds
#define ENC_DEBOUNCE_MICRO 100
#define N_DISPLAY_ELEMENTS 4
#define N_LIST_ELEMENTS 4
#define TITLE_X_POS 0
#define DATA_X_POS 90

// U8G2 constructor for SH1106 128x64 I2C display
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE, /* clock=*/SCL_PIN, /* data=*/SDA_PIN);

// rotary encoder handling
volatile unsigned int rawEncoderPos = 1 << 29;
volatile unsigned long lastInterruptTime = 0;
volatile uint8_t encoderPrevState = 0;

unsigned int safePrevEncoderPos = rawEncoderPos / 4;
int encoderChange = 0;

// rotary encoder button handling
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
bool uiScroll = true;


void drawUI();
void IRAM_ATTR handleEncoder();
void handleButton();

// UI element constants
char* title = "CONFIGURE TIMELAPSE";


char* menuLinesTitles[] = {
  "Move A1",
  "Move A2",
  "Duration",
  "Shots",
};

uint8_t menuLineYPos[] = { 19, 36, 49, 62 };

const uint8_t* menuLineFonts[] = {
  u8g2_font_6x10_tf,
  u8g2_font_t0_17_tf,
  u8g2_font_6x10_tf,
  u8g2_font_6x10_tf 
};

// dynamic timelapse parameters
int axis1deg = 120;
int axis2deg = 90;
int nShots = 1000;
int nDurationMins = 300;

int* timelapseParams[] = { &axis1deg, &axis2deg, &nDurationMins, &nShots };

ListInterface TimelapseMenu(
  &u8g2,
  N_LIST_ELEMENTS,
  N_DISPLAY_ELEMENTS,
  menuLineYPos,
  menuLineFonts,
  DATA_X_POS,
  TITLE_X_POS,
  menuLinesTitles,
  timelapseParams,
  title
);



void setup() {
  //Serial.begin(115200);
  // Initialize display
  u8g2.begin();

  // Encoder setup
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);
  pinMode(BUTTON_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), handleEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_DT), handleEncoder, CHANGE);
}

void loop() {
  unsigned int safeEncoderPos = 0;

  // get the position from the rotary encoder
  noInterrupts();
  safeEncoderPos = rawEncoderPos;
  interrupts();

  encoderChange = proccessEncoderPosition(safeEncoderPos);

  //Serial.println(encoderChange);

  handleButton();

  TimelapseMenu.drawList(encoderChange, uiScroll);
  // u8g2.clearBuffer();
  // drawUI(encoderChange);
  // u8g2.sendBuffer();
}
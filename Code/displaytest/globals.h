#ifndef GLOBALS_H
#define GLOBALS_H

#include <U8g2lib.h>
#include "ListInterface.h"
#include "Button.h"

// --- Pin definitions ---
#define SDA_PIN 21
#define SCL_PIN 22
#define ENCODER_CLK 34
#define ENCODER_DT 35
#define BUTTON_PIN 23
#define BACK_PIN 32
#define CONFIRM_PIN 33

// --- Constants ---
#define BUTTON_DEBOUNCE_MILLIS 50  // milliseconds
#define ENC_DEBOUNCE_MICRO 100

#define N_TL_SCREEN_ITEMS 4
#define N_TL_TOTAL_ITEMS 4
#define N_MM_SCREEN_ITEMS 2
#define N_MM_TOTAL_ITEMS 2
#define N_AM_SCREEN_ITEMS 2
#define N_AM_TOTAL_ITEMS 2

// --- Enums ---
enum menus { MAIN, TIMELAPSE, AXES };

// --- Globals ---
extern U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;

extern volatile unsigned int rawEncoderPos;
extern volatile unsigned long lastInterruptTime;
extern int8_t encChange;
extern unsigned int safeEncoderPos;

extern menus menuState;

// Main Menu
extern OLED_128x64_ListInterface MainMenu;

// Timelapse Menu
struct TLParams {
  int axis1deg;
  int axis2deg;
  int nShots;
  int nDurationMins;
};
extern TLParams defaultTL;
extern OLED_128x64_ListInterface TimelapseMenu;

// Axes Menu
extern int moveAxis1Deg;
extern int moveAxis2Deg;
extern OLED_128x64_ListInterface AxesMovementMenu;

// Buttons
extern Button EncButton;
extern Button BackButton;
extern Button ConfirmButton;

// ISR
void IRAM_ATTR handleEncoder();
int8_t proccessEncoderPosition(unsigned int safeEncoderPos);  // Declare if needed elsewhere

#endif
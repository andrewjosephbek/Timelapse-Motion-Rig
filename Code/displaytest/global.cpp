#include "globals.h"

// --- Display ---
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, SCL_PIN, SDA_PIN);

// --- Encoder State ---
volatile unsigned int rawEncoderPos = 1 << 31;
volatile unsigned long lastInterruptTime = 0;
int8_t encChange = 0;
unsigned int safeEncoderPos = 0;

// --- Menu State ---
menus menuState = MAIN;

// --- Main Menu ---
char* MM_supTitle = (char*)"MAIN MENU";
char* MM_menuLinesTitles[] = { (char*)"Conf Timelapse", (char*)"Move Axes" };
OLED_128x64_ListInterface MainMenu(&u8g2, N_MM_TOTAL_ITEMS, N_MM_SCREEN_ITEMS, MM_menuLinesTitles, nullptr, MM_supTitle);

// --- Timelapse Menu ---
char* TL_supTitle = (char*)"CONFIGURE TIMELAPSE";
char* TL_menuLinesTitles[] = { (char*)"Move A1", (char*)"Move A2", (char*)"Duration", (char*)"Shots" };

TLParams defaultTL = { 120, 90, 1000, 300 };
int* TL_dynamicListParams[] = { &defaultTL.axis1deg, &defaultTL.axis2deg, &defaultTL.nDurationMins, &defaultTL.nShots };

OLED_128x64_ListInterface TimelapseMenu(&u8g2, N_TL_TOTAL_ITEMS, N_TL_SCREEN_ITEMS, TL_menuLinesTitles, TL_dynamicListParams, TL_supTitle);

// --- Axes Menu ---
char* AM_supTitle = (char*)"MOVE AXES";
char* AM_menuLinesTitles[] = { (char*)"Move A1", (char*)"Move A2" };
int moveAxis1Deg = 0;
int moveAxis2Deg = 0;

int* AM_dynamicListParams[] = { &moveAxis1Deg, &moveAxis2Deg };

OLED_128x64_ListInterface AxesMovementMenu(&u8g2, N_AM_TOTAL_ITEMS, N_AM_SCREEN_ITEMS, AM_menuLinesTitles, AM_dynamicListParams, AM_supTitle);

// --- Buttons ---
Button EncButton(BUTTON_PIN, BUTTON_DEBOUNCE_MILLIS, false);
Button BackButton(BACK_PIN, BUTTON_DEBOUNCE_MILLIS);
Button ConfirmButton(CONFIRM_PIN, BUTTON_DEBOUNCE_MILLIS);

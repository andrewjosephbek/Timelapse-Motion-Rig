#include <U8g2lib.h>

#define SDA_PIN 21
#define SCL_PIN 22
#define ENCODER_CLK 34
#define ENCODER_DT  35
#define BUTTON_PIN 23
#define DEBOUNCE_DELAY 50  // milliseconds

#define N_DISPLAY_ELEMENTS 4
#define TITLE_X_POS 0
#define DATA_X_POS 90

// U8G2 constructor for SH1106 128x64 I2C display
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/SCL_PIN, /* data=*/SDA_PIN);

// rotary encoder handling
volatile unsigned int encoderPos = 1 << 31;
volatile bool lastClkState = LOW;
volatile unsigned long lastInterruptTime = 0;

// rotary encoder button handling 
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
bool uiScroll = true; 


void drawUI();
void IRAM_ATTR handleEncoder();
void handleButton();

// UI element constants 
const int menuLineYPos[] = {16, 29, 38, 48};

const char * menuLinesTitles[] = {
  "Move A1:",
  "Move A2:",
  "Duration:",
  "Shots:",
};

const uint8_t* menuLineFonts[] = {
  u8g2_font_tiny5_tf,
  u8g2_font_7x13_tr,
  u8g2_font_tiny5_tf,
  u8g2_font_tiny5_tf
};

// dynamic timelapse parameters
int axis1deg = 120;
int axis2deg = 90;
int nShots = 1000;
int nDurationMins = 300;

const int * timelapseParams[] = {&axis1deg, &axis2deg, &nShots, &nDurationMins};




void setup() {
  Serial.begin(115200);
  // Initialize display
  u8g2.begin();

  // Encoder setup
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);
  pinMode(BUTTON_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), handleEncoder, CHANGE);
}

void loop() {
  static int lastPos = 0;

  handleButton();

  //Serial.println(digitalRead(BUTTON_PIN));

  if (encoderPos != lastPos)
    lastPos = encoderPos;

  u8g2.clearBuffer();

  drawUI();

  u8g2.sendBuffer();

}

void drawUI() {

  // draw suptitle
  //Serial.println(encoderPos);
  u8g2.setFont(u8g2_font_tinytim_tf);
  u8g2.drawStr(0, 5, "CONFIGURE TIMELAPSE");
  u8g2.drawLine(0, 6, 120, 6);

  char buffer[5];

  for (int i = 0; i < N_DISPLAY_ELEMENTS; i++) {
    int j = (encoderPos - i + N_DISPLAY_ELEMENTS) % N_DISPLAY_ELEMENTS;
    int y_pos = menuLineYPos[i];

    // draw line titles
    u8g2.setFont(menuLineFonts[i]);
    u8g2.drawStr(TITLE_X_POS, y_pos, menuLinesTitles[j]);

    // draw line params/data-vals
    snprintf(buffer, sizeof(buffer), "%d", *timelapseParams[j]);
    u8g2.drawStr(DATA_X_POS, y_pos, buffer);
  }
}

void handleButton() {
  int reading = digitalRead(BUTTON_PIN);

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading == LOW && lastButtonState == HIGH) {
      uiScroll = !uiScroll;
      lastDebounceTime = millis();
    }
  }
  lastButtonState = reading;
}

void IRAM_ATTR handleEncoder() {
  unsigned long now = micros();
  if (now - lastInterruptTime < 100) return; // debounce
  
  if (!uiScroll) return;

  lastInterruptTime = now;

  bool clkState = digitalRead(ENCODER_CLK);
  bool dtState  = digitalRead(ENCODER_DT);

  if (clkState != lastClkState && clkState == HIGH) {
    if (dtState == clkState) {
      encoderPos++;
    } else {
      encoderPos--; 
    }
  }

  lastClkState = clkState;
}
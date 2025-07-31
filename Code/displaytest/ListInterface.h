// ListInterface.h

#ifndef LISTINTERFACE_H
#define LISTINTERFACE_H

#include <cstdint>
#include <U8g2lib.h>

class ListInterface {

public:
  ListInterface(U8G2_SH1106_128X64_NONAME_F_HW_I2C* u8g2_display,
                uint8_t nListElements, 
                uint8_t nUIElements, 
                uint8_t* lineYPos, 
                const uint8_t** staticLineFonts,
                uint8_t dynamicLineXPos, 
                uint8_t staticLineXPos, 
                char** staticTextLines, 
                int** dynamicParamsLines,
                char* title);

  ~ListInterface();

  void drawList(int8_t scroll, bool uiScroll);

private:
  U8G2_SH1106_128X64_NONAME_F_HW_I2C* u8g2_display;
  uint8_t nListElements;
  uint8_t nUIElements;

  uint8_t* lineYPos;                     // Allocated and copied from const input
  const uint8_t** staticLineFonts; // Shallow copy of const font pointers

  uint8_t dynamicLineXPos;
  uint8_t staticLineXPos;

  char** staticTextLines;               // Deep copy of input lines
  int** dynamicParamsLines;            // Shallow copy (you want to edit the original ints)
  char* title;                          // Deep copy

  unsigned int uiState;
};

#endif

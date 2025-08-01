// ListInterface.h

#ifndef LIST_INTERFACE
#define LIST_INTERFACE

#include <cstdint>
#include <U8g2lib.h>

class OLED_128x64_ListInterface {

public:
  OLED_128x64_ListInterface(U8G2_SH1106_128X64_NONAME_F_HW_I2C* u8g2_display,
                const uint8_t nListElements, 
                const uint8_t nUIElements, 
                char** staticTextLines, 
                int** dynamicParamsLines,
                char* supTitle);

  ~OLED_128x64_ListInterface();

  void drawList(int8_t scroll, bool uiScroll);
  inline uint8_t getListIndex() {
    return listIdx;
  }

private:
  inline int wrappedIndex(int index, int mod) {
    return (index % mod + mod) % mod;
  }
  
  U8G2_SH1106_128X64_NONAME_F_HW_I2C* u8g2_display;
  const uint8_t nListElements;
  const uint8_t nUIElements;

  static const uint8_t lineYPos[4];         
  static const uint8_t* lineFonts[4];

  uint8_t dynamicLineXPos;
  uint8_t staticLineXPos;

  char** staticTextLines;              
  int** dynamicParamsLines;            
  char* supTitle;                
  
  uint8_t listIdx;
  unsigned int uiState;

};

#endif

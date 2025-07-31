#include "ListInterface.h"
#include <cstdint>
#include <cstring>
#include <U8g2lib.h>

ListInterface::ListInterface(U8G2_SH1106_128X64_NONAME_F_HW_I2C* u8g2_display,
                             uint8_t nListElements,
                             uint8_t nUIElements,
                             uint8_t* lineYPos,
                             const uint8_t** staticLineFonts,
                             uint8_t dynamicLineXPos,
                             uint8_t staticLineXPos,
                             char** staticTextLines,
                             int** dynamicParamsLines,
                             char* title)
{
    uiState = 1 << 31;

    this->u8g2_display = u8g2_display;
    this->nListElements = nListElements;
    this->nUIElements = nUIElements;
    this->dynamicLineXPos = dynamicLineXPos;
    this->staticLineXPos = staticLineXPos;

    // Deep copy title
    size_t titleLength = std::strlen(title);
    this->title = new char[titleLength + 1];
    std::strcpy(this->title, title);

    // Deep copy lineYPos
    this->lineYPos = new uint8_t[nUIElements];
    for (uint8_t i = 0; i < nUIElements; ++i) {
        this->lineYPos[i] = lineYPos[i];
    }

    // Shallow copy staticLineFonts (now const)
    this->staticLineFonts = new const uint8_t*[nUIElements];
    for (uint8_t i = 0; i < nUIElements; ++i) {
        this->staticLineFonts[i] = staticLineFonts[i];
    }

    // Deep copy staticTextLines
    this->staticTextLines = new char*[nListElements];
    for (uint8_t i = 0; i < nListElements; ++i) {
        size_t len = std::strlen(staticTextLines[i]);
        this->staticTextLines[i] = new char[len + 1];
        std::strcpy(this->staticTextLines[i], staticTextLines[i]);
    }

    // Shallow copy dynamicParamsLines
    this->dynamicParamsLines = new int*[nListElements];
    for (uint8_t i = 0; i < nListElements; ++i) {
        this->dynamicParamsLines[i] = dynamicParamsLines[i];
    }
}

ListInterface::~ListInterface() {
    delete[] lineYPos;

    delete[] staticLineFonts;  // shallow

    for (uint8_t i = 0; i < nListElements; ++i) {
        delete[] staticTextLines[i];
    }
    delete[] staticTextLines;

    delete[] dynamicParamsLines;  // shallow

    delete[] title;  // deep
}

void ListInterface::drawList(int8_t scroll, bool uiScroll) {
    Serial.println("working");
    u8g2_display->clearBuffer();

    // draw title
    u8g2_display->setFont(u8g2_font_tinytim_tf);
    u8g2_display->drawStr(0, 5, this->title);
    u8g2_display->drawLine(0, 6, 125, 6);

    char buffer[5];

    if (uiScroll)
        uiState += scroll;

    for (int i = 0; i < nUIElements; i++) {
        int j = ((uiState - i) % nUIElements + nUIElements) % nUIElements;
        int y_pos = lineYPos[i];

        if (!uiScroll && i == 1)
            *dynamicParamsLines[j] += 10 * scroll;

        // draw line titles
        u8g2_display->setFont(staticLineFonts[i]);
        u8g2_display->drawStr(staticLineXPos, y_pos, staticTextLines[j]);

        // draw data vals
        snprintf(buffer, sizeof(buffer), "%d", *dynamicParamsLines[j]);
        u8g2_display->drawStr(dynamicLineXPos, y_pos, buffer);
        u8g2_display->sendBuffer();
    }
}

#include "ListInterface.h"
#include <cstdint>
#include <cstring>
#include <U8g2lib.h>

ListInterface::ListInterface(U8G2_SH1106_128X64_NONAME_F_HW_I2C* u8g2_display,
                const uint8_t nListElements, 
                const uint8_t nUIElements, 
                uint8_t* lineYPos, 
                const uint8_t** lineFonts,
                uint8_t dynamicLineXPos, 
                uint8_t staticLineXPos, 
                char** staticTextLines, 
                int** dynamicParamsLines,
                char* supTitle)

:  u8g2_display(u8g2_display), 
    nListElements(nListElements), 
    nUIElements(nUIElements), 
    dynamicLineXPos(dynamicLineXPos), 
    staticLineXPos(staticLineXPos),
    uiState(1 << 31)
{
                 
    // Deep copy title
    size_t titleLength = std::strlen(supTitle);
    this->supTitle = new char[titleLength + 1];
    std::strcpy(this->supTitle, supTitle);

    // Deep copy lineYPos
    this->lineYPos = new uint8_t[nUIElements];
    for (uint8_t i = 0; i < nUIElements; ++i) {
        this->lineYPos[i] = lineYPos[i];
    }

    // Shallow copy lineFonts (now const)
    this->lineFonts = new const uint8_t*[nUIElements];
    for (uint8_t i = 0; i < nUIElements; ++i) {
        this->lineFonts[i] = lineFonts[i];
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

    delete[] lineFonts;  // shallow

    for (uint8_t i = 0; i < nListElements; ++i) {
        delete[] staticTextLines[i];
    }
    delete[] staticTextLines;

    delete[] dynamicParamsLines;  // shallow

    delete[] supTitle;  // deep
}

void ListInterface::drawList(int8_t scrollDir, bool uiScroll) {
    u8g2_display->clearBuffer();

    // draw supTitle
    u8g2_display->setFont(u8g2_font_tinytim_tf);
    u8g2_display->drawStr(0, 5, this->supTitle);
    u8g2_display->drawLine(0, 6, 125, 6);

    char buffer[5];

    if (uiScroll)
        uiState += scrollDir;

    for (int i = 0; i < nUIElements; i++) {
        // list position index
        int j = ((uiState - i) % nListElements + nListElements) % nListElements;

        // y position of screen elements
        int y_pos = lineYPos[i];

        if (!uiScroll && i == 1) {
            // edit list values
            *dynamicParamsLines[j] += 10 * scrollDir;

            // draw editing icon
            u8g2_display->setFont(u8g2_font_m2icon_5_tf);
            u8g2_display->drawStr(120, 5, "N");
        }

        // draw list element titles
        u8g2_display->setFont(lineFonts[i]);
        u8g2_display->drawStr(staticLineXPos, y_pos, staticTextLines[j]);

        // draw list element data vals
        snprintf(buffer, sizeof(buffer), "%d", *dynamicParamsLines[j]);
        u8g2_display->drawStr(dynamicLineXPos, y_pos, buffer);
    }

    u8g2_display->sendBuffer();
}

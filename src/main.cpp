#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <menu.h>
#include <menuIO/u8g2Out.h>
#include <menuIO/serialIO.h>
#include <menuIO/chainStream.h>
#include <menuIO/serialIO.h>
#include "SerialMenu.h"

using namespace Menu;

#define MAX_DEPTH 3

#define DISPLAY_SDA (PIN_WIRE_SDA)
#define DISPLAY_SCL (PIN_WIRE_SCL)

#define fontName u8g2_font_5x7_tf
#define fontX 5
#define fontY 9
#define offsetX 0
#define offsetY 0
#define U8_Width 128
#define U8_Height 64

// define menu colors --------------------------------------------------------
//each color is in the format:
//  {{disabled normal,disabled selected},{enabled normal,enabled selected, enabled editing}}
// this is a monochromatic color table
const colorDef<uint8_t> colors[] MEMMODE = {
    {{0, 0}, {0, 1, 1}}, //bgColor
    {{1, 1}, {1, 0, 0}}, //fgColor
    {{1, 1}, {1, 0, 0}}, //valColor
    {{1, 1}, {1, 0, 0}}, //unitColor
    {{0, 1}, {0, 0, 1}}, //cursorColor
    {{1, 1}, {1, 0, 0}}, //titleColor
};

U8G2_SSD1309_128X64_NONAME0_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, DISPLAY_SCL, DISPLAY_SDA);

// Serial File Listing /////////////////////////////////////
result filePick(eventMask event, navNode &nav, prompt &item);
SerialMenu filePickMenu("Serial File Listing", "/", filePick, enterEvent);

// implementing the handler here after filePick is defined...
result filePick(eventMask event, navNode &nav, prompt &item)
{
  return proceed;
}

MENU(mainMenu, "Serial File Listing", Menu::doNothing, Menu::noEvent, Menu::wrapStyle,
     SUBMENU(filePickMenu),
     OP("Sub2", Menu::doNothing, anyEvent),
     EXIT("<Back\r\n"));

serialIn serial(SerialUSB);
MENU_INPUTS(in, &serial);
MENU_OUTPUTS(out, MAX_DEPTH, U8G2_OUT(u8g2, colors, fontX, fontY, offsetX, offsetY, {0, 0, U8_Width / fontX, U8_Height / fontY}), SERIAL_OUT(SerialUSB));

NAVROOT(nav, mainMenu, MAX_DEPTH, in, out);

void setup() {
  SerialUSB.begin(115200);
  while(!SerialUSB);
  Wire.begin();
  u8g2.begin();
  u8g2.setFont(fontName);
}

void loop() {
  u8g2.firstPage();
  do
    nav.poll();
  while (u8g2.nextPage());
}
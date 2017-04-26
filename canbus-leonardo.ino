#include "HID-Project.h"
#include <SPI.h> 
#include "mcp_can.h"

//  \ 1  2  3  4  5  6  7  8  /
//   \9 10 11 12 13 14 15 16/

//#define CAN_SPEED CAN_33KBPS
//Low Speed CAN 33.3kbps
//LSCAN-H - 1
//LSCAN-L - GND - 5

#define CAN_SPEED CAN_95KBPS
//Medium Speed CAN 95kbps
//MSCAN-H - 3
//MSCAN-L - 11

//#define CAN_SPEED CAN_500KBPS
//High Speed CAN 500kbps
//HSCAN-H - 6
//HSCAN-L - 14

MCP_CAN CAN(10);

INT32U canId = 0x000;
unsigned char len = 0;
unsigned char buf[8];

unsigned char STEERING_WHEEL_NEXT[3] = {0x1, 0x91, 0x0};
unsigned char STEERING_WHEEL_PREVIOUS[3] = {0x1, 0x92, 0x0};
unsigned char STEERING_WHEEL_RELEASE[3] = {0x1, 0x0, 0x0};

unsigned char CD30_NUMBER2[3] = {0x1, 0x32, 0x0}; // 2
unsigned char CD30_NUMBER4[3] = {0x1, 0x34, 0x0}; // 4
unsigned char CD30_NUMBER5[3] = {0x1, 0x35, 0x0}; // 5
unsigned char CD30_NUMBER6[3] = {0x1, 0x36, 0x0}; // 6
unsigned char CD30_NUMBER7[3] = {0x1, 0x37, 0x0}; // 7
unsigned char CD30_NUMBER8[3] = {0x1, 0x38, 0x0}; // 8
unsigned char CD30_NUMBER9[3] = {0x1, 0x39, 0x0}; // 9

void setup() {
  Keyboard.begin();
  Serial.begin(115200);

  START_INIT:

    if (CAN_OK == CAN.begin(CAN_SPEED)) {
      //Serial.println("CAN BUS Shield init ok!>");
    } else {
      delay(100);
      goto START_INIT;
    }
}

void loop() {
  // Check CAN BUS
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf( & len, buf);
    canId = CAN.getCanId();

    // canId 0x206 -> steering wheel buttons
    if (canId == 0x206) {      
      if (buf[0] == 0x1 && buf[1] == 0x91 && buf[2] == 0x0) {
        Keyboard.write(MEDIA_NEXT);
      } else if (buf[0] == 0x1 && buf[1] == 0x92 && buf[2] == 0x0) {
        Keyboard.write(MEDIA_PREVIOUS);
      } else if (buf[0] == 0x1 && buf[1] == 0x0 && buf[2] == 0x0) {
        Keyboard.releaseAll();
      }
    }

    // canId 0x206 -> cd30 buttons
    if (canId == 0x201) {      
      if (buf[0] == 0x1 && buf[1] == 0x36 && buf[2] == 0x0) {
        Keyboard.write(KEY_RIGHT_ARROW);
      } else if (buf[0] == 0x1 && buf[1] == 0x34 && buf[2] == 0x0) {
        Keyboard.write(KEY_LEFT_ARROW);
      } else if (buf[0] == 0x1 && buf[1] == 0x32 && buf[2] == 0x0) {
        Keyboard.write(KEY_UP_ARROW);
      } else if (buf[0] == 0x1 && buf[1] == 0x38 && buf[2] == 0x0) {
        Keyboard.write(KEY_DOWN_ARROW);
      } else if (buf[0] == 0x1 && buf[1] == 0x37 && buf[2] == 0x0) {
        Keyboard.write(KEY_ESC);
      } else if (buf[0] == 0x1 && buf[1] == 0x35 && buf[2] == 0x0) {
        Keyboard.write(KEY_ENTER);
      } else if (buf[0] == 0x1 && buf[1] == 0x39 && buf[2] == 0x0) {
        Keyboard.write(KEY_TAB);
      } else {
        Keyboard.releaseAll();
      }
    
    }
    
  }
 
}

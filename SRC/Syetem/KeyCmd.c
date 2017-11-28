#define  KCMDLABEL
#include "AllHead.h"

typedef struct {//mcu version information data struct
  u8 cMode[12];
  u8 cVersion[4];
  u8 cDate[4];
  u8 cEepVersion[4];//eeprom version
  u8 cRes[8];
}MCU_VERSION;

static const MCU_VERSION Mcu_Version = {//mcu vwersion information
  {"A508"},				//product version
  {0x10,0x10,0x10,0x01},                //V1.01.01.001
  {0x20,0x17,0x11,0x28},                //2017-11-28
  {"res"},			        //4//4byte eeprom version
  {"res"}
};

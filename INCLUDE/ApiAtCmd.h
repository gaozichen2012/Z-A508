#ifndef __APIATCMD_H
#define __APIATCMD_H

#include "AllHead.h"

typedef enum{
  ATCOMM0_OSSYSHWID             = 0x00,
  ATCOMM1_PPPCFG                = 0x01,
}AtCommType;

extern u8 BootProcess_SIMST_Flag;
extern u8 BootProcess_PPPCFG_Flag;
extern u8 BootProcess_OpenPoc_Flag;

extern bool ApiAtCmd_WritCommand(AtCommType id, u8 *buf, u16 len);
extern void ApiCaretCmd_10msRenew(void);
extern void ApiAtCmd_10msRenew(void);
extern u8 ApiAtCmd_GetIccId(u8 **pBuf);
extern void ApiGetIccidBuf(void);
#endif
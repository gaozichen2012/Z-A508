#ifndef __APIATCMD_H
#define __APIATCMD_H

#include "AllHead.h"

typedef enum{
  ATCOMM0_OSSYSHWID             = 0x00,
  ATCOMM1_PPPCFG                = 0x01,
  ATCOMM2_ZTTS_Abell            = 0x02,
  ATCOMM3_GD83Reset             = 0x03,
  ATCOMM4_GD83Mode              = 0x04,
  ATCOMM5_CODECCTL              = 0x05,
  ATCOMM6_CSQ                   = 0x06,
  ATCOMM7_VGR                   = 0x07,
  ATCOMM8_CheckTcp              = 0x08,
  ATCOMM9_SetIp                 = 0x09,
  ATCOMM10_SendTcp              = 0x0A,
  ATCOMM11_ZpppOpen             = 0x0B,
  ATCOMM12_CheckPPP             = 0x0C,
  ATCOMM13_CheckRssi            = 0x0D,
  ATCOMM14_CheckCard            = 0x0E,
}AtCommType;

extern u8 BootProcess_SIMST_Flag;
extern u8 BootProcess_PPPCFG_Flag;
extern u8 BootProcess_PPPCFG_Flag_Zanshi;//¡Ÿ ±¥˙ÃÊPPPCFGºÏ≤‚
extern u8 BootProcess_OpenPoc_Flag;
extern u8 VoiceEnd_Flag;
extern u8 KeyDownUpChoose_GroupOrUser_Flag;
extern u8 CSQ_Flag;
extern bool ApiAtCmd_WritCommand(AtCommType id, u8 *buf, u16 len);
extern bool ApiAtCmd_PlayVoice(AtVoiceType id, u8 *buf, u8 len);
extern void ApiAtCmd_100msRenew(void);
extern void ApiCaretCmd_10msRenew(void);
extern void ApiAtCmd_10msRenew(void);
extern u8 ApiAtCmd_GetIccId(u8 **pBuf);
extern void ApiGetIccidBuf(void);
extern u8 ApiAtCmd_tcp_state(void);
extern u8 ApiAtCmd_Ppp_state(void);
#endif
#ifndef __APIPOCCMD_H
#define __APIPOCCMD_H

#include "AllHead.h"

typedef enum{
  PocComm_OpenPOC               = 0x00,
  PocComm_SetParam		= 0x01,
  PocComm_GetParam		= 0x02,
  PocComm_Login			= 0x03,
  PocComm_Logout		= 0x04,
  PocComm_Cancel		= 0x05,
  PocComm_ModifyPwd		= 0x06,
  PocComm_EnterGroup		= 0x09,
  PocComm_Invite                = 0x0A,
  PocComm_StartPTT		= 0x0B,
  PocComm_EndPTT		= 0x0C,
  PocComm_GroupListInfo	        = 0x0D,
  PocComm_UserListInfo          = 0x0E,
  PocComm_Key			= 0x10
}PocCommType;

//void ApiPocCmd_PowerOnInitial(void);
bool ApiPocCmd_WritCommand(PocCommType id, u8 *buf, u16 len);
void ApiPocCmd_10msRenew(void);
bool ApiAtCmd_GetLoginState(void);
void ListenState(void);
void ApiGetPocBuf(void);
u8 *ApiAtCmd_GetMainWorkName(void);
u8 ApiAtCmd_GetMainWorkNameLen(void);
u8 ApiAtCmd_GetGroupNum(void);
u8 ApiAtCmd_GetMainGroupId(void);
u8 *ApiAtCmd_GetGroupName(u8 n);
u8 ApiAtCmd_GetGroupNameLen(u8 n);
#endif
#include "ALLHead.h"

#define DrvMC8332_GroupName_Len		16			//define UART Tx buffer length value
#define DrvMC8332_UseId_Len			100			//define UART Tx buffer length value
#define APIPOC_UserList_Len			16
#define APIPOC_UserLoad_Len			8
#define APIPOC_UserName_Len			20

const u8 *ucAtPocHead   = "AT+POC=";
const u8 *ucTingEnd   = "0B0000";
const u8 *ucTingStart   = "0B0001";

typedef struct{
	struct{
		union{
			struct{
				u16 bUserInfo	: 3;
				u16 bUserWrite	: 1;
				u16 bPocReset	: 1;
				u16 bPocOpen		: 1;
				u16 bModeChange	: 1;
				u16	 bMode			: 3;
				u16 bNetStat		: 2;
				u16 bUnline		: 1;
				u16					: 3;
			}Bits;
			u16 Byte;
		}Msg;
		u8 Buf[10];
		u8 Timer;
		u8 Times;
		u8 ResetTimes;
		struct{
			struct{
				u8	bSet	: 1;
				u8	Len	: 7;
			}Msg;
			u8 Buf[DrvMC8332_UseId_Len];
		}LoginInfo;
	}NetState;
#if 1//added by Tom
        struct{
          u8 Buf[30];
          u8 Len;
        }PocDateTest;
#endif
	struct{
		struct{
			union{
				struct{
					u16 bInitial			: 1;
					u16 bPttState		: 1;			//0: ptt involide; 1 :ptt volide
					u16 bPttUser			: 1;			//ptt operrtor 0: oneself; 1: other
					u16 bWorkGrpVolide	: 3;
					u16 bEnterGroup		: 1;
					u16 bPlayState		: 1;
					u16 bCallFail		: 1;
					u16						: 7;
				}Bits;
				u16 Byte;
			}Msg;
			u8 Timer;
			struct{
				u8 Index[8];
				u8 Id[8];
				u8 Name[APIPOC_UserName_Len];
				u8 NameLen;
			}MainWorkGroup;
			
			struct{
				//u8 Index[8];
				u8 Id[8];
				u8 Name[APIPOC_UserName_Len];
				u8 NameLen;
			}WorkGroup;
			
			struct{
				u8 Id[8];
				u8 Name[APIPOC_UserName_Len];
				u8 NameLen;
			}WorkUserName;

			struct{
				u8 Id[8];
				u8 Name[APIPOC_UserName_Len];
				u8 NameLen;
			}PttUserName;

		}UseState;
		
		struct{
			union{
				struct{
					u8 InvEnable			: 1;
					u8 InvPreEnable		: 1;
					u8 	InvMode			: 3;
					u8						: 3;
				}Bits;
				u8 Byte;
			}Msg;
			u8 InviteIndex;
			struct{
				u8 Id[8];
				u8 Name[APIPOC_UserName_Len];
				u8 Len;
			}InviteGroupName;
			struct{
				u8 Buf[APIPOC_UserName_Len];
				u8 Len;
			}FailInfo;

		}InviteInfo;
		
		struct{
			struct{
				u8 Id[8];
				u8 Name[DrvMC8332_GroupName_Len];
				u8 Len;
			}MonitorGroupName;
		}MonitorInfo;
		u8 InvFirstPlay;
	}WorkState;
	
	struct{
		union{
			struct{
				u8	GroupList	: 1;
				u8	UserList	: 1;
				u8				: 6;
			}Bits;
			u8 Byte;
		}Msg;
		struct{
			u8 Id[8];
			u8 Name[DrvMC8332_GroupName_Len];
			u8 Len;
		}GroupInfo[1];
		u8 GroupInfoLen;
		
		struct{
			u8 Id[8];
			u8 Name[APIPOC_UserName_Len];
			u8 Len;
		}UseInfo[APIPOC_UserList_Len];
		struct{
			u16 Start;
			u16 Stop;
			u16 Index;
		}UseInfoLoadPoint;
		u16 UseIndex;
		u16 UseListLen;
	}CfgInfo;
}PocCmdDrv;

static PocCmdDrv PocCmdDrvobj;
/*void ApiPocCmd_PowerOnInitial(void)
{
  PocCmdDrvobj.NetState.Msg.Byte = 0x00;
  PocCmdDrvobj.NetState.Timer = 0x00;
  PocCmdDrvobj.NetState.Times = 0x00;
  PocCmdDrvobj.NetState.ResetTimes = 0x00;
  PocCmdDrvobj.WorkState.InvFirstPlay = 0x00;
  PocCmdDrvobj.WorkState.UseState.Msg.Byte = 0x00;
  PocCmdDrvobj.WorkState.UseState.Msg.Bits.bInitial = 0x01;
  PocCmdDrvobj.WorkState.InviteInfo.Msg.Byte = 0x00;
  PocCmdDrvobj.WorkState.InviteInfo.InviteIndex = 0x00;
  PocCmdDrvobj.CfgInfo.Msg.Byte = 0x00;
  PocCmdDrvobj.CfgInfo.GroupInfoLen = 0x00;
  PocCmdDrvobj.CfgInfo.UseListLen = 0x00;
  PocCmdDrvobj.CfgInfo.UseInfoLoadPoint.Start = 0;
  PocCmdDrvobj.CfgInfo.UseInfoLoadPoint.Index = 0;
  PocCmdDrvobj.CfgInfo.UseInfoLoadPoint.Stop = 16;
  PocCmdDrvobj.CfgInfo.UseIndex = 0;
}*/

bool ApiPocCmd_WritCommand(PocCommType id, u8 *buf, u16 len)
{
  bool r = TRUE;
  DrvMC8332_TxPort_SetValidable(ON);
  DrvGD83_UART_TxCommand((u8 *)ucAtPocHead,strlen((char const *)ucAtPocHead));
  switch(id)
  {
  case PocComm_OpenPOC://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_SetParam://2
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_GetParam:
  case PocComm_Login:
  case PocComm_Logout:
  case PocComm_Cancel:
  case PocComm_ModifyPwd:
    break;
  case PocComm_EnterGroup:
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_Invite:
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_StartPTT://3
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_EndPTT://4
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_GroupListInfo://5
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_UserListInfo://6
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_Key://7
    DrvGD83_UART_TxCommand(buf, len);
    break;
  default:
    break;
  }
  r = DrvMc8332_UART_TxTail();
  DrvMC8332_TxPort_SetValidable(ON);
  return r;
}

void ApiPocCmd_10msRenew(void)
{
  u8 ucId, i, Len;
  u8 * pBuf, ucRet;
  while((Len = DrvMC8332_PocNotify_Queue_front(&pBuf)) != 0x00)
  {
    ucId = COML_AscToHex(pBuf, 0x02);
    switch(ucId)
    {
    case 0x8B:
      ucId = COML_AscToHex(pBuf+4, 0x02);
      if(ucId == 0x00)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.bPlayState = 0x00;
      }
      if(ucId == 0x01)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.bPlayState = 0x01;
      }
      
      break;
    default:
      break;
    }
  }
}
  
void ApiGetPocBuf(void)
{
  DrvGD83_UART_TxCommand((u8 *)"AT+Printf=",strlen((char const *)"AT+Printf="));
  DrvGD83_UART_TxCommand((u8 *)PocCmdDrvobj.PocDateTest.Buf,strlen((char const *)PocCmdDrvobj.PocDateTest.Buf));
  DrvGD83_UART_TxCommand((u8 *)"\r\n",strlen((char const *)"\r\n"));                      
}


void ListenState(void)
{
  if(PocCmdDrvobj.WorkState.UseState.Msg.Bits.bPlayState==0)
  {
    Set_GreenLed(LED_OFF);
  }
  if(PocCmdDrvobj.WorkState.UseState.Msg.Bits.bPlayState==1)
  {
    Set_GreenLed(LED_ON);

  }
}
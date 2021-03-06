#include "ALLHead.h"

#define DrvMC8332_GroupName_Len		16			//define UART Tx buffer length value
#define DrvMC8332_UseId_Len			100			//define UART Tx buffer length value
#define APIPOC_UserList_Len			16
#define APIPOC_UserLoad_Len			8
#define APIPOC_UserName_Len			30

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
				u16 bPocOpen	: 1;
				u16 bModeChange	: 1;
				u16	 bMode	: 3;
				u16 bNetStat	: 2;
				u16 bUnline	: 1;
				u16       : 1;
                                u16             : 2;
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
					u16 bCallFail	        : 1;
                                        u16 bLogin              : 1;
					u16 		: 1;
                                        u16 			: 5;
				}Bits;
				u16 Byte;
			}Msg;
			u8 Timer;
			struct{
                          
				u8 PresentGroupId;
                                u8 GroupNum;
				u8 Index[8];
				u8 Id[8];
				u8 Name[APIPOC_UserName_Len];
				u8 NameLen;
			}MainWorkGroup;
                        struct{
                                u8 PresentUserId;
                                u8 UserNum;
				u8 Id[8];
				u8 Name[APIPOC_UserName_Len];
				u8 NameLen;
			}PttUserName;
#if 1//POC:80 added by Tom in 2017.11.21
			struct{
                                u8 Id[8];
				u8 Name[APIPOC_UserName_Len];
				u8 NameLen;
			}Group[40];
                        struct{
                                u8 Id[8];
				u8 Name[APIPOC_UserName_Len];
				u8 NameLen;
			}UserName[30];
#endif
			struct{
				u8 Id[8];
				u8 Name[APIPOC_UserName_Len];
				u8 NameLen;
			}WorkGroup;
			
			struct{
				u8 Id[8];
				u8 Name[APIPOC_UserName_Len];
				u8 NameLen;
			}WorkUserName;



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
    DrvGD83_UART_TxCommand("090000000000", 12);
    PocCmdDrvobj.NetState.Buf[0] = ((GroupCallingNum&0xf0)>>4)+0x30;	// 0x03+0x30
    PocCmdDrvobj.NetState.Buf[1] = (GroupCallingNum&0x0f)+0x30;
    DrvGD83_UART_TxCommand(PocCmdDrvobj.NetState.Buf, 2);
    break;
  case PocComm_Invite:
    DrvGD83_UART_TxCommand("0A00000000", 10);
    PocCmdDrvobj.NetState.Buf[0] = ((GroupCallingNum&0xf0)>>4)+0x30;	// 0x03+0x30
    PocCmdDrvobj.NetState.Buf[1] = (GroupCallingNum&0x0f)+0x30;
    DrvGD83_UART_TxCommand(PocCmdDrvobj.NetState.Buf, 2);
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
  u8 ucId,j,i, Len;
  u8 * pBuf, ucRet;
  while((Len = DrvMC8332_PocNotify_Queue_front(&pBuf)) != 0x00)
  {
    ucId = COML_AscToHex(pBuf, 0x02);
    switch(ucId)
    {
    case 0x0E://群组个数
      ucId = COML_AscToHex(pBuf+8, 0x04);
      PocCmdDrvobj.WorkState.UseState.PttUserName.UserNum = ucId;
      break;
    case 0x0d://群组个数
      ucId = COML_AscToHex(pBuf+10, 0x02);
      PocCmdDrvobj.WorkState.UseState.MainWorkGroup.GroupNum = ucId;
      break;
    case 0x80://获取工作组列表
      ucId = COML_AscToHex(pBuf+10, 0x02);
      if(Len >= 24)//如果群组id后面还有群组名
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = Len - 24;
      }
      else//无群组名
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = 0x00;
      }
      for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen; i++)
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i] = pBuf[i+24];//存入获取的群组名
        PocCmdDrvobj.WorkState.UseState.Group[ucId].Name[i] = 
          PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i];
      }
      PocCmdDrvobj.WorkState.UseState.Group[ucId].NameLen = PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen;
      break;
    case 0x81://获取组内成员列表
      ucId = COML_AscToHex(pBuf+10, 0x02);//
      if(Len >= 20)//如果用户ID后面还有用户名
      {
        PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen = Len - 20;
      }
      else//无用户名
      {
        PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen = 0x00;
      }
      for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen; i++)
      {
        PocCmdDrvobj.WorkState.UseState.WorkUserName.Name[i] = pBuf[i+20];//存入获取的群组名
        PocCmdDrvobj.WorkState.UseState.UserName[ucId].Name[i] = 
          PocCmdDrvobj.WorkState.UseState.WorkUserName.Name[i];
      }
      PocCmdDrvobj.WorkState.UseState.UserName[ucId].NameLen = PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen;
      
    case 0x82://判断是否登录成功
      ucId = COML_AscToHex(pBuf+3, 0x01);
      if(ucId == 0x02)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.bLogin = 0x01;
      }
      else
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.bLogin = 0x00;
      }
      break;
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
    case 0x86:
      ucId = COML_AscToHex(pBuf+10, 0x02);
      PocCmdDrvobj.WorkState.UseState.MainWorkGroup.PresentGroupId = ucId;
      ucId = 0x00;
      for(i = 0x00; i < 0x08; i++)
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i] = pBuf[i+4];
        PocCmdDrvobj.WorkState.UseState.MainWorkGroup.Id[i] = 
            PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i];
      if(PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i] != 'f') //=f为离开群组
        ucId++;
      }
      if(ucId==0x00)//如果为指令代表离开群组
      {
        
      }
      else//r如果为在群组内
      {
        
      }
      if(Len >= 12)//如果群组id后面还有群组名
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = Len - 12;
        if(PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen > APIPOC_UserName_Len)
        {
          PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = APIPOC_UserName_Len;
        }
      }
      else//无群组名
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = 0x00;
      }
      for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen; i++)
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i] = pBuf[i+12];//存入获取的群组名
        PocCmdDrvobj.WorkState.UseState.MainWorkGroup.Name[i] = 
            PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i];
      }
      PocCmdDrvobj.WorkState.UseState.MainWorkGroup.NameLen = PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen;
    default:
      break;
    }
  }
}

u8 *ApiAtCmd_GetGroupName(u8 n)//获取所有群组名
{
  return PocCmdDrvobj.WorkState.UseState.Group[n].Name;
}
u8 ApiAtCmd_GetGroupNameLen(u8 n)
{
  return PocCmdDrvobj.WorkState.UseState.Group[n].NameLen;
}

u8 *ApiAtCmd_GetUserName(u8 n)//获取所有在线用户名（个呼）
{
  return PocCmdDrvobj.WorkState.UseState.UserName[n].Name;
}
u8 ApiAtCmd_GetUserNameLen(u8 n)
{
  return PocCmdDrvobj.WorkState.UseState.UserName[n].NameLen;
}

u8 *ApiAtCmd_GetMainWorkName(void)//获取工作群组名
{
  return PocCmdDrvobj.WorkState.UseState.MainWorkGroup.Name;
}
u8 ApiAtCmd_GetMainWorkNameLen(void)
{
  return PocCmdDrvobj.WorkState.UseState.MainWorkGroup.NameLen;
}
u8 *ApiAtCmd_GetMainUserName(void)//获取工作群组名（个呼）
{
  return PocCmdDrvobj.WorkState.UseState.PttUserName.Name;
}
u8 ApiAtCmd_GetMainUserNameLen(void)
{
  return PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen;
}

u8 ApiAtCmd_GetGroupNum(void)//获取群组数
{
  return PocCmdDrvobj.WorkState.UseState.MainWorkGroup.GroupNum;
}
u8 ApiAtCmd_GetUserNum(void)//获取在线成员数（个呼）
{
  return PocCmdDrvobj.WorkState.UseState.PttUserName.UserNum;
}

u8 ApiAtCmd_GetMainGroupId(void)//获取当前群组id
{
  return PocCmdDrvobj.WorkState.UseState.MainWorkGroup.PresentGroupId;
}
u8 ApiAtCmd_GetMainUserId(void)//获取当前用户id（个呼）
{
  return PocCmdDrvobj.WorkState.UseState.PttUserName.PresentUserId;
}

bool ApiAtCmd_GetLoginState(void)
{
	return PocCmdDrvobj.WorkState.UseState.Msg.Bits.bLogin;
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
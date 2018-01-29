#include "ALLHead.h"
#define DrvMC8332_GroupName_Len		16			//define UART Tx buffer length value
#define DrvMC8332_UseId_Len			100			//define UART Tx buffer length value
#define APIPOC_UserList_Len			16
#define APIPOC_UserLoad_Len			8
#define APIPOC_UserName_Len			30

u8 ReadBuffer[80];//Test 存EEPROM读取的数据使用
u8 ASCII_ActiveUserID[22];//Test 存EEPROM读取的数据使用
u8 Get_Unicode_ActiveUserIDBuf[45];//
u8 Get_0X_ActiveUserIDBuf[11];//
//u8 TestReadBuffer[300];//存EEPROM读取部标数据测试
u8 UnicodeForGbk_MainWorkNameBuf[15];
u8 UnicodeForGbk_MainUserNameBuf[15];

const u8 *ucAtPocHead   = "AT+POC=";
const u8 *ucTingEnd   = "0B0000";
const u8 *ucTingStart   = "0B0001";
const u8 *ucSetIPAndID    = "010000";
u8 POC_EnterPersonalCalling_Flag=0;
u8 POC_QuitPersonalCalling_Flag=0;
u8 POC_AtEnterPersonalCalling_Flag=0;
u8 POC_AtQuitPersonalCalling_Flag=0;
u8 POC_EnterGroupCalling_Flag=0;
u8 POC_QuitGroupCalling_Flag=0;
bool POC_ReceivedVoice_Flag=FALSE;
bool ApiPocCmd_Tone_Flag=FALSE;
u8 POC_ReceivedVoiceStart_Flag=0;
u8 POC_ReceivedVoiceEnd_Flag=0;
bool POC_Receive86_Flag=FALSE;
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
        u16             : 1;
        u16             : 2;
      }Bits;
      u16 Byte;
    }Msg;
    u8 Buf[10];
    u8 Buf2[10];
    u8 Buf3[3];
    u8 Buf4[3];
    u8 Buf5[3];
    u8 Buf6[3];
    u8 Timer;
    u8 Times;
    u8 ResetTimes;
    struct{
      struct{
        u8 bSet	: 1;
        u8 Len	: 7;
      }Msg;
      u8 Buf[DrvMC8332_UseId_Len];
    }LoginInfo;
  }NetState;
  struct{
    struct{
      union{
        struct{
          u16 bInitial		        : 1;
          u16 bPttState		        : 1;			//0: ptt involide; 1 :ptt volide
          u16 bPttUser		        : 1;			//ptt operrtor 0: oneself; 1: other
          u16 bWorkGrpVolide	        : 3;
          u16 bEnterGroup		: 1;
          u16 bCallFail	                : 1;
          u16 bLogin                    : 1;
          u16 AlarmMode            	: 1;
          u16 PersonalCallingMode 	: 1;
          u16 			        : 5;
        }Bits;u16 Byte;
			}Msg;
			u8 Timer;
			struct{
                          
				u8 PresentGroupId;
                                u8 GroupNum;
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
			}Group[50];//原50，内存测试改为10---------------------------------------test----------------------------------------------------------
                        struct{
                                u8 Id[8];
				u8 Name[APIPOC_UserName_Len];
				u8 NameLen;
			}UserName[20];
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
	}WorkState;
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

void ApiPocCmd_WritCommand(PocCommType id, u8 *buf, u16 len)
{
  DrvMC8332_TxPort_SetValidable(ON);
  DrvGD83_UART_TxCommand((u8 *)ucAtPocHead,strlen((char const *)ucAtPocHead));
  switch(id)
  {
  case PocComm_OpenPOC://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_SetParam://设置账号密码
    DrvGD83_UART_TxCommand((u8 *)ucSetIPAndID,strlen((char const *)ucSetIPAndID));
    FILE_Read(0,80,ReadBuffer);//80位
   // FILE_Read(28,22,ActiveUserID);
//    FILE_Read(0x230,250,TestReadBuffer);//0x260-0x2cc
    DrvGD83_UART_TxCommand(ReadBuffer, strlen((char const *)ReadBuffer));
    break;
  case PocComm_GetParam:
  case PocComm_Login:
    break;
  case PocComm_Logout:
  case PocComm_Cancel:
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_ModifyPwd:
    break;
  case PocComm_EnterGroup:
    DrvGD83_UART_TxCommand("090000000000", 12);
    PocCmdDrvobj.NetState.Buf[0] = ((GroupCallingNum&0xf0)>>4)+0x30;	// 0x03+0x30
    PocCmdDrvobj.NetState.Buf[1] = (GroupCallingNum&0x0f)+0x30;
    DrvGD83_UART_TxCommand(PocCmdDrvobj.NetState.Buf, 2);
    break;
  case PocComm_Invite:
    DrvGD83_UART_TxCommand("0A0000000000", 12);
    PocCmdDrvobj.NetState.Buf2[0] = (((PersonalCallingNum+0x64)&0xf0)>>4)+0x30;	// 0x03+0x30
    PocCmdDrvobj.NetState.Buf2[1] = ((PersonalCallingNum+0x64)&0x0f)+0x30;
    DrvGD83_UART_TxCommand(PocCmdDrvobj.NetState.Buf2, 2);
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
  DrvMc8332_UART_TxTail();
  DrvMC8332_TxPort_SetValidable(ON);
}

//写频写入数据存入EEPROM
bool ApiPocCmd_user_info_set(u8 *pBuf, u8 len)//cTxBuf为存放ip账号密码的信息
{
	bool r;
	u8 i, uRet = 0;
	//ADRLEN_INF	adr;

	for(i = 0; i < len; i++)
	{
		if(pBuf[i] == ';')
		{
			uRet++;
		}
	}
	if(uRet >= 2)
	{
		Dec2Hex(pBuf, len, PocCmdDrvobj.NetState.LoginInfo.Buf);//将收到的数转化为字符串//LoginInfo.Buf为存放
		PocCmdDrvobj.NetState.LoginInfo.Msg.Len = len << 0x01;//为什么要len*2？
		PocCmdDrvobj.NetState.LoginInfo.Msg.bSet = ON;
		//adr = CFG_GetCurAdr(ADR_IDLocalUserInfo);
		//FILE_Write(adr.Adr,adr.Len,(u8*)(&PocCmdDrvobj.NetState.LoginInfo));
                FILE_Write(0,PocCmdDrvobj.NetState.LoginInfo.Msg.Len,(u8*)(&PocCmdDrvobj.NetState.LoginInfo));
                
		for(i = 0; i < len; i++)
		{
			PocCmdDrvobj.NetState.LoginInfo.Buf[i] = pBuf[i];
		}
		PocCmdDrvobj.NetState.LoginInfo.Msg.Len = len;
		
		//SYS_BufReset();
		PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen = 0;
		PocCmdDrvobj.NetState.Msg.Bits.bUserInfo = 3;
		PocCmdDrvobj.NetState.Msg.Bits.bUserWrite = ON;
		r = TRUE;
	}
	else
	{
		r = FALSE;
	}
	return r;
}

#if 1//绿灯bUG
/*void ApiPocCmd_83_1msRenew(void)
{
  u8 ucId,i, Len;
  u8 * pBuf, ucRet;
  while((Len = DrvMC8332_PocNotify_Queue_front(&pBuf)) != 0x00)
  {
    ucId = COML_AscToHex(pBuf, 0x02);
    if(ucId==0x83)
    {
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId == 0x00)
      {
        POC_ReceivedVoice_Flag=TRUE;
        POC_ReceivedVoiceStart_Flag=2;//0:正常 1：收到语音 2：刚开始语音
        POC_ReceivedVoiceEnd_Flag=1;//0:正常 1：收到语音 2：刚结束语音
      }
      if(ucId == 0xff)
      {
        POC_ReceivedVoice_Flag=FALSE;
        POC_ReceivedVoiceEnd_Flag=2;//0:正常 1：收到语音 2：刚结束语音
        POC_ReceivedVoiceStart_Flag=0;//0:正常 1：收到语音 2：刚开始语音
      }
    }
  }
}*/
#endif

void ApiPocCmd_10msRenew(void)
{
  u8 ucId,i, Len;
  u8 * pBuf, ucRet;
  while((Len = DrvMC8332_PocNotify_Queue_front(&pBuf)) != 0x00)
  {
    ucId = COML_AscToHex(pBuf, 0x02);
    switch(ucId)
    {
    case 0x0A://判断讲话状态
#if 1
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x00)
      {
        if(TASK_Ptt_StartPersonCalling_Flag==TRUE)//如果按下PTT键单呼某用户
        {
          POC_AtEnterPersonalCalling_Flag=2;
          POC_AtQuitPersonalCalling_Flag=1;
          TASK_Ptt_StartPersonCalling_Flag=FALSE;
        }
      }
#else
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x00)
      {
        if(TASK_Ptt_StartPersonCalling_Flag==TRUE)//如果按下PTT键单呼某用户
        {
          POC_AtEnterPersonalCalling_Flag=2;
          POC_AtQuitPersonalCalling_Flag=1;
          TASK_Ptt_StartPersonCalling_Flag=FALSE;
        }
        else//接收到结束单呼的指令
        {
          POC_AtEnterPersonalCalling_Flag=0;
          POC_AtQuitPersonalCalling_Flag=2;
        }
      }
#endif
      break;

    case 0x0E://在线用户个数
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
      break;
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
/********判断是否是被呼状态******************************/
    case 0x83:
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId == 0x00)
      {
        POC_ReceivedVoice_Flag=TRUE;
        POC_ReceivedVoiceStart_Flag=2;//0:正常 1：收到语音 2：刚开始语音
        POC_ReceivedVoiceEnd_Flag=1;//0:正常 1：收到语音 2：刚结束语音
      }
      if(ucId == 0xff)
      {
        POC_ReceivedVoice_Flag=FALSE;
        POC_ReceivedVoiceEnd_Flag=2;//0:正常 1：收到语音 2：刚结束语音
        POC_ReceivedVoiceStart_Flag=0;//0:正常 1：收到语音 2：刚开始语音
      }
      break;
/**************************************/
    case 0x8B:
      ucId = COML_AscToHex(pBuf+4, 0x02);
      if(ucId==0x03)
      {
        ApiPocCmd_Tone_Flag=TRUE;
      }
      break;
    case 0x86:
      POC_Receive86_Flag=TRUE;
/****************判断接入单呼**************************************************************/
      ucId = COML_AscToHex(pBuf+4, 0x02);
      if(ucId==0x0a)//接入单呼
      { 
        POC_EnterPersonalCalling_Flag=2;//用于判断进入单呼,0:正常 2：正在进入单呼 1：单呼状态
        POC_QuitPersonalCalling_Flag=1;//用于判断退出单呼
      }
      else
      {
        if(ucId==0x00&&POC_QuitPersonalCalling_Flag==1)//收到则退出单呼（退出单呼、进组状态）
        {
          POC_EnterPersonalCalling_Flag=0;
          POC_QuitPersonalCalling_Flag=2;
        }
      }
/****************群组状态判断及群组信息获取**************************************************************/
#if 1//将群组名与单呼名分开
      ucId = COML_AscToHex(pBuf+10, 0x02);
      if(ucId==0xff)
      {
        POC_EnterGroupCalling_Flag=0;//0默认 1在群组内 2正在进入
        POC_QuitGroupCalling_Flag=2;//0默认 1在群组内 2正在退出
#if 1//被呼主动结束单呼不显示群组名BUG
        POC_EnterPersonalCalling_Flag=0;
        POC_QuitPersonalCalling_Flag=2;
#endif
      }
      else//正在进入群组或单呼
      {
        if(PocCmdDrvobj.WorkState.UseState.Msg.Bits.PersonalCallingMode == 0x01)//如果是进入单呼模式则86存入单呼名
        {
          POC_EnterGroupCalling_Flag=2;
          POC_QuitGroupCalling_Flag=1;

            if(Len >= 12)//如果群组id后面还有群组名
            {
              PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen = Len - 12;
              if(PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen > APIPOC_UserName_Len)
              {
                PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen = APIPOC_UserName_Len;
              }
            }
            else//无群组名
            {
              PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen = 0x00;
            }
            for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen; i++)
            {
              PocCmdDrvobj.WorkState.UseState.PttUserName.Name[i] = pBuf[i+12];//存入获取的群组名
            }          
        }
        else//进组存组名
        {
          POC_EnterGroupCalling_Flag=2;
          POC_QuitGroupCalling_Flag=1;
          PocCmdDrvobj.WorkState.UseState.MainWorkGroup.PresentGroupId = ucId;
          ucId = 0x00;
          for(i = 0x00; i < 0x08; i++)
          {
            PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i] = pBuf[i+4];
            PocCmdDrvobj.WorkState.UseState.MainWorkGroup.Id[i] = PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i];
            if(PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i] != 'f') //=f为离开群组
              ucId++;
          }
          if(ucId==0x00)//如果为指令代表离开群组
          {}
          else//r如果为在群组内
          {
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
              PocCmdDrvobj.WorkState.UseState.MainWorkGroup.Name[i] = PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i];
            }
            PocCmdDrvobj.WorkState.UseState.MainWorkGroup.NameLen = PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen;
          }
        }
      }

#else
      ucId = COML_AscToHex(pBuf+10, 0x02);
      if(ucId==0xff)
      {
        POC_EnterGroupCalling_Flag=0;//0默认 1在群组内 2正在进入
        POC_QuitGroupCalling_Flag=2;//0默认 1在群组内 2正在退出
      }
      else//正在进入群组
      {
        POC_EnterGroupCalling_Flag=2;
        POC_QuitGroupCalling_Flag=1;
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
      }
      }
#endif
      break;
    case 0x91://通知进入某种模式（进入退出一键告警、单呼模式）
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId == 0x00)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.AlarmMode = 0x00;//退出一键告警
      }
      if(ucId == 0x01)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.AlarmMode = 0x01;//进入一键告警
      }
      if(ucId == 0x02)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.PersonalCallingMode = 0x01;//进入单呼
      }
      if(ucId == 0x03)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.PersonalCallingMode = 0x00;//退出单呼 
        AUDIO_IOAFPOW(ON);//打开功放，解决'组呼模式'播报问题

      }
      break;
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

u8 *ApiAtCmd_GetMainUserName(void)//获取当前用户名（个呼）
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

bool ApiPocCmd_GetPttState(void)//判断PPT状态，是否有话权
{
  return (bool)PocCmdDrvobj.WorkState.UseState.Msg.Bits.bPttState;
}
bool ApiAtCmd_GetLoginState(void)
{
  return (bool)PocCmdDrvobj.WorkState.UseState.Msg.Bits.bLogin;
}
/*
void ApiGetPocBuf(void)
{
  DrvGD83_UART_TxCommand((u8 *)"AT+Printf=",strlen((char const *)"AT+Printf="));
  DrvGD83_UART_TxCommand((u8 *)PocCmdDrvobj.PocDateTest.Buf,strlen((char const *)PocCmdDrvobj.PocDateTest.Buf));
  DrvGD83_UART_TxCommand((u8 *)"\r\n",strlen((char const *)"\r\n"));                      
}
*/


bool GetPersonalCallingMode(void)
{
  return (bool)PocCmdDrvobj.WorkState.UseState.Msg.Bits.PersonalCallingMode;
}

u8 *HexToChar_MainGroupId(void)//16进制转字符串 当前群组ID 显示屏数据使用
{
  u8 i;
  i=ApiAtCmd_GetMainGroupId();
  PocCmdDrvobj.NetState.Buf3[0]=((i&0xf0)>>4)+0x30;
  PocCmdDrvobj.NetState.Buf3[1]=(i&0x0f)+0x30;
  PocCmdDrvobj.NetState.Buf3[2]='\0';
  return PocCmdDrvobj.NetState.Buf3;
}
u8 *HexToChar_GroupCallingNum(void)//16进制转字符串 按键播报当前群组ID 显示屏数据使用
{
  u8 i;
  i=GroupCallingNum;
  PocCmdDrvobj.NetState.Buf4[0]=((i&0xf0)>>4)+0x30;
  PocCmdDrvobj.NetState.Buf4[1]=(i&0x0f)+0x30;
  PocCmdDrvobj.NetState.Buf4[2]='\0';
  return PocCmdDrvobj.NetState.Buf4;
}

u8 *HexToChar_MainUserId(void)//16进制转字符串 当前用户ID 显示屏数据使用
{
  u8 i;
  i=ApiAtCmd_GetMainUserId()+0x01;
  PocCmdDrvobj.NetState.Buf5[0]=((i&0xf0)>>4)+0x30;
  PocCmdDrvobj.NetState.Buf5[1]=(i&0x0f)+0x30;
  PocCmdDrvobj.NetState.Buf5[2]='\0';
  return PocCmdDrvobj.NetState.Buf5;
}
u8 *HexToChar_PersonalCallingNum(void)//16进制转字符串 按键播报当前用户ID 显示屏数据使用
{
  u8 i;
  i=PersonalCallingNum+0x01;
  PocCmdDrvobj.NetState.Buf6[0]=((i&0xf0)>>4)+0x30;
  PocCmdDrvobj.NetState.Buf6[1]=(i&0x0f)+0x30;
  PocCmdDrvobj.NetState.Buf6[2]='\0';
  return PocCmdDrvobj.NetState.Buf6;
}

//显示屏显示当前群组名
u8 *UnicodeForGbk_MainWorkName(void)
{
  u8 *Buf1;
  u8 Buf2[30];

  u8 Len=0;
  u8 i=0;
  Buf1=ApiAtCmd_GetMainWorkName();
  Len=strlen((char const *)ApiAtCmd_GetMainWorkName());
  while(1)
  {
    if(4*i<=Len)
    {
      Buf2[4*i+0]=Buf1[4*i+2];
      Buf2[4*i+1]=Buf1[4*i+3];
      Buf2[4*i+2]=Buf1[4*i+0];
      Buf2[4*i+3]=Buf1[4*i+1];
      UnicodeForGbk_MainWorkNameBuf[2*i+0]=COML_AscToHex(Buf2+(4*i), 0x02);
      UnicodeForGbk_MainWorkNameBuf[2*i+1]=COML_AscToHex(Buf2+(4*i)+2, 0x02);
      i++;
    }
    else
    {
      Buf2[Len]='\0';

      return UnicodeForGbk_MainWorkNameBuf;
    }
      
  }
}


//显示屏显示当前用户昵称
u8 *UnicodeForGbk_MainUserName(void)
{
  u8 *Buf1;
  u8 Buf2[30];

  u8 Len=0,i=0;
  Buf1=ApiAtCmd_GetMainUserName();
  Len=strlen((char const *)ApiAtCmd_GetMainUserName());
  while(1)
  {
    if(4*i<=Len)
    {
      Buf2[4*i+0]=Buf1[4*i+2];
      Buf2[4*i+1]=Buf1[4*i+3];
      Buf2[4*i+2]=Buf1[4*i+0];
      Buf2[4*i+3]=Buf1[4*i+1];
      UnicodeForGbk_MainUserNameBuf[2*i+0]=COML_AscToHex(Buf2+(4*i), 0x02);
      UnicodeForGbk_MainUserNameBuf[2*i+1]=COML_AscToHex(Buf2+(4*i)+2, 0x02);
      i++;
    }
    else
    {
      Buf2[Len]='\0';

      return UnicodeForGbk_MainUserNameBuf;
    }
  }
}
//当前用户ID（播报使用）
u8 *Get_Unicode_ActiveUserID(void)
{
  u8 Len=0,i=0;
  FILE_Read(28,22,ASCII_ActiveUserID);//3139383030333037343732
  Len=strlen((char const *)ASCII_ActiveUserID);
  for(i=0;i<Len;i++)
  {
    if(i%2==0)//偶数
    {
      Get_Unicode_ActiveUserIDBuf[2*i]=ASCII_ActiveUserID[i];
    }
    else//奇数
    {
      Get_Unicode_ActiveUserIDBuf[2*i-1]=ASCII_ActiveUserID[i];
      Get_Unicode_ActiveUserIDBuf[2*i]=0x30;
      Get_Unicode_ActiveUserIDBuf[2*i+1]=0x30;
    }
  }
  //31003900380030003000330030003700340037003200
  return Get_Unicode_ActiveUserIDBuf;
}

//当前用户ID（显示屏使用）
u8 *Get_GBK_ActiveUserID(void)
{
  u8 i=0,Len=0;
  FILE_Read(28,22,ASCII_ActiveUserID);//3139383030333037343732
  Len=strlen((char const *)ASCII_ActiveUserID);
  for(i=0;i<Len;i++)
  {
    if(i%2!=0)
    {
      Get_0X_ActiveUserIDBuf[(i-1)/2]=ASCII_ActiveUserID[i];
    }
  }
  return Get_0X_ActiveUserIDBuf;
}
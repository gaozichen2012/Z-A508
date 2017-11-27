#include "AllHead.h"

const u8 *ucPlayZtts = "AT+ZTTS=";

#define DrvMC8332_IccId_Len 30
typedef struct{
	struct{
		union{
			struct{
				u16 bFunOff	: 1;
				u16 bFun	: 1;
				u16 bEsn	: 1;
				u16 bCard	: 1;
				u16 bRssi	: 1;
				u16 bPppStep	: 2;
				u16 bPppOk	: 1;
				u16 bTcp	: 1;
				u16 bTcpOk	: 1;
				u16 bUdp	: 1;
				u16 bUdpOk	: 1;
				u16 bFirstPlay  : 1;
				u16		: 3;
			}Bits;
			u16 Byte;
		}Msg;
		u8 ucCardPassword[50];
		struct{
			u8 Buf[DrvMC8332_IccId_Len];
			u8 Len;
		}IccId;
		u8 RssiValue;
		u8 Buf[30];
		u8 Len;
		u8 Timer;
		u8 Times;
		u8 ResetTimer;
		u8 OpenPppTimer;
	}NetState;
}AtCmdDrv;
static AtCmdDrv AtCmdDrvobj;

u8 BootProcess_SIMST_Flag=0;
u8 BootProcess_PPPCFG_Flag=0;
u8 BootProcess_OpenPoc_Flag=0;
u8 VoiceEnd_Flag=0;
u8 KeyDownUpChoose_GroupOrUser_Flag=0;


const u8 *ucRxCheckCard = "GETICCID:";
const u8 *ucRxZTTS0 = "ZTTS:0";
const u8 *ucSIMST="^SIMST:";
const u8 *ucCaretPPPCFG="^PPPCFG:";

bool ApiAtCmd_WritCommand(AtCommType id, u8 *buf, u16 len)
{
  bool r = TRUE;
  DrvMC8332_TxPort_SetValidable(ON);
  switch(id)
  {
  case ATCOMM3_GD83Reset://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case ATCOMM0_OSSYSHWID://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case ATCOMM1_PPPCFG://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case ATCOMM2_ZTTS_Abell://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case ATCOMM4_GD83Mode://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
    
  default:
    break;
  }
  r = DrvMc8332_UART_TxTail();
  DrvMC8332_TxPort_SetValidable(OFF);
  return r;
}


void ApiCaretCmd_10msRenew(void)
{
  u8 * pBuf, ucRet, Len, i;
  while((Len = DrvMC8332_CaretNotify_Queue_front(&pBuf)) != 0)
  {
    ucRet = memcmp(pBuf, ucSIMST, 7);//GETICCID
    if(ucRet == 0x00)
    {
      BootProcess_SIMST_Flag=1;
    }
    ucRet = memcmp(pBuf, ucCaretPPPCFG, 8);//GETICCID
    if(ucRet == 0x00)
    {
      BootProcess_PPPCFG_Flag=1;
    }
  }
}

void ApiAtCmd_10msRenew(void)
{
  u8 * pBuf, ucRet, Len, i;
  while((Len = DrvMC8332_AtNotify_Queue_front(&pBuf)) != 0)
  {
    ucRet = memcmp(pBuf, ucRxCheckCard, 9);//GETICCID
    if(ucRet == 0x00)
    {
      if(Len > 0x09)//È¥î^
      {
        Len -= 0x09;
      }
       for(i = 0x00; i < Len; i++)
       {
         AtCmdDrvobj.NetState.IccId.Buf[i] = pBuf[i + 9];
       }
       AtCmdDrvobj.NetState.IccId.Len = i;
    }
    ucRet = memcmp(pBuf, ucRxZTTS0, 6);//GETICCID
    if(ucRet == 0x00)
    {
      VoiceEnd_Flag=1;
    }
    else
    {
      VoiceEnd_Flag=0;
    }
  }
}

bool ApiAtCmd_PlayVoice(AtVoiceType id, u8 *buf, u8 len)
{
  bool r = TRUE;
  DrvMC8332_TxPort_SetValidable(ON);
  DrvGD83_UART_TxCommand((u8*)ucPlayZtts, strlen((char const *)ucPlayZtts));
  AtCmdDrvobj.NetState.Buf[0] = 0x31;	// 1
  AtCmdDrvobj.NetState.Buf[1] = 0x2C;	// ,
  AtCmdDrvobj.NetState.Buf[2] = 0x22;	// "
  DrvGD83_UART_TxCommand(AtCmdDrvobj.NetState.Buf, 3);
  switch(id)
  {
  case ATVOICE_FreePlay :
    DrvGD83_UART_TxCommand(buf, len);
    break;
  default:
    break;
  }
  AtCmdDrvobj.NetState.Buf[0] = 0x22;	// "
  DrvGD83_UART_TxCommand(AtCmdDrvobj.NetState.Buf, 1);
  r  = DrvMc8332_UART_TxTail();
  DrvMC8332_TxPort_SetValidable(OFF);
  return r;
}


u8 ApiAtCmd_GetIccId(u8 **pBuf)
{
	*pBuf = AtCmdDrvobj.NetState.IccId.Buf;
	return AtCmdDrvobj.NetState.IccId.Len;
}
void ApiGetIccidBuf(void)
{
  DrvGD83_UART_TxCommand((u8 *)"AT+Printf=",strlen((char const *)"AT+Printf="));
  DrvGD83_UART_TxCommand((u8 *)AtCmdDrvobj.NetState.IccId.Buf,strlen((char const *)AtCmdDrvobj.NetState.IccId.Buf));
  DrvGD83_UART_TxCommand((u8 *)"\r\n",strlen((char const *)"\r\n"));                      
}
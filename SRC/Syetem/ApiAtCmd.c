#include "AllHead.h"

#define DrvMC8332_IccId_Len 30
typedef struct{
	struct{
		union{
			struct{
				u16 bFunOff	: 1;
				u16 bFun		: 1;
				u16 bEsn		: 1;
				u16 bCard		: 1;
				u16 bRssi		: 1;
				u16 bPppStep	: 2;
				u16 bPppOk	: 1;
				u16 bTcp		: 1;
				u16 bTcpOk	: 1;
				u16 bUdp		: 1;
				u16 bUdpOk	: 1;
				u16 bFirstPlay: 1;
				u16				: 3;
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

const u8 *ucRxCheckCard = "GETICCID:";

void ApiAtCmd_10msRenew(void)
{
  u8 * pBuf, ucRet, Len, i;
  while((Len = DrvMC8332_AtNotify_Queue_front(&pBuf)) != 0)
  {
    ucRet = memcmp(pBuf, ucRxCheckCard, 9);
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
  }
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
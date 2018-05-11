#ifndef  _API_GPS_Cmd_H
#define  _API_GPS_Cmd_H
#include "AllHead.h"

typedef enum{
  GpsServerType_BuBiao          =       0x00,
  GpsServerType_ZTE             =       0x01,
  GpsServerType_BuBiaoAndZTE    =       0x02
}GpsServerType;

typedef enum{
	GPSCOMM_Ack		= 0x00,
	GPSCOMM_Puls		= 0x01,
	GPSCOMM_Login		= 0x02,
	GPSCOMM_Logout		= 0x03,
	GPSCOMM_Authentication	= 0x04,
	GPSCOMM_Position	= 0x05,
	GPSCOMM_PositionAck     = 0x06,
	GPSCOMM_PhotoAck	= 0x07,
	GPSCOMM_PhotoData	= 0x08,
	GPSCOMM_CheckTcp	= 0x09,
	GPSCOMM_SetIp		= 0x0A,
	GPSCOMM_CheckUdp	= 0x0B,
	GPSCOMM_SetUpu		= 0x0C,
	GPSCOMM_SendTcp		= 0x0D,
	GPSCOMM_SendUdp		= 0x0E,
	GPSCOMM_PlayZtts	= 0x0F,
	GPSCOMM_ZGSN		= 0x10
}GpsCommType;
extern bool GpsCmd_GbWritCommand(GpsCommType id, u8 *buf, u8 len);
extern GpsServerType ApiGpsServerType;
extern void ApiGpsCmd_PowerOnInitial(void);
extern void ApiGpsCmd_100msRenew(void);//决定什么时候发送什么数据;
extern void ApGpsCmd_10msRenew(void);//获取收到的指令，并解析

extern u8 *ApiGps_GetTcpIpAddress();
extern u8 *ApiGps_GetTcpPortAddress();
extern u8 ApiGps_GetTcpPortAddress_No5();
//extern void ApiGpsCmd_PowerReset(void);
//extern void ApiGpsCmd_1sRenew(void);
//extern u8 * ApiGpsCmd_GetIP(void);
//extern u8 * ApiGpsCmd_GetPort(void);
//extern BOOL ApiGpsCmd_GetCountDown(void);
//extern BOOL ApiGpsCmd_GetEnable(u8 Index);
//extern u8 ApiGpsCmd_GetServicer(void);
//extern u8 ApiGpsCmd_GetWorkMode(void);
//extern u8 * ApiGpsCmd_GetMapBuf(void);
//extern BOOL ApiGpsCmd_SetMapBuf(u8 ucData);
//extern void ApiGpsCmd_SetMapStart(void);
//extern u8 ApiGpsCmd_GetGpsProtocal(void);

#endif

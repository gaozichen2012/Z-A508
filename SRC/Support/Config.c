#define  CFGLABEL
#include "AllHead.h"

static const u8 CONFIG_NEWTBL[] = {	//new eeprom table value
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
	0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,
};

/******************************************************************************
; Function Description	: Confing id identify process
; Enter Parameter		: cId	: get current address id value
; Return Parameter		: address information value
******************************************************************************/	
ADRLEN_INF CFG_GetCurAdr(CFG_ADRTYPE cId)
{
	ADRLEN_INF adr;

	adr.Adr 	= 0x00;
	adr.Len 	= 0x00;
	adr.Group 	= 0x00;	
	switch (cId) //
	{
	case ADR_IDEepromChk:
		adr.Adr		= nAddrEepromID;
		adr.Len		= nLenEepromID;
		adr.Group 	= nGrpEepromID;
		break;
	case ADR_IDSerialNo:
		adr.Adr		= nAddrIDSeriesNo;
		adr.Len		= nLenIDSeriesNo;
		adr.Group 	= nGrpIDSeriesNo;
		break;
	case ADR_IDDealerID:
		adr.Adr		= nAddrDealerID;
		adr.Len		= nLenDealerID;
		adr.Group 	= nGrpDealerID;
		break;
	case ADR_IDMaxUserSum:
		adr.Adr		= nAddrMaxUserSum;
		adr.Len		= nLenMaxUserSum;
		adr.Group 	= nGrpMaxUserSum;
		break;
	case ADR_IDMaxGrpSum:
		adr.Adr		= nAddrMaxGrpSum;
		adr.Len		= nLenMaxGrpSum;
		adr.Group 	= nGrpMaxGrpSum;
		break;
	case ADR_IDSysMode:
		adr.Adr		= nAddrSysMode;
		adr.Len		= nLenSysMode;
		adr.Group 	= nGrpSysMode;
		break;
	case ADR_IDRadioInfo:
		adr.Adr		= nAddrRadioInfo;
		adr.Len		= nLenRadioInfo;
		adr.Group 	= nGrpRadioInfo;
		break;
	case ADR_IDUserID:
		adr.Adr		= nAddrUserID;
		adr.Len		= nLenUserID;
		adr.Group 	= nGrpUserID;
		break;
	case ADR_IDRadioMode:
		adr.Adr		= nAddrRadioMode;
		adr.Len		= nLenRadioMode;
		adr.Group 	= nGrpRadioMode;
		break;
	case ADR_IDEEPROMVER:
		adr.Adr		= nAddrEepromVer;
		adr.Len		= nLenEepromVer;
		adr.Group 	= nGrpEepromVer;
		break;

	case ADR_IDSoftPowerOn:
		adr.Adr		= nAddrSoftPowerOn;
		adr.Len		= nLenSoftPowerOn;
		adr.Group 	= nGrpSoftPowerOn;
		break;
	case nAddrBatteryConfig:
		adr.Adr		= nAddrBatteryCfg;	
		adr.Len		= nLenBatteryCfg;
		adr.Group		= nGrpBatteryCfg;
		break;

	case nAddr_CardUserPassword:
		adr.Adr		= nAddrCardUserPassword;	
		adr.Len		= nLenCardUserPassword;
		adr.Group		= nGrpCardUserPassword;
		break;
	case ADR_IDLocalUserInfo:
		adr.Adr		= nAddrLocalUserInfo;
		adr.Len		= nLenLocalUserInfo;
		adr.Group 	= nGrpLocalUserInfo;
		break;
	case ADR_IDWorkMode:
		adr.Adr		= nAddrWorkMode;	
		adr.Len		= nLenWorkMode;
		adr.Group		= nGrpWorkMode;
		break;
	case ADR_IDWarningTone:
		adr.Adr		= nAddrWarningTone;
		adr.Len		= nLenWarningTone;
		adr.Group 	= nGrpWarningTone;
		break;
	case ADR_IDSpeech:
		adr.Adr		= nAddrSpeech;
		adr.Len		= nLenSpeech;
		adr.Group 	= nGrpSpeech;
		break;
	case ADR_IDDisplay:
		adr.Adr		= nAddrDisplay;
		adr.Len		= nLenDisplay;
		adr.Group 	= nGrpDisplay;
		break;
	case ADR_IDPowerManage:
		adr.Adr		= nAddrPowerManage;
		adr.Len		= nLenPowerManage;
		adr.Group 	= nGrpPowerManage;
		break;
	case ADR_IDKeyProg:
		adr.Adr		= nAddrKeyProg;
		adr.Len		= nLenKeyProg;
		adr.Group 	= nGrpKeyProg;
		break;
	case ADR_IDTOT:
		adr.Adr		= nAddrTOT;
		adr.Len		= nLenTOT;
		adr.Group 	= nGrpTOT;
		break;
	case ADR_IDGpsFun:				//BEEP address process
		adr.Adr		= nAddrGpsFun;
		adr.Len		= nLenGpsFun;
		adr.Group 	= nGrpGpsFun;
		break;
	case nAddr_AudioConfig:
		adr.Adr		= nAddrAudioParam;	
		adr.Len		= 1;
		adr.Group	= 1;
		break;
	default:
		break;
	}
	return adr;
}

ADRLEN_GPSRecordInf CFG_GetGpsAdr(CFG_GpsInfoADRTYPE cId)
{
	ADRLEN_GPSRecordInf adr;

	switch(cId)
	{
		case nAddr_GpsInfoChecksum:
			adr.Adr	= 0;
			adr.Len	= 16;
			adr.Group	= 0x00;	
			break;
		case nAddr_GpsInfoBackupHead:
			adr.Adr	= 16;
			adr.Len	= 16;
			adr.Group	= 0x00;	
			break;
		case nAddr_GpsInfoBackup:
			adr.Adr	= 0x00000200;
			adr.Len	= 512;
			adr.Group	= 0x00;	
			break;
		default:
			break;
	}

	return adr;
}

/******************************************************************************
; Function Description	: Confing id identify process
; Enter Parameter		: cId	: get configne id value select
; Return Parameter		: get data infomation value
******************************************************************************/	
u8 CFG_GetValue(CFG_OPTTYPE cId)
{
	u16 i;
	u8 t,buf[16];
	ADRLEN_INF adr;
	
	i = 0x00;
	switch (cId)
	{
	case CFG_IDEEPMAX:						//get mac eeprom page
		break;
	case CFG_IDNEWOLD:						//new/old falg process
		adr  = CFG_GetCurAdr(ADR_IDEepromChk);
		i = (u8)(FILE_Read(adr.Adr,adr.Len,buf));	//read eeprom new flag
		if (i == TRUE)
		{
			for(t = 0; t < 16; t++)
			{
				if (buf[t] != CONFIG_NEWTBL[t])
				{
					i = FALSE;
					break;
				}
			}
		}
		break;
	case CFG_IDCHMAX:						//channel max value
		break;
	default:
		break;
	}
	return i;
}
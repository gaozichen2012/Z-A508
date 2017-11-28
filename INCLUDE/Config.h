#ifndef	CONFIG_H
#define CONFIG_H

#ifdef	CFGLABEL
	#define CFGAPI	
#else
	#define CFGAPI	extern
#endif

#define CFG_NEWADR			0x0010	//16  byte new/old flag address
#define CFG_IDENTIFY		0x0020	//16  byte
#define CFG_IDAGENT			0x0030	//16  byte agent id
#define CFG_MAXADR			0x0040	//16  byte max value address
#define CFG_MaxLen			0x0010
#define CFG_IDWRITE			0x0050	//16  byte 
#define CFG_IDDATE			0x0060	//16  byte read/write time
#define CFG_IDSTRING		0x0070	//112 byte
#define CFG_IDPASS			0x00E0	//32  byte
#define CFG_CH_DEBUG	 	0x0100	//32  byte test start address
#define CFG_TXFREQ_DEBUG 	0x0120	//32  byte (TX freq )
#define CFG_RXFREQ_DEBUG 	0x0140	//32  byte (RX freq )
#define CFG_WCTC_DEBUG	 	0x0160	//16  byte (ctcss/dcs freq) 
#define CFG_NCTC_DEBUG		0x0170	//16  byte (ctcss/dcs freq)

#define CFG_MODEL_NAME		0x1FE0	//12  byte product name,PC user
#define CFG_EEP_VERSION		0x1FEC	//4  //4 byte eeprom version,PC user
#define CFG_EEP_MODIFY		0x1FF0	//16  byte eeprom MODIFY FLAG,PC user

CFGAPI	ADRLEN_INF CFG_GetCurAdr(CFG_ADRTYPE cId);
CFGAPI	u8 CFG_GetValue(CFG_OPTTYPE cId);
extern ADRLEN_GPSRecordInf CFG_GetGpsAdr(CFG_GpsInfoADRTYPE cId);

#endif
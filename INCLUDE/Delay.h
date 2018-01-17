/******************************************************************************
; File Name		: Delay.h
; Author		: ZhangYaLing
; Version		: 1.0
; Description	: delay driver program process
; Date			: 2009.05.04
; Modify		:
******************************************************************************/
#ifndef	DELAY_H
#define DELAY_H

#ifdef	DELLABEL
	#define DELAPI
#else
	#define DELAPI	extern
#endif

DELAPI bool LockingState_Flag;
DELAPI u8 TimeCount;//³¬Ê±Ê±¼ä
DELAPI void DEL_PowerOnInitial(void);
DELAPI void DEL_Interrupt(void);
DELAPI void DEL_Renew(void);
DELAPI void DEL_Soft1ms(u16 iLen); 
DELAPI bool DEL_SetTimer(u8 cId,u16 iLen);
DELAPI bool DEL_GetTimer(u8 cId);
DELAPI void Delay_Test(void);


#endif
/******************************************************************************
;------------end page
******************************************************************************/

#ifndef __TASK_H
#define __TASK_H

#ifdef TASKABLE
#define TASKAPI
#else 
#define TASKAPI extern
#endif

typedef enum {					//task id code process
	TASK_IDLE 		= 0x00,
	TASK_ERROR		= 0x01,
	TASK_COPY		= 0x02,
	TASK_LOBATT 	= 0x03,
	TASK_ORDER 		= 0x04,
	TASK_SCAN		= 0x05,
	TASK_EMGE 		= 0x06,
	TASK_REPEAT		= 0x07,
	TASK_WRITEFREQ	= 0x08,
	TASK_mPowerOff	= 0x09,
	TASK_UpdataCdma	= 0x0A,
	TASK_HIBATT		= 0x0B,
	Task_Start              =0x0C,//Tom added in 2017.11.17
	TASK_RESET		= 0xFF
        
}TASK_CODE;

TASKAPI void Task_RunStart(void);
TASKAPI void TASK_WriteFreq(void);

#endif
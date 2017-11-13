#ifndef __APIDISPLAY_H
#define __APIDISPLAY_H

#include "stm8s.h"
#ifndef EXT_HZK
	#include "DrvGT20L16S1Y.h"
#endif
#include "DrvFont.h"

#define	DISP_IDLE	0x00
#define	DISP_RUN	0x01
#define	BASETYPE	0x0f
#define	NROWTYPE	0x10
#define	PAGE_MAX	0x04
#define	COL_MAX		0x80

#define LCD_DISP_LEN_MAX 	16

typedef struct
{
	DISP_TYPE DispType;
	u8 DispAddX;
	u8 DispAddY;
	u8 DispLenth;
}DISP_CHAR;
extern void api_disp_all_screen_refresh(void);
void api_disp_all_screen_refresh(void);
extern void api_lcd_pwr_on_hint(u8 *CharData);
#endif
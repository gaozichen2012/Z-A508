#include "AllHead.h"
#include <string.h>

/*-----------------------------------------------------------------------------
 Macro Define (macro definitions used in this file internally)
-----------------------------------------------------------------------------*/
const u8 CHAR_HIGH[2][8]  = { 0x02, 0x02, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02,
							  0x02, 0x02, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02 };

const u8 CHAR_WIDTH[2][8] = { 0x08, 0x10, 0x08, 0x08, 0x08, 0x08, 0x10, 0x10,
							  0x08, 0x10, 0x06, 0x08, 0x08, 0x08, 0x10, 0x10 };


#define DISPRELAOD_TIMER_PERIOD ( 10*OS_TICKS_PER_10MS )  /* 定时周期，每100ms回调一次函数 */
#define LCD_ALIAS_LEN_MAX 10
u16 CharCode;
typedef struct
{
	union
	{
		struct
		{
			u32	bRefresh	: 1;
			u32	bAllRef		: 1;
		}Bit;
		u32	Byte;
	}Msg;
}DIS_DRV;

//static DIS_DRV DisDrvObj;

static void DISP_DataBuf(DISP_CHAR DisInfo, u8 *CharData);
static void DISP_MulTypePro(DISP_CHAR CharInfo, u8 *CharData);
static void DISP_MulTypePro2(DISP_CHAR CharInfo, u8 *CharData);//UNICODE显示
/* ------------------------------------------------------------------------------- */
/* 因编写以上代码时,以下函数尚未定义，故临时定义（便于编译通过），后续请重新编写   */
/* ------------------------------------------------------------------------------- */
const u8 BitTab[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };


u8 DisDataBit[64]  = {0};
u8 DisDataBuf[512] = {0};
const u8 Ico_DataBuf[32][32]=
{
{0x00,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
 0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},

{0x00,0xF8,0x08,0xE8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},

{0x00,0xF8,0x08,0xE8,0x08,0xE8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},

/*{0X00,0X00,0XF0,0X08,0XE4,0X10,0X08,0X40,0XE0,0X40,0X08,0X10,0XE4,0X08,0XF0,0X00,
0X00,0X00,0X01,0X02,0X04,0X01,0X02,0X30,0X3E,0X30,0X02,0X01,0X04,0X02,0X01,0X00},*/

{0x00,0xF8,0x08,0xE8,0x08,0xE8,0x08,0xE8,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,   
0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},

{0x00,0xF8,0x08,0xE8,0x08,0xE8,0x08,0xE8,0x08,0xE8,0x08,0x08,0x08,0xF8,0xE0,0x00,
0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},

{0x00,0xF8,0x08,0xE8,0x08,0xE8,0x08,0xE8,0x08,0xE8,0x08,0xE8,0x08,0xF8,0xE0,0x00,
0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},
 
{0x00,0x00,0xE0,0x3C,0x22,0xA2,0xA2,0xA2,0xA2,0xA2,0x22,0x3C,0xE0,0x00,0x00,0x00,
0x00,0x00,0x0F,0x08,0x08,0x08,0x08,0x0B,0x08,0x08,0x08,0x08,0x0F,0x00,0x00,0x00},

{0x00,0x00,0xFC,0x0C,0x14,0x24,0x44,0x84,0x84,0x44,0x24,0x14,0x0C,0xFC,0x00,0x00,
0x00,0x00,0x07,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x07,0x00,0x00},
 
{0x00,0x02,0x0E,0x12,0x22,0xFE,0x22,0x12,0x8E,0x02,0x00,0xF0,0x00,0x00,0xFE,0x00,
0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x0F,0x00,0x00,0x0F,0x00,0x00,0x0F,0x00 },
 
{0x00,0x02,0x0E,0x12,0x22,0xFE,0x22,0x12,0x0E,0x02,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
 
{0x00,0x00,0xB8,0x30,0x28,0x04,0x02,0x02,0x02,0x02,0x02,0x04,0x84,0x88,0xB0,0x00,
0x00,0x00,0x01,0x02,0x04,0x04,0x08,0x08,0x08,0x08,0x08,0x04,0x02,0x01,0x03,0x00 },
 
{0x00,0x00,0xB8,0x30,0x28,0x04,0x42,0x42,0xF2,0x42,0x42,0x04,0x84,0x88,0xB0,0x00,
0x00,0x00,0x01,0x02,0x04,0x04,0x08,0x08,0x09,0x08,0x08,0x04,0x02,0x01,0x03,0x00 },
 
{0x00,0xF8,0x08,0xF8,0x04,0x02,0xFE,0x00,0x10,0xE0,0x08,0xF0,0x04,0xF8,0x00,0x00,
0x00,0x03,0x02,0x03,0x04,0x08,0x0F,0x00,0x01,0x00,0x02,0x01,0x04,0x03,0x00,0x00 },
 
{0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},//无发射无接收状态（空图标）
 
{0X00,0X7C,0X82,0X39,0X44,0X82,0X10,0XB8,0X10,0X82,0X44,0X39,0X82,0X7C,0X00,0X00,
0X00,0X00,0X00,0X01,0X00,0X00,0X0C,0X0F,0X0C,0X00,0X00,0X01,0X00,0X00,0X00,0X00},//发射新

{0X82,0X44,0X39,0X82,0X7C,0X00,0X10,0XB8,0X10,0X00,0X7C,0X82,0X39,0X44,0X82,0X00,
0X00,0X00,0X01,0X00,0X00,0X00,0X0C,0X0F,0X0C,0X00,0X00,0X00,0X01,0X00,0X00,0X00},//接收新

 {0X00,0X00,0X20,0X90,0X90,0X60,0X00,0X00,0XC0,0X20,0X10,0X10,0X10,0X20,0X00,0X00,
0X00,0X00,0X0C,0X08,0X08,0X07,0X00,0X00,0X03,0X04,0X08,0X09,0X09,0X0F,0X00,0X00},//3G

{0X00,0X00,0X20,0X10,0X10,0XE0,0X00,0X00,0XC0,0X20,0X10,0X10,0X10,0X20,0X00,0X00,
0X00,0X00,0X0C,0X0E,0X0B,0X08,0X00,0X00,0X03,0X04,0X08,0X09,0X09,0X0F,0X00,0X00},//2G

 
{0XC0,0XE0,0XEC,0XFE,0XFE,0XEC,0XC0,0X80,0XC0,0XEC,0XFE,0XFE,0XEC,0XE0,0XC0,0X00,
0X01,0X03,0X03,0X03,0X03,0X03,0X01,0X01,0X01,0X03,0X03,0X03,0X03,0X03,0X01,0X00},//组呼
 
{0X00,0X00,0X00,0X00,0XC0,0XE0,0XEC,0XFE,0XFE,0XEC,0XE0,0XC0,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X01,0X03,0X03,0X03,0X03,0X03,0X03,0X01,0X00,0X00,0X00,0X00},//个呼
 
{0x00,0x00,0x0C,0x12,0x12,0x0C,0xC0,0xF0,0x38,0x0C,0x0C,0x0C,0x18,0x30,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x0F,0x1C,0x30,0x30,0x30,0x18,0x0C,0x00,0x00 },
 
{0x00,0xF8,0x08,0xF8,0x04,0x02,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x03,0x02,0x03,0x04,0x08,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
 
{0x00,0x00,0x00,0x00,0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x03,0x0C,0x07,0x01,0x07,0x0C,0x03,0x00,0x00,0x00,0x00,0x00},
 
{0x00,0x00,0x00,0x00,0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x08,0x0F,0x08,0x00,0x03,0x04,0x0F,0x00,0x00,0x00,0x00,0x00
 },
 
{0x00,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
 0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},
 
{0x00,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
 0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},
 
{0x00,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
 0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},
 
{0x00,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
 0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},
 
{0x00,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
 0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},
 
{0x00,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
 0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},
 
{0x00,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
 0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},
 
{0x00,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
 0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},
};
/*******************************************************************************
* Description	: char-data display process
* Input			: CharInfo	: char data location information and dispaly char type
				: CharData	: display char data
* Output		: void
* Return		: void
* Others		: void
********************************************************************************/
void api_disp_char_output(DISP_CHAR CharInfo, u8 *CharData)
{	
	DISP_MulTypePro(CharInfo, CharData);
	return;
}
void api_disp_char_output2(DISP_CHAR CharInfo, u8 *CharData)//UNICODE显示，群组信息显示屏显示使用
{	
	DISP_MulTypePro2(CharInfo, CharData);
	return;
}
void api_lcd_pwr_on_hint(u8 *CharData)
{
	DISP_CHAR stCharInfo;
	stCharInfo.DispType  = DISP_IDCNASC816;
	//stCharInfo.DispAddX  = 0;
	stCharInfo.DispAddY  = 0x02;
	stCharInfo.DispAddX  = 0;//一行16个英文字符
        stCharInfo.DispLenth = LCD_DISP_LEN_MAX;
        
	api_disp_char_output(stCharInfo,CharData);

	//MCU_LCD_BACKLIGTH(OFF);
	api_disp_all_screen_refresh();// 全屏统一刷新
}
void api_lcd_pwr_on_hint2(u8 *CharData)
{
	DISP_CHAR stCharInfo;
	stCharInfo.DispType  = DISP_IDCNASC816B;
	//stCharInfo.DispAddX  = 0;
	stCharInfo.DispAddY  = 0x02;
	stCharInfo.DispAddX  = 5;//一行16个英文字符
        stCharInfo.DispLenth = LCD_DISP_LEN_MAX;
        
	api_disp_char_output(stCharInfo,CharData);

	//MCU_LCD_BACKLIGTH(OFF);
	api_disp_all_screen_refresh();// 全屏统一刷新
}
void api_lcd_pwr_on_hint3(u8 *CharData)
{
	DISP_CHAR stCharInfo;
	stCharInfo.DispType  = DISP_IDCNASC816;
	//stCharInfo.DispAddX  = 0;
	stCharInfo.DispAddY  = 0x00;//左上角显示汉字
	stCharInfo.DispAddX  = 0;//一行16个英文字符
        stCharInfo.DispLenth = LCD_DISP_LEN_MAX;
        
	api_disp_char_output(stCharInfo,CharData);

	//MCU_LCD_BACKLIGTH(OFF);
	api_disp_all_screen_refresh();// 全屏统一刷新
}
void api_lcd_pwr_on_hint4(u8 *CharData)//UNICODE显示
{
	DISP_CHAR stCharInfo;
	stCharInfo.DispType  = DISP_IDCNASC816;
	//stCharInfo.DispAddX  = 0;
	stCharInfo.DispAddY  = 0x02;//左上角显示汉字
	stCharInfo.DispAddX  = 3;//一行16个英文字符
        stCharInfo.DispLenth = LCD_DISP_LEN_MAX;
        
	api_disp_char_output2(stCharInfo,CharData);//UNICODE显示，群组信息显示屏显示使用

	//MCU_LCD_BACKLIGTH(OFF);
	api_disp_all_screen_refresh();// 全屏统一刷新
}
void api_lcd_pwr_on_hint5(u8 *CharData)//菜单页右上角序号
{
	DISP_CHAR stCharInfo;
	stCharInfo.DispType  = DISP_IDCNASC816;
	//stCharInfo.DispAddX  = 0;
	stCharInfo.DispAddY  = 0;//左上角显示汉字
	stCharInfo.DispAddX  = 13;//一行16个英文字符
        stCharInfo.DispLenth = LCD_DISP_LEN_MAX;
        
	api_disp_char_output(stCharInfo,CharData);

	//MCU_LCD_BACKLIGTH(OFF);
	api_disp_all_screen_refresh();// 全屏统一刷新
}
/*******************************************************************************
* Description	: 刷新屏幕数据显示
* Input		: void
* Output		: void
* Return		: void
* Others		: void
********************************************************************************/
void api_disp_all_screen_refresh(void)
{
	u8 Page, XAlias;
	u16 iPoint;

	for (Page = 0; Page < PAGE_MAX; Page++)
	{
		iPoint = Page * 0x80;
		drv_htg_set_address(0, Page);
		for (XAlias = 0; XAlias < COL_MAX; XAlias++)
		{
			drv_htg_write_data(DisDataBuf[iPoint]);
			iPoint++;
		}
	}
	//bDisDataBufUsed = FALSE;
}





/*******************************************************************************
* Description	: Mixed type display process
* Input			: CharInfo : char display location and type information
				: *CharData: display char data
* Output		: void
* Return		: void
* Others		: void
********************************************************************************/
static void DISP_MulTypePro(DISP_CHAR CharInfo, u8 *CharData)
{

  u8 CharCodeH;
  u8 CharCodeL;
	u16 CharCode;
	DISP_CHAR DisInfo;
	u8  iLen = 0;//34
        u8  CharBuf[34];
	DisInfo = CharInfo;

	if ((CharInfo.DispType & 0x80) != 0x00)
	{
		DisInfo.DispLenth = 0x00;
                
                

		for (; *CharData != 0x00; DisInfo.DispLenth++)
		{
			if (DisInfo.DispLenth >= CharInfo.DispLenth) { return; }//（修改当显示长度为16时的显示问题）
			CharCode = *CharData;

			DisInfo.DispAddX = CharInfo.DispAddX + iLen;
			DisInfo.DispType = (DISP_TYPE)(CharInfo.DispType & BASETYPE);
			
			if (*CharData >= 0x80)//为中文字符
			{
				iLen++;
				CharData++;
				CharCode <<= 0x08;
				CharCode |= (*CharData);
				DisInfo.DispType = DISP_IDCN1516;
				DisInfo.DispLenth++;//当显示中文，长度为16时的显示问题
                        CharCodeH=(CharCode&0xff00)>>8;
                        CharCodeL=CharCode&0x00ff;
			}
                        //GB2312_16_GetData(0xa3,0x65+0x80,CharBuf);//CharData[0]
                        //GB2312_16_GetData(0xa3,0x42+0x80,CharBuf);
			drv_gt20_data_output(DisInfo.DispType, CharCode, CharBuf);
                        
                        
                        //UNICODE_16_GetData(0xff42,CharBuf);
			DISP_DataBuf(DisInfo, CharBuf);
                        
			CharData++;
			iLen++;
		}
	}

	return;
}
static void DISP_MulTypePro2(DISP_CHAR CharInfo, u8 *CharData)//UNICODE显示
{
  u8  CharBuf2[32];
  DISP_CHAR DisInfo;
  u8  iLen = 0;//34
  DisInfo = CharInfo;
  if ((CharInfo.DispType & 0x80) != 0x00)
  {
    DisInfo.DispLenth = 0x00;
    for (; (*CharData != 0x00)||(*(CharData+1) != 0x00); DisInfo.DispLenth++)//添加*(CharData+1)判断，导致0x00也会进入循环，故在循环内处理0x0000
    {
      if (DisInfo.DispLenth >= CharInfo.DispLenth) { return; }//（修改当显示长度为16时的显示问题）
      CharCode = *CharData;
      if(CharCode!=0x0000)//解决0x00显示屏占半个字符的问题
      {
        DisInfo.DispAddX = CharInfo.DispAddX + iLen;
        DisInfo.DispType = (DISP_TYPE)(CharInfo.DispType & BASETYPE);
        if (*CharData >= 0x4E)//为中文字符//UNICODE范围：4E00-9FA5
        {
          iLen++;
          CharData++;
          CharCode <<= 0x08;
          CharCode |= (*CharData);
          DisInfo.DispType = DISP_IDCN1516;
          DisInfo.DispLenth++;//当显示中文，长度为16时的显示问题
        }
        drv_gt20_data_output2(DisInfo.DispType, CharCode, CharBuf2);
        DISP_DataBuf(DisInfo, CharBuf2);
        iLen++;
      }
      CharData++;
    }
  }
  return;
}
/*******************************************************************************
* Description	: display data process
* Input			: DisInfo : display char location and type information
				: *CharData: display char data
* Output		: void
* Return		: void
* Others		: void
********************************************************************************/
static void DISP_DataBuf(DISP_CHAR DisInfo, u8 *CharData)
{
	u16 iPt;
	u8  High, Width, xCol, yPage, nType, bType;
        //DisInfo.DispType = DISP_IDCN1516;//添加后数字和字母可以显示一半
	//bDisDataBufUsed = TRUE;
	yPage = DisInfo.DispAddY;
	nType = (DisInfo.DispType & NROWTYPE);//汉字：0x01&0x10=0 字母：0x05&0x10=0
	bType = (DisInfo.DispType & BASETYPE);//汉字：0x01&0x0f=1 字母：0x05&0x0f=5
	// if (DISP_OverBound(DisInfo) != TRUE) return;
	xCol  = (nType != 0x00) ? DisInfo.DispAddX : (DisInfo.DispAddX * 0x08);//一直取DisInfo.DispAddX * 0x08
	for (High = 0; High < CHAR_HIGH[0][bType]; High++)//汉字：CHAR_HIGH=2 字母：CHAR_HIGH=2
	{
		iPt = xCol + yPage * 0x80;//iPt为8*8像素
		for (Width = 0; Width < CHAR_WIDTH[0][bType]; Width++)//汉字：CHAR_WIDTH=16 字母：CHAR_WIDTH=8
		{
			if (iPt < 512)//修改当显示长度为16时的显示问题
			{
				if (DisDataBuf[iPt] != (*CharData))
				{
					if (iPt == 3*0x80)
					{
						iPt = iPt;
					}
					DisDataBuf[iPt] = *CharData;//[512]
					DisDataBit[iPt / 0x08] |= BitTab[iPt % 0x08];//[64]
				}
				CharData++;
				iPt++;
			}
		}
		yPage++;
	}
	//bDisDataBufUsed = FALSE;
}

//显示屏图标
/*******************************************************************************
* Description   : ico-id display process
* Input		: IcoInfo : ico data location information
		: IcoID : display ico-id
		: IcoDefault : ico display default location
* Output	: void
* Return	: void
* Others	: void
********************************************************************************/
void api_disp_icoid_output(u8 IcoIdIndex, bool IcoDefault,bool on_off)
{
	u16 IcoDataLen;
	ICO_INF	IcoInf;
	DISP_ICO IcoInfo;
	u8 IcoDataBuf[LCD_ICO_BUF_LEN];
        /* 两个字节存一个像素 */
        
        IcoDataLen = ((u16)(IcoInf.xLen) * (u16)(IcoInf.yLen)) / 0x08;
        if(IcoDataLen > LCD_ICO_BUF_LEN)
        {
          IcoDataLen = LCD_ICO_BUF_LEN;
        }
        if (on_off == TRUE)
        {
          //api_read_eeprom_data(IcoInf.iAdr, IcoDataLen, &IcoDataBuf[0]);
          memcpy(IcoDataBuf, &Ico_DataBuf[IcoIdIndex][0], LCD_ICO_BUF_LEN);
        }
        else
        {
          memset(&IcoDataBuf[0], 0x00, LCD_ICO_BUF_LEN);
        }
	if (IcoDefault == TRUE)	//使用默认图标位置配置信息
	{
          api_diap_ico_pos_get(&IcoInfo, IcoIdIndex);
	}
	
	api_disp_ico_output(IcoInfo, &IcoDataBuf[0]);
	//api_disp_all_screen_refresh();// 全屏统一刷新//屏蔽，添加此指令会使开机时获取群组名发生错误
	return;
}

/*******************************************************************************
* Description	: ico data display process
* Input			: IcoInfo : ico display location information
				: *IcoData: display ico data
* Output		: void
* Return		: void
* Others		: void
********************************************************************************/
void api_disp_ico_output(DISP_ICO IcoInfo, u8 *IcoData)
{
	u16 iPt;
	u8  High, Width, yPage, xAxis;
	u8 *ptr;
        
	yPage = IcoInfo.DispAddY;
	xAxis = IcoInfo.DispAddX * 0x08;
	ptr = IcoData;
	for (High = 0; High < IcoInfo.DispHigh / 0x08; High++)
	{
		iPt = xAxis + yPage * 0x80;
		for (Width = 0; Width < IcoInfo.DispWidth; Width++)
		{
			if (DisDataBuf[iPt] != (*ptr))
			{
				DisDataBuf[iPt] = *ptr;
				//DisDrvObj.Msg.Bit.bRefresh = DISP_RUN;
				DisDataBit[iPt / 0x08] |= BitTab[iPt % 0x08];
			}
			ptr++;
			iPt++;
		}
		yPage++;
	}
        
}
/*******************************************************************************
* Description	: ico default location display process
* Input			: IcoInfo : ico display location information
				: IcoID : ico-id
* Output		: void
* Return		: void
* Others		: void
********************************************************************************/
void api_diap_ico_pos_get(DISP_ICO *pIcoInfo, u16 IcoID)
{
	switch (IcoID)
	{

	case eICO_IDRXFULL://GPRS三格信号图标
	case eICO_IDRXNULL://GPRS无信号图标
		pIcoInfo->DispAddX = 0x00;
		break;
	case eICO_IDEmergency://图标：3G
	case eICO_IDPOWERL://图标：2G
		pIcoInfo->DispAddX = 0x02;
		break;
	case eICO_IDPOWERM://图标：组呼
	case eICO_IDPOWERH://图标：个呼
		pIcoInfo->DispAddX = 0x0c;//0x04
		break;
	case eICO_IDLOCKED://锁屏图标
	case eICO_IDLOCKEDOff://空电池图标
		pIcoInfo->DispAddX = 0x06;
		break;
	case eICO_IDTALKAR://无发射无接收状态（无图标）
        case eICO_IDTX://发射信号图标
	case eICO_IDVOX://图标：接收，听状态
          pIcoInfo->DispAddX = 0x08;
          break;
        case eICO_IDSCAN://刷新图标
	case eICO_IDSCANPA://图标：刷新园+加号
	case eICO_IDSCANOff://空电池图标
	case eICO_IDBANDWIDTHW://图标：W
	case eICO_IDBANDWIDTHN://图标：N
	case eICO_IDVOXOff://空电池图标
	case eICO_IDSPEAKER://喇叭图标-------------------
	case eICO_IDSPEAKEROff://空电池图标
	case eICO_IDMONITER://喇叭无声音图标
	case eICO_IDMONITEROff://空电池图标

		
	case eICO_IDMESSAGE://邮件图标
	case eICO_IDMESSAGEOff://空电池图标	
		pIcoInfo->DispAddX = 0x0a;
		break;
		
	
	case eICO_IDTALKAROff://空电池图标
		pIcoInfo->DispAddX = 0x0c;
		break;
		
	case eICO_IDBATT://空电池图标
	case eICO_IDBATT1:
	case eICO_IDBATT2:
	case eICO_IDBATT3:
	case eICO_IDBATT4:
	case eICO_IDBATT5://--------------------
		pIcoInfo->DispAddX = 0x0e;
		break;
		
	default:
		break;
	}
	
	pIcoInfo->DispAddY = 0x00;
	pIcoInfo->DispWidth= 0x10;
	pIcoInfo->DispHigh = 0x10;
}
//附：从字库中读数据函数说明 u8 r_dat_bat(u32 address,u8 byte_long,u8 *p_arr)实现参考代码。
/****************************************************
u8 r_dat_bat(u32 address,u8 byte_long,u8 *p_arr)
Address  ： 表示字符点阵在芯片中的字节地址。
byte_long： 是读点阵数据字节数。
*p_arr   ： 是保存读出的点阵数据的数组。
*****************************************************/
//u8 r_dat_bat(u32 address,u8 byte_long,u8 *p_arr)
unsigned char r_dat_bat(unsigned long address,unsigned long byte_long,unsigned char *p_arr)
{
	unsigned int j=0;
	MCU_GT20_CS(LO);
	SendByte(address); //发送指令及地址
	for(j=0;j<byte_long;j++)
	{
	 p_arr[j]=ReadByte();//读取数据到数组中
	}
	MCU_GT20_CS(HI);
	return p_arr[0];	
}
void SendByte(u32 cmd)
{
	u8 i;
	cmd=cmd|0x03000000;
	for(i=0;i<32;i++)
	{
		MCU_GT20_CLK(LO);
		if(cmd&0x80000000)
			MCU_GT20_SI(HI);
		else 
			MCU_GT20_SI(LO);
		MCU_GT20_CLK(HI);
		cmd=cmd<<1;
	}					
}

u8 ReadByte(void)
{
	u8 i;
	u8 dat=0;
	MCU_GT20_CLK(HI); 	
	for(i=0;i<8;i++)
	{
		MCU_GT20_CLK(LO); 
		dat=dat<<1;
		if(MCU_GT20_SO_Read)
			dat=dat|0x01;
		else 
			dat&=0xfe;
		MCU_GT20_CLK(HI); 	
	}	
	return dat;
        
        

}

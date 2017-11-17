#ifndef __ALLHEAD_H
#define __ALLHEAD_H
//
#include "stm8s.h"
#include "System.h"
#include "sysclock.h"
#include "McuConfig.h"
#include "stm8s_it.h"
#include "CommonLib.h"
#include "Define.h"// 
#include "Delay.h"
//通讯模块
#include "DrvGD83.h"
#include <string.h>
#include "ApiPocCmd.h"
#include "ApiAtCmd.h"

//键盘
#include "DrvKeyboard.h"
#include "ApiKeyboard.h"

//显示屏
#include "ApiDisplay.h"
#include "DrvHTG12832.h"
#include "DrvGT20L16S1Y.h"

//BEEP
#include "Beep.h"
#include "Tone.h"

//任务
#include "Task.h"

#endif
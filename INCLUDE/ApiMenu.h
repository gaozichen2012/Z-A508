#ifndef __APIMENU_H
#define __APIMENU_H

typedef enum{
  Menu0             = 0x00,//菜单标志位=0
  Menu1             = 0x01,
  Menu2             = 0x02,
  Menu3             = 0x03,
  Menu4             = 0x04,
  Menu5             = 0x05,
  Menu6             = 0x06,
  Menu7             = 0x07,
  Menu8             = 0x08,
  Menu_Locking_NoOperation    =0x09,
  Menu_unLocking              =0x0A,
  Menu_UnlockStep1_Ok         = 0x0F,
}MenuDisplayType;

extern void MenuDisplay(MenuDisplayType id);
#endif
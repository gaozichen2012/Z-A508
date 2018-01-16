#ifndef __APIMENU_H
#define __APIMENU_H

typedef enum{
  Menu0             = 0x00,//菜单标志位=0
  Menu1             = 0x01,
  Menu2             = 0x02,
  Menu3             = 0x03,
  Menu4             = 0x04,
  Menu_Lock         = 0x0F,
}MenuDisplayType;

extern void MenuDisplay(MenuDisplayType id);
#endif
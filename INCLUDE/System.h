#ifndef	__SYSTEM_H
#define __SYSTEM_H

#ifdef	SYSLABLE
	#define SYSAPI
#else
	#define SYSAPI	extern
#endif

SYSAPI	void main_app(void);

#endif
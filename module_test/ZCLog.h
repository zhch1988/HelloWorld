#ifndef _ZCLOG_H_
#define _ZCLOG_H_

//#include <stdio.h>

#define ZCLOG
#ifdef ZCLOG
#define ZCPRINT(format, ...) printk(KERN_INFO "[ZC]" format, ##__VA_ARGS__)
#else
#define ZCPRINT(format, ...)
#endif

static inline void printBuf(const char* buf, unsigned int len, const char* title)
{
	int i;
	char printBuf[len * 6 + 1];
	sprintf(printBuf, "");
	for(i = 0; i < len; i++)
	{
		sprintf(printBuf, "%s0x%02x ", printBuf, buf[i]);
	}
	ZCPRINT("%s[%d]: %s\n", title, len, printBuf);
}

#endif /* _ZCLOG_H_ */
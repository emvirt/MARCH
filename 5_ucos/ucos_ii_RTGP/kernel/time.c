#include <ucos_ii.h>

#define __raw_writeb(v,a)       (*(volatile unsigned char *)(a) = (v))
#define __raw_writew(v,a)       (*(volatile unsigned short *)(a) = (v))
#define __raw_writel(v,a)       (*(volatile unsigned int *)(a) = (v))

#define __raw_readb(a)          (*(volatile unsigned char *)(a))
#define __raw_readw(a)          (*(volatile unsigned short *)(a))
#define __raw_readl(a)          (*(volatile unsigned int *)(a))


#define TZIC0_BASE_ADDR				(0xE5000000)

#define TZIC_UNLOCK_CODE			(0xACCE550)

#define IRQ_SYSTEM_TIMER_MASK		(0x04000000)

#define oINTEN				(0x08)		// TZIC INT EN
#define oFIQEN				(0x0C)		// TZIC FIQ EN
#define oFIQENCLEAR			(0x10)		// TZIC FIQ EN CLEAR
#define oLOCKEN				(0x1C)		// TZIC LOCK EN


extern void s5pc1xx_timer_clear(void);

void timer_handler(void)
{
//	__raw_writel(TZIC_UNLOCK_CODE, TZIC0_BASE_ADDR+oLOCKEN);
//	__raw_writel(IRQ_SYSTEM_TIMER_MASK, TZIC0_BASE_ADDR+oFIQENCLEAR);
	tegra_timer_clear();
//	OSIntEnter();
	OSTimeTick();
	//OS_Sched();
//	OSIntExit();
//	__raw_writel(IRQ_SYSTEM_TIMER_MASK, TZIC0_BASE_ADDR+oFIQEN);
//	__raw_writel(0x1, TZIC0_BASE_ADDR+oLOCKEN);
}

#include "arm_arch_timer.h"
#include "arch_timer.h"
#include "ucos_ii.h"


#define CLOCK_EVT_MODE_UNUSED 0
#define CLOCK_EVT_MODE_SHUTDOWN 1


#ifdef PROFILE_TIMER_INTERVAL

#define TIMER_INTERVAL_TABLE_SIZE 1000

INT32U timer_interval_h = 0;
INT32U timer_interval_l = 0;

INT32U time_interval_table[TIMER_INTERVAL_TABLE_SIZE][2];

INT32U loop_count = 0;

#endif


void arndale_timer_handler()
{
	unsigned long ctrl;

#ifdef PROFILE_TIMER_INTERVAL


	__asm("MRRC p15, 0, %0, %1, c14":"=r"(timer_interval_l),"=r"(timer_interval_h):);

	if( loop_count < TIMER_INTERVAL_TABLE_SIZE ){
                time_interval_table[loop_count][0] = timer_interval_h;
                time_interval_table[loop_count][1] = timer_interval_l;

		loop_count++;
        }else{
                __asm("BKPT");
        }



#endif

        ctrl = arch_timer_reg_read(ARCH_TIMER_VIRT_ACCESS, ARCH_TIMER_REG_CTRL);
        if (ctrl & ARCH_TIMER_CTRL_IT_STAT) {
                ctrl |= ARCH_TIMER_CTRL_IT_MASK;
                arch_timer_reg_write(ARCH_TIMER_VIRT_ACCESS, ARCH_TIMER_REG_CTRL, ctrl);
		OSTimeTick();
                //ucos_TMR_handler();
                //evt->event_handler(evt);
                //return IRQ_HANDLED;
        }

        //return IRQ_NONE;
}
static inline void timer_set_mode(const int access, int mode)
{
        unsigned long ctrl;
        switch (mode) {
        case CLOCK_EVT_MODE_UNUSED:
        case CLOCK_EVT_MODE_SHUTDOWN:
                ctrl = arch_timer_reg_read(access, ARCH_TIMER_REG_CTRL);
                ctrl &= ~ARCH_TIMER_CTRL_ENABLE;
                arch_timer_reg_write(access, ARCH_TIMER_REG_CTRL, ctrl);
                break;
        default:
                break;
        }
}

static void arch_timer_set_mode_virt(int mode)
{
        timer_set_mode(ARCH_TIMER_VIRT_ACCESS, mode);
}

void arndale_arch_timer_init()
{
	arch_timer_set_mode_virt(CLOCK_EVT_MODE_SHUTDOWN);
}

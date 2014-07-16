/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                                 (c) Copyright 2004, Micrium, Weston, FL
*                                          All Rights Reserved
*
*                                            Freescale i.MX21ADS
*                                              Sample code
* File : APP.C
* By   : Eric Shufro
*
* Xen-Arm version by OS LAB, Korea University (mrpark@os.korea.ac.kr)
*********************************************************************************************************
*/


#include <includes.h>
#include <net/net.h>

#include <os/lib.h>

#include <trustzone/switch.h>
#include <trustzone/communication.h>
#include "arm-gic.h"
#include "io.h"


#ifdef PROFILE_REALTIME_SCHEDULE

#define SYSTEM_TIME_MONITOR_SIZE 5000

INT32U task1_start_p_time_h = 0;
INT32U task1_start_p_time_l = 0;
INT32U task1_end_p_time_h = 0;
INT32U task1_end_p_time_l = 0;
INT32U task1_start_v_time_h = 0;
INT32U task1_start_v_time_l = 0;
INT32U task1_end_v_time_h = 0;
INT32U task1_end_v_time_l = 0;

INT32U task2_start_p_time_h = 0;
INT32U task2_start_p_time_l = 0;
INT32U task2_end_p_time_h = 0;
INT32U task2_end_p_time_l = 0;
INT32U task2_start_v_time_h = 0;
INT32U task2_start_v_time_l = 0;
INT32U task2_end_v_time_h = 0;
INT32U task2_end_v_time_l = 0;


INT32U task_time_table[SYSTEM_TIME_MONITOR_SIZE][10];

INT32U time_table_index = 0;


#endif



extern void trustzone_initialize(void);

extern void tegra_timer_init(void);
extern void arndale_arch_timer_init();
#define PUSH_DEAD 0

/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskStart(void *p_arg);
static  void  AppTaskCreate(void);

#if (uC_PROBE_OS_PLUGIN > 0)
static  void  AppProbeCallback(void);
#endif

//#if uC_TCPIP_MODULE > 0
void AppInit_TCPIP();
//#endif

/*
*********************************************************************************************************
*                                               VARIABLES
*********************************************************************************************************
*/

static  OS_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];

#if uC_TCPIP_MODULE > 0
        //NET_IP_ADDR   ip;
        //NET_IP_ADDR   msk;
        //NET_IP_ADDR   gateway;
        //NET_ERR       err;
#endif


/*
*********************************************************************************************************
*                                             C ENTRY POINT
*********************************************************************************************************
*/
int QbSerialProbe();

int sta_ker(void)
{
	int i;
        CPU_INT08U  err;


	local_irq_disable();			

        
	trustzone_initialize();
   	OSInit();                                                           /* Initialize uC/OS-II                                      */
                                                                     /* Create start task    */  

   	OSTaskCreateExt(AppTaskStart,
                   NULL,
                   (OS_STK *)&AppTaskStartStk[APP_TASK_START_STK_SIZE - 1],
                   APP_TASK_START_PRIO,
                   APP_TASK_START_PRIO,
                   (OS_STK *)&AppTaskStartStk[0],
                   APP_TASK_START_STK_SIZE,
                   NULL,
                   OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

        
        /* Assign names to created tasks                            */
#if OS_TASK_NAME_SIZE > 11
   	OSTaskNameSet(APP_TASK_START_PRIO, (INT8U *)"Start Task", (INT8U *)&err);
#endif


	// TODO 
	
	local_irq_enable();

	OSStart();                                                          /* Start uC/OS-II                                           */

	return 0;
	
}





/*$PAGE*/
/*
*********************************************************************************************************
*                                              STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
* Arguments   : p_arg is the argument passed to 'AppStartTask()' by 'OSTaskCreate()'.
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*               2) Interrupts are enabled once the task start because the I-bit of the CCR register was
*                  set to 0 by 'OSTaskCreate()'.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    (void)p_arg;                                                        /* Prevent compiler warning                                 */

    AppTaskCreate();                                                    /* Create additional applicaiton tasks                      */
    

    while (DEF_TRUE) {                                                  /* Task body, always written as an infinite loop.           */
      OSTimeDlyHMSM(1, 0, 0, 500);
    
    }
}

/*
*********************************************************************************************************
*                              CREATE ADDITIONAL APPLICATION TASKS
static  void  AppTaskCreate (void)
{
    return;
}
*********************************************************************************************************
*/

#include <ucos_ii.h>

#define N_TASKS	2   
//jjw_org ::#define TASK_STK_SIZE	0x500
#define N_FRAMES    0x100
#define N_BLOCKS    0x10
#define PRODUCER_PRIO	1
#define CONSUMER_PRIO	0

// Default data Structure
//jjw_org :: OS_STK		    TaskStk[N_TASKS][TASK_STK_SIZE];
/*INT32S		    result;
OS_EVENT	    *prod_sem;
OS_EVENT	    *fail_sem;
OS_EVENT	    *begin_sem;
INT32U		    fail;
INT32S		    base[16][16];
INT32S		    target[N_FRAMES][N_BLOCKS][16][16];
INT32S		    src[N_FRAMES][N_BLOCKS][16][16];
INT32S		    res_tick_diff[0x10];
INT32S		    total_count;
INT32S		    fail_count;
INT32S		    begin;*/

/* 
* Traditional Producer/Consumer example
* producer 0 
* consumer 1
*/
//extern void Producer(void *pdata);
//extern void Consumer(void *pdata);


#define TASK_STK_SIZE   0x400
OS_STK  FFT_STK[TASK_STK_SIZE];
OS_STK  FFT_STK2[TASK_STK_SIZE];
//#define PRIO_FFT       OS_LOWEST_PRIO+1 
#define PRIO_FFT       0 

#define TEST1_PERIOD 50 
#define TEST1_EXCUTION 7
#define TEST2_PERIOD 75
#define TEST2_EXCUTION 9


	INT32U TEST2_START_CLK;
	INT32U TEST1_START_CLK ;
	INT32U COUNT_CLK ;
void test1()
{
	INT32U PREV_PERIOD;
	INT32U CUR_PERIOD;

	INT32U end_clk = 0;
	INT32U count_clk = 0;
	INT32U count = 0;
	INT32U temp=1;
	INT32U ALL_TEST_DID;

	volatile INT32U systime_offset;	
	
	systime_offset = Physical_Time;
	//Init OS time tick to 0
	OSTimeSet(0);
	
	temp = OSTimeGet();
	TEST1_START_CLK = temp;
	
	PREV_PERIOD = temp;
	
	//TEST2_START_CLK = OSTimeGet();
	//TEST1_START_CLK = TEST2_START_CLK;
	
	for(;;)
	{
	
	
		temp = OSTimeGet();//TEST1_START_CLK; 
		
		CUR_PERIOD = PREV_PERIOD + TEST1_PERIOD;
		TEST1_START_CLK = temp;
#ifdef PROFILE_REALTIME_SCHEDULE

	__asm("MRRC p15, 0, %0, %1, c14":"=r"(task1_start_p_time_l),"=r"(task1_start_p_time_h):);
	__asm("MRRC p15, 1, %0, %1, c14":"=r"(task1_start_v_time_l),"=r"(task1_start_v_time_h):);

	if(time_table_index < SYSTEM_TIME_MONITOR_SIZE ){
		task_time_table[time_table_index][0] = 1;
		task_time_table[time_table_index][1] = task1_start_p_time_h;
		task_time_table[time_table_index][2] = task1_start_p_time_l;
		task_time_table[time_table_index][3] = task1_start_v_time_h;
		task_time_table[time_table_index][4] = task1_start_v_time_l;
	}else{
		__asm("BKPT");
	}

#endif
		while(	count < TEST1_EXCUTION)
		{
			//workload
			if(temp != count_clk)
			{
				count++;
				count_clk = temp;
				 	
			}
			//workload
			temp = OSTimeGet();//TEST1_START_CLK; 
		}

#ifdef PROFILE_REALTIME_SCHEDULE

	__asm("MRRC p15, 0, %0, %1, c14":"=r"(task1_end_p_time_l),"=r"(task1_end_p_time_h):);
	__asm("MRRC p15, 1, %0, %1, c14":"=r"(task1_end_v_time_l),"=r"(task1_end_v_time_h):);

        if(time_table_index < SYSTEM_TIME_MONITOR_SIZE ){
                task_time_table[time_table_index][5] = task1_end_p_time_h;
                task_time_table[time_table_index][6] = task1_end_p_time_l;
                task_time_table[time_table_index][7] = task1_end_v_time_h;
                task_time_table[time_table_index][8] = task1_end_v_time_l;
        }

#endif

		count = 0;
		
		//get end_clk
		end_clk = temp;
		
		
		PREV_PERIOD = CUR_PERIOD;

		//end_clk = Physical_Time;
//jjw
		if ((INT32S)(CUR_PERIOD - end_clk) >= 0) {

#ifdef PROFILE_REALTIME_SCHEDULE

	if(time_table_index < SYSTEM_TIME_MONITOR_SIZE ){
	
		task_time_table[time_table_index][9] = CUR_PERIOD - end_clk;		
		time_table_index++;

	}

#endif 

			OSTimeDly(CUR_PERIOD - end_clk);
		} else {

		#if PUSH_DEAD
			TEST1_START_CLK = Physical_Time;
		#endif
		}
	}
}


void test2()
{
	INT32U end_clk = 0;		// end of time of workload
	INT32U count_clk = 0;		// compare with temp because we check tick counter
	INT32U count = 0;		// real execution tick counter 
	INT32U temp=1;
	INT32U ALL_TEST_DID;
	INT32U CUR_PERIOD;
	INT32U PREV_PERIOD;

	temp = OSTimeGet();
	count_clk = temp;

	TEST2_START_CLK = temp;
	PREV_PERIOD = temp;

	for(;;)
	{
		temp = OSTimeGet();
		CUR_PERIOD = PREV_PERIOD + TEST2_PERIOD;
		TEST2_START_CLK = temp;
//		printk("2: second start : %d incoming %d virtual %d\n",TEST2_START_CLK,temp,virtual_time);


#ifdef PROFILE_REALTIME_SCHEDULE

        __asm("MRRC p15, 0, %0, %1, c14":"=r"(task2_start_p_time_l),"=r"(task2_start_p_time_h):);
        __asm("MRRC p15, 1, %0, %1, c14":"=r"(task2_start_v_time_l),"=r"(task2_start_v_time_h):);

        if(time_table_index < SYSTEM_TIME_MONITOR_SIZE ){
                task_time_table[time_table_index][0] = 2;
                task_time_table[time_table_index][1] = task2_start_p_time_h;
                task_time_table[time_table_index][2] = task2_start_p_time_l;
                task_time_table[time_table_index][3] = task2_start_v_time_h;
                task_time_table[time_table_index][4] = task2_start_v_time_l;
        }else{
                __asm("BKPT");
        }

#endif


		while(	count < TEST2_EXCUTION)
		{
			//workload
			if(temp != count_clk)
			{
				count++;		//pasted tick
				count_clk = temp;	
//				printk("2: my second test:: virtual : %d count : %d systemtime: %d \n", virtual_time, count, Physical_Time);
				// struct vcpu_time_info *src = &HYPERVISOR_shared_info->vcpu_info[0].time;
			}
			//workload

			temp = OSTimeGet();
			
		}	

#ifdef PROFILE_REALTIME_SCHEDULE

        __asm("MRRC p15, 0, %0, %1, c14":"=r"(task2_end_p_time_l),"=r"(task2_end_p_time_h):);
        __asm("MRRC p15, 1, %0, %1, c14":"=r"(task2_end_v_time_l),"=r"(task2_end_v_time_h):);

        if(time_table_index < SYSTEM_TIME_MONITOR_SIZE ){
                task_time_table[time_table_index][5] = task2_end_p_time_h;
                task_time_table[time_table_index][6] = task2_end_p_time_l;
                task_time_table[time_table_index][7] = task2_end_v_time_h;
                task_time_table[time_table_index][8] = task2_end_v_time_l;
        }

#endif
	
		count = 0;

		end_clk = temp;
	
		ALL_TEST_DID = end_clk - TEST2_START_CLK;		
		PREV_PERIOD = CUR_PERIOD;

		TEST2_START_CLK += TEST2_PERIOD;
//jjw
		//end_clk = Physical_Time;
		if ((INT32S)(CUR_PERIOD - end_clk) >= 0) {

#ifdef PROFILE_REALTIME_SCHEDULE

        if(time_table_index < SYSTEM_TIME_MONITOR_SIZE ){

                task_time_table[time_table_index][9] = CUR_PERIOD - end_clk;
                time_table_index++;

        }

#endif


			OSTimeDly(CUR_PERIOD - end_clk);
		} else {
			//printk("2: -------->\t deadline miss! end_clk %d deadline %d \n", end_clk, TEST2_START_CLK);
		#if PUSH_DEAD
			TEST2_START_CLK = Physical_Time;
//		#else
//			OSTimeDly(TEST2_PERIOD - ALL_TEST_DID +1 );
		#endif
		}
	}
}
static  void  AppTaskCreate (void)
{
// simple main_gsm function call

	OSSchedLock();
	virtual_time = OSTime;	
	OSTaskCreate(test2,NULL , &FFT_STK2[TASK_STK_SIZE-1], PRIO_FFT+1);
	OSTaskCreate(test1,NULL , &FFT_STK[TASK_STK_SIZE-1], PRIO_FFT);
	OSSchedUnlock();

//jjw

}




/*
*********************************************************************************************************
*                                      AppInit_TCPIP()
*
* Description : This function is called by AppTaskStart() and is responsible for initializing uC/TCP-IP
*               uC/HTTPs, uC/TFTPs and uC/DHCPc if enabled.
*********************************************************************************************************
*/

#if 0
#if uC_TCPIP_MODULE > 0
void AppInit_TCPIP (void)
{
#if EMAC_CFG_MAC_ADDR_SEL == EMAC_MAC_ADDR_SEL_CFG
/*  
    NetIF_MAC_Addr[0] = 0x00;
    NetIF_MAC_Addr[1] = 0x50;
    NetIF_MAC_Addr[2] = 0xC2;
    NetIF_MAC_Addr[3] = 0x25;
    NetIF_MAC_Addr[4] = 0x60;
    NetIF_MAC_Addr[5] = 0x01;
*/
    NetIF_MAC_Addr[0] = 0x00;
    NetIF_MAC_Addr[1] = 0x04;
    NetIF_MAC_Addr[2] = 0x9F;
    NetIF_MAC_Addr[3] = 0x00;
    NetIF_MAC_Addr[4] = 0x59;
    NetIF_MAC_Addr[5] = 0xD4;
    
#endif

    err     = Net_Init();                                               /* Initialize uC/TCP-IP                                     */

    ip      = NetASCII_Str_to_IP("192.168.0.2", &err);                  /* Configure the IP address                                 */
    msk     = NetASCII_Str_to_IP("255.255.255.0", &err);                /* Configure the netmask                                    */
    gateway = NetASCII_Str_to_IP("192.168.0.1", &err);                /* Configure the default gateway                            */
    
    err     = NetIP_CfgAddrThisHost(ip, msk);
    err     = NetIP_CfgAddrDfltGateway(gateway); 
}
#endif

#endif


/*
*********************************************************************************************************
*                                         uC/Probe Callback
*
* Description : This task is called by the uC/Probe uC/OS-II plug-in after updating task information.
*
* Arguments   : none.
*
* Returns     : none
*********************************************************************************************************
*/

#if (uC_PROBE_OS_PLUGIN > 0)
static  void  AppProbeCallback (void)
{
}
#endif

/*
*********************************************************************************************************
*                                          App_TaskCreateHook()
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

#if (OS_APP_HOOKS_EN > 0)
void  App_TaskCreateHook (OS_TCB *ptcb)
{
#if (uC_PROBE_OS_PLUGIN > 0) && (OS_PROBE_HOOKS_EN > 0)
    OSProbe_TaskCreateHook(ptcb);
#endif
}

/*
*********************************************************************************************************
*                                           App_TaskDelHook()
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void  App_TaskDelHook (OS_TCB *ptcb)
{
    (void)ptcb;
}

/*
*********************************************************************************************************
*                                             App_TaskIdleHook()
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do  
*              such things as STOP the CPU to conserve power.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are enabled during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 251
void  App_TaskIdleHook (void)
{
}
#endif

/*
*********************************************************************************************************
*                                   App_TaskStatHook()
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your 
*              application to add functionality to the statistics task.
*
* Returns    : none
*********************************************************************************************************
*/

void  App_TaskStatHook (void)
{
}

/*
*********************************************************************************************************
*                                           App_TaskSwHook()
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the 
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/

//#if OS_TASK_SW_HOOK_EN > 0
void  App_TaskSwHook (void)
{
	
//	printk("switch in prio : %d\n",OSTCBCur->OSTCBPrio);
	//printk("switch in prio : %d, switch out prio : %d\n",OSTCBHighRdy->OSTCBPrio,OSTCBCur->OSTCBPrio);
	
#if (uC_PROBE_OS_PLUGIN > 0) && (OS_PROBE_HOOKS_EN > 0)
    OSProbe_TaskSwHook();
#endif
}
//#endif

/*
*********************************************************************************************************
*                                           App_TCBInitHook()
*
* Description: This function is called by OS_TCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 204
void  App_TCBInitHook (OS_TCB *ptcb)
{
    (void)ptcb;
}
#endif

/*
*********************************************************************************************************
*                                               App_TimeTickHook()
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if OS_TIME_TICK_HOOK_EN > 0
void  App_TimeTickHook (void)
{

#if (uC_PROBE_OS_PLUGIN > 0) && (OS_PROBE_HOOKS_EN > 0)
    OSProbe_TickHook();
#endif
}
#endif
#endif

/*
*********************************************************************************************************
*                                          EXCEPTION HANDLER
*
* Description : This function should be used to handle any exceptions.  It is called by
*               OS_CPU_ARM_ExceptHndlr(), which is declared in os_cpu_a.asm
*
* Arguments   : ID, an identifier used to indicate what type of ARM exception has been triggered
*               Possible ID values are shown below.
*                  OS_CPU_ARM_EXCEPT_RESET             0x00
*                  OS_CPU_ARM_EXCEPT_UNDEF_INSTR       0x01
*                  OS_CPU_ARM_EXCEPT_SWI               0x02
*                  OS_CPU_ARM_EXCEPT_PREFETCH_ABORT    0x03
*                  OS_CPU_ARM_EXCEPT_DATA_ABORT        0x04
*                  OS_CPU_ARM_EXCEPT_ADDR_ABORT        0x05
*                  OS_CPU_ARM_EXCEPT_IRQ               0x06
*                  OS_CPU_ARM_EXCEPT_FIQ               0x07
*
* Notes       : The ISR handler being called MUST clear the AIC by clearing any external sources and
*               then writting the AIC ICCR and EOICR registers.
*********************************************************************************************************
*/

#define __raw_writeb(v,a)       (*(volatile unsigned char *)(a) = (v))
#define __raw_writew(v,a)       (*(volatile unsigned short *)(a) = (v))
#define __raw_writel(v,a)       (*(volatile unsigned int *)(a) = (v))

#define __raw_readb(a)          (*(volatile unsigned char *)(a))
#define __raw_readw(a)          (*(volatile unsigned short *)(a))
#define __raw_readl(a)          (*(volatile unsigned int *)(a))


#define TZIC0_BASE_ADDR				(0xE5000000)

#define IRQ_SYSTEM_TIMER_MASK		(0x04000000)

#define TZIC_UNLOCK_CODE			(0xACCE550)

#define oSTATUS				(0x00)		// TZIC INT STAT
#define oLOCKEN				(0x1C)		// TZIC LOCK EN

#define TZPCR0SIZE					0xE3800000

extern void timer_handler(void);
extern void tz_communication_handler(void);

void OS_CPU_ExceptHndlr(CPU_DATA  except_id)
{
	void * gic_dist_base = 0x10481000;
	unsigned int pending_status = 0;
	unsigned int active_status = 0;
	//disable gic
	writel_relaxed(0, gic_dist_base + GIC_DIST_CTRL);

	//read pending bit
	pending_status = readl_relaxed(gic_dist_base +GIC_DIST_PENDING_SET );

	active_status = readl_relaxed(gic_dist_base +GIC_DIST_ACTIVE_SET );	

		arndale_timer_handler();


	//enable gic
	writel_relaxed(0, gic_dist_base + GIC_DIST_CTRL);
}

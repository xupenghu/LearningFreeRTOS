/*
 * FreeRTOS Kernel V10.1.0
 * Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/* Ensure stdint is only used by the compiler, and not the assembler. */
#if defined (__ICCARM__) ||  defined(__CC_ARM) || defined (__GNUC__)
	#include <stdint.h>
	extern uint32_t SystemCoreClock;
#endif
/* 为1时，rtos使用抢占式调度器； 为0时rtos使用协作式调度器(时间片) 
*  协作式操作系统的调度是任务主动释放cpu后，切换到下一个任务，任务切换的时机取决于当前任务什么时候结束*/
#define configUSE_PREEMPTION			1
/* 使用空闲钩子函数开启宏 */
#define configUSE_IDLE_HOOK				0
/*使用时间片钩子函数开启宏 */
#define configUSE_TICK_HOOK				0
/* CPU时钟 STM32F4为168MHz */
#define configCPU_CLOCK_HZ				( SystemCoreClock )
/* 系统时钟节拍 1ms*/
#define configTICK_RATE_HZ				( ( TickType_t ) 1000 )
/* 配置应用程序有效优先级数目  一般原则是应用程序数量要小于等于此值 Free RTOS允许应用程序使用相同的优先级*/
#define configMAX_PRIORITIES			( 5 )
/* 定义空闲任务使用的堆栈大小 */
#define configMINIMAL_STACK_SIZE		( ( unsigned short ) 130 )
/* RTOS总计可用的RAM内存大小 */
#define configTOTAL_HEAP_SIZE			( ( size_t ) ( 75 * 1024 ) )
/* 调用任务函数时，需要设置描述任务信息的字符串，这个宏用来定义该字符串的最大长度，注意：这里的长度包括字符串的结束符'\0' */
#define configMAX_TASK_NAME_LEN			( 10 )
/* 启动可视化跟踪调试宏 */
#define configUSE_TRACE_FACILITY		1
/* 定义系统节拍计数器的变量类型 1表示16位 0表示32位*/
#define configUSE_16_BIT_TICKS			0
/* 控制任务在空闲优先级种的行为 */
#define configIDLE_SHOULD_YIELD			1
/* 是否使用互斥信号量 */
#define configUSE_MUTEXES				1
#define configQUEUE_REGISTRY_SIZE		8
/* 检测堆栈溢出 为1时要设置堆栈溢出检测的钩子函数 */
#define configCHECK_FOR_STACK_OVERFLOW	0
/* 是否使用递归信号量 */
#define configUSE_RECURSIVE_MUTEXES		1
/* 内存分配错误调用钩子函数开启宏 */
#define configUSE_MALLOC_FAILED_HOOK	0
#define configUSE_APPLICATION_TASK_TAG	0
/* 是否使用计数信号量 */
#define configUSE_COUNTING_SEMAPHORES	1
#define configGENERATE_RUN_TIME_STATS	0
/* 任务通知功能开启宏 */
#define configUSE_TASK_NOTIFICATIONS    0


/* Co-routine definitions. */
/* 是否使用协程 */
#define configUSE_CO_ROUTINES 		0
/* 应用程序协程的有效优先级数目 */
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

/* Software timer definitions. */
/* 是否使用软件定时器 */
#define configUSE_TIMERS				1
/* 软件定时器的服务/守护进程的优先级*/
#define configTIMER_TASK_PRIORITY		( 2 )
/* 设置软件定时器命令队列的长度 */
#define configTIMER_QUEUE_LENGTH		10
/* 设置软件定时器服务/守护进程的任务堆栈深度 */
#define configTIMER_TASK_STACK_DEPTH	( configMINIMAL_STACK_SIZE * 2 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
/* 1表示允许用户不编译那些应用程序不需要的实时内核组件 */
#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	1
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1

/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
	/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4        /* 15 priority levels */
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			0xf

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
	
/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
/* 断言 判断传入参数是否合法 调试阶段使用 */
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }	
	
/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
//#define xPortSysTickHandler SysTick_Handler

//调度系统使用特殊方法完成调度 如果此项被定义 则中断优先级最大为31 
//#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1


#endif /* FREERTOS_CONFIG_H */


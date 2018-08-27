#include "delay.h"
#include "sys.h"
#include "FreeRTOS.h"					//支持OS时，使用	  
#include "task.h"

extern void xPortSysTickHandler(void);
static uint32_t fac_us;
static uint32_t fac_ms;
 
//systick中断服务函数,使用OS时用到
void SysTick_Handler(void)
{	
	if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) //如果系统已经运行
    {
        xPortSysTickHandler();
    }
}


//初始化延迟函数
//当使用OS的时候,此函数会初始化OS的时钟节拍
//SYSTICK的时钟固定为AHB时钟
//SYSCLK:系统时钟频率
void delay_init(u8 SYSCLK)
{
	u32 reload;
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); 
	fac_us=SYSCLK;						//不论是否使用OS,fac_us都需要使用
	reload=SYSCLK;						//每秒钟的计数次数 单位为M	   
	reload*=1000000/configTICK_RATE_HZ;	//根据delay_ostickspersec设定溢出时间
											//reload为24位寄存器,最大值:16777216,在168M下,约合0.7989s左右	
	fac_ms=1000/configTICK_RATE_HZ;		//代表OS可以延时的最少单位	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD=reload; 					//每1/delay_ostickspersec秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; 	//开启SYSTICK    

}								    

//延时nus
//nus:要延时的us数.	
//nus:0~204522252(最大值即2^32/fac_us@fac_us=21)	    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD的值	    	 
	ticks=nus*fac_us; 						//需要的节拍数 
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	};										    
}  
//延时nms
//nms:要延时的ms数
//nms:0~65535
void delay_ms(u16 nms)
{	
	if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) //系统已经与逆行
    {
        if(nms >= fac_ms)   //延时时间大于os的最小时间周期
        {
            vTaskDelay(nms/fac_ms);     //调用FreeRTOS延时
        }
        nms %= fac_ms;  //剩余的时间os无法提供 
    }
    delay_us((uint32_t ) (nms*1000));   //普通方式延时
}

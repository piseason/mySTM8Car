#ifndef  __DELAY_H 
#define  __DELAY_H 
 
//****************************************************************************** 
//V1.0 20130625 
//支持不同时钟频率 
//提供delay_us,delay_ms两个延时函数.  
////////////////////////////////////////////////////////////////////////////////  
 
#include "stm8s.h"

//#define u8  unsigned char 
//#define u16 unsigned int 
//#define u32 int 
 
 
void delay_init(u8 clk);	//延时函数初始化 
void delay_us(u16 nus);		//us级延时函数,最大65536us. 
void delay_ms(u32 nms);		//ms级延时函数 
#endif 
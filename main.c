/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    18-November-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "math.h"
#include "delay.h"
#include "uart.h"
#include "iompu6050.h"
#include "timer1_config.h"
#include "M5StickC.h"
u8 send_flag = 0;
/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void testDelay(){
  /* Initialize I/Os in Output Mode */
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  delay_init(16);
  GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);

  while (1)
  {
    /* Toggles LEDs */
    GPIO_WriteReverse(GPIOB, GPIO_PIN_5);
    delay_ms(500);
  }
}

void testMPU6050(void)
{
  short G_X,G_Y,G_Z,W_X,W_Y,W_Z;
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  delay_init(16);
  IOMPU6050_Init();
  delay_ms(100);
  Uart1_Init();
  delay_ms(20);
  //enableInterrupts();                 //使能中断
  while (1)
  {
//    G_X=MPU6050_Read_Byte(MPU6050_DEVICE_ID_REG);
//    G_Y=MPU6050_Read_Byte(MPU6050_ACCEL_XOUTH_REG);
//    G_Z=MPU6050_Read_Byte(MPU6050_ACCEL_XOUTL_REG);
    MPU6050_Get_Accelerometer(&G_X,&G_Y,&G_Z);
    //mprintf("%d %d %d\r\n",G_X,G_Y,G_Z);
  }
  
}

#define paraNumEx 8
extern int16_t accX,accY,accZ;
int testTheta()
{
    float tmpWy;
    u16 times=0;	//控制角度		控制角度的累计
    float paras[paraNumEx+3]; //Kp,Ki,Kd,Vmin,Vmax,Kvp,Kvi,Kvd,shiftT,Speed,Turn
    float tmpL,tmpR,VdataL,VdataR,powerL=0,powerR=0;
		
    paras[0] = 100;
    //paras[1] = 5.6;
    paras[1] = 0;
    paras[2] = 100; 
    paras[3] = 1.5;
    paras[4] = 2.7;
    //速度PID
    paras[5] = 0;
    paras[6] = 0.2;
    paras[7] = 0;
    
    paras[8] = 0;
    paras[9] = 0;
    paras[10] = 0;
	
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    delay_init(16);     //16M
    //LED与SDA冲突
    //GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);
    
    IOMPU6050_Init();
    delay_ms(100);
    Uart1_Init();
    delay_ms(20);

    ini_theta();      //在上电之前拿到测量偏差和统计信息
    delay_ms(100);
    //GPIO_WriteLow(GPIOB, GPIO_PIN_5);	//点亮LED,提示系统初始化完成
	
//    GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);
//    GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);
//    GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST);
//    GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_OUT_PP_LOW_FAST);
    //timer1_init();


    while(1)
    {
        //Kp = paras[0];Ki = paras[1];Kd = paras[2];
        //thetaV = paras[8];
        
        mprintf("AX=%d,AY=%d,AZ=%d,P=%d,I=%d,D=%d\r\n",accX,accY,accZ,(u16)(thetaP*100),(u16)thetaI,(u16)(thetaD*250));   
				
        //PID控制
        update_thetaEx();	//ThetaITar
//        if(fabs(thetaP) > 0.5){
//            powerL=0;powerR=0;
//            thetaI=0;thetaII=0;
//            thetaOffset=0;thetaV=0;
//        }
//        else{
//                //角度PID, 控制thetaP
//            if( fabs(paras[10])<0.1/210 ) tmpWy = 0; //tmpWy = (gyroZ/65.536) /9000.0;
//            else tmpWy = 0;
//            powerL = (thetaP-paras[10]+tmpWy) * paras[0] + thetaI * paras[1] + thetaD * paras[2];
//            powerR = (thetaP+paras[10]-tmpWy) * paras[0] + thetaI * paras[1] + thetaD * paras[2];
//            powerL = fmaxf(-1.0, fminf(1.0, powerL));
//            powerR = fmaxf(-1.0, fminf(1.0, powerR));
//					
//            //速度PID, 控制thetaI
////            paras[8] = -(thetaII * paras[6]);
////            paras[8] = fmaxf(-10.0, fminf(10.0, paras[8]));		//假定角度的偏差在10度之内
//	}
//            VdataL = ((paras[4] - paras[3]) * fabs(powerL)) + paras[3];
//            VdataR = ((paras[4] - paras[3]) * fabs(powerR)) + paras[3];
//            tmpL = VdataL*100;
//            tmpR = VdataR*100;
//            if (powerL>0)
//            {   
//                  TIM1_SetCompare1(0);
//                  TIM1_SetCompare2((u16)tmpL);
//            }
//            else if (powerL<0)
//            {
//                  TIM1_SetCompare1((u16)tmpL);
//                  TIM1_SetCompare2(0);
//            }
//            else{
//                  TIM1_SetCompare1(0);        //停止
//                  TIM1_SetCompare2(0);
//            }
//            if (powerR>0)
//            {   
//                  TIM1_SetCompare3(0);
//                  TIM1_SetCompare4((u16)tmpL);
//            }
//            else if (powerR<0)
//            {
//                  TIM1_SetCompare3((u16)tmpL);
//                  TIM1_SetCompare4(0);
//            }
//            else{
//                  TIM1_SetCompare3(0);        //停止
//                  TIM1_SetCompare4(0);
//            }
				
            //分频
            times++;
//            if(times%10==0){
//                    angleCorrection();
//            } 
//            if(times%500==0){
//                    GPIO_WriteReverse(GPIOB, GPIO_PIN_5);    //闪烁LED,提示系统正在运行.
//            }
            //delay_ms(2);     
                 
    }   
}

extern int16_t gyroZ;
int testOffsetTheta()
{
    float tmpWy;
    u16 times=0;	//控制角度		控制角度的累计
    float paras[paraNumEx+3]; //Kp,Ki,Kd,Vmin,Vmax,Kvp,Kvi,Kvd,shiftT,Speed,Turn
    float tmpL,tmpR,VdataL,VdataR,powerL=0,powerR=0;
		
    paras[0] = 100;
    paras[1] = 5.6;
    paras[2] = 2.8; 
    paras[3] = 0;
    paras[4] = 5;
    //速度PID
    paras[5] = 0;
    paras[6] = 0.2;
    paras[7] = 0;
    
    paras[8] = 0;
    paras[9] = 0;
    paras[10] = 0;
	
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    delay_init(16);     //16M
    //LED与SDA冲突
    //GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);
    
    IOMPU6050_Init();
    delay_ms(100);
    //Uart1_Init();
    //delay_ms(20);

    ini_theta();      //在上电之前拿到测量偏差和统计信息
    //delay_ms(100);
    //GPIO_WriteLow(GPIOB, GPIO_PIN_5);	//点亮LED,提示系统初始化完成
	
//    GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);
//    GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);
//    GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST);
//    GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_OUT_PP_LOW_FAST);
    timer1_init();


    while(1)
    {
        //Kp = paras[0];Ki = paras[1];Kd = paras[2];
        thetaV = paras[8];
        
        //mprintf("AX=%d,AY=%d,AZ=%d,P=%d,I=%d,D=%d\r\n",accX,accY,accZ,(u16)(thetaP*100),(u16)thetaI,(u16)(thetaD*250));   
				
        //PID控制
        update_thetaEx();	//ThetaITar
        if(fabs(thetaP) > 0.3){
            powerL=0;powerR=0;
            thetaI=0;thetaII=0;
            thetaOffset=0;thetaV=0;
        }
        else{
                //角度PID, 控制thetaP
            tmpWy = (-gyroZ/65.536) /9000.0;
            powerL = (thetaP-paras[10]+tmpWy) * paras[0] + thetaI * paras[1] + thetaD * paras[2];
            powerR = (thetaP+paras[10]-tmpWy) * paras[0] + thetaI * paras[1] + thetaD * paras[2];
            powerL = fmaxf(-1.0, fminf(1.0, powerL));
            powerR = fmaxf(-1.0, fminf(1.0, powerR));
					
            //速度PID, 控制thetaI
            paras[8] = -(thetaII * paras[6]);
            paras[8] = fmaxf(-5.0, fminf(5.0, paras[8]));		//假定角度的偏差在10度之内
	}
            VdataL = ((paras[4] - paras[3]) * fabs(powerL)) + paras[3];
            VdataR = ((paras[4] - paras[3]) * fabs(powerR)) + paras[3];
            tmpL = VdataL*100;
            tmpR = VdataR*100;
            if (powerL>0)
            {   
                  TIM1_SetCompare1(0);
                  TIM1_SetCompare2((u16)tmpL);
            }
            else if (powerL<0)
            {
                  TIM1_SetCompare1((u16)tmpL);
                  TIM1_SetCompare2(0);
            }
            else{
                  TIM1_SetCompare1(0);        //停止
                  TIM1_SetCompare2(0);
            }
            if (powerR>0)
            {   
                  TIM1_SetCompare3(0);
                  TIM1_SetCompare4((u16)tmpL);
            }
            else if (powerR<0)
            {
                  TIM1_SetCompare3((u16)tmpL);
                  TIM1_SetCompare4(0);
            }
            else{
                  TIM1_SetCompare3(0);        //停止
                  TIM1_SetCompare4(0);
            }
				
            //分频
            times++;
            if(times%10==0){
                    angleCorrection();
            } 
//            if(times%500==0){
//                    GPIO_WriteReverse(GPIOB, GPIO_PIN_5);    //闪烁LED,提示系统正在运行.
//            }
            delay_ms(2);     
                 
    }   
}


void main(void){
    //testMPU6050();
    //testTheta();
    testOffsetTheta();
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

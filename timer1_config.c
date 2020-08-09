#include "timer1_config.h"
/***************************************
*               TIMER1初始化
*           检测外部频率和脉冲
***************************************/
void timer1_init(void)
{

  TIM1_DeInit();  
  
  TIM1_TimeBaseInit(32-1, TIM1_COUNTERMODE_UP, 1000-1, 0x00);        //500Hz

  TIM1_OC1Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_DISABLE, 
               0x0000, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH,
               TIM1_OCIDLESTATE_SET, TIM1_OCNIDLESTATE_SET);
  TIM1_OC2Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_DISABLE, 
               0x0000, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH,
               TIM1_OCIDLESTATE_SET, TIM1_OCNIDLESTATE_SET);
  TIM1_OC3Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_DISABLE, 
               0x0000, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH,
               TIM1_OCIDLESTATE_SET, TIM1_OCNIDLESTATE_SET);
  TIM1_OC4Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE, 
               0x0000, TIM1_OCPOLARITY_HIGH,
               TIM1_OCIDLESTATE_SET);

  TIM1_Cmd(ENABLE);
  TIM1_CtrlPWMOutputs(ENABLE);
  
}


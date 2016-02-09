#include "init.h"
#include "gpio_ex.h"



void device_gpio_init ()
{
    gpio_init(LCD_RS_PORT, LCD_RS_PIN, GPIO_Speed_50MHz, GPIO_Mode_OUT);
    gpio_init(LCD_E_PORT, LCD_E_PIN, GPIO_Speed_50MHz, GPIO_Mode_OUT);
    gpio_init(LCD_D4_PORT, LCD_D4_PIN, GPIO_Speed_50MHz, GPIO_Mode_OUT);
    gpio_init(LCD_D5_PORT, LCD_D5_PIN, GPIO_Speed_50MHz, GPIO_Mode_OUT);
    gpio_init(LCD_D6_PORT, LCD_D6_PIN, GPIO_Speed_50MHz, GPIO_Mode_OUT);
    gpio_init(LCD_D7_PORT, LCD_D7_PIN, GPIO_Speed_50MHz, GPIO_Mode_OUT);
    gpio_init(ADC_IN0_PORT, ADC_IN0_PIN, GPIO_Speed_50MHz, GPIO_Mode_AN);
    
    gpio_init(BUTTON_L_PORT, BUTTON_L_PIN, GPIO_Speed_50MHz, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);
    gpio_init(BUTTON_C_PORT, BUTTON_C_PIN, GPIO_Speed_50MHz, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);
    gpio_init(BUTTON_R_PORT, BUTTON_R_PIN, GPIO_Speed_50MHz, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);
    
    gpio_init(RELAY_PORT, RELAY_PIN, GPIO_Speed_50MHz, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP);
}

void device_enable_clock ()
{
    if (SysTick_Config(SystemCoreClock / 1000))
    { 
    /* Capture error */ 
    while (1);
    }
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
}

void device_adc_init ()
{
  ADC_InitTypeDef     ADC_InitStructure;
  GPIO_InitTypeDef    GPIO_InitStructure;
 
  
  /* ADCs DeInit */  
  ADC_DeInit(ADC1);
  
  /* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStructure);
  
  /* Configure the ADC1 in continuous mode with a resolution equal to 12 bits  */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStructure); 
  
  ADC_ChannelConfig(ADC1, ADC_Channel_0 , ADC_SampleTime_239_5Cycles);

  /* ADC Calibration */
  ADC_GetCalibrationFactor(ADC1);
  
  /* Enable the ADC peripheral */
  ADC_Cmd(ADC1, ENABLE);     
  
  /* Wait the ADRDY flag */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 
  
  /* ADC1 regular Software Start Conv */ 
  ADC_StartOfConversion(ADC1); 
}


void device_timer_init ()
{
    __IO uint16_t CCR4_Val = 6000;
    uint16_t PrescalerValue = 0;
    
     TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* Enable the TIM3 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  
    /* -----------------------------------------------------------------------
    TIM3 Configuration: Output Compare Timing Mode:
    
    In this example TIM3 input clock (TIM3CLK) is set to APB1 clock (PCLK1),  
      => TIM3CLK = PCLK1 = SystemCoreClock = 48 MHz
          
    To get TIM3 counter clock at 6 MHz, the prescaler is computed as follows:
       Prescaler = (TIM3CLK / TIM3 counter clock) - 1
       Prescaler = (PCLK1 /6 MHz) - 1
                                              
    
    CC4 update rate = TIM3 counter clock / CCR4_Val = 878.9 Hz
    ==> Toggling frequency = 439.4 Hz

    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
     function to update SystemCoreClock variable value. Otherwise, any configuration
     based on this variable will be incorrect.    
  ----------------------------------------------------------------------- */   


  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) (SystemCoreClock  / 6000000) - 1;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 6000;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* Prescaler configuration */
  TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);


  /* Output Compare Timing Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;

  TIM_OC4Init(TIM3, &TIM_OCInitStructure);

  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Disable);
   
  /* TIM Interrupts enable */
  TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
}
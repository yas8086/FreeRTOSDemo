// delay_us.c
#include "delay.h"

/**
 * @brief 初始化 DWT 计数器
 */
void DWT_Init(void)
{
    // 使能 DWT 时钟
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    // 使能循环计数
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    // 清除计数器
    DWT->CYCCNT = 0;
}

/**
 * @brief 微秒延时
 * @param us: 延时的微秒数
 */
void delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (SystemCoreClock / 1000000);
    
    // 等待足够的计数
    while ((DWT->CYCCNT - start) < ticks);
}

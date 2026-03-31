/* key.c - 优化的按键扫描函数（解决有时按了没反应的问题） */
#include "./BSP/KEY/key.h"
//#include "./SYSTEM/delay/delay.h"

uint8_t key_scan(uint8_t mode)
{
    static uint8_t key_up = 1;  /* 按键松开标志 */
    uint8_t keyval = 0;

    if (mode) key_up = 1;       /* 支持连按 */

    /* 检测是否有按键按下（低电平或高电平有效） */
    if (key_up && (KEY0 == 0 || KEY1 == 0 || WK_UP == 1))
    {
        HAL_Delay(10);           /* 去抖动 10ms */
        
        /* 再次确认按键状态，确保不是干扰 */
        if (KEY0 == 0) {
            keyval = KEY0_PRES;
            key_up = 0;
        }
        else if (KEY1 == 0) {
            keyval = KEY1_PRES;
            key_up = 0;
        }
        else if (WK_UP == 1) {
            keyval = WKUP_PRES;
            key_up = 0;
        }
    }
    else if (KEY0 == 1 && KEY1 == 1 && WK_UP == 0)
    {
        /* 所有按键都松开，重置标志 */
        key_up = 1;
    }

    return keyval;              /* 返回键值 */
}

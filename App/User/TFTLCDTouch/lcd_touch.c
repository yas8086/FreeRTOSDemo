/* lcd_touch.c - 完整优化版 */
#include "./BSP/TOUCH/touch.h"
#include "./User/TFTLCDTouch/lcd_touch.h"

static uint8_t Last_Touch_Btn = 0;
static uint32_t Last_Touch_Time = 0;

/* 触摸按钮区域检测函数（带扩展区域和去抖动） */
uint8_t Check_Touch_Button(void)
{
    uint16_t touch_x, touch_y;
    uint32_t current_time = osKernelGetTickCount();
    uint8_t current_btn = 0;
    
    /* 扫描触摸屏（0=屏幕模式） */
    if (tp_dev.scan(0)) {
        /* 有触摸，获取坐标 */
        touch_x = tp_dev.x[0];
        touch_y = tp_dev.y[0];
        
        /* 检测按钮 1 - 快（使用扩展区域，提高灵敏度） */
        if (touch_x >= (BTN1_X - TOUCH_EXTEND) && 
            touch_x <= (BTN1_X + BTN1_W + TOUCH_EXTEND) &&
            touch_y >= (BTN1_Y - TOUCH_EXTEND) && 
            touch_y <= (BTN1_Y + BTN1_H + TOUCH_EXTEND)) {
            current_btn = 1;
        }
        
        /* 检测按钮 2 - 慢（使用扩展区域） */
        else if (touch_x >= (BTN2_X - TOUCH_EXTEND) && 
                 touch_x <= (BTN2_X + BTN2_W + TOUCH_EXTEND) &&
                 touch_y >= (BTN2_Y - TOUCH_EXTEND) && 
                 touch_y <= (BTN2_Y + BTN2_H + TOUCH_EXTEND)) {
            current_btn = 2;
        }
        
        /* 检测按钮 3 - 停（使用扩展区域） */
        else if (touch_x >= (BTN3_X - TOUCH_EXTEND) && 
                 touch_x <= (BTN3_X + BTN3_W + TOUCH_EXTEND) &&
                 touch_y >= (BTN3_Y - TOUCH_EXTEND) && 
                 touch_y <= (BTN3_Y + BTN3_H + TOUCH_EXTEND)) {
            current_btn = 3;
        }
        
        /* 去抖动和连续触发控制 */
        if (current_btn != 0) {
            /* 如果是新按钮，立即响应（去抖动 80ms） */
            if (current_btn != Last_Touch_Btn) {
                Last_Touch_Btn = current_btn;
                Last_Touch_Time = current_time;
                return current_btn;
            }
            
            /* 如果是同一个按钮，检查是否满足连续触发条件（200ms） */
            if ((current_time - Last_Touch_Time) >= TOUCH_REPEAT_MS) {
                Last_Touch_Time = current_time;
                return current_btn;
            }
            
            /* 不满足连续触发条件，返回 0 */
            return 0;
        }
        
        /* 没有按钮被按下，重置状态 */
        Last_Touch_Btn = 0;
    }
    
    return 0;  /* 未按下任何按钮 */
}

#include "main.h"

/* 按钮位置定义 */
#define BTN1_X      50       /* 快按钮左上角 X */
#define BTN1_Y      300       /* 快按钮左上角 Y */
#define BTN1_W      150       /* 快按钮宽度 */
#define BTN1_H      80        /* 快按钮高度 */

#define BTN2_X      250       /* 慢按钮左上角 X */
#define BTN2_Y      300       /* 慢按钮左上角 Y */
#define BTN2_W      150       /* 慢按钮宽度 */
#define BTN2_H      80        /* 慢按钮高度 */

#define BTN3_X      110       /* 停按钮左上角 X */
#define BTN3_Y      420       /* 停按钮左上角 Y */
#define BTN3_W      230       /* 停按钮宽度 */
#define BTN3_H      80        /* 停按钮高度 */

/* 触摸扩展区域（提高灵敏度） */
#define TOUCH_EXTEND  15      /* 向外扩展 15 像素，更容易触发 */

/* 触摸时间控制 */
#define TOUCH_DEBOUNCE_MS   20      /* 去抖动时间 50ms */
#define TOUCH_REPEAT_MS     80      /* 连续触发间隔 150ms */

uint8_t Check_Touch_Button(void);

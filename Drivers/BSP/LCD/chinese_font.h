/**
 ****************************************************************************************************
 * @file        chinese_font.h
 * @brief       汉字字库定义
 * @author      User
 * @version     V1.0
 * @date        2026-03-27
 * @attention   使用 PCtoLCD2002 等工具生成字模数据
 *              格式：阴码、逐行式、顺向、16x16 点阵
 ****************************************************************************************************
 */

#ifndef __CHINESE_FONT_H
#define __CHINESE_FONT_H

#include "stdint.h"

/* 汉字字模数据 (24x24 点阵，每个汉字 72 字节) */
extern const unsigned char chinese[][72];

/* 汉字总数 */
extern const unsigned short ChineseFontCount;

/* 汉字索引表 (GB2312 编码 -> 数组索引) */
extern const unsigned char ChineseFontIndex[][3];

/* 索引表大小 */
extern const unsigned short ChineseIndexCount;

/* 字体大小定义 */
#define FONT_SIZE_16    16
#define FONT_SIZE_24    24
#define FONT_SIZE_32    32

/* 字模数据大小计算 */
#define FONT_DATA_SIZE(size)  ((size) * (size) / 8)

#endif

/**
 ****************************************************************************************************
 * @file        24cxx.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-24
 * @brief       24CXX 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20200424
 * 第一次发布
 *
 ****************************************************************************************************
 */
#include "i2c.h"
#include "./BSP/24CXX/24cxx.h"
#include "./SYSTEM/delay/delay.h"
#include "usart.h"

/**
 * @brief       初始化IIC接口
 * @param       无
 * @retval      无
 */
void at24cxx_init(void)
{
    // 硬件 IIC 由 CubeMX 自动初始化（MX_I2C1_Init）
    // 这里只需要检查 I2C 是否正常
    if (HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_READY) {
        printf("I2C1 initialized successfully\n");
    } else {
        printf("I2C1 initialization failed\n");
    }
}

/**
 * @brief       在AT24CXX指定地址读出一个数据
 * @param       readaddr: 开始读数的地址
 * @retval      读到的数据
 */
uint8_t at24cxx_read_one_byte(uint16_t addr)
{
    uint8_t data = 0;
    
    // 使用 HAL 库的内存读取函数
    // 参数：I2C 句柄，设备地址，内存地址，地址大小，数据缓冲，数据长度，超时
    HAL_I2C_Mem_Read(&hi2c1,              // I2C 句柄
                     0xA1,                // 设备读地址（0xA0 | 0x01）
                     addr,                // 内存地址
                     I2C_MEMADD_SIZE_8BIT,// 内存地址大小（8 位）
                     &data,               // 数据缓冲
                     1,                   // 读取 1 字节
                     100);                // 超时 100ms
    
    return data;
}

/**
 * @brief       在AT24CXX指定地址写入一个数据
 * @param       addr: 写入数据的目的地址
 * @param       data: 要写入的数据
 * @retval      无
 */
void at24cxx_write_one_byte(uint16_t addr, uint8_t data)
{
   // 使用 HAL 库的内存写入函数
    HAL_I2C_Mem_Write(&hi2c1,              // I2C 句柄
                      0xA0,                // 设备写地址
                      addr,                // 内存地址
                      I2C_MEMADD_SIZE_8BIT,// 内存地址大小（8 位）
                      &data,               // 数据缓冲
                      1,                   // 写入 1 字节
                      100);                // 超时 100ms
    
    HAL_Delay(10);               // 等待 EEPROM 写入完成（必须保留！）
}
 
/**
 * @brief       检查AT24CXX是否正常
 *   @note      检测原理: 在器件的末地址写如0X55, 然后再读取, 如果读取值为0X55
 *              则表示检测正常. 否则,则表示检测失败.
 *
 * @param       无
 * @retval      检测结果
 *              0: 检测成功
 *              1: 检测失败
 */
uint8_t at24cxx_check(void)
{
    uint8_t temp;
    uint16_t addr = EE_TYPE;
    temp = at24cxx_read_one_byte(addr); /* 避免每次开机都写AT24CXX */

    if (temp == 0X55)   /* 读取数据正常 */
    {
        return 0;
    }
    else    /* 排除第一次初始化的情况 */
    {
        at24cxx_write_one_byte(addr, 0X55); /* 先写入数据 */
        temp = at24cxx_read_one_byte(255);  /* 再读取数据 */

        if (temp == 0X55)return 0;
    }

    return 1;
}

/**
 * @brief       在AT24CXX里面的指定地址开始读出指定个数的数据
 * @param       addr    : 开始读出的地址 对24c02为0~255
 * @param       pbuf    : 数据数组首地址
 * @param       datalen : 要读出数据的个数
 * @retval      无
 */
void at24cxx_read(uint16_t addr, uint8_t *pbuf, uint16_t datalen)
{
    HAL_I2C_Mem_Read(&hi2c1,               // I2C 句柄
                     0xA1,                 // 设备读地址
                     addr,                 // 起始内存地址
                     I2C_MEMADD_SIZE_8BIT, // 内存地址大小
                     pbuf,                 // 数据缓冲
                     datalen,              // 数据长度
                     100);                 // 超时
}

/**
 * @brief       在AT24CXX里面的指定地址开始写入指定个数的数据
 * @param       addr    : 开始写入的地址 对24c02为0~255
 * @param       pbuf    : 数据数组首地址
 * @param       datalen : 要写入数据的个数
 * @retval      无
 */
void at24cxx_write(uint16_t addr, uint8_t *pbuf, uint16_t datalen)
{
    // 直接写入多个字节（硬件自动处理地址递增）
    HAL_I2C_Mem_Write(&hi2c1,              // I2C 句柄
                      0xA0,                // 设备写地址
                      addr,                // 起始内存地址
                      I2C_MEMADD_SIZE_8BIT,// 内存地址大小
                      pbuf,                // 数据缓冲
                      datalen,             // 数据长度
                      100);                // 超时
    
    HAL_Delay(10);  // 等待写入完成
}







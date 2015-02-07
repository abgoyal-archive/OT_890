/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

/******************************************************************************
 * mt3351_gpio.c - MT3351 Linux GPIO Device Driver
 * 
 * Copyright 2008-2009 MediaTek Co.,Ltd.
 * 
 * DESCRIPTION:
 *     This file provid the other drivers GPIO relative functions
 *
 * modification history
 * ----------------------------------------
 * 01a, 08 oct 2008, Koshi,Chiu written
 * ----------------------------------------
 ******************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/autoconf.h>
#include <linux/platform_device.h>

#include <linux/types.h>
#include <linux/device.h>
//#include <linux/miscdevice.h>
#include <linux/spinlock.h>
#include <asm/uaccess.h>

#include <mach/mt3351_reg_base.h>
#include <mach/mt3351_gpio.h>
#include <asm/io.h>
//#include <Rhine/MT3351_Battery.h>

#define  GPIO_DEVICE "mt3351-gpio"
#define  VERSION    "v 0.1"

/******************************************************************************
MT3351 GPIO Base Addresses
******************************************************************************/
#define  GPIO_DIR0	(GPIO_BASE+0x0000)
#define  GPIO_DIR1	(GPIO_BASE+0x0010)
#define  GPIO_DIR2	(GPIO_BASE+0x0020)
#define  GPIO_DIR3	(GPIO_BASE+0x0030)

#define  GPIO_PU0   (GPIO_BASE+0x0100)
#define  GPIO_PU1   (GPIO_BASE+0x0110)
#define  GPIO_PU2   (GPIO_BASE+0x0120)
#define  GPIO_PU3   (GPIO_BASE+0x0130)

#define  GPIO_PD0	(GPIO_BASE+0x0200)
#define  GPIO_PD1	(GPIO_BASE+0x0210)
#define  GPIO_PD2	(GPIO_BASE+0x0220)
#define  GPIO_PD3	(GPIO_BASE+0x0230)

#define  GPIO_OUT0  (GPIO_BASE+0x0300)
#define  GPIO_OUT1  (GPIO_BASE+0x0310)
#define  GPIO_OUT2  (GPIO_BASE+0x0320)
#define  GPIO_OUT3  (GPIO_BASE+0x0330)

#define  GPIO_IN0   (GPIO_BASE+0x0400)
#define  GPIO_IN1   (GPIO_BASE+0x0410)
#define  GPIO_IN2   (GPIO_BASE+0x0420)
#define  GPIO_IN3   (GPIO_BASE+0x0430)

#define  GPIO_CFG0  (GPIO_BASE+0x0500)
#define  GPIO_CFG1  (GPIO_BASE+0x0510)
#define  GPIO_CFG2  (GPIO_BASE+0x0520)
#define  GPIO_CFG3  (GPIO_BASE+0x0530)
#define  GPIO_CFG4  (GPIO_BASE+0x0540)
#define  GPIO_CFG5  (GPIO_BASE+0x0550)
#define  GPIO_CFG6  (GPIO_BASE+0x0560)
#define  GPIO_CFG7  (GPIO_BASE+0x0570)
#define  GPIO_CFG8  (GPIO_BASE+0x0580)
#define  GPIO_CFG9  (GPIO_BASE+0x0590)
#define  GPIO_CFGA  (GPIO_BASE+0x05A0)
#define  GPIO_CFGB  (GPIO_BASE+0x05B0)
#define  GPIO_CFGC  (GPIO_BASE+0x05C0)
#define  GPIO_CFGD  (GPIO_BASE+0x05D0)
#define  GPIO_CFGE  (GPIO_BASE+0x05E0)
#define  GPIO_CFGF  (GPIO_BASE+0x05F0)

#define  GPIO_OCFG  (GPIO_BASE+0x0600)
#define  GPIO_ICFG  (GPIO_BASE+0x0610)

#define  GPIO_DBG0  (GPIO_BASE+0x0700)
#define  GPIO_DBG1  (GPIO_BASE+0x0710)
#define  GPIO_DBG2  (GPIO_BASE+0x0720)
#define  GPIO_DBG3  (GPIO_BASE+0x0730)
#define  GPIO_TM    (GPIO_BASE+0x0750)

#define  PINMUXCFG  (CONFIG_BASE+0x0700)
#define  DEBUG_SEL0 (CONFIG_BASE+0x0704)
#define  DEBUG_SEL1 (CONFIG_BASE+0x0708)
#define  DEBUG_SEL2 (CONFIG_BASE+0x070C)

/******************************************************************************
Read / Write Register functions
******************************************************************************/
#define  DRV_WriteReg32(addr, data)  __raw_writel(data, addr)
#define  DRV_Reg32(addr)             __raw_readl(addr)

#define  TRUE                   1
#define  FALSE                  0

#define  MAX_GPIO_PIN           123
#define  MAX_GPIO_REG_BITS      32

#define  MAX_GPIO_DC_PER_REG    8

#define  MISC_DYNAMIC_MINOR     255

static spinlock_t gpio_lock = SPIN_LOCK_UNLOCKED;

HARDWARE_STATUS gpio_hw_status;
EXPORT_SYMBOL(gpio_hw_status);

static int statusChanged = 0;

/******************************************************************************
 * mt_set_gpio_dir
 * 
 * DESCRIPTION:
 *   Set the gpio u4pin as u4Dir direction ! 
 * 
 * PARAMETERS: 
 *   u4Pin: gpio pin number
 *   u4Dir: gpio pin direction
 * 
 * RETURNS: 
 *   RSUCCESS : success
 *   -EEXCESSPINNO : Excess Pin number
 *   -EBADDIR : Bad Direction number
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

s32 mt_set_gpio_dir(u32 u4Pin, u32 u4Dir)
{
    u32 dirNo;
    u32 bitOffset;
    u32 regValue;
    u32 pinReg;

    if (u4Pin >= MAX_GPIO_PIN)
        return -EEXCESSPINNO;

    if (u4Dir > GPIO_DIR_OUT)
        return -EBADDIR;
    
    dirNo = u4Pin / MAX_GPIO_REG_BITS;
    bitOffset = u4Pin % MAX_GPIO_REG_BITS;

    if (dirNo == 0) pinReg =  GPIO_DIR0;
    else if (dirNo == 1) pinReg =  GPIO_DIR1;
    else if (dirNo == 2) pinReg =  GPIO_DIR2;
    else if (dirNo == 3) pinReg =  GPIO_DIR3;
    else 
    {         
        return -EEXCESSPINNO;
    }

    spin_lock(&gpio_lock);
    regValue = DRV_Reg32(pinReg);
    if (u4Dir == 1)    
        regValue |= (1L << bitOffset);    
    else     
        regValue &= ~(1L << bitOffset);                

    DRV_WriteReg32(pinReg, regValue);    
    spin_unlock(&gpio_lock);

    return RSUCCESS;
}
EXPORT_SYMBOL(mt_set_gpio_dir);

/******************************************************************************
 * mt_get_gpio_dir
 * 
 * DESCRIPTION:
 *   Get the gpio u4pin's direction ! 
 * 
 * PARAMETERS: 
 *   u4Pin: gpio pin number
 * 
 * RETURNS: 
 *   The direction of u4Pin, 0 IN 1 OUT
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

s32 mt_get_gpio_dir(u32 u4Pin)
{    
    u32 dirNo;
    u32 bitOffset;
    u32 regValue;
    u32 pinReg;

    if (u4Pin >= MAX_GPIO_PIN)
        return -EEXCESSPINNO;
    
    dirNo = u4Pin / MAX_GPIO_REG_BITS;
    bitOffset = u4Pin % MAX_GPIO_REG_BITS;

    if (dirNo == 0) pinReg =  GPIO_DIR0;
    else if (dirNo == 1) pinReg =  GPIO_DIR1;
    else if (dirNo == 2) pinReg =  GPIO_DIR2;
    else if (dirNo == 3) pinReg =  GPIO_DIR3;
    else 
    { 
        return -EEXCESSPINNO;
    }

    spin_lock(&gpio_lock);
    regValue = DRV_Reg32(pinReg);
    spin_unlock(&gpio_lock);

    return (((regValue & (1L << bitOffset)) != 0)? 1: 0);
}
EXPORT_SYMBOL(mt_get_gpio_dir);

/******************************************************************************
 * mt_set_gpio_pullup
 * 
 * DESCRIPTION:
 *   Set the gpio u4pin as PullUp enable value ! 
 * 
 * PARAMETERS: 
 *   u4Pin: gpio pin number
 *   bPullUpEn:  PullUp enable/disable, enable 1, disable 0
 * 
 * RETURNS: 
 *   RSUCCESS : Success
 *   -EEXCESSPINNO : Excess Pin number
 *   -EBADDIR : Bad Direction number
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/
 
s32 mt_set_gpio_pullup(u32 u4Pin, u8 bPullUpEn)
{
    u32 pullUpEnNo;
    u32 bitOffset;
    u32 regValue;
    u32 pinReg;

    if (u4Pin >= MAX_GPIO_PIN)
        return -EEXCESSPINNO;

    pullUpEnNo = u4Pin / MAX_GPIO_REG_BITS;
    bitOffset = u4Pin % MAX_GPIO_REG_BITS;

    if (pullUpEnNo == 0) pinReg =  GPIO_PU0;
    else if (pullUpEnNo == 1) pinReg =  GPIO_PU1;
    else if (pullUpEnNo == 2) pinReg =  GPIO_PU2;
    else if (pullUpEnNo == 3) pinReg =  GPIO_PU3;
    else 
    { 
        return -EEXCESSPINNO;
    }

    spin_lock(&gpio_lock);
    regValue = DRV_Reg32(pinReg);
    
    if (bPullUpEn == TRUE)
        regValue |= (1L << bitOffset);
    else 
        regValue &= ~(1L << bitOffset);        

    DRV_WriteReg32(pinReg, regValue);
    spin_unlock(&gpio_lock);

    return RSUCCESS;
}
EXPORT_SYMBOL(mt_set_gpio_pullup);

/******************************************************************************
 * mt_get_gpio_pullup
 * 
 * DESCRIPTION: 
 *   Get the gpio u4pin's pullup enable value ! 
 * 
 * PARAMETERS: 
 *   u4Pin: gpio pin number
 * 
 * RETURNS: 
 *    The value of u4Pin's pullup enable, 0 disable, 1 enable
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/
 
s32 mt_get_gpio_pullup(u32 u4Pin)
{
    u32 pullUpEnNo;
    u32 bitOffset;
    u32 regValue;
    u32 pinReg;

    if (u4Pin >= MAX_GPIO_PIN)
        return -EEXCESSPINNO;
    
    pullUpEnNo = u4Pin / MAX_GPIO_REG_BITS;
    bitOffset = u4Pin % MAX_GPIO_REG_BITS;

    if (pullUpEnNo == 0) pinReg =  GPIO_PU0;
    else if (pullUpEnNo == 1) pinReg =  GPIO_PU1;
    else if (pullUpEnNo == 2) pinReg =  GPIO_PU2;
    else if (pullUpEnNo == 3) pinReg =  GPIO_PU3;
    else 
    { 	
        return -EEXCESSPINNO;
    }
    spin_lock(&gpio_lock);
    regValue = DRV_Reg32(pinReg);
    spin_unlock(&gpio_lock);
    
    return (((regValue & (1L << bitOffset)) != 0)? TRUE: FALSE);
}
EXPORT_SYMBOL(mt_get_gpio_pullup);

/******************************************************************************
 * mt_set_gpio_pulldown
 * 
 * DESCRIPTION: 
 *   Set the gpio u4pin as PullDown value ! 
 * 
 * PARAMETERS: 
 *   u4Pin: gpio pin number
 *   bPullDownEn : PullDown enable/disable, enable 1, disable 0
 * 
 * RETURNS: 
 *   RSUCCESS : success
 *   -EEXCESSPINNO : Excess Pin number
 *   -EBADDIR : Bad Direction number
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

s32 mt_set_gpio_pulldown(u32 u4Pin, u8 bPullDownEn)
{
    u32 pullDownEnNo;
    u32 bitOffset;
    u32 regValue;
    u32 pinReg;

    if (u4Pin >= MAX_GPIO_PIN)
        return -EEXCESSPINNO;

    pullDownEnNo = u4Pin / MAX_GPIO_REG_BITS;
    bitOffset = u4Pin % MAX_GPIO_REG_BITS;

    if (pullDownEnNo == 0) pinReg =  GPIO_PD0;
    else if (pullDownEnNo == 1) pinReg =  GPIO_PD1;
    else if (pullDownEnNo == 2) pinReg =  GPIO_PD2;
    else if (pullDownEnNo == 3) pinReg =  GPIO_PD3;
    else 
    { 
        return -EEXCESSPINNO;
    }

    spin_lock(&gpio_lock);
    regValue = DRV_Reg32(pinReg);
    
    if (bPullDownEn == TRUE)
        regValue |= (1L << bitOffset);
    else 
        regValue &= ~(1L << bitOffset);        

    DRV_WriteReg32(pinReg, regValue);
    spin_unlock(&gpio_lock);

    return RSUCCESS;
}
EXPORT_SYMBOL(mt_set_gpio_pulldown);

/******************************************************************************
 * mt_get_gpio_pulldown
 * 
 * DESCRIPTION: 
 *   Get the gpio u4pin's pulldown enable value! 
 * 
 * PARAMETERS: 
 *   u4Pin: gpio pin number
 * 
 * RETURNS: 
 *    The value of u4Pin's pulldown enable, 0 disable, 1 enable
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

s32 mt_get_gpio_pulldown(u32 u4Pin)
{
    u32 pullDownEnNo;
    u32 bitOffset;
    u32 regValue;
    u32 pinReg;

    if (u4Pin >= MAX_GPIO_PIN)
        return -EEXCESSPINNO;
    
    pullDownEnNo = u4Pin / MAX_GPIO_REG_BITS;
    bitOffset = u4Pin % MAX_GPIO_REG_BITS;

    if (pullDownEnNo == 0) pinReg =  GPIO_PD0;
    else if (pullDownEnNo == 1) pinReg =  GPIO_PD1;
    else if (pullDownEnNo == 2) pinReg =  GPIO_PD2;
    else if (pullDownEnNo == 3) pinReg =  GPIO_PD3;
    else 
    { 
        return -EEXCESSPINNO;
    }

    spin_lock(&gpio_lock);
    regValue = DRV_Reg32(pinReg);
    spin_unlock(&gpio_lock);
    
    return (((regValue & (1L << bitOffset)) != 0)? TRUE: FALSE);
}
EXPORT_SYMBOL(mt_get_gpio_pulldown);

/******************************************************************************
 * mt_set_gpio_out
 * 
 * DESCRIPTION: 
 *   Set the gpio u4pin as output value u4PinOut! 
 * 
 * PARAMETERS: 
 *   u4Pin: gpio pin number
 *   u4PinOut: gpio output value, 0/1
 * 
 * RETURNS: 
 *   RSUCCESS : success
 *   -EEXCESSPINNO : Excess Pin number   
 *   -EBADINOUTVAL : Bad output value
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

s32 mt_set_gpio_out(u32 u4Pin, u32 u4PinOut)
{
    u32 pinOutNo;
    u32 bitOffset;
    u32 regValue;
    u32 pinReg;

    if (u4Pin >= MAX_GPIO_PIN)
        return -EEXCESSPINNO;

    if (u4PinOut > GPIO_OUT_ONE)
        return -EBADINOUTVAL;
    
    pinOutNo = u4Pin / MAX_GPIO_REG_BITS;
    bitOffset = u4Pin % MAX_GPIO_REG_BITS;

    if (pinOutNo == 0) pinReg =  GPIO_OUT0;
    else if (pinOutNo == 1) pinReg =  GPIO_OUT1;
    else if (pinOutNo == 2) pinReg =  GPIO_OUT2;
    else if (pinOutNo == 3) pinReg =  GPIO_OUT3;
    else 
    { 
        return -EEXCESSPINNO;
    }

    spin_lock(&gpio_lock);
    regValue = DRV_Reg32(pinReg);
    
    if (u4PinOut == 1)
        regValue |= (1L << bitOffset);
    else 
        regValue &= ~(1L << bitOffset);        

    DRV_WriteReg32(pinReg, regValue);
    spin_unlock(&gpio_lock);

    return RSUCCESS;
}
EXPORT_SYMBOL(mt_set_gpio_out);

/******************************************************************************
 * mt_get_gpio_out
 * 
 * DESCRIPTION: 
 *   Get the gpio u4pin's output value! 
 * 
 * PARAMETERS: 
 *   u4Pin: gpio pin number
 * 
 * RETURNS: 
 *   Output value of the u4Pin, 0/1
 *   -EEXCESSPINNO : Excess Pin number   
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

s32 mt_get_gpio_out(u32 u4Pin)
{
    u32 pinOutNo;
    u32 bitOffset;
    u32 regValue;
    u32 pinReg;

    if (u4Pin >= MAX_GPIO_PIN)
        return -EEXCESSPINNO;
    
    pinOutNo = u4Pin / MAX_GPIO_REG_BITS;
    bitOffset = u4Pin % MAX_GPIO_REG_BITS;


    if (pinOutNo == 0) pinReg =  GPIO_OUT0;
    else if (pinOutNo == 1) pinReg =  GPIO_OUT1;
    else if (pinOutNo == 2) pinReg =  GPIO_OUT2;
    else if (pinOutNo == 3) pinReg =  GPIO_OUT3;
    else 
    { 
        return -EEXCESSPINNO;
    }

    spin_lock(&gpio_lock);
    regValue = DRV_Reg32(pinReg);
    spin_unlock(&gpio_lock);
    
    return (((regValue & (1L << bitOffset)) != 0)? 1: 0);
}
EXPORT_SYMBOL(mt_get_gpio_out);

/******************************************************************************
 * mt_get_gpio_in
 * 
 * DESCRIPTION: 
 *   Get the gpio u4pin's input value! 
 * 
 * PARAMETERS: 
 *   u4Pin: gpio pin number
 * 
 * RETURNS: 
 *   Input value of the u4Pin, 0/1
 *   -EEXCESSPINNO : Excess Pin number   
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

s32 mt_get_gpio_in(u32 u4Pin)
{
    u32 pinInNo;
    u32 bitOffset;
    u32 regValue;
    u32 pinReg;

    if (u4Pin >= MAX_GPIO_PIN)
        return -EEXCESSPINNO;
    
    pinInNo = u4Pin / MAX_GPIO_REG_BITS;
    bitOffset = u4Pin % MAX_GPIO_REG_BITS;

    if (pinInNo == 0) pinReg =  GPIO_IN0;
    else if (pinInNo == 1) pinReg =  GPIO_IN1;
    else if (pinInNo == 2) pinReg =  GPIO_IN2;
    else if (pinInNo == 3) pinReg =  GPIO_IN3;
    else 
    { 
        return -EEXCESSPINNO;
    }

    spin_lock(&gpio_lock);
    regValue = DRV_Reg32(pinReg);
    spin_unlock(&gpio_lock);
    
    return (((regValue & (1L << bitOffset)) != 0)? 1: 0);
}
EXPORT_SYMBOL(mt_get_gpio_in);

/******************************************************************************
 * mt_set_gpio_driving_current
 * 
 * DESCRIPTION: 
 *   Set the gpio u4pin's driving current register value! 
 * 
 * PARAMETERS: 
 *   u4Pin: gpio pin number
 *   u4PinCurrent: PAD driving and slew rate control
 *      Bit0 : Add 2mA driving current (for 8mA PAD only)
 *      Bit1 : Add 4mA driving current
 *      Bit2 : Add 8mA driving current (for 16mA PAD only)
 *      Bit3 : Slew rate control
 * 
 * RETURNS: 
 *   RSUCCESS
 *   -EEXCESSPINNO : Excess Pin number 
 *   -EEXCESSCURRENT: Excess Driving Current number
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

s32 mt_set_gpio_driving_current(u32 u4Pin, u32 u4PinCurrent)
{
    u32 pinDrivingCurrentNo;
    u32 bitOffset;
    u32 regValue;
    u32 pinReg;
    u32 pinMask = 0x0f;

    if (u4Pin >= MAX_GPIO_PIN)
        return -EEXCESSPINNO;

    if (u4PinCurrent > 0x0f)
    {
        return -EEXCESSCURRENT;
	}      	

    pinDrivingCurrentNo = u4Pin / MAX_GPIO_DC_PER_REG;
    bitOffset = u4Pin % MAX_GPIO_DC_PER_REG;

    if (pinDrivingCurrentNo == 0) pinReg =  GPIO_CFG0;
    else if (pinDrivingCurrentNo == 1 ) pinReg =  GPIO_CFG1;
    else if (pinDrivingCurrentNo == 2 ) pinReg =  GPIO_CFG2;
    else if (pinDrivingCurrentNo == 3 ) pinReg =  GPIO_CFG3;
    else if (pinDrivingCurrentNo == 4 ) pinReg =  GPIO_CFG4;
    else if (pinDrivingCurrentNo == 5 ) pinReg =  GPIO_CFG5;
    else if (pinDrivingCurrentNo == 6 ) pinReg =  GPIO_CFG6;
    else if (pinDrivingCurrentNo == 7 ) pinReg =  GPIO_CFG7;
    else if (pinDrivingCurrentNo == 8 ) pinReg =  GPIO_CFG8;
    else if (pinDrivingCurrentNo == 9 ) pinReg =  GPIO_CFG9;
    else if (pinDrivingCurrentNo == 10) pinReg =  GPIO_CFGA;
    else if (pinDrivingCurrentNo == 11) pinReg =  GPIO_CFGB;
    else if (pinDrivingCurrentNo == 12) pinReg =  GPIO_CFGC;    	    	
    else if (pinDrivingCurrentNo == 13) pinReg =  GPIO_CFGD;
    else if (pinDrivingCurrentNo == 14) pinReg =  GPIO_CFGE;
    else if (pinDrivingCurrentNo == 15) pinReg =  GPIO_CFGF;    	    	
    else 
    {        	
        return -EEXCESSPINNO;
    }

    spin_lock(&gpio_lock);
    regValue = DRV_Reg32(pinReg);

    regValue &= ~(pinMask << (4*bitOffset));
    regValue |= (u4PinCurrent << (4*bitOffset));
    
    DRV_WriteReg32(pinReg, regValue);
    spin_unlock(&gpio_lock);
    
    return RSUCCESS;
}
EXPORT_SYMBOL(mt_set_gpio_driving_current);

/******************************************************************************
 * mt_get_gpio_driving_current
 * 
 * DESCRIPTION: 
 *   Get the gpio u4pin's driving current register value! 
 * 
 * PARAMETERS: 
 *   u4Pin: gpio pin number
 *   
 * 
 * RETURNS: 
 *   u4Pin's gpio driving current
 *      Bit0 : Add 2mA driving current (for 8mA PAD only)
 *      Bit1 : Add 4mA driving current
 *      Bit2 : Add 8mA driving current (for 16mA PAD only)
 *      Bit3 : Slew rate control
 *   -EEXCESSPINNO : Excess Pin number   
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

s32 mt_get_gpio_driving_current(u32 u4Pin)
{
    u32 pinDrivingCurrentNo;
    u32 bitOffset;
    u32 regValue;
    u32 pinReg;

    if (u4Pin >= MAX_GPIO_PIN)
        return -EEXCESSPINNO;
    
    pinDrivingCurrentNo = u4Pin / MAX_GPIO_REG_BITS;
    bitOffset = u4Pin % MAX_GPIO_REG_BITS;

    if (pinDrivingCurrentNo == 0) pinReg =  GPIO_CFG0;
    else if (pinDrivingCurrentNo == 1 ) pinReg =  GPIO_CFG1;
    else if (pinDrivingCurrentNo == 2 ) pinReg =  GPIO_CFG2;
    else if (pinDrivingCurrentNo == 3 ) pinReg =  GPIO_CFG3;
    else if (pinDrivingCurrentNo == 4 ) pinReg =  GPIO_CFG4;
    else if (pinDrivingCurrentNo == 5 ) pinReg =  GPIO_CFG5;
    else if (pinDrivingCurrentNo == 6 ) pinReg =  GPIO_CFG6;
    else if (pinDrivingCurrentNo == 7 ) pinReg =  GPIO_CFG7;
    else if (pinDrivingCurrentNo == 8 ) pinReg =  GPIO_CFG8;
    else if (pinDrivingCurrentNo == 9 ) pinReg =  GPIO_CFG9;
    else if (pinDrivingCurrentNo == 10) pinReg =  GPIO_CFGA;
    else if (pinDrivingCurrentNo == 11) pinReg =  GPIO_CFGB;
    else if (pinDrivingCurrentNo == 12) pinReg =  GPIO_CFGC;    	    	
    else if (pinDrivingCurrentNo == 13) pinReg =  GPIO_CFGD;
    else if (pinDrivingCurrentNo == 14) pinReg =  GPIO_CFGE;
    else if (pinDrivingCurrentNo == 15) pinReg =  GPIO_CFGF;    	    	
    else 
    { 
        return -EEXCESSPINNO;
    }
    spin_lock(&gpio_lock);
    regValue = DRV_Reg32(pinReg);
    spin_unlock(&gpio_lock);
    
    return ((regValue & (0x0f << (4*bitOffset))) >> (4*bitOffset));
}
EXPORT_SYMBOL(mt_get_gpio_driving_current);

/******************************************************************************
 * mt_set_gpio_OCFG
 * 
 * DESCRIPTION: 
 *   Set the gpio's OCFG register! 
 * 
 * PARAMETERS: 
 *   u4Value: OCFG set value !
 *   u4Field: OCFG fields
 * 
 * RETURNS: 
 *   RSUCCESS
 *   -EBADOCFGFIELD: BAD OCFG field
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

s32 mt_set_gpio_OCFG(u32 u4Value, u32 u4Field)
{
    u32 regValue;
    u32 pinMask;

    if((u4Field == GPIO12_OCTL)||(u4Field == GPIO13_OCTL)||(u4Field == GPIO44_OCTL)||
       (u4Field == GPIO45_OCTL)||(u4Field == GPIO46_OCTL)||(u4Field == GPIO62_OCTL)||
       (u4Field == GPIO66_OCTL)||(u4Field == GPIO68_OCTL))
    {
        pinMask = 0x3;
    }
    else if((u4Field == GPIO22_OCTL)||(u4Field == GPIO106_OCTL)||
            (u4Field == GPIO105_OCTL)||(u4Field == GPIO104_OCTL)||
            (u4Field == GPIO64_OCTL)||(u4Field == GPIO10_OCTL)||
            (u4Field == GPIO8_OCTL)||(u4Field == GPIO6_OCTL))
    {
        pinMask = 0x1;
    }
    else
    {
        return -EBADOCFGFIELD;
    }
    
    u4Value = ((u4Value & pinMask) << u4Field);    
    spin_lock(&gpio_lock);
    regValue = DRV_Reg32(GPIO_OCFG);
    regValue &= ~(pinMask << u4Field);
    regValue |= u4Value;
    DRV_WriteReg32(GPIO_OCFG, regValue);
    spin_unlock(&gpio_lock);

    return RSUCCESS;
}
EXPORT_SYMBOL(mt_set_gpio_OCFG);

/******************************************************************************
 * mt_get_gpio_OCFG
 * 
 * DESCRIPTION: 
 *   Get the gpio's OCFG register! 
 * 
 * PARAMETERS: 
 *   u4Field: OCFG fields
 * 
 * RETURNS: 
 *   The value of the u4Field in OCFG
 *   -EBADOCFGFIELD: BAD OCFG field
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

s32 mt_get_gpio_OCFG(u32 u4Field)
{
    u32 regValue;
    u32 pinMask;
    
    spin_lock(&gpio_lock);
    regValue = DRV_Reg32(GPIO_OCFG);
    spin_unlock(&gpio_lock);

    if((u4Field == GPIO12_OCTL)||(u4Field == GPIO13_OCTL)||(u4Field == GPIO44_OCTL)||
       (u4Field == GPIO45_OCTL)||(u4Field == GPIO46_OCTL)||(u4Field == GPIO62_OCTL)||
       (u4Field == GPIO66_OCTL)||(u4Field == GPIO68_OCTL))
    {
        pinMask = 0x3;
    }
    else if((u4Field == GPIO22_OCTL)||(u4Field == GPIO106_OCTL)||(u4Field == GPIO105_OCTL)||
			(u4Field == GPIO104_OCTL)||(u4Field == GPIO64_OCTL)||(u4Field == GPIO10_OCTL)||
			(u4Field == GPIO8_OCTL)||(u4Field == GPIO6_OCTL))
    {
        pinMask = 0x1;
    }
    else
    {
        return -EBADOCFGFIELD;
    }

    regValue &= (pinMask << u4Field);
    regValue = regValue >> u4Field;
    
    return regValue;
}
EXPORT_SYMBOL(mt_get_gpio_OCFG);

/******************************************************************************
 * mt_set_gpio_ICFG
 * 
 * DESCRIPTION: 
 *   Set the gpio's ICFG register! 
 * 
 * PARAMETERS: 
 *   u4Value: ICFG set value !
 *   u4Field: ICFG fields
 * 
 * RETURNS: 
 *   RSUCCESS
 *   -EBADICFGFIELD: BAD ICFG field
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

s32 mt_set_gpio_ICFG(u32 u4Value, u32 u4Field)
{
    u32 regValue;
    u32 pinMask;

    if((u4Field == EINT4_SRC)||(u4Field == EINT5_SRC)||(u4Field == IRDARXD_SRC)||
       (u4Field == KPCOL7_SRC))
    {
        pinMask = 0x3;
    }
    else if((u4Field == EINT0_SRC)||(u4Field == EINT1_SRC)||(u4Field == EINT2_SRC)||
            (u4Field == EINT3_SRC)||(u4Field == EINT6_SRC)||(u4Field == EINT7_SRC)||
            (u4Field == EINT8_SRC)||(u4Field == EINT9_SRC)||(u4Field == EINT10_SRC)||
            (u4Field == EINT11_SRC)||(u4Field == EINT12_SRC)||(u4Field == EINT13_SRC)||
            (u4Field == EINT14_SRC)||(u4Field == EINT15_SRC)||(u4Field == UCTS1_SRC)||
            (u4Field == UCTS2_SRC)||(u4Field == URXD4_SRC)||(u4Field == PWRKEY_SRC)||
            (u4Field == DSPEINT_SRC))
    {
        pinMask = 0x1;
    }
    else
    {
        return -EBADICFGFIELD;
    }
    
    u4Value = ((u4Value & pinMask) << u4Field);    
    spin_lock(&gpio_lock);
    regValue = DRV_Reg32(GPIO_ICFG);
    regValue &= ~(pinMask << u4Field);
    regValue |= u4Value;
    DRV_WriteReg32(GPIO_ICFG, regValue);
    spin_unlock(&gpio_lock);

    return RSUCCESS;
}
EXPORT_SYMBOL(mt_set_gpio_ICFG);

/******************************************************************************
 * mt_get_gpio_ICFG
 * 
 * DESCRIPTION: 
 *   Get the gpio's ICFG register! 
 * 
 * PARAMETERS: 
 *   u4Field: ICFG fields
 * 
 * RETURNS: 
 *   The value of the u4Field in ICFG
 *   -EBADICFGFIELD: BAD ICFG field
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

s32 mt_get_gpio_ICFG(u32 u4Field)
{
    u32 regValue;
    u32 pinMask;
    
    spin_lock(&gpio_lock);
    regValue = DRV_Reg32(GPIO_ICFG);
    spin_unlock(&gpio_lock);

    if((u4Field == EINT4_SRC)||(u4Field == EINT5_SRC)||(u4Field == IRDARXD_SRC)||
       (u4Field == KPCOL7_SRC))
    {
        pinMask = 0x3;
    }
    else if((u4Field == EINT0_SRC)||(u4Field == EINT1_SRC)||(u4Field == EINT2_SRC)||
            (u4Field == EINT3_SRC)||(u4Field == EINT6_SRC)||(u4Field == EINT7_SRC)||
            (u4Field == EINT8_SRC)||(u4Field == EINT9_SRC)||(u4Field == EINT10_SRC)||
            (u4Field == EINT11_SRC)||(u4Field == EINT12_SRC)||(u4Field == EINT13_SRC)||
            (u4Field == EINT14_SRC)||(u4Field == EINT15_SRC)||(u4Field == UCTS1_SRC)||
            (u4Field == UCTS2_SRC)||(u4Field == URXD4_SRC)||(u4Field == PWRKEY_SRC)||
            (u4Field == DSPEINT_SRC))
    {
        pinMask = 0x1;
    }
    else
    {
        return -EBADICFGFIELD;
    }

    regValue &= (pinMask << u4Field);
    regValue = regValue >> u4Field;
    
    return regValue;
}
EXPORT_SYMBOL(mt_get_gpio_ICFG);

/******************************************************************************
 * mt_set_gpio_debug
 * 
 * DESCRIPTION: 
 *   Set the gpio debug output! 
 * 
 * PARAMETERS: 
 *   u4Pin: gpio pin number
 *   u4PinDBG: 
 *      0 : Other GPIO function
 *      1 : Debug signals output enable
 * 
 * RETURNS: 
 *   RSUCCESS
 *   -EEXCESSPINNO : Excess Pin number   
 * 
 * NOTES: 
 *   Must set the gpio pin as output direction !
 *   This function is only used for MTK internal test!
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

s32 mt_set_gpio_debug(u32 u4Pin, u32 u4PinDBG)
{
    u32 pinDebugNo;
    u32 bitOffset;
    u32 regValue;
    u32 pinReg;

    if (u4Pin >= MAX_GPIO_PIN)
        return -EEXCESSPINNO;
    
    if (u4PinDBG > GPIO_DEBUG_ENABLE)
        return -EBADDEBUGVAL;

    pinDebugNo = u4Pin / MAX_GPIO_REG_BITS;
    bitOffset = u4Pin % MAX_GPIO_REG_BITS;

    if (pinDebugNo == 0) pinReg =  GPIO_DBG0;
    else if (pinDebugNo == 1) pinReg =  GPIO_DBG1;
    else if (pinDebugNo == 2) pinReg =  GPIO_DBG2;
    else if (pinDebugNo == 3) pinReg =  GPIO_DBG3;
    else 
    {
        return -EEXCESSPINNO;
    }

    spin_lock(&gpio_lock);
    regValue = DRV_Reg32(pinReg);
    
    if (u4PinDBG == 1)
        regValue |= (1L << bitOffset);
    else 
        regValue &= ~(1L << bitOffset);        

    DRV_WriteReg32(pinReg, regValue);
    spin_unlock(&gpio_lock);
    
    return RSUCCESS;
}
EXPORT_SYMBOL(mt_set_gpio_debug);

/******************************************************************************
 * mt_get_gpio_debug
 * 
 * DESCRIPTION: 
 *   Get the gpio debug output register's value! 
 * 
 * PARAMETERS: 
 *   u4Pin: gpio pin number
 * 
 * RETURNS: 
 *   Gpio debug output register value
 *      0 : Other GPIO function
 *      1 : Debug signals output enable
 *   -EEXCESSPINNO : Excess Pin number   
 * 
 * NOTES: 
 *   This function is only used to read the gpio debug register !
 *   Not means that it can read the debug info from the hardware !
 *   This function is only used for MTK internal test!
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

s32 mt_get_gpio_debug(u32 u4Pin)
{
    u32 pinDebugNo;
    u32 bitOffset;
    u32 regValue;
    u32 pinReg;

    if (u4Pin >= MAX_GPIO_PIN)
        return -EEXCESSPINNO;
    
    pinDebugNo = u4Pin / MAX_GPIO_REG_BITS;
    bitOffset = u4Pin % MAX_GPIO_REG_BITS;

    if (pinDebugNo == 0) pinReg =  GPIO_DBG0;
    else if (pinDebugNo == 1) pinReg = GPIO_DBG1;
    else if (pinDebugNo == 2) pinReg = GPIO_DBG2;
    else if (pinDebugNo == 3) pinReg = GPIO_DBG3;
    else 
    { 
        return -EEXCESSPINNO;
    }

    spin_lock(&gpio_lock);
    regValue = DRV_Reg32(pinReg);
    spin_unlock(&gpio_lock);
    
    return (((regValue & (1L << bitOffset)) != 0)? 1: 0);
}
EXPORT_SYMBOL(mt_get_gpio_debug);

/******************************************************************************
 * mt_set_gpio_TM
 * 
 * DESCRIPTION: 
 *   Set the source of GPIO_DIRX read value! 
 * 
 * PARAMETERS: 
 *   bTM: the source of GPIO_DIRX read value
 *      0 : from the registers GPIO_DIRX
 *      1 : from the real direction control signals at PADs
 * 
 * RETURNS: 
 *   RSUCCESS
 *   -EEXCESSPINNO : Excess Pin number   
 * 
 * NOTES: 
 *   This function is only used for MTK internal test!
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

s32 mt_set_gpio_TM(u8 bTM)
{    
    DRV_WriteReg32(GPIO_TM, bTM);    
    return RSUCCESS;
}
EXPORT_SYMBOL(mt_set_gpio_TM);

/******************************************************************************
 * mt_set_gpio_PinMux
 * 
 * DESCRIPTION: 
 *   Set the gpio's PinMux register field! 
 * 
 * PARAMETERS: 
 *   u4PinMux: PinMux set value
 *   u4Field: PinMux fields
 * 
 * RETURNS: 
 *   RSUCCESS
 *   -EBADMUXFIELD: BAD PINMUX field
 * 
 * NOTES: 
 *   Please reference the MT3351 PinMux table !
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

s32 mt_set_gpio_PinMux(u32 u4PinMux, u32 u4Field)
{
    u32 regValue;
    u32 pinMask;

    if((u4Field == JTAG_CTL)||(u4Field == SPI_CTL)||(u4Field == MC0_CTL)||
       (u4Field == CAM0_CTL)||(u4Field == CAM1_CTL)||(u4Field == NLD_CTLL)||
       (u4Field == DPI_CTL))
    {
        pinMask = 0x3;
    }
    else if((u4Field == UT0_CTL)||(u4Field == UT1_CTL)||(u4Field == UT2_CTL)||
            (u4Field == UT3_CTL)||(u4Field == UT4_CTL)||(u4Field == DAI_CTL)||
            (u4Field == PWM_CTL)||(u4Field == MC1_CTL))
    {
        pinMask = 0x1;
    }
    else if(u4Field == NLD_CTLH)
    {
        pinMask = 0x7;
    }
    else if(u4Field == EP_CTL)
    {
        pinMask = 0xf;
    }
    else
    {        
        return -EBADMUXFIELD;
    }
    
    u4PinMux = ((u4PinMux & pinMask) << u4Field);
    spin_lock(&gpio_lock);
    regValue = DRV_Reg32(PINMUXCFG);
    regValue &= ~(pinMask << u4Field);
    regValue |= u4PinMux;
    DRV_WriteReg32(PINMUXCFG, regValue);
    spin_unlock(&gpio_lock);
    return RSUCCESS;
}
EXPORT_SYMBOL(mt_set_gpio_PinMux);

/******************************************************************************
 * mt_get_gpio_PinMux
 * 
 * DESCRIPTION: 
 *   Get the gpio's PinMux register's field! 
 * 
 * PARAMETERS: 
 *   u4Field: PinMux fields
 * 
 * RETURNS: 
 *   The value of the u4Field in PinMux
 *   -EBADMUXFIELD: BAD PinMux field
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

s32 mt_get_gpio_PinMux(u32 u4Field)
{
    u32 regValue;
    u32 pinMask;
    
    spin_lock(&gpio_lock);
    regValue = DRV_Reg32(PINMUXCFG);
    spin_unlock(&gpio_lock);

    if((u4Field == JTAG_CTL)||(u4Field == SPI_CTL)||(u4Field == MC0_CTL)||
       (u4Field == CAM0_CTL)||(u4Field == CAM1_CTL)||(u4Field == NLD_CTLL)||
       (u4Field == DPI_CTL))
    {
        pinMask = 0x3;
    }
    else if((u4Field == UT0_CTL)||(u4Field == UT1_CTL)||(u4Field == UT2_CTL)||
            (u4Field == UT3_CTL)||(u4Field == UT4_CTL)||(u4Field == DAI_CTL)||
            (u4Field == PWM_CTL)||(u4Field == MC1_CTL))
    {
        pinMask = 0x1;
    }
    else if(u4Field == NLD_CTLH)
    {
        pinMask = 0x7;
    }
    else if(u4Field == EP_CTL)
    {
        pinMask = 0xf;
    }
    else
    {
        return -EBADMUXFIELD;
    }

    regValue &= (pinMask << u4Field);
    regValue = regValue >> u4Field;
    
    return regValue;
}
EXPORT_SYMBOL(mt_get_gpio_PinMux);

/******************************************************************************
 * mt3351_gpio_open
 * 
 * DESCRIPTION: 
 *   For user program to open the gpio device ! 
 * 
 * PARAMETERS: 
 *   struct inode *inode
 *   struct file *file
 * 
 * RETURNS: 
 *   0 : success
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

static int mt_gpio_open(struct inode *inode, struct file *file)
{
    return nonseekable_open(inode, file);
}

/******************************************************************************
 * mt3351_gpio_release
 * 
 * DESCRIPTION:
 *   For user program to release the gpio device ! 
 * 
 * PARAMETERS: 
 *   struct inode *inode
 *   struct file *file
 * 
 * RETURNS: 
 *   0 : success
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

static int mt_gpio_release(struct inode *inode, struct file *file)
{
    return 0;
}

/******************************************************************************
 * mt_gpio_ioctl
 * 
 * DESCRIPTION: 
 *   For user program to control the gpio devices ! 
 * 
 * PARAMETERS: 
 *   struct inode *inode
 *   struct file *file
 *   unsigned int cmd
 *   unsigned long arg
 * 
 * RETURNS: 
 *   0 : success
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

static int mt_gpio_ioctl(struct inode *inode, struct file *file, 
                             unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	
	switch (cmd) 
	{
	case IOW_POKE_RESET_BUTTON:
		printk("GPIO IOCTL : POKE_RESET_BUTTON\n");
		break;

	case IOW_RTCALARM_SUICIDE:
   		printk("GPIO IOCTL : RTCALARM_SUICIDE\n");
		break;
		
	case IOR_HWSTATUS:
		ret = copy_to_user((void __user *) arg, &gpio_hw_status, sizeof gpio_hw_status) ? -EFAULT : 0;
    
		if((gpio_hw_status.u8InputStatus & ONOFF_MASK) == 0x0001)
		{
		    gpio_hw_status.u8InputStatus &= (~(ONOFF_MASK));
		}
      
		statusChanged = 0;
		break;

	case IOR_DISK_ACCESS:
        //printk("GPIO IOCTL : DISK_ACCESS\n");
		break;
	
	case IOR_BT_MODE:
        printk("GPIO IOCTL : BT_MODE\n");
		break;

	case IOR_GET_BT_ERROR:
        //printk("GPIO IOCTL : GET_BT_ERROR\n");
		break;

	case IOR_SET_BT_ERROR:
        //printk("GPIO IOCTL : SET_BT_ERROR\n");
		break;

	case IOW_RESET_ONOFF_STATE:
        printk("GPIO IOCTL : RESET_ONOFF_STATE\n");
        gpio_hw_status.u8InputStatus &= (~(ONOFF_MASK));
		break;

	case IOW_ENABLE_DOCK_UART:
        printk("GPIO IOCTL : ENABLE_DOCK_UART\n");
		break;

	case OBSOLETE_IOW_SET_FM_FREQUENCY:
	    printk("GPIO IOCTL : SET_FM_FREQUENCY\n");
		return -ENOSYS;		
		break;

	case IOW_SET_MEMBUS_SPEED:
	    printk("GPIO IOCTL : SET_MEMBUS_SPEED\n");
		break;
		
	case IOW_CYCLE_DOCK_POWER:
	    printk("GPIO IOCTL : CYCLE_DOCK_POWER\n");
		break;

	case IOW_SET_DVS_HACK:
        printk("GPIO IOCTL : SET_DVS_HACK\n");
		break;
		
	case IOW_USB_VBUS_WAKEUP:
        printk("GPIO IOCTL : USB_VBUS_WAKEUP\n");
		break;

	case IOW_KRAKOW_RDSTMC_HACK_VBUSOFF:
		printk("GPIO IOCTL : KRAKOW_RDSTMC_HACK_VBUSOFF\n");
		break;

	case IOW_KRAKOW_RDSTMC_HACK_VBUSINP:
        printk("GPIO IOCTL : KRAKOW_RDSTMC_HACK_VBUSINP\n");
		break;
  default:
		printk("Invalid ioctl command %u\n", cmd);
		ret = -EINVAL;
		break;
		
	case IOW_FACTORY_TEST_POINT:
        printk("GPIO IOCTL : FACTORY_TEST_POINT\n");
		break;
	}
	return ret;
}

static struct file_operations mt_gpio_fops = 
{
	.owner=		THIS_MODULE,
    .ioctl=		mt_gpio_ioctl,
	.open=		mt_gpio_open,    
	.release=	mt_gpio_release,
};

/*
static struct miscdevice mt_gpio_miscdev=
{
    MISC_DYNAMIC_MINOR, 
    "mt_gpio", 
    &mt_gpio_fops
};
*/

static int mt_gpio_probe(struct device *dev)
{
	int ret;

#if 0
	printk("Initializing wait queue\n");
	init_waitqueue_head(&mt_gpio_wait);
	
	// Get new state
	INIT_WORK(&mt_gpio_workqueue_handle, gpio_status_poll, NULL);
	schedule_work(&mt_gpio_workqueue_handle);
	atomic_set(&mt_disable_gpio_wq, 0);
#endif

    gpio_hw_status.u8DockStatus = GODOCK_NONE;
    gpio_hw_status.u8InputStatus &= ~USB_DETECT_MASK; 
    gpio_hw_status.u8InputStatus &= ~DOCK_LIGHTS_MASK;
    gpio_hw_status.u8InputStatus &= ~DOCK_IGNITION_MASK;
    gpio_hw_status.u8InputStatus &= ~DOCK_EXTMIC_MASK;
    gpio_hw_status.u8InputStatus &= ~DOCK_HEADPHONE_MASK;
    gpio_hw_status.u8InputStatus &= ~DOCK_LINEIN_MASK;
    gpio_hw_status.u8InputStatus &= ~DOCK_IPOD_DETECT;
    gpio_hw_status.u8InputStatus &= ~DOCK_TMC_DETECT;
    gpio_hw_status.u8InputStatus &= ~DOCK_RXD_DETECT_MASK;
    gpio_hw_status.u8InputStatus &= ~DOCK_MAINPOWER_ONOFF;
    //gpio_hw_status.u8ChargeStatus = CHARGE_STATE_FAST_CHARGING;
	
	printk("Registering GPIO device\n");
	ret = register_chrdev(GPIO_MAJOR, GPIO_DEVNAME, &mt_gpio_fops);
	
	if (ret < 0) 
	{
		printk("Unable to register chardev on major=%d (%d)\n", GPIO_MAJOR, ret);
		return ret;
	}

	printk("Success to register chardev on major=%d (%d)\n", GPIO_MAJOR, ret);		

	return 0;
}

static int mt_gpio_remove(struct device *dev)
{
    unregister_chrdev(GPIO_MAJOR, GPIO_DEVNAME);
    return RSUCCESS;
}

static void mt_gpio_shutdown(struct device *dev)
{
    printk("GPIO Shut down\n");
}

static int mt_gpio_suspend(struct device *dev, pm_message_t state)
{
    printk("GPIO Suspend !\n");
    return RSUCCESS;
}

static int mt_gpio_resume(struct device *dev)
{
    printk("GPIO Resume !\n");
    return RSUCCESS;
}

static struct device_driver gpio_driver = 
{
	.name		= "mt3351-gpio",
	.bus		= &platform_bus_type,
	.probe		= mt_gpio_probe,
	.remove		= mt_gpio_remove,
	.shutdown	= mt_gpio_shutdown,
	.suspend	= mt_gpio_suspend,
	.resume		= mt_gpio_resume,
};

/******************************************************************************
Platform release function
******************************************************************************/

static void gpio_release_dev(struct device *dev)
{
	/* Nothing to release? */
}

/******************************************************************************
Platform device structure
******************************************************************************/

static struct platform_device gpio_dev = 
{
    .name = GPIO_DEVICE,
    .id   = -1,
    .dev  = 
    {
        .release  = gpio_release_dev,
    },
};

/******************************************************************************
 * mt_gpio_init
 * 
 * DESCRIPTION:
 *   Initialize the gpio device driver ! 
 * 
 * PARAMETERS: 
 *   None
 * 
 * RETURNS: 
 *   None
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

static int __init mt_gpio_init(void)
{
    u32 ret = 0;
    printk("MediaTek MT3351 gpio driver, version %s\n", VERSION);
    ret = driver_register(&gpio_driver);
    ret = platform_device_register(&gpio_dev);
    //misc_register(&mt_gpio_miscdev);
    return 0;
}

/******************************************************************************
 * mt_gpio_exit
 * 
 * DESCRIPTION: 
 *   Free the device driver ! 
 * 
 * PARAMETERS: 
 *   None
 * 
 * RETURNS: 
 *   None
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 06 oct 2008, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/
 
static void __exit mt_gpio_exit(void)
{
    driver_unregister(&gpio_driver);
    //misc_deregister(&mt_gpio_miscdev);
    return;
}

module_init(mt_gpio_init);
module_exit(mt_gpio_exit);
MODULE_AUTHOR("Koshi, Chiu <koshi.chiu@mediatek.com>");
MODULE_DESCRIPTION("MT3351 General Purpose Driver (GPIO)");
MODULE_LICENSE("GPL");

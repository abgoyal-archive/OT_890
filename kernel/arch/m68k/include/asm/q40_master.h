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

/*
 * Q40 master Chip Control
 * RTC stuff merged for compactnes..
*/

#ifndef _Q40_MASTER_H
#define _Q40_MASTER_H

#include <asm/raw_io.h>


#define q40_master_addr 0xff000000

#define IIRQ_REG            0x0       /* internal IRQ reg */
#define EIRQ_REG            0x4       /* external ... */
#define KEYCODE_REG         0x1c      /* value of received scancode  */
#define DISPLAY_CONTROL_REG 0x18
#define FRAME_CLEAR_REG     0x24
#define LED_REG             0x30

#define Q40_LED_ON()        master_outb(1,LED_REG)
#define Q40_LED_OFF()       master_outb(0,LED_REG)

#define INTERRUPT_REG       IIRQ_REG  /* "native" ints */
#define KEY_IRQ_ENABLE_REG  0x08      /**/
#define KEYBOARD_UNLOCK_REG 0x20      /* clear kb int */

#define SAMPLE_ENABLE_REG   0x14      /* generate SAMPLE ints */
#define SAMPLE_RATE_REG     0x2c
#define SAMPLE_CLEAR_REG    0x28
#define SAMPLE_LOW          0x00
#define SAMPLE_HIGH         0x01

#define FRAME_RATE_REG       0x38      /* generate FRAME ints at 200 HZ rate */

#if 0
#define SER_ENABLE_REG      0x0c      /* allow serial ints to be generated */
#endif
#define EXT_ENABLE_REG      0x10      /* ... rest of the ISA ints ... */


#define master_inb(_reg_)      in_8((unsigned char *)q40_master_addr+_reg_)
#define master_outb(_b_,_reg_)  out_8((unsigned char *)q40_master_addr+_reg_,_b_)

/* RTC defines */

#define Q40_RTC_BASE	    (0xff021ffc)

#define Q40_RTC_YEAR        (*(volatile unsigned char *)(Q40_RTC_BASE+0))
#define Q40_RTC_MNTH        (*(volatile unsigned char *)(Q40_RTC_BASE-4))
#define Q40_RTC_DATE        (*(volatile unsigned char *)(Q40_RTC_BASE-8))
#define Q40_RTC_DOW         (*(volatile unsigned char *)(Q40_RTC_BASE-12))
#define Q40_RTC_HOUR        (*(volatile unsigned char *)(Q40_RTC_BASE-16))
#define Q40_RTC_MINS        (*(volatile unsigned char *)(Q40_RTC_BASE-20))
#define Q40_RTC_SECS        (*(volatile unsigned char *)(Q40_RTC_BASE-24))
#define Q40_RTC_CTRL        (*(volatile unsigned char *)(Q40_RTC_BASE-28))

/* some control bits */
#define Q40_RTC_READ   64  /* prepare for reading */
#define Q40_RTC_WRITE  128

/* define some Q40 specific ints */
#include "q40ints.h"

/* misc defs */
#define DAC_LEFT  ((unsigned char *)0xff008000)
#define DAC_RIGHT ((unsigned char *)0xff008004)

#endif /* _Q40_MASTER_H */

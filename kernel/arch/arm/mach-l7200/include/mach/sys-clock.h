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

/****************************************************************************/
/*
 *  arch/arm/mach-l7200/include/mach/sys-clock.h
 *
 *   Registers and  helper functions for the L7200 Link-Up Systems
 *   System clocks.
 *
 *   (C) Copyright 2000, S A McConnell  (samcconn@cotw.com)
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License. See the file COPYING in the main directory of this archive for
 *  more details.
 */

/****************************************************************************/

#define SYS_CLOCK_OFF   0x00050030  /* Offset from IO_START. */

/* IO_START and IO_BASE are defined in hardware.h */

#define SYS_CLOCK_START (IO_START + SYS_CLCOK_OFF)  /* Physical address */
#define SYS_CLOCK_BASE  (IO_BASE  + SYS_CLOCK_OFF)  /* Virtual address  */

/* Define the interface to the SYS_CLOCK */

typedef struct
{
     unsigned int ENABLE;
     unsigned int ESYNC;
     unsigned int SELECT;
} sys_clock_interface;

#define SYS_CLOCK   ((volatile sys_clock_interface *)(SYS_CLOCK_BASE))

//#define CLOCK_EN    (*(volatile unsigned long *)(PMU_BASE+CLOCK_EN_OFF))
//#define CLOCK_ESYNC (*(volatile unsigned long *)(PMU_BASE+CLOCK_ESYNC_OFF))
//#define CLOCK_SEL   (*(volatile unsigned long *)(PMU_BASE+CLOCK_SEL_OFF))

/* SYS_CLOCK -> ENABLE */

#define SYN_EN          1<<0
#define B18M_EN         1<<1
#define CLK3M6_EN       1<<2
#define BUART_EN        1<<3
#define CLK18MU_EN      1<<4
#define FIR_EN          1<<5
#define MIRN_EN         1<<6
#define UARTM_EN        1<<7
#define SIBADC_EN       1<<8
#define ALTD_EN         1<<9
#define CLCLK_EN        1<<10

/* SYS_CLOCK -> SELECT */

#define CLK18M_DIV      1<<0
#define MIR_SEL         1<<1
#define SSP_SEL         1<<4
#define MM_DIV          1<<5
#define MM_SEL          1<<6
#define ADC_SEL_2       0<<7
#define ADC_SEL_4       1<<7
#define ADC_SEL_8       3<<7
#define ADC_SEL_16      7<<7
#define ADC_SEL_32      0x0f<<7
#define ADC_SEL_64      0x1f<<7
#define ADC_SEL_128     0x3f<<7
#define ALTD_SEL        1<<13

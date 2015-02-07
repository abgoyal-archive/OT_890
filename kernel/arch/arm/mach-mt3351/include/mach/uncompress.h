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
 *  linux/include/asm-arm/arch-mt3351/uncompress.h
 *
 *  Copyright (C) 2006 MediaTek Inc.
 *
 */

#ifndef __ARCH_ARM_UNCOMPRESS_H
#define __ARCH_ARM_UNCOMPRESS_H

//#define MT3351_UART0_DR  (*(volatile unsigned char *)0x2000c000)
//#define MT3351_UART0_ST  (*(volatile unsigned int *) 0x2000c04C)

#define MT3351_UART0_LSR (*(volatile unsigned char *)0x80025014)
#define MT3351_UART0_THR (*(volatile unsigned char *)0x80025000)

/*
 * Append a newline
 */

/*Koshi: Marked for porting */

#if 0
 
static void putstr(const char *s)
{
    while (*s) {
        /* wait for empty slot */
        while ((MT3351_UART0_ST & 0x1f00) == 0);

        MT3351_UART0_DR = *s;

        if (*s == '\n') {
            while ((MT3351_UART0_ST & 0x1f00) == 0);

            MT3351_UART0_DR = '\r';
        }
        s++;
    }
    //while (AMBA_UART_FR & (1 << 3));
}

#endif

#define MT3351_UART0_LCR (*(volatile unsigned char *)0x8002500c)
#define MT3351_UART0_DLL (*(volatile unsigned char *)0x80025000)
#define MT3351_UART0_DLH (*(volatile unsigned char *)0x80025004)
#define MT3351_UART0_FCR (*(volatile unsigned char *)0x80025008)
#define MT3351_UART0_MCR (*(volatile unsigned char *)0x80025010)

#define MT3351_UART0_PWR (*(volatile unsigned char *)0x80001340)

static void decomp_setup(void)
{
	u32 tmp = 1 << 6;
	
	MT3351_UART0_PWR = tmp; /* power on */

	MT3351_UART0_LCR = 0x3;
	tmp = MT3351_UART0_LCR;
	MT3351_UART0_LCR = (tmp | 0x80);
	MT3351_UART0_DLL = 16;
	MT3351_UART0_DLH = 0;
	MT3351_UART0_LCR = tmp;
	MT3351_UART0_FCR = 0x0047;
	MT3351_UART0_MCR = (0x1 | 0x2);
	MT3351_UART0_FCR = (0x10 | 0x80 | 0x07);
}

static inline void putc(char c)
{
    while (!(MT3351_UART0_LSR & 0x20));    
    MT3351_UART0_THR = c;        
}

static inline void flush(void)
{
}

/*
 * nothing to do
 */
#define arch_decomp_setup() decomp_setup()

#define arch_decomp_wdog()

#endif /* __ARCH_ARM_UNCOMPRESS_H */


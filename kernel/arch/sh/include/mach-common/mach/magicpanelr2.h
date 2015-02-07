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
 *  include/asm-sh/magicpanelr2.h
 *
 *  Copyright (C) 2007  Markus Brunner, Mark Jonas
 *
 *  I/O addresses and bitmasks for Magic Panel Release 2 board
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#ifndef __ASM_SH_MAGICPANELR2_H
#define __ASM_SH_MAGICPANELR2_H

#include <asm/gpio.h>

#define __IO_PREFIX mpr2
#include <asm/io_generic.h>


#define SETBITS_OUTB(mask, reg)   ctrl_outb(ctrl_inb(reg) | mask, reg)
#define SETBITS_OUTW(mask, reg)   ctrl_outw(ctrl_inw(reg) | mask, reg)
#define SETBITS_OUTL(mask, reg)   ctrl_outl(ctrl_inl(reg) | mask, reg)
#define CLRBITS_OUTB(mask, reg)   ctrl_outb(ctrl_inb(reg) & ~mask, reg)
#define CLRBITS_OUTW(mask, reg)   ctrl_outw(ctrl_inw(reg) & ~mask, reg)
#define CLRBITS_OUTL(mask, reg)   ctrl_outl(ctrl_inl(reg) & ~mask, reg)


#define PA_LED          PORT_PADR      /* LED */


/* BSC */
#define CMNCR           0xA4FD0000UL
#define CS0BCR          0xA4FD0004UL
#define CS2BCR          0xA4FD0008UL
#define CS3BCR          0xA4FD000CUL
#define CS4BCR          0xA4FD0010UL
#define CS5ABCR         0xA4FD0014UL
#define CS5BBCR         0xA4FD0018UL
#define CS6ABCR         0xA4FD001CUL
#define CS6BBCR         0xA4FD0020UL
#define CS0WCR          0xA4FD0024UL
#define CS2WCR          0xA4FD0028UL
#define CS3WCR          0xA4FD002CUL
#define CS4WCR          0xA4FD0030UL
#define CS5AWCR         0xA4FD0034UL
#define CS5BWCR         0xA4FD0038UL
#define CS6AWCR         0xA4FD003CUL
#define CS6BWCR         0xA4FD0040UL


/* usb */

#define PORT_UTRCTL		0xA405012CUL
#define PORT_UCLKCR_W		0xA40A0008UL

#define INTC_ICR0		0xA414FEE0UL
#define INTC_ICR1		0xA4140010UL
#define INTC_ICR2		0xA4140012UL

/* MTD */

#define MPR2_MTD_BOOTLOADER_SIZE	0x00060000UL
#define MPR2_MTD_KERNEL_SIZE		0x00200000UL

#endif  /* __ASM_SH_MAGICPANELR2_H */

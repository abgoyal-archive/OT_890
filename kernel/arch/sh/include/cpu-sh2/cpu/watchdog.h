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
 * include/asm-sh/cpu-sh2/watchdog.h
 *
 * Copyright (C) 2002, 2003 Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#ifndef __ASM_CPU_SH2_WATCHDOG_H
#define __ASM_CPU_SH2_WATCHDOG_H

/*
 * More SH-2 brilliance .. its not good enough that we can't read
 * and write the same sizes to WTCNT, now we have to read and write
 * with different sizes at different addresses for WTCNT _and_ RSTCSR.
 *
 * At least on the bright side no one has managed to screw over WTCSR
 * in this fashion .. yet.
 */
/* Register definitions */
#define WTCNT		0xfffffe80
#define WTCSR		0xfffffe80
#define RSTCSR		0xfffffe82

#define WTCNT_R		(WTCNT + 1)
#define RSTCSR_R	(RSTCSR + 1)

/* Bit definitions */
#define WTCSR_IOVF	0x80
#define WTCSR_WT	0x40
#define WTCSR_TME	0x20
#define WTCSR_RSTS	0x00

#define RSTCSR_RSTS	0x20

/**
 * 	sh_wdt_read_rstcsr - Read from Reset Control/Status Register
 *
 *	Reads back the RSTCSR value.
 */
static inline __u8 sh_wdt_read_rstcsr(void)
{
	/*
	 * Same read/write brain-damage as for WTCNT here..
	 */
	return ctrl_inb(RSTCSR_R);
}

/**
 * 	sh_wdt_write_csr - Write to Reset Control/Status Register
 *
 * 	@val: Value to write
 *
 * 	Writes the given value @val to the lower byte of the control/status
 * 	register. The upper byte is set manually on each write.
 */
static inline void sh_wdt_write_rstcsr(__u8 val)
{
	/*
	 * Note: Due to the brain-damaged nature of this register,
	 * we can't presently touch the WOVF bit, since the upper byte
	 * has to be swapped for this. So just leave it alone..
	 */
	ctrl_outw((WTCNT_HIGH << 8) | (__u16)val, RSTCSR);
}

#endif /* __ASM_CPU_SH2_WATCHDOG_H */


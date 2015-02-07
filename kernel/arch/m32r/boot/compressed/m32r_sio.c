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
 * arch/m32r/boot/compressed/m32r_sio.c
 *
 * 2003-02-12:	Takeo Takahashi
 * 2006-11-30:	OPSPUT support by Kazuhiro Inaoka
 *
 */

#include <asm/processor.h>

static void putc(char c);

static int puts(const char *s)
{
	char c;
	while ((c = *s++)) putc(c);
	return 0;
}

#if defined(CONFIG_PLAT_M32700UT) || defined(CONFIG_PLAT_OPSPUT)
#include <asm/m32r.h>
#include <asm/io.h>

#define USE_FPGA_MAP	0

#if USE_FPGA_MAP
/*
 * fpga configuration program uses MMU, and define map as same as
 * M32104 uT-Engine board.
 */
#define BOOT_SIO0STS	(volatile unsigned short *)(0x02c00000 + 0x20006)
#define BOOT_SIO0TXB	(volatile unsigned short *)(0x02c00000 + 0x2000c)
#else
#undef PLD_BASE
#if defined(CONFIG_PLAT_OPSPUT)
#define PLD_BASE	0x1cc00000
#else
#define PLD_BASE	0xa4c00000
#endif
#define BOOT_SIO0STS	PLD_ESIO0STS
#define BOOT_SIO0TXB	PLD_ESIO0TXB
#endif

static void putc(char c)
{
	while ((*BOOT_SIO0STS & 0x3) != 0x3)
		cpu_relax();
	if (c == '\n') {
		*BOOT_SIO0TXB = '\r';
		while ((*BOOT_SIO0STS & 0x3) != 0x3)
			cpu_relax();
	}
	*BOOT_SIO0TXB = c;
}
#else /* !(CONFIG_PLAT_M32700UT) */
#if defined(CONFIG_PLAT_MAPPI2)
#define SIO0STS	(volatile unsigned short *)(0xa0efd000 + 14)
#define SIO0TXB	(volatile unsigned short *)(0xa0efd000 + 30)
#else
#define SIO0STS	(volatile unsigned short *)(0x00efd000 + 14)
#define SIO0TXB	(volatile unsigned short *)(0x00efd000 + 30)
#endif

static void putc(char c)
{
	while ((*SIO0STS & 0x1) == 0)
		cpu_relax();
	if (c == '\n') {
		*SIO0TXB = '\r';
		while ((*SIO0STS & 0x1) == 0)
			cpu_relax();
	}
	*SIO0TXB = c;
}
#endif

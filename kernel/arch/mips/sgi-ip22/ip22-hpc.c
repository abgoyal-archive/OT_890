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
 * ip22-hpc.c: Routines for generic manipulation of the HPC controllers.
 *
 * Copyright (C) 1996 David S. Miller (dm@engr.sgi.com)
 * Copyright (C) 1998 Ralf Baechle
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>

#include <asm/io.h>
#include <asm/sgi/hpc3.h>
#include <asm/sgi/ioc.h>
#include <asm/sgi/ip22.h>

struct hpc3_regs *hpc3c0, *hpc3c1;

EXPORT_SYMBOL(hpc3c0);
EXPORT_SYMBOL(hpc3c1);

struct sgioc_regs *sgioc;

EXPORT_SYMBOL(sgioc);

/* We need software copies of these because they are write only. */
u8 sgi_ioc_reset, sgi_ioc_write;

extern char *system_type;

void __init sgihpc_init(void)
{
	/* ioremap can't fail */
	hpc3c0 = (struct hpc3_regs *)
		 ioremap(HPC3_CHIP0_BASE, sizeof(struct hpc3_regs));
	hpc3c1 = (struct hpc3_regs *)
		 ioremap(HPC3_CHIP1_BASE, sizeof(struct hpc3_regs));
	/* IOC lives in PBUS PIO channel 6 */
	sgioc = (struct sgioc_regs *)hpc3c0->pbus_extregs[6];

	hpc3c0->pbus_piocfg[6][0] |= HPC3_PIOCFG_DS16;
	if (ip22_is_fullhouse()) {
		/* Full House comes with INT2 which lives in PBUS PIO
		 * channel 4 */
		sgint = (struct sgint_regs *)hpc3c0->pbus_extregs[4];
		system_type = "SGI Indigo2";
	} else {
		/* Guiness comes with INT3 which is part of IOC */
		sgint = &sgioc->int3;
		system_type = "SGI Indy";
	}

	sgi_ioc_reset = (SGIOC_RESET_PPORT | SGIOC_RESET_KBDMOUSE |
			 SGIOC_RESET_EISA | SGIOC_RESET_ISDN |
			 SGIOC_RESET_LC0OFF);

	sgi_ioc_write = (SGIOC_WRITE_EASEL | SGIOC_WRITE_NTHRESH |
			 SGIOC_WRITE_TPSPEED | SGIOC_WRITE_EPSEL |
			 SGIOC_WRITE_U0AMODE | SGIOC_WRITE_U1AMODE);

	sgioc->reset = sgi_ioc_reset;
	sgioc->write = sgi_ioc_write;
}

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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Miscellaneous ARCS PROM routines.
 *
 * Copyright (C) 1996 David S. Miller (dm@engr.sgi.com)
 * Copyright (C) 1999 Ralf Baechle (ralf@gnu.org)
 * Copyright (C) 1999 Silicon Graphics, Inc.
 */
#include <linux/init.h>
#include <linux/kernel.h>

#include <asm/bcache.h>

#include <asm/fw/arc/types.h>
#include <asm/sgialib.h>
#include <asm/bootinfo.h>
#include <asm/system.h>

VOID
ArcHalt(VOID)
{
	bc_disable();
	local_irq_disable();
	ARC_CALL0(halt);
never:	goto never;
}

VOID
ArcPowerDown(VOID)
{
	bc_disable();
	local_irq_disable();
	ARC_CALL0(pdown);
never:	goto never;
}

/* XXX is this a soft reset basically? XXX */
VOID
ArcRestart(VOID)
{
	bc_disable();
	local_irq_disable();
	ARC_CALL0(restart);
never:	goto never;
}

VOID
ArcReboot(VOID)
{
	bc_disable();
	local_irq_disable();
	ARC_CALL0(reboot);
never:	goto never;
}

VOID
ArcEnterInteractiveMode(VOID)
{
	bc_disable();
	local_irq_disable();
	ARC_CALL0(imode);
never:	goto never;
}

LONG
ArcSaveConfiguration(VOID)
{
	return ARC_CALL0(cfg_save);
}

struct linux_sysid *
ArcGetSystemId(VOID)
{
	return (struct linux_sysid *) ARC_CALL0(get_sysid);
}

VOID __init
ArcFlushAllCaches(VOID)
{
	ARC_CALL0(cache_flush);
}

DISPLAY_STATUS * __init ArcGetDisplayStatus(ULONG FileID)
{
	return (DISPLAY_STATUS *) ARC_CALL1(GetDisplayStatus, FileID);
}

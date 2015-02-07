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
 * Derived from IRIX <sys/SN/gda.h>.
 *
 * Copyright (C) 1992 - 1997, 2000 Silicon Graphics, Inc.
 *
 * gda.h -- Contains the data structure for the global data area,
 * 	The GDA contains information communicated between the
 *	PROM, SYMMON, and the kernel.
 */
#ifndef _ASM_SN_GDA_H
#define _ASM_SN_GDA_H

#include <asm/sn/addrs.h>

#define GDA_MAGIC	0x58464552

/*
 * GDA Version History
 *
 * Version #	| Change
 * -------------+-------------------------------------------------------
 * 	1	| Initial SN0 version
 * 	2	| Prom sets g_partid field to the partition number. 0 IS
 *		| a valid partition #.
 */

#define GDA_VERSION	2	/* Current GDA version # */

#define G_MAGICOFF	0
#define G_VERSIONOFF	4
#define G_PROMOPOFF	6
#define G_MASTEROFF	8
#define G_VDSOFF	12
#define G_HKDNORMOFF	16
#define G_HKDUTLBOFF	24
#define G_HKDXUTLBOFF	32
#define G_PARTIDOFF	40
#define G_TABLEOFF	128

#ifndef __ASSEMBLY__

typedef struct gda {
	u32	g_magic;	/* GDA magic number */
	u16	g_version;	/* Version of this structure */
	u16	g_masterid;	/* The NASID:CPUNUM of the master cpu */
	u32	g_promop;	/* Passes requests from the kernel to prom */
	u32	g_vds;		/* Store the virtual dipswitches here */
	void	**g_hooked_norm;/* ptr to pda loc for norm hndlr */
	void	**g_hooked_utlb;/* ptr to pda loc for utlb hndlr */
	void	**g_hooked_xtlb;/* ptr to pda loc for xtlb hndlr */
	int	g_partid;	/* partition id */
	int	g_symmax;	/* Max symbols in name table. */
	void	*g_dbstab;	/* Address of idbg symbol table */
	char	*g_nametab;	/* Address of idbg name table */
	void	*g_ktext_repmask;
				/* Pointer to a mask of nodes with copies
				 * of the kernel. */
	char	g_padding[56];	/* pad out to 128 bytes */
	nasid_t	g_nasidtable[MAX_COMPACT_NODES]; /* NASID of each node,
						  * indexed by cnodeid.
						  */
} gda_t;

#define GDA ((gda_t*) GDA_ADDR(get_nasid()))

#endif /* !__ASSEMBLY__ */
/*
 * Define:	PART_GDA_VERSION
 * Purpose:	Define the minimum version of the GDA required, lower
 *		revisions assume GDA is NOT set up, and read partition
 *		information from the board info.
 */
#define	PART_GDA_VERSION	2

/*
 * The following requests can be sent to the PROM during startup.
 */

#define PROMOP_MAGIC		0x0ead0000
#define PROMOP_MAGIC_MASK	0x0fff0000

#define PROMOP_BIST_SHIFT       11
#define PROMOP_BIST_MASK        (0x3 << 11)

#define PROMOP_REG		PI_ERR_STACK_ADDR_A

#define PROMOP_INVALID		(PROMOP_MAGIC | 0x00)
#define PROMOP_HALT             (PROMOP_MAGIC | 0x10)
#define PROMOP_POWERDOWN        (PROMOP_MAGIC | 0x20)
#define PROMOP_RESTART          (PROMOP_MAGIC | 0x30)
#define PROMOP_REBOOT           (PROMOP_MAGIC | 0x40)
#define PROMOP_IMODE            (PROMOP_MAGIC | 0x50)

#define PROMOP_CMD_MASK		0x00f0
#define PROMOP_OPTIONS_MASK	0xfff0

#define PROMOP_SKIP_DIAGS	0x0100		/* don't bother running diags */
#define PROMOP_SKIP_MEMINIT	0x0200		/* don't bother initing memory */
#define PROMOP_SKIP_DEVINIT	0x0400		/* don't bother initing devices */
#define PROMOP_BIST1		0x0800		/* keep track of which BIST ran */
#define PROMOP_BIST2		0x1000		/* keep track of which BIST ran */

#endif /* _ASM_SN_GDA_H */

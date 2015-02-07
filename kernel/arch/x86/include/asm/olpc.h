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

/* OLPC machine specific definitions */

#ifndef _ASM_X86_OLPC_H
#define _ASM_X86_OLPC_H

#include <asm/geode.h>

struct olpc_platform_t {
	int flags;
	uint32_t boardrev;
	int ecver;
};

#define OLPC_F_PRESENT		0x01
#define OLPC_F_DCON		0x02
#define OLPC_F_VSA		0x04

#ifdef CONFIG_OLPC

extern struct olpc_platform_t olpc_platform_info;

/*
 * OLPC board IDs contain the major build number within the mask 0x0ff0,
 * and the minor build number withing 0x000f.  Pre-builds have a minor
 * number less than 8, and normal builds start at 8.  For example, 0x0B10
 * is a PreB1, and 0x0C18 is a C1.
 */

static inline uint32_t olpc_board(uint8_t id)
{
	return (id << 4) | 0x8;
}

static inline uint32_t olpc_board_pre(uint8_t id)
{
	return id << 4;
}

static inline int machine_is_olpc(void)
{
	return (olpc_platform_info.flags & OLPC_F_PRESENT) ? 1 : 0;
}

/*
 * The DCON is OLPC's Display Controller.  It has a number of unique
 * features that we might want to take advantage of..
 */
static inline int olpc_has_dcon(void)
{
	return (olpc_platform_info.flags & OLPC_F_DCON) ? 1 : 0;
}

/*
 * The VSA is software from AMD that typical Geode bioses will include.
 * It is used to emulate the PCI bus, VGA, etc.  OLPC's Open Firmware does
 * not include the VSA; instead, PCI is emulated by the kernel.
 *
 * The VSA is described further in arch/x86/pci/olpc.c.
 */
static inline int olpc_has_vsa(void)
{
	return (olpc_platform_info.flags & OLPC_F_VSA) ? 1 : 0;
}

/*
 * The "Mass Production" version of OLPC's XO is identified as being model
 * C2.  During the prototype phase, the following models (in chronological
 * order) were created: A1, B1, B2, B3, B4, C1.  The A1 through B2 models
 * were based on Geode GX CPUs, and models after that were based upon
 * Geode LX CPUs.  There were also some hand-assembled models floating
 * around, referred to as PreB1, PreB2, etc.
 */
static inline int olpc_board_at_least(uint32_t rev)
{
	return olpc_platform_info.boardrev >= rev;
}

#else

static inline int machine_is_olpc(void)
{
	return 0;
}

static inline int olpc_has_dcon(void)
{
	return 0;
}

static inline int olpc_has_vsa(void)
{
	return 0;
}

#endif

/* EC related functions */

extern int olpc_ec_cmd(unsigned char cmd, unsigned char *inbuf, size_t inlen,
		unsigned char *outbuf, size_t outlen);

extern int olpc_ec_mask_set(uint8_t bits);
extern int olpc_ec_mask_unset(uint8_t bits);

/* EC commands */

#define EC_FIRMWARE_REV		0x08

/* SCI source values */

#define EC_SCI_SRC_EMPTY	0x00
#define EC_SCI_SRC_GAME		0x01
#define EC_SCI_SRC_BATTERY	0x02
#define EC_SCI_SRC_BATSOC	0x04
#define EC_SCI_SRC_BATERR	0x08
#define EC_SCI_SRC_EBOOK	0x10
#define EC_SCI_SRC_WLAN		0x20
#define EC_SCI_SRC_ACPWR	0x40
#define EC_SCI_SRC_ALL		0x7F

/* GPIO assignments */

#define OLPC_GPIO_MIC_AC	geode_gpio(1)
#define OLPC_GPIO_DCON_IRQ	geode_gpio(7)
#define OLPC_GPIO_THRM_ALRM	geode_gpio(10)
#define OLPC_GPIO_SMB_CLK	geode_gpio(14)
#define OLPC_GPIO_SMB_DATA	geode_gpio(15)
#define OLPC_GPIO_WORKAUX	geode_gpio(24)
#define OLPC_GPIO_LID		geode_gpio(26)
#define OLPC_GPIO_ECSCI		geode_gpio(27)

#endif /* _ASM_X86_OLPC_H */

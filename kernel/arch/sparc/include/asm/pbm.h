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
 *
 * pbm.h: PCI bus module pseudo driver software state
 *        Adopted from sparc64 by V. Roganov and G. Raiko
 *
 * Original header:
 * pbm.h: U2P PCI bus module pseudo driver software state.
 *
 * Copyright (C) 1997 David S. Miller (davem@caip.rutgers.edu)
 *
 * To put things into perspective, consider sparc64 with a few PCI controllers.
 * Each type would have an own structure, with instances related one to one.
 * We have only pcic on sparc, but we want to be compatible with sparc64 pbm.h.
 * All three represent different abstractions.
 *   pci_bus  - Linux PCI subsystem view of a PCI bus (including bridged buses)
 *   pbm      - Arch-specific view of a PCI bus (sparc or sparc64)
 *   pcic     - Chip-specific information for PCIC.
 */

#ifndef __SPARC_PBM_H
#define __SPARC_PBM_H

#include <linux/pci.h>
#include <asm/oplib.h>
#include <asm/prom.h>

struct linux_pbm_info {
	int		prom_node;
	char		prom_name[64];
	/* struct linux_prom_pci_ranges	pbm_ranges[PROMREG_MAX]; */
	/* int		num_pbm_ranges; */

	/* Now things for the actual PCI bus probes. */
	unsigned int	pci_first_busno;	/* Can it be nonzero? */
	struct pci_bus	*pci_bus;		/* Was inline, MJ allocs now */
};

/* PCI devices which are not bridges have this placed in their pci_dev
 * sysdata member.  This makes OBP aware PCI device drivers easier to
 * code.
 */
struct pcidev_cookie {
	struct linux_pbm_info		*pbm;
	struct device_node		*prom_node;
};

#endif /* !(__SPARC_PBM_H) */

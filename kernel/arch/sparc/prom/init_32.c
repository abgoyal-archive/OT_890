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
 * init.c:  Initialize internal variables used by the PROM
 *          library functions.
 *
 * Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 * Copyright (C) 1998 Jakub Jelinek (jj@sunsite.mff.cuni.cz)
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

#include <asm/openprom.h>
#include <asm/oplib.h>

struct linux_romvec *romvec;
EXPORT_SYMBOL(romvec);

enum prom_major_version prom_vers;
unsigned int prom_rev, prom_prev;

/* The root node of the prom device tree. */
int prom_root_node;
EXPORT_SYMBOL(prom_root_node);

/* Pointer to the device tree operations structure. */
struct linux_nodeops *prom_nodeops;

/* You must call prom_init() before you attempt to use any of the
 * routines in the prom library.  It returns 0 on success, 1 on
 * failure.  It gets passed the pointer to the PROM vector.
 */

extern void prom_meminit(void);
extern void prom_ranges_init(void);

void __init prom_init(struct linux_romvec *rp)
{
	romvec = rp;

	switch(romvec->pv_romvers) {
	case 0:
		prom_vers = PROM_V0;
		break;
	case 2:
		prom_vers = PROM_V2;
		break;
	case 3:
		prom_vers = PROM_V3;
		break;
	default:
		prom_printf("PROMLIB: Bad PROM version %d\n",
			    romvec->pv_romvers);
		prom_halt();
		break;
	};

	prom_rev = romvec->pv_plugin_revision;
	prom_prev = romvec->pv_printrev;
	prom_nodeops = romvec->pv_nodeops;

	prom_root_node = prom_getsibling(0);
	if((prom_root_node == 0) || (prom_root_node == -1))
		prom_halt();

	if((((unsigned long) prom_nodeops) == 0) || 
	   (((unsigned long) prom_nodeops) == -1))
		prom_halt();

	prom_meminit();

	prom_ranges_init();

	printk("PROMLIB: Sun Boot Prom Version %d Revision %d\n",
	       romvec->pv_romvers, prom_rev);

	/* Initialization successful. */
	return;
}

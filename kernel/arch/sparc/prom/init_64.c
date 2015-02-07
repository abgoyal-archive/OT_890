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
 * Copyright (C) 1996,1997 Jakub Jelinek (jj@sunsite.mff.cuni.cz)
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/ctype.h>

#include <asm/openprom.h>
#include <asm/oplib.h>

/* OBP version string. */
char prom_version[80];

/* The root node of the prom device tree. */
int prom_stdin, prom_stdout;
int prom_chosen_node;

/* You must call prom_init() before you attempt to use any of the
 * routines in the prom library.  It returns 0 on success, 1 on
 * failure.  It gets passed the pointer to the PROM vector.
 */

extern void prom_cif_init(void *, void *);

void __init prom_init(void *cif_handler, void *cif_stack)
{
	int node;

	prom_cif_init(cif_handler, cif_stack);

	prom_chosen_node = prom_finddevice(prom_chosen_path);
	if (!prom_chosen_node || prom_chosen_node == -1)
		prom_halt();

	prom_stdin = prom_getint(prom_chosen_node, "stdin");
	prom_stdout = prom_getint(prom_chosen_node, "stdout");

	node = prom_finddevice("/openprom");
	if (!node || node == -1)
		prom_halt();

	prom_getstring(node, "version", prom_version, sizeof(prom_version));

	prom_printf("\n");
}

void __init prom_init_report(void)
{
	printk("PROMLIB: Sun IEEE Boot Prom '%s'\n", prom_version);
	printk("PROMLIB: Root node compatible: %s\n", prom_root_compatible);
}

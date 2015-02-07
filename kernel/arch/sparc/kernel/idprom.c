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
 * idprom.c: Routines to load the idprom into kernel addresses and
 *           interpret the data contained within.
 *
 * Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/module.h>

#include <asm/oplib.h>
#include <asm/idprom.h>

struct idprom *idprom;
EXPORT_SYMBOL(idprom);

static struct idprom idprom_buffer;

#ifdef CONFIG_SPARC32
#include <asm/machines.h>  /* Fun with Sun released architectures. */

/* Here is the master table of Sun machines which use some implementation
 * of the Sparc CPU and have a meaningful IDPROM machtype value that we
 * know about.  See asm-sparc/machines.h for empirical constants.
 */
static struct Sun_Machine_Models Sun_Machines[NUM_SUN_MACHINES] = {
/* First, Sun4's */
{ .name = "Sun 4/100 Series",        .id_machtype = (SM_SUN4 | SM_4_110) },
{ .name = "Sun 4/200 Series",        .id_machtype = (SM_SUN4 | SM_4_260) },
{ .name = "Sun 4/300 Series",        .id_machtype = (SM_SUN4 | SM_4_330) },
{ .name = "Sun 4/400 Series",        .id_machtype = (SM_SUN4 | SM_4_470) },
/* Now, Sun4c's */
{ .name = "Sun4c SparcStation 1",    .id_machtype = (SM_SUN4C | SM_4C_SS1) },
{ .name = "Sun4c SparcStation IPC",  .id_machtype = (SM_SUN4C | SM_4C_IPC) },
{ .name = "Sun4c SparcStation 1+",   .id_machtype = (SM_SUN4C | SM_4C_SS1PLUS) },
{ .name = "Sun4c SparcStation SLC",  .id_machtype = (SM_SUN4C | SM_4C_SLC) },
{ .name = "Sun4c SparcStation 2",    .id_machtype = (SM_SUN4C | SM_4C_SS2) },
{ .name = "Sun4c SparcStation ELC",  .id_machtype = (SM_SUN4C | SM_4C_ELC) },
{ .name = "Sun4c SparcStation IPX",  .id_machtype = (SM_SUN4C | SM_4C_IPX) },
/* Finally, early Sun4m's */
{ .name = "Sun4m SparcSystem600",    .id_machtype = (SM_SUN4M | SM_4M_SS60) },
{ .name = "Sun4m SparcStation10/20", .id_machtype = (SM_SUN4M | SM_4M_SS50) },
{ .name = "Sun4m SparcStation5",     .id_machtype = (SM_SUN4M | SM_4M_SS40) },
/* One entry for the OBP arch's which are sun4d, sun4e, and newer sun4m's */
{ .name = "Sun4M OBP based system",  .id_machtype = (SM_SUN4M_OBP | 0x0) } };

static void __init display_system_type(unsigned char machtype)
{
	char sysname[128];
	register int i;

	for (i = 0; i < NUM_SUN_MACHINES; i++) {
		if (Sun_Machines[i].id_machtype == machtype) {
			if (machtype != (SM_SUN4M_OBP | 0x00) ||
			    prom_getproperty(prom_root_node, "banner-name",
					     sysname, sizeof(sysname)) <= 0)
				printk(KERN_WARNING "TYPE: %s\n",
				       Sun_Machines[i].name);
			else
				printk(KERN_WARNING "TYPE: %s\n", sysname);
			return;
		}
	}

	prom_printf("IDPROM: Warning, bogus id_machtype value, 0x%x\n", machtype);
}
#else
static void __init display_system_type(unsigned char machtype)
{
}
#endif
/* Calculate the IDPROM checksum (xor of the data bytes). */
static unsigned char __init calc_idprom_cksum(struct idprom *idprom)
{
	unsigned char cksum, i, *ptr = (unsigned char *)idprom;

	for (i = cksum = 0; i <= 0x0E; i++)
		cksum ^= *ptr++;

	return cksum;
}

/* Create a local IDPROM copy, verify integrity, and display information. */
void __init idprom_init(void)
{
	prom_get_idprom((char *) &idprom_buffer, sizeof(idprom_buffer));

	idprom = &idprom_buffer;

	if (idprom->id_format != 0x01)
		prom_printf("IDPROM: Warning, unknown format type!\n");

	if (idprom->id_cksum != calc_idprom_cksum(idprom))
		prom_printf("IDPROM: Warning, checksum failure (nvram=%x, calc=%x)!\n",
			    idprom->id_cksum, calc_idprom_cksum(idprom));

	display_system_type(idprom->id_machtype);

	printk(KERN_WARNING "Ethernet address: %pM\n", idprom->id_ethaddr);
}

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
 * PCBIT-D module support
 *
 * Copyright (C) 1996 Universidade de Lisboa
 * 
 * Written by Pedro Roque Marques (roque@di.fc.ul.pt)
 *
 * This software may be used and distributed according to the terms of 
 * the GNU General Public License, incorporated herein by reference.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>

#include <linux/isdnif.h>
#include "pcbit.h"

MODULE_DESCRIPTION("ISDN4Linux: Driver for PCBIT-T card");
MODULE_AUTHOR("Pedro Roque Marques");
MODULE_LICENSE("GPL");

static int mem[MAX_PCBIT_CARDS];
static int irq[MAX_PCBIT_CARDS];

module_param_array(mem, int, NULL, 0);
module_param_array(irq, int, NULL, 0);

static int num_boards;
struct pcbit_dev * dev_pcbit[MAX_PCBIT_CARDS];

static int __init pcbit_init(void)
{
	int board;

	num_boards = 0;

	printk(KERN_NOTICE 
	       "PCBIT-D device driver v 0.5-fjpc0 19991204 - "
	       "Copyright (C) 1996 Universidade de Lisboa\n");

	if (mem[0] || irq[0]) 
	{
		for (board=0; board < MAX_PCBIT_CARDS && mem[board] && irq[board]; board++)
		{
			if (!mem[board])
				mem[board] = 0xD0000;
			if (!irq[board])
				irq[board] = 5;
			
			if (pcbit_init_dev(board, mem[board], irq[board]) == 0)
				num_boards++;
		
			else 
			{
				printk(KERN_WARNING 
				       "pcbit_init failed for dev %d", 
				       board + 1);
				return -EIO;
			}
		}
	}

	/* Hardcoded default settings detection */

	if (!num_boards)
	{
		printk(KERN_INFO 
		       "Trying to detect board using default settings\n");
		if (pcbit_init_dev(0, 0xD0000, 5) == 0)
			num_boards++;
		else
			return -EIO;
	}
	return 0;
}

static void __exit pcbit_exit(void)
{
#ifdef MODULE
	int board;

	for (board = 0; board < num_boards; board++)
		pcbit_terminate(board);
	printk(KERN_NOTICE 
	       "PCBIT-D module unloaded\n");
#endif
}

#ifndef MODULE
#define MAX_PARA	(MAX_PCBIT_CARDS * 2)
static int __init pcbit_setup(char *line)
{
	int i, j, argc;
	char *str;
	int ints[MAX_PARA+1];

	str = get_options(line, MAX_PARA, ints);
	argc = ints[0];
	i = 0;
	j = 1;

	while (argc && (i<MAX_PCBIT_CARDS)) {

		if (argc) {
			mem[i]	= ints[j];
			j++; argc--;
		}
		
		if (argc) {
			irq[i]	= ints[j];
			j++; argc--;
		}

		i++;
	}
	return(1);
}
__setup("pcbit=", pcbit_setup);
#endif

module_init(pcbit_init);
module_exit(pcbit_exit);


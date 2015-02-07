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
 * Copyright (C) 1994, 1995, 1996, 1999, 2000 by Ralf Baechle
 * Copyright (C) 1999, 2000 by Silicon Graphics
 * Copyright (C) 2002  Maciej W. Rozycki
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/signal.h>	/* for SIGBUS */
#include <linux/sched.h>	/* schow_regs(), force_sig() */

#include <asm/module.h>
#include <asm/sn/addrs.h>
#include <asm/sn/arch.h>
#include <asm/sn/sn0/hub.h>
#include <asm/tlbdebug.h>
#include <asm/traps.h>
#include <asm/uaccess.h>

static void dump_hub_information(unsigned long errst0, unsigned long errst1)
{
	static char *err_type[2][8] = {
		{ NULL, "Uncached Partial Read PRERR", "DERR", "Read Timeout",
		  NULL, NULL, NULL, NULL },
		{ "WERR", "Uncached Partial Write", "PWERR", "Write Timeout",
		  NULL, NULL, NULL, NULL }
	};
	int wrb = errst1 & PI_ERR_ST1_WRBRRB_MASK;

	if (!(errst0 & PI_ERR_ST0_VALID_MASK)) {
		printk("Hub does not contain valid error information\n");
		return;
	}


	printk("Hub has valid error information:\n");
	if (errst0 & PI_ERR_ST0_OVERRUN_MASK)
		printk("Overrun is set.  Error stack may contain additional "
		       "information.\n");
	printk("Hub error address is %08lx\n",
	       (errst0 & PI_ERR_ST0_ADDR_MASK) >> (PI_ERR_ST0_ADDR_SHFT - 3));
	printk("Incoming message command 0x%lx\n",
	       (errst0 & PI_ERR_ST0_CMD_MASK) >> PI_ERR_ST0_CMD_SHFT);
	printk("Supplemental field of incoming message is 0x%lx\n",
	       (errst0 & PI_ERR_ST0_SUPPL_MASK) >> PI_ERR_ST0_SUPPL_SHFT);
	printk("T5 Rn (for RRB only) is 0x%lx\n",
	       (errst0 & PI_ERR_ST0_REQNUM_MASK) >> PI_ERR_ST0_REQNUM_SHFT);
	printk("Error type is %s\n", err_type[wrb]
	       [(errst0 & PI_ERR_ST0_TYPE_MASK) >> PI_ERR_ST0_TYPE_SHFT]
		? : "invalid");
}

int ip27_be_handler(struct pt_regs *regs, int is_fixup)
{
	unsigned long errst0, errst1;
	int data = regs->cp0_cause & 4;
	int cpu = LOCAL_HUB_L(PI_CPU_NUM);

	if (is_fixup)
		return MIPS_BE_FIXUP;

	printk("Slice %c got %cbe at 0x%lx\n", 'A' + cpu, data ? 'd' : 'i',
	       regs->cp0_epc);
	printk("Hub information:\n");
	printk("ERR_INT_PEND = 0x%06lx\n", LOCAL_HUB_L(PI_ERR_INT_PEND));
	errst0 = LOCAL_HUB_L(cpu ? PI_ERR_STATUS0_B : PI_ERR_STATUS0_A);
	errst1 = LOCAL_HUB_L(cpu ? PI_ERR_STATUS1_B : PI_ERR_STATUS1_A);
	dump_hub_information(errst0, errst1);
	show_regs(regs);
	dump_tlb_all();
	while(1);
	force_sig(SIGBUS, current);
}

void __init ip27_be_init(void)
{
	/* XXX Initialize all the Hub & Bridge error handling here.  */
	int cpu = LOCAL_HUB_L(PI_CPU_NUM);
	int cpuoff = cpu << 8;

	board_be_handler = ip27_be_handler;

	LOCAL_HUB_S(PI_ERR_INT_PEND,
	            cpu ? PI_ERR_CLEAR_ALL_B : PI_ERR_CLEAR_ALL_A);
	LOCAL_HUB_S(PI_ERR_INT_MASK_A + cpuoff, 0);
	LOCAL_HUB_S(PI_ERR_STACK_ADDR_A + cpuoff, 0);
	LOCAL_HUB_S(PI_ERR_STACK_SIZE, 0);	/* Disable error stack */
	LOCAL_HUB_S(PI_SYSAD_ERRCHK_EN, PI_SYSAD_CHECK_ALL);
}

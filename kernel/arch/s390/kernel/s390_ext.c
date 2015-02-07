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
 *  arch/s390/kernel/s390_ext.c
 *
 *  S390 version
 *    Copyright (C) 1999,2000 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Holger Smolinski (Holger.Smolinski@de.ibm.com),
 *               Martin Schwidefsky (schwidefsky@de.ibm.com)
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/kernel_stat.h>
#include <linux/interrupt.h>
#include <asm/cpu.h>
#include <asm/lowcore.h>
#include <asm/s390_ext.h>
#include <asm/irq_regs.h>
#include <asm/irq.h>
#include "entry.h"

/*
 * ext_int_hash[index] is the start of the list for all external interrupts
 * that hash to this index. With the current set of external interrupts 
 * (0x1202 external call, 0x1004 cpu timer, 0x2401 hwc console, 0x4000
 * iucv and 0x2603 pfault) this is always the first element. 
 */
ext_int_info_t *ext_int_hash[256] = { NULL, };

static inline int ext_hash(__u16 code)
{
	return (code + (code >> 9)) & 0xff;
}

int register_external_interrupt(__u16 code, ext_int_handler_t handler)
{
        ext_int_info_t *p;
        int index;

	p = kmalloc(sizeof(ext_int_info_t), GFP_ATOMIC);
        if (p == NULL)
                return -ENOMEM;
        p->code = code;
        p->handler = handler;
	index = ext_hash(code);
        p->next = ext_int_hash[index];
        ext_int_hash[index] = p;
        return 0;
}

int register_early_external_interrupt(__u16 code, ext_int_handler_t handler,
				      ext_int_info_t *p)
{
        int index;

        if (p == NULL)
                return -EINVAL;
        p->code = code;
        p->handler = handler;
	index = ext_hash(code);
        p->next = ext_int_hash[index];
        ext_int_hash[index] = p;
        return 0;
}

int unregister_external_interrupt(__u16 code, ext_int_handler_t handler)
{
        ext_int_info_t *p, *q;
        int index;

	index = ext_hash(code);
        q = NULL;
        p = ext_int_hash[index];
        while (p != NULL) {
                if (p->code == code && p->handler == handler)
                        break;
                q = p;
                p = p->next;
        }
        if (p == NULL)
                return -ENOENT;
        if (q != NULL)
                q->next = p->next;
        else
                ext_int_hash[index] = p->next;
	kfree(p);
        return 0;
}

int unregister_early_external_interrupt(__u16 code, ext_int_handler_t handler,
					ext_int_info_t *p)
{
	ext_int_info_t *q;
	int index;

	if (p == NULL || p->code != code || p->handler != handler)
		return -EINVAL;
	index = ext_hash(code);
	q = ext_int_hash[index];
	if (p != q) {
		while (q != NULL) {
			if (q->next == p)
				break;
			q = q->next;
		}
		if (q == NULL)
			return -ENOENT;
		q->next = p->next;
	} else
		ext_int_hash[index] = p->next;
	return 0;
}

void do_extint(struct pt_regs *regs, unsigned short code)
{
        ext_int_info_t *p;
        int index;
	struct pt_regs *old_regs;

	old_regs = set_irq_regs(regs);
	s390_idle_check();
	irq_enter();
	if (S390_lowcore.int_clock >= S390_lowcore.clock_comparator)
		/* Serve timer interrupts first. */
		clock_comparator_work();
	kstat_cpu(smp_processor_id()).irqs[EXTERNAL_INTERRUPT]++;
        index = ext_hash(code);
	for (p = ext_int_hash[index]; p; p = p->next) {
		if (likely(p->code == code))
			p->handler(code);
	}
	irq_exit();
	set_irq_regs(old_regs);
}

EXPORT_SYMBOL(register_external_interrupt);
EXPORT_SYMBOL(unregister_external_interrupt);

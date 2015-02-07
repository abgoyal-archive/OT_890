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
 * arch/sh/kernel/machvec.c
 *
 * The SuperH machine vector setup handlers, yanked from setup.c
 *
 *  Copyright (C) 1999  Niibe Yutaka
 *  Copyright (C) 2002 - 2007 Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/string.h>
#include <asm/machvec.h>
#include <asm/sections.h>
#include <asm/setup.h>
#include <asm/io.h>
#include <asm/irq.h>

#define MV_NAME_SIZE 32

#define for_each_mv(mv) \
	for ((mv) = (struct sh_machine_vector *)&__machvec_start; \
	     (mv) && (unsigned long)(mv) < (unsigned long)&__machvec_end; \
	     (mv)++)

static struct sh_machine_vector * __init get_mv_byname(const char *name)
{
	struct sh_machine_vector *mv;

	for_each_mv(mv)
		if (strcasecmp(name, mv->mv_name) == 0)
			return mv;

	return NULL;
}

static unsigned int __initdata machvec_selected;

static int __init early_parse_mv(char *from)
{
	char mv_name[MV_NAME_SIZE] = "";
	char *mv_end;
	char *mv_comma;
	int mv_len;
	struct sh_machine_vector *mvp;

	mv_end = strchr(from, ' ');
	if (mv_end == NULL)
		mv_end = from + strlen(from);

	mv_comma = strchr(from, ',');
	mv_len = mv_end - from;
	if (mv_len > (MV_NAME_SIZE-1))
		mv_len = MV_NAME_SIZE-1;
	memcpy(mv_name, from, mv_len);
	mv_name[mv_len] = '\0';
	from = mv_end;

	machvec_selected = 1;

	/* Boot with the generic vector */
	if (strcmp(mv_name, "generic") == 0)
		return 0;

	mvp = get_mv_byname(mv_name);
	if (unlikely(!mvp)) {
		printk("Available vectors:\n\n\t'%s', ", sh_mv.mv_name);
		for_each_mv(mvp)
			printk("'%s', ", mvp->mv_name);
		printk("\n\n");
		panic("Failed to select machvec '%s' -- halting.\n",
		      mv_name);
	} else
		sh_mv = *mvp;

	return 0;
}
early_param("sh_mv", early_parse_mv);

void __init sh_mv_setup(void)
{
	/*
	 * Only overload the machvec if one hasn't been selected on
	 * the command line with sh_mv=
	 */
	if (!machvec_selected) {
		unsigned long machvec_size;

		machvec_size = ((unsigned long)&__machvec_end -
				(unsigned long)&__machvec_start);

		/*
		 * Sanity check for machvec section alignment. Ensure
		 * __initmv hasn't been misused.
		 */
		if (machvec_size % sizeof(struct sh_machine_vector))
			panic("machvec misaligned, invalid __initmv use?");

		/*
		 * If the machvec hasn't been preselected, use the first
		 * vector (usually the only one) from .machvec.init.
		 */
		if (machvec_size >= sizeof(struct sh_machine_vector))
			sh_mv = *(struct sh_machine_vector *)&__machvec_start;
	}

	printk(KERN_NOTICE "Booting machvec: %s\n", get_system_type());

	/*
	 * Manually walk the vec, fill in anything that the board hasn't yet
	 * by hand, wrapping to the generic implementation.
	 */
#define mv_set(elem) do { \
	if (!sh_mv.mv_##elem) \
		sh_mv.mv_##elem = generic_##elem; \
} while (0)

	mv_set(inb);	mv_set(inw);	mv_set(inl);
	mv_set(outb);	mv_set(outw);	mv_set(outl);

	mv_set(inb_p);	mv_set(inw_p);	mv_set(inl_p);
	mv_set(outb_p);	mv_set(outw_p);	mv_set(outl_p);

	mv_set(insb);	mv_set(insw);	mv_set(insl);
	mv_set(outsb);	mv_set(outsw);	mv_set(outsl);

	mv_set(ioport_map);
	mv_set(ioport_unmap);
	mv_set(irq_demux);

	if (!sh_mv.mv_nr_irqs)
		sh_mv.mv_nr_irqs = NR_IRQS;
}

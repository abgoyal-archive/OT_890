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

/* Low-level parallel port routines for the Atari builtin port
 *
 * Author: Andreas Schwab <schwab@issan.informatik.uni-dortmund.de>
 *
 * Based on parport_amiga.c.
 *
 * The built-in Atari parallel port provides one port at a fixed address
 * with 8 output data lines (D0 - D7), 1 output control line (STROBE)
 * and 1 input status line (BUSY) able to cause an interrupt.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/parport.h>
#include <linux/interrupt.h>
#include <asm/setup.h>
#include <asm/atarihw.h>
#include <asm/irq.h>
#include <asm/atariints.h>

static struct parport *this_port = NULL;

static unsigned char
parport_atari_read_data(struct parport *p)
{
	unsigned long flags;
	unsigned char data;

	local_irq_save(flags);
	sound_ym.rd_data_reg_sel = 15;
	data = sound_ym.rd_data_reg_sel;
	local_irq_restore(flags);
	return data;
}

static void
parport_atari_write_data(struct parport *p, unsigned char data)
{
	unsigned long flags;

	local_irq_save(flags);
	sound_ym.rd_data_reg_sel = 15;
	sound_ym.wd_data = data;
	local_irq_restore(flags);
}

static unsigned char
parport_atari_read_control(struct parport *p)
{
	unsigned long flags;
	unsigned char control = 0;

	local_irq_save(flags);
	sound_ym.rd_data_reg_sel = 14;
	if (!(sound_ym.rd_data_reg_sel & (1 << 5)))
		control = PARPORT_CONTROL_STROBE;
	local_irq_restore(flags);
	return control;
}

static void
parport_atari_write_control(struct parport *p, unsigned char control)
{
	unsigned long flags;

	local_irq_save(flags);
	sound_ym.rd_data_reg_sel = 14;
	if (control & PARPORT_CONTROL_STROBE)
		sound_ym.wd_data = sound_ym.rd_data_reg_sel & ~(1 << 5);
	else
		sound_ym.wd_data = sound_ym.rd_data_reg_sel | (1 << 5);
	local_irq_restore(flags);
}

static unsigned char
parport_atari_frob_control(struct parport *p, unsigned char mask,
			   unsigned char val)
{
	unsigned char old = parport_atari_read_control(p);
	parport_atari_write_control(p, (old & ~mask) ^ val);
	return old;
}

static unsigned char
parport_atari_read_status(struct parport *p)
{
	return ((st_mfp.par_dt_reg & 1 ? 0 : PARPORT_STATUS_BUSY) |
		PARPORT_STATUS_SELECT | PARPORT_STATUS_ERROR);
}

static void
parport_atari_init_state(struct pardevice *d, struct parport_state *s)
{
}

static void
parport_atari_save_state(struct parport *p, struct parport_state *s)
{
}

static void
parport_atari_restore_state(struct parport *p, struct parport_state *s)
{
}

static void
parport_atari_enable_irq(struct parport *p)
{
	enable_irq(IRQ_MFP_BUSY);
}

static void
parport_atari_disable_irq(struct parport *p)
{
	disable_irq(IRQ_MFP_BUSY);
}

static void
parport_atari_data_forward(struct parport *p)
{
	unsigned long flags;

	local_irq_save(flags);
	/* Soundchip port B as output. */
	sound_ym.rd_data_reg_sel = 7;
	sound_ym.wd_data = sound_ym.rd_data_reg_sel | 0x40;
	local_irq_restore(flags);
}

static void
parport_atari_data_reverse(struct parport *p)
{
#if 0 /* too dangerous, can kill sound chip */
	unsigned long flags;

	local_irq_save(flags);
	/* Soundchip port B as input. */
	sound_ym.rd_data_reg_sel = 7;
	sound_ym.wd_data = sound_ym.rd_data_reg_sel & ~0x40;
	local_irq_restore(flags);
#endif
}

static struct parport_operations parport_atari_ops = {
	.write_data	= parport_atari_write_data,
	.read_data	= parport_atari_read_data,

	.write_control	= parport_atari_write_control,
	.read_control	= parport_atari_read_control,
	.frob_control	= parport_atari_frob_control,

	.read_status	= parport_atari_read_status,

	.enable_irq	= parport_atari_enable_irq,
	.disable_irq	= parport_atari_disable_irq,

	.data_forward	= parport_atari_data_forward,
	.data_reverse	= parport_atari_data_reverse,

	.init_state	= parport_atari_init_state,
	.save_state	= parport_atari_save_state,
	.restore_state	= parport_atari_restore_state,

	.epp_write_data	= parport_ieee1284_epp_write_data,
	.epp_read_data	= parport_ieee1284_epp_read_data,
	.epp_write_addr	= parport_ieee1284_epp_write_addr,
	.epp_read_addr	= parport_ieee1284_epp_read_addr,

	.ecp_write_data	= parport_ieee1284_ecp_write_data,
	.ecp_read_data	= parport_ieee1284_ecp_read_data,
	.ecp_write_addr	= parport_ieee1284_ecp_write_addr,

	.compat_write_data	= parport_ieee1284_write_compat,
	.nibble_read_data	= parport_ieee1284_read_nibble,
	.byte_read_data		= parport_ieee1284_read_byte,

	.owner		= THIS_MODULE,
};


static int __init parport_atari_init(void)
{
	struct parport *p;
	unsigned long flags;

	if (MACH_IS_ATARI) {
		local_irq_save(flags);
		/* Soundchip port A/B as output. */
		sound_ym.rd_data_reg_sel = 7;
		sound_ym.wd_data = (sound_ym.rd_data_reg_sel & 0x3f) | 0xc0;
		/* STROBE high. */
		sound_ym.rd_data_reg_sel = 14;
		sound_ym.wd_data = sound_ym.rd_data_reg_sel | (1 << 5);
		local_irq_restore(flags);
		/* MFP port I0 as input. */
		st_mfp.data_dir &= ~1;
		/* MFP port I0 interrupt on high->low edge. */
		st_mfp.active_edge &= ~1;
		p = parport_register_port((unsigned long)&sound_ym.wd_data,
					  IRQ_MFP_BUSY, PARPORT_DMA_NONE,
					  &parport_atari_ops);
		if (!p)
			return -ENODEV;
		if (request_irq(IRQ_MFP_BUSY, parport_irq_handler,
				IRQ_TYPE_SLOW, p->name, p)) {
			parport_put_port (p);
			return -ENODEV;
		}

		this_port = p;
		printk(KERN_INFO "%s: Atari built-in port using irq\n", p->name);
		parport_announce_port (p);

		return 0;
	}
	return -ENODEV;
}

static void __exit parport_atari_exit(void)
{
	parport_remove_port(this_port);
	if (this_port->irq != PARPORT_IRQ_NONE)
		free_irq(IRQ_MFP_BUSY, this_port);
	parport_put_port(this_port);
}

MODULE_AUTHOR("Andreas Schwab");
MODULE_DESCRIPTION("Parport Driver for Atari builtin Port");
MODULE_SUPPORTED_DEVICE("Atari builtin Parallel Port");
MODULE_LICENSE("GPL");

module_init(parport_atari_init)
module_exit(parport_atari_exit)

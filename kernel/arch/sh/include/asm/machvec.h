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
 * include/asm-sh/machvec.h
 *
 * Copyright 2000 Stuart Menefy (stuart.menefy@st.com)
 *
 * May be copied or modified under the terms of the GNU General Public
 * License.  See linux/COPYING for more information.
 */

#ifndef _ASM_SH_MACHVEC_H
#define _ASM_SH_MACHVEC_H

#include <linux/types.h>
#include <linux/time.h>
#include <asm/machtypes.h>

struct sh_machine_vector {
	void (*mv_setup)(char **cmdline_p);
	const char *mv_name;
	int mv_nr_irqs;

	u8 (*mv_inb)(unsigned long);
	u16 (*mv_inw)(unsigned long);
	u32 (*mv_inl)(unsigned long);
	void (*mv_outb)(u8, unsigned long);
	void (*mv_outw)(u16, unsigned long);
	void (*mv_outl)(u32, unsigned long);

	u8 (*mv_inb_p)(unsigned long);
	u16 (*mv_inw_p)(unsigned long);
	u32 (*mv_inl_p)(unsigned long);
	void (*mv_outb_p)(u8, unsigned long);
	void (*mv_outw_p)(u16, unsigned long);
	void (*mv_outl_p)(u32, unsigned long);

	void (*mv_insb)(unsigned long, void *dst, unsigned long count);
	void (*mv_insw)(unsigned long, void *dst, unsigned long count);
	void (*mv_insl)(unsigned long, void *dst, unsigned long count);
	void (*mv_outsb)(unsigned long, const void *src, unsigned long count);
	void (*mv_outsw)(unsigned long, const void *src, unsigned long count);
	void (*mv_outsl)(unsigned long, const void *src, unsigned long count);

	int (*mv_irq_demux)(int irq);

	void (*mv_init_irq)(void);

	void __iomem *(*mv_ioport_map)(unsigned long port, unsigned int size);
	void (*mv_ioport_unmap)(void __iomem *);
};

extern struct sh_machine_vector sh_mv;

#define get_system_type()	sh_mv.mv_name

#define __initmv \
	__used __section(.machvec.init)

#endif /* _ASM_SH_MACHVEC_H */

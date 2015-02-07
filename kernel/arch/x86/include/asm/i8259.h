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

#ifndef _ASM_X86_I8259_H
#define _ASM_X86_I8259_H

#include <linux/delay.h>

extern unsigned int cached_irq_mask;

#define __byte(x, y)		(((unsigned char *)&(y))[x])
#define cached_master_mask	(__byte(0, cached_irq_mask))
#define cached_slave_mask	(__byte(1, cached_irq_mask))

/* i8259A PIC registers */
#define PIC_MASTER_CMD		0x20
#define PIC_MASTER_IMR		0x21
#define PIC_MASTER_ISR		PIC_MASTER_CMD
#define PIC_MASTER_POLL		PIC_MASTER_ISR
#define PIC_MASTER_OCW3		PIC_MASTER_ISR
#define PIC_SLAVE_CMD		0xa0
#define PIC_SLAVE_IMR		0xa1

/* i8259A PIC related value */
#define PIC_CASCADE_IR		2
#define MASTER_ICW4_DEFAULT	0x01
#define SLAVE_ICW4_DEFAULT	0x01
#define PIC_ICW4_AEOI		2

extern spinlock_t i8259A_lock;

extern void init_8259A(int auto_eoi);
extern void enable_8259A_irq(unsigned int irq);
extern void disable_8259A_irq(unsigned int irq);
extern unsigned int startup_8259A_irq(unsigned int irq);

/* the PIC may need a careful delay on some platforms, hence specific calls */
static inline unsigned char inb_pic(unsigned int port)
{
	unsigned char value = inb(port);

	/*
	 * delay for some accesses to PIC on motherboard or in chipset
	 * must be at least one microsecond, so be safe here:
	 */
	udelay(2);

	return value;
}

static inline void outb_pic(unsigned char value, unsigned int port)
{
	outb(value, port);
	/*
	 * delay for some accesses to PIC on motherboard or in chipset
	 * must be at least one microsecond, so be safe here:
	 */
	udelay(2);
}

extern struct irq_chip i8259A_chip;

extern void mask_8259A(void);
extern void unmask_8259A(void);

#endif /* _ASM_X86_I8259_H */

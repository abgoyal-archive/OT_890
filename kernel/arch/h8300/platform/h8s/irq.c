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
 * linux/arch/h8300/platform/h8s/ints_h8s.c
 * Interrupt handling CPU variants
 *
 * Yoshinori Sato <ysato@users.sourceforge.jp>
 *
 */

#include <linux/init.h>
#include <linux/errno.h>
#include <linux/kernel.h>

#include <asm/ptrace.h>
#include <asm/traps.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <asm/regs267x.h>

/* saved vector list */
const int __initdata h8300_saved_vectors[]={
#if defined(CONFIG_GDB_DEBUG)
	TRACE_VEC,
	TRAP3_VEC,
#endif
	-1
};

/* trap entry table */
const H8300_VECTOR __initdata h8300_trap_table[] = {
	0,0,0,0,0,
	trace_break,  /* TRACE */
	0,0,
	system_call,  /* TRAPA #0 */
	0,0,0,0,0,0,0
};

/* IRQ pin assignment */
struct irq_pins {
	unsigned char port_no;
	unsigned char bit_no;
} __attribute__((aligned(1),packed));
/* ISTR = 0 */
static const struct irq_pins irq_assign_table0[16]={
        {H8300_GPIO_P5,H8300_GPIO_B0},{H8300_GPIO_P5,H8300_GPIO_B1},
	{H8300_GPIO_P5,H8300_GPIO_B2},{H8300_GPIO_P5,H8300_GPIO_B3},
	{H8300_GPIO_P5,H8300_GPIO_B4},{H8300_GPIO_P5,H8300_GPIO_B5},
	{H8300_GPIO_P5,H8300_GPIO_B6},{H8300_GPIO_P5,H8300_GPIO_B7},
	{H8300_GPIO_P6,H8300_GPIO_B0},{H8300_GPIO_P6,H8300_GPIO_B1},
	{H8300_GPIO_P6,H8300_GPIO_B2},{H8300_GPIO_P6,H8300_GPIO_B3},
	{H8300_GPIO_P6,H8300_GPIO_B4},{H8300_GPIO_P6,H8300_GPIO_B5},
	{H8300_GPIO_PF,H8300_GPIO_B1},{H8300_GPIO_PF,H8300_GPIO_B2},
};
/* ISTR = 1 */
static const struct irq_pins irq_assign_table1[16]={
	{H8300_GPIO_P8,H8300_GPIO_B0},{H8300_GPIO_P8,H8300_GPIO_B1},
	{H8300_GPIO_P8,H8300_GPIO_B2},{H8300_GPIO_P8,H8300_GPIO_B3},
	{H8300_GPIO_P8,H8300_GPIO_B4},{H8300_GPIO_P8,H8300_GPIO_B5},
	{H8300_GPIO_PH,H8300_GPIO_B2},{H8300_GPIO_PH,H8300_GPIO_B3},
	{H8300_GPIO_P2,H8300_GPIO_B0},{H8300_GPIO_P2,H8300_GPIO_B1},
	{H8300_GPIO_P2,H8300_GPIO_B2},{H8300_GPIO_P2,H8300_GPIO_B3},
	{H8300_GPIO_P2,H8300_GPIO_B4},{H8300_GPIO_P2,H8300_GPIO_B5},
	{H8300_GPIO_P2,H8300_GPIO_B6},{H8300_GPIO_P2,H8300_GPIO_B7},
};

/* IRQ to GPIO pin translation */
#define IRQ_GPIO_MAP(irqbit,irq,port,bit)			  \
do {								  \
	if (*(volatile unsigned short *)ITSR & irqbit) {	  \
		port = irq_assign_table1[irq - EXT_IRQ0].port_no; \
		bit  = irq_assign_table1[irq - EXT_IRQ0].bit_no;  \
	} else {						  \
		port = irq_assign_table0[irq - EXT_IRQ0].port_no; \
		bit  = irq_assign_table0[irq - EXT_IRQ0].bit_no;  \
	}							  \
} while(0)

int h8300_enable_irq_pin(unsigned int irq)
{
	if (irq >= EXT_IRQ0 && irq <= EXT_IRQ15) {
		unsigned short ptn = 1 << (irq - EXT_IRQ0);
		unsigned int port_no,bit_no;
		IRQ_GPIO_MAP(ptn, irq, port_no, bit_no);
		if (H8300_GPIO_RESERVE(port_no, bit_no) == 0)
			return -EBUSY;                   /* pin already use */
		H8300_GPIO_DDR(port_no, bit_no, H8300_GPIO_INPUT);
		*(volatile unsigned short *)ISR &= ~ptn; /* ISR clear */
	}

	return 0;
}

void h8300_disable_irq_pin(unsigned int irq)
{
	if (irq >= EXT_IRQ0 && irq <= EXT_IRQ15) {
		/* disable interrupt & release IRQ pin */
		unsigned short ptn = 1 << (irq - EXT_IRQ0);
		unsigned short port_no,bit_no;
		*(volatile unsigned short *)ISR &= ~ptn;
		*(volatile unsigned short *)IER &= ~ptn;
		IRQ_GPIO_MAP(ptn, irq, port_no, bit_no);
		H8300_GPIO_FREE(port_no, bit_no);
	}
}

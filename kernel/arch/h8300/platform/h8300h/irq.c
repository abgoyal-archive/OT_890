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
 * Interrupt handling H8/300H depend.
 * Yoshinori Sato <ysato@users.sourceforge.jp>
 *
 */

#include <linux/init.h>
#include <linux/errno.h>

#include <asm/ptrace.h>
#include <asm/traps.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <asm/regs306x.h>

const int __initdata h8300_saved_vectors[] = {
#if defined(CONFIG_GDB_DEBUG)
	TRAP3_VEC,	/* TRAPA #3 is GDB breakpoint */
#endif
	-1,
};

const h8300_vector __initdata h8300_trap_table[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	system_call,
	0,
	0,
	trace_break,
};

int h8300_enable_irq_pin(unsigned int irq)
{
	int bitmask;
	if (irq < EXT_IRQ0 || irq > EXT_IRQ5)
		return 0;

	/* initialize IRQ pin */
	bitmask = 1 << (irq - EXT_IRQ0);
	switch(irq) {
	case EXT_IRQ0:
	case EXT_IRQ1:
	case EXT_IRQ2:
	case EXT_IRQ3:
		if (H8300_GPIO_RESERVE(H8300_GPIO_P8, bitmask) == 0)
			return -EBUSY;
		H8300_GPIO_DDR(H8300_GPIO_P8, bitmask, H8300_GPIO_INPUT);
		break;
	case EXT_IRQ4:
	case EXT_IRQ5:
		if (H8300_GPIO_RESERVE(H8300_GPIO_P9, bitmask) == 0)
			return -EBUSY;
		H8300_GPIO_DDR(H8300_GPIO_P9, bitmask, H8300_GPIO_INPUT);
		break;
	}

	return 0;
}

void h8300_disable_irq_pin(unsigned int irq)
{
	int bitmask;
	if (irq < EXT_IRQ0 || irq > EXT_IRQ5)
		return;

	/* disable interrupt & release IRQ pin */
	bitmask = 1 << (irq - EXT_IRQ0);
	switch(irq) {
	case EXT_IRQ0:
	case EXT_IRQ1:
	case EXT_IRQ2:
	case EXT_IRQ3:
		*(volatile unsigned char *)IER &= ~bitmask;
		H8300_GPIO_FREE(H8300_GPIO_P8, bitmask);
		break ;
	case EXT_IRQ4:
	case EXT_IRQ5:
		*(volatile unsigned char *)IER &= ~bitmask;
		H8300_GPIO_FREE(H8300_GPIO_P9, bitmask);
		break;
	}
}

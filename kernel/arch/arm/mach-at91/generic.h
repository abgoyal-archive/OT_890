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
 * linux/arch/arm/mach-at91/generic.h
 *
 *  Copyright (C) 2005 David Brownell
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

 /* Processors */
extern void __init at91rm9200_initialize(unsigned long main_clock, unsigned short banks);
extern void __init at91sam9260_initialize(unsigned long main_clock);
extern void __init at91sam9261_initialize(unsigned long main_clock);
extern void __init at91sam9263_initialize(unsigned long main_clock);
extern void __init at91sam9rl_initialize(unsigned long main_clock);
extern void __init at91x40_initialize(unsigned long main_clock);
extern void __init at91cap9_initialize(unsigned long main_clock);

 /* Interrupts */
extern void __init at91rm9200_init_interrupts(unsigned int priority[]);
extern void __init at91sam9260_init_interrupts(unsigned int priority[]);
extern void __init at91sam9261_init_interrupts(unsigned int priority[]);
extern void __init at91sam9263_init_interrupts(unsigned int priority[]);
extern void __init at91sam9rl_init_interrupts(unsigned int priority[]);
extern void __init at91x40_init_interrupts(unsigned int priority[]);
extern void __init at91cap9_init_interrupts(unsigned int priority[]);
extern void __init at91_aic_init(unsigned int priority[]);

 /* Timer */
struct sys_timer;
extern struct sys_timer at91rm9200_timer;
extern struct sys_timer at91sam926x_timer;
extern struct sys_timer at91x40_timer;

 /* Clocks */
extern int __init at91_clock_init(unsigned long main_clock);
struct device;
extern void __init at91_clock_associate(const char *id, struct device *dev, const char *func);

 /* Power Management */
extern void at91_irq_suspend(void);
extern void at91_irq_resume(void);

 /* GPIO */
#define AT91RM9200_PQFP		3	/* AT91RM9200 PQFP package has 3 banks */
#define AT91RM9200_BGA		4	/* AT91RM9200 BGA package has 4 banks */

struct at91_gpio_bank {
	unsigned chipbase;		/* bank's first GPIO number */
	void __iomem *regbase;		/* base of register bank */
	struct at91_gpio_bank *next;	/* bank sharing same IRQ/clock/... */
	unsigned short id;		/* peripheral ID */
	unsigned long offset;		/* offset from system peripheral base */
	struct clk *clock;		/* associated clock */
};
extern void __init at91_gpio_init(struct at91_gpio_bank *, int nr_banks);
extern void __init at91_gpio_irq_setup(void);

extern void (*at91_arch_reset)(void);
extern int at91_extern_irq;

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
 *  linux/arch/arm/mach-pxa/generic.h
 *
 * Author:	Nicolas Pitre
 * Copyright:	MontaVista Software Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

typedef int (*set_wake_t)(unsigned int, unsigned int);

struct sys_timer;

extern struct sys_timer pxa_timer;
extern void __init pxa_init_irq(int irq_nr, set_wake_t fn);
extern void __init pxa_init_gpio(int gpio_nr, set_wake_t fn);
extern void __init pxa25x_init_irq(void);
extern void __init pxa27x_init_irq(void);
extern void __init pxa3xx_init_irq(void);
extern void __init pxa_map_io(void);

extern unsigned int get_clk_frequency_khz(int info);
extern int pxa_last_gpio;

#define SET_BANK(__nr,__start,__size) \
	mi->bank[__nr].start = (__start), \
	mi->bank[__nr].size = (__size), \
	mi->bank[__nr].node = (((unsigned)(__start) - PHYS_OFFSET) >> 27)

#define ARRAY_AND_SIZE(x)	(x), ARRAY_SIZE(x)

#ifdef CONFIG_PXA25x
extern unsigned pxa25x_get_clk_frequency_khz(int);
extern unsigned pxa25x_get_memclk_frequency_10khz(void);
#else
#define pxa25x_get_clk_frequency_khz(x)		(0)
#define pxa25x_get_memclk_frequency_10khz()	(0)
#endif

#ifdef CONFIG_PXA27x
extern unsigned pxa27x_get_clk_frequency_khz(int);
extern unsigned pxa27x_get_memclk_frequency_10khz(void);
#else
#define pxa27x_get_clk_frequency_khz(x)		(0)
#define pxa27x_get_memclk_frequency_10khz()	(0)
#endif

#if defined(CONFIG_PXA25x) || defined(CONFIG_PXA27x)
extern void pxa2xx_clear_reset_status(unsigned int);
#else
static inline void pxa2xx_clear_reset_status(unsigned int mask) {}
#endif

#ifdef CONFIG_PXA3xx
extern unsigned pxa3xx_get_clk_frequency_khz(int);
extern unsigned pxa3xx_get_memclk_frequency_10khz(void);
extern void pxa3xx_clear_reset_status(unsigned int);
#else
#define pxa3xx_get_clk_frequency_khz(x)		(0)
#define pxa3xx_get_memclk_frequency_10khz()	(0)
static inline void pxa3xx_clear_reset_status(unsigned int mask) {}
#endif

extern struct sysdev_class pxa_irq_sysclass;
extern struct sysdev_class pxa_gpio_sysclass;
extern struct sysdev_class pxa2xx_mfp_sysclass;
extern struct sysdev_class pxa3xx_mfp_sysclass;

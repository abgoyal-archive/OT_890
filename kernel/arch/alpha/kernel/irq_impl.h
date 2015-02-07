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
 *	linux/arch/alpha/kernel/irq_impl.h
 *
 *	Copyright (C) 1995 Linus Torvalds
 *	Copyright (C) 1998, 2000 Richard Henderson
 *
 * This file contains declarations and inline functions for interfacing
 * with the IRQ handling routines in irq.c.
 */

#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/profile.h>


#define RTC_IRQ    8

extern void isa_device_interrupt(unsigned long);
extern void isa_no_iack_sc_device_interrupt(unsigned long);
extern void srm_device_interrupt(unsigned long);
extern void pyxis_device_interrupt(unsigned long);

extern struct irqaction timer_irqaction;
extern struct irqaction isa_cascade_irqaction;
extern struct irqaction timer_cascade_irqaction;
extern struct irqaction halt_switch_irqaction;

extern void init_srm_irqs(long, unsigned long);
extern void init_pyxis_irqs(unsigned long);
extern void init_rtc_irq(void);

extern void common_init_isa_dma(void);

extern void i8259a_enable_irq(unsigned int);
extern void i8259a_disable_irq(unsigned int);
extern void i8259a_mask_and_ack_irq(unsigned int);
extern unsigned int i8259a_startup_irq(unsigned int);
extern void i8259a_end_irq(unsigned int);
extern struct hw_interrupt_type i8259a_irq_type;
extern void init_i8259a_irqs(void);

extern void handle_irq(int irq);

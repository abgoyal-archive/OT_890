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

#include <asm/btfixup.h>

/* Dave Redman (djhr@tadpole.co.uk)
 * changed these to function pointers.. it saves cycles and will allow
 * the irq dependencies to be split into different files at a later date
 * sun4c_irq.c, sun4m_irq.c etc so we could reduce the kernel size.
 * Jakub Jelinek (jj@sunsite.mff.cuni.cz)
 * Changed these to btfixup entities... It saves cycles :)
 */

BTFIXUPDEF_CALL(void, disable_irq, unsigned int)
BTFIXUPDEF_CALL(void, enable_irq, unsigned int)
BTFIXUPDEF_CALL(void, disable_pil_irq, unsigned int)
BTFIXUPDEF_CALL(void, enable_pil_irq, unsigned int)
BTFIXUPDEF_CALL(void, clear_clock_irq, void)
BTFIXUPDEF_CALL(void, load_profile_irq, int, unsigned int)

static inline void __disable_irq(unsigned int irq)
{
	BTFIXUP_CALL(disable_irq)(irq);
}

static inline void __enable_irq(unsigned int irq)
{
	BTFIXUP_CALL(enable_irq)(irq);
}

static inline void disable_pil_irq(unsigned int irq)
{
	BTFIXUP_CALL(disable_pil_irq)(irq);
}

static inline void enable_pil_irq(unsigned int irq)
{
	BTFIXUP_CALL(enable_pil_irq)(irq);
}

static inline void clear_clock_irq(void)
{
	BTFIXUP_CALL(clear_clock_irq)();
}

static inline void load_profile_irq(int cpu, int limit)
{
	BTFIXUP_CALL(load_profile_irq)(cpu, limit);
}

extern void (*sparc_init_timers)(irq_handler_t lvl10_irq);

extern void claim_ticker14(irq_handler_t irq_handler,
			   int irq,
			   unsigned int timeout);

#ifdef CONFIG_SMP
BTFIXUPDEF_CALL(void, set_cpu_int, int, int)
BTFIXUPDEF_CALL(void, clear_cpu_int, int, int)
BTFIXUPDEF_CALL(void, set_irq_udt, int)

#define set_cpu_int(cpu,level) BTFIXUP_CALL(set_cpu_int)(cpu,level)
#define clear_cpu_int(cpu,level) BTFIXUP_CALL(clear_cpu_int)(cpu,level)
#define set_irq_udt(cpu) BTFIXUP_CALL(set_irq_udt)(cpu)
#endif

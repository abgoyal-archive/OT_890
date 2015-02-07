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
 * Copyright 2003 PathScale, Inc.
 *
 * Licensed under the GPL
 */

#ifndef __UM_PROCESSOR_X86_64_H
#define __UM_PROCESSOR_X86_64_H

/* include faultinfo structure */
#include "sysdep/faultinfo.h"

struct arch_thread {
        unsigned long debugregs[8];
        int debugregs_seq;
        unsigned long fs;
        struct faultinfo faultinfo;
};

/* REP NOP (PAUSE) is a good thing to insert into busy-wait loops. */
static inline void rep_nop(void)
{
	__asm__ __volatile__("rep;nop": : :"memory");
}

#define cpu_relax()   rep_nop()

#define INIT_ARCH_THREAD { .debugregs  		= { [ 0 ... 7 ] = 0 }, \
			   .debugregs_seq	= 0, \
			   .fs			= 0, \
			   .faultinfo		= { 0, 0, 0 } }

static inline void arch_flush_thread(struct arch_thread *thread)
{
}

static inline void arch_copy_thread(struct arch_thread *from,
                                    struct arch_thread *to)
{
	to->fs = from->fs;
}

#include <asm/user.h>

#define current_text_addr() \
	({ void *pc; __asm__("movq $1f,%0\n1:":"=g" (pc)); pc; })

#define ARCH_IS_STACKGROW(address) \
        (address + 128 >= UPT_SP(&current->thread.regs.regs))

#define KSTK_EIP(tsk) KSTK_REG(tsk, RIP)
#define KSTK_ESP(tsk) KSTK_REG(tsk, RSP)

#include "asm/processor-generic.h"

#endif

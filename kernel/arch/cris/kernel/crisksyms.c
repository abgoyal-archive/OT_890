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

#include <linux/module.h>
#include <linux/user.h>
#include <linux/elfcore.h>
#include <linux/sched.h>
#include <linux/in6.h>
#include <linux/interrupt.h>
#include <linux/pm.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/tty.h>

#include <asm/processor.h>
#include <asm/uaccess.h>
#include <asm/checksum.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <asm/irq.h>
#include <asm/pgtable.h>
#include <asm/fasttimer.h>

extern unsigned long get_cmos_time(void);
extern void __Udiv(void);
extern void __Umod(void);
extern void __Div(void);
extern void __Mod(void);
extern void __ashldi3(void);
extern void __ashrdi3(void);
extern void __lshrdi3(void);
extern void __negdi2(void);
extern void iounmap(volatile void * __iomem);

/* Platform dependent support */
EXPORT_SYMBOL(kernel_thread);
EXPORT_SYMBOL(get_cmos_time);
EXPORT_SYMBOL(loops_per_usec);

/* Math functions */
EXPORT_SYMBOL(__Udiv);
EXPORT_SYMBOL(__Umod);
EXPORT_SYMBOL(__Div);
EXPORT_SYMBOL(__Mod);
EXPORT_SYMBOL(__ashldi3);
EXPORT_SYMBOL(__ashrdi3);
EXPORT_SYMBOL(__lshrdi3);
EXPORT_SYMBOL(__negdi2);

/* Memory functions */
EXPORT_SYMBOL(__ioremap);
EXPORT_SYMBOL(iounmap);

/* Userspace access functions */
EXPORT_SYMBOL(__copy_user_zeroing);
EXPORT_SYMBOL(__copy_user);

#undef memcpy
#undef memset
extern void * memset(void *, int, __kernel_size_t);
extern void * memcpy(void *, const void *, __kernel_size_t);
EXPORT_SYMBOL(memcpy);
EXPORT_SYMBOL(memset);

#ifdef CONFIG_ETRAX_FAST_TIMER
/* Fast timer functions */
EXPORT_SYMBOL(fast_timer_list);
EXPORT_SYMBOL(start_one_shot_timer);
EXPORT_SYMBOL(del_fast_timer);
EXPORT_SYMBOL(schedule_usleep);
#endif
EXPORT_SYMBOL(csum_partial);

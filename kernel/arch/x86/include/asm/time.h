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

#ifndef _ASM_X86_TIME_H
#define _ASM_X86_TIME_H

extern void hpet_time_init(void);

#include <asm/mc146818rtc.h>
#ifdef CONFIG_X86_32
#include <linux/efi.h>

static inline unsigned long native_get_wallclock(void)
{
	unsigned long retval;

	if (efi_enabled)
		retval = efi_get_time();
	else
		retval = mach_get_cmos_time();

	return retval;
}

static inline int native_set_wallclock(unsigned long nowtime)
{
	int retval;

	if (efi_enabled)
		retval = efi_set_rtc_mmss(nowtime);
	else
		retval = mach_set_rtc_mmss(nowtime);

	return retval;
}

#else
extern void native_time_init_hook(void);

static inline unsigned long native_get_wallclock(void)
{
	return mach_get_cmos_time();
}

static inline int native_set_wallclock(unsigned long nowtime)
{
	return mach_set_rtc_mmss(nowtime);
}

#endif

extern void time_init(void);

#ifdef CONFIG_PARAVIRT
#include <asm/paravirt.h>
#else /* !CONFIG_PARAVIRT */

#define get_wallclock() native_get_wallclock()
#define set_wallclock(x) native_set_wallclock(x)
#define choose_time_init() hpet_time_init

#endif /* CONFIG_PARAVIRT */

extern unsigned long __init calibrate_cpu(void);

#endif /* _ASM_X86_TIME_H */

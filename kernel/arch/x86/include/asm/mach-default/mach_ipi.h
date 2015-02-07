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

#ifndef _ASM_X86_MACH_DEFAULT_MACH_IPI_H
#define _ASM_X86_MACH_DEFAULT_MACH_IPI_H

/* Avoid include hell */
#define NMI_VECTOR 0x02

void send_IPI_mask_bitmask(const struct cpumask *mask, int vector);
void send_IPI_mask_allbutself(const struct cpumask *mask, int vector);
void __send_IPI_shortcut(unsigned int shortcut, int vector);

extern int no_broadcast;

#ifdef CONFIG_X86_64
#include <asm/genapic.h>
#define send_IPI_mask (genapic->send_IPI_mask)
#define send_IPI_mask_allbutself (genapic->send_IPI_mask_allbutself)
#else
static inline void send_IPI_mask(const struct cpumask *mask, int vector)
{
	send_IPI_mask_bitmask(mask, vector);
}
void send_IPI_mask_allbutself(const struct cpumask *mask, int vector);
#endif

static inline void __local_send_IPI_allbutself(int vector)
{
	if (no_broadcast || vector == NMI_VECTOR)
		send_IPI_mask_allbutself(cpu_online_mask, vector);
	else
		__send_IPI_shortcut(APIC_DEST_ALLBUT, vector);
}

static inline void __local_send_IPI_all(int vector)
{
	if (no_broadcast || vector == NMI_VECTOR)
		send_IPI_mask(cpu_online_mask, vector);
	else
		__send_IPI_shortcut(APIC_DEST_ALLINC, vector);
}

#ifdef CONFIG_X86_64
#define send_IPI_allbutself (genapic->send_IPI_allbutself)
#define send_IPI_all (genapic->send_IPI_all)
#else
static inline void send_IPI_allbutself(int vector)
{
	/*
	 * if there are no other CPUs in the system then we get an APIC send 
	 * error if we try to broadcast, thus avoid sending IPIs in this case.
	 */
	if (!(num_online_cpus() > 1))
		return;

	__local_send_IPI_allbutself(vector);
	return;
}

static inline void send_IPI_all(int vector)
{
	__local_send_IPI_all(vector);
}
#endif

#endif /* _ASM_X86_MACH_DEFAULT_MACH_IPI_H */

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

#ifndef __ASM_NUMAQ_WAKECPU_H
#define __ASM_NUMAQ_WAKECPU_H

/* This file copes with machines that wakeup secondary CPUs by NMIs */

#define TRAMPOLINE_PHYS_LOW (0x8)
#define TRAMPOLINE_PHYS_HIGH (0xa)

/* We don't do anything here because we use NMI's to boot instead */
static inline void wait_for_init_deassert(atomic_t *deassert)
{
}

/*
 * Because we use NMIs rather than the INIT-STARTUP sequence to
 * bootstrap the CPUs, the APIC may be in a weird state. Kick it.
 */
static inline void smp_callin_clear_local_apic(void)
{
	clear_local_APIC();
}

static inline void store_NMI_vector(unsigned short *high, unsigned short *low)
{
	printk("Storing NMI vector\n");
	*high =
	  *((volatile unsigned short *)phys_to_virt(TRAMPOLINE_PHYS_HIGH));
	*low =
	  *((volatile unsigned short *)phys_to_virt(TRAMPOLINE_PHYS_LOW));
}

static inline void restore_NMI_vector(unsigned short *high, unsigned short *low)
{
	printk("Restoring NMI vector\n");
	*((volatile unsigned short *)phys_to_virt(TRAMPOLINE_PHYS_HIGH)) =
								 *high;
	*((volatile unsigned short *)phys_to_virt(TRAMPOLINE_PHYS_LOW)) =
								 *low;
}

static inline void inquire_remote_apic(int apicid)
{
}

#endif /* __ASM_NUMAQ_WAKECPU_H */

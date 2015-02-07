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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1992 - 1997, 2000-2005 Silicon Graphics, Inc. All rights reserved.
 */
#ifndef _ASM_IA64_SN_PDA_H
#define _ASM_IA64_SN_PDA_H

#include <linux/cache.h>
#include <asm/percpu.h>
#include <asm/system.h>


/*
 * CPU-specific data structure.
 *
 * One of these structures is allocated for each cpu of a NUMA system.
 *
 * This structure provides a convenient way of keeping together 
 * all SN per-cpu data structures. 
 */

typedef struct pda_s {

	/*
	 * Support for SN LEDs
	 */
	volatile short	*led_address;
	u8		led_state;
	u8		hb_state;	/* supports blinking heartbeat leds */
	unsigned int	hb_count;

	unsigned int	idle_flag;
	
	volatile unsigned long *bedrock_rev_id;
	volatile unsigned long *pio_write_status_addr;
	unsigned long pio_write_status_val;
	volatile unsigned long *pio_shub_war_cam_addr;

	unsigned long	sn_in_service_ivecs[4];
	int		sn_lb_int_war_ticks;
	int		sn_last_irq;
	int		sn_first_irq;
} pda_t;


#define CACHE_ALIGN(x)	(((x) + SMP_CACHE_BYTES-1) & ~(SMP_CACHE_BYTES-1))

/*
 * PDA
 * Per-cpu private data area for each cpu. The PDA is located immediately after
 * the IA64 cpu_data area. A full page is allocated for the cp_data area for each
 * cpu but only a small amout of the page is actually used. We put the SNIA PDA
 * in the same page as the cpu_data area. Note that there is a check in the setup
 * code to verify that we don't overflow the page.
 *
 * Seems like we should should cache-line align the pda so that any changes in the
 * size of the cpu_data area don't change cache layout. Should we align to 32, 64, 128
 * or 512 boundary. Each has merits. For now, pick 128 but should be revisited later.
 */
DECLARE_PER_CPU(struct pda_s, pda_percpu);

#define pda		(&__ia64_per_cpu_var(pda_percpu))

#define pdacpu(cpu)	(&per_cpu(pda_percpu, cpu))

#endif /* _ASM_IA64_SN_PDA_H */

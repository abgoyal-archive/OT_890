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

#ifndef _ASM_X86_GENAPIC_64_H
#define _ASM_X86_GENAPIC_64_H

#include <linux/cpumask.h>

/*
 * Copyright 2004 James Cleverdon, IBM.
 * Subject to the GNU Public License, v.2
 *
 * Generic APIC sub-arch data struct.
 *
 * Hacked for x86-64 by James Cleverdon from i386 architecture code by
 * Martin Bligh, Andi Kleen, James Bottomley, John Stultz, and
 * James Cleverdon.
 */

struct genapic {
	char *name;
	int (*acpi_madt_oem_check)(char *oem_id, char *oem_table_id);
	u32 int_delivery_mode;
	u32 int_dest_mode;
	int (*apic_id_registered)(void);
	const struct cpumask *(*target_cpus)(void);
	void (*vector_allocation_domain)(int cpu, struct cpumask *retmask);
	void (*init_apic_ldr)(void);
	/* ipi */
	void (*send_IPI_mask)(const struct cpumask *mask, int vector);
	void (*send_IPI_mask_allbutself)(const struct cpumask *mask,
					 int vector);
	void (*send_IPI_allbutself)(int vector);
	void (*send_IPI_all)(int vector);
	void (*send_IPI_self)(int vector);
	/* */
	unsigned int (*cpu_mask_to_apicid)(const struct cpumask *cpumask);
	unsigned int (*cpu_mask_to_apicid_and)(const struct cpumask *cpumask,
					       const struct cpumask *andmask);
	unsigned int (*phys_pkg_id)(int index_msb);
	unsigned int (*get_apic_id)(unsigned long x);
	unsigned long (*set_apic_id)(unsigned int id);
	unsigned long apic_id_mask;
	/* wakeup_secondary_cpu */
	int (*wakeup_cpu)(int apicid, unsigned long start_eip);
};

extern struct genapic *genapic;

extern struct genapic apic_flat;
extern struct genapic apic_physflat;
extern struct genapic apic_x2apic_cluster;
extern struct genapic apic_x2apic_phys;
extern int acpi_madt_oem_check(char *, char *);

extern void apic_send_IPI_self(int vector);
enum uv_system_type {UV_NONE, UV_LEGACY_APIC, UV_X2APIC, UV_NON_UNIQUE_APIC};
extern enum uv_system_type get_uv_system_type(void);
extern int is_uv_system(void);

extern struct genapic apic_x2apic_uv_x;
DECLARE_PER_CPU(int, x2apic_extra_bits);
extern void uv_cpu_init(void);
extern void uv_system_init(void);
extern int uv_wakeup_secondary(int phys_apicid, unsigned int start_rip);

extern void setup_apic_routing(void);

#endif /* _ASM_X86_GENAPIC_64_H */

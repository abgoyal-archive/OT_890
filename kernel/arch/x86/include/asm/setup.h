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

#ifndef _ASM_X86_SETUP_H
#define _ASM_X86_SETUP_H

#define COMMAND_LINE_SIZE 2048

#ifndef __ASSEMBLY__

/* Interrupt control for vSMPowered x86_64 systems */
void vsmp_init(void);


void setup_bios_corruption_check(void);


#ifdef CONFIG_X86_VISWS
extern void visws_early_detect(void);
extern int is_visws_box(void);
#else
static inline void visws_early_detect(void) { }
static inline int is_visws_box(void) { return 0; }
#endif

extern int wakeup_secondary_cpu_via_nmi(int apicid, unsigned long start_eip);
extern int wakeup_secondary_cpu_via_init(int apicid, unsigned long start_eip);
/*
 * Any setup quirks to be performed?
 */
struct mpc_cpu;
struct mpc_bus;
struct mpc_oemtable;
struct x86_quirks {
	int (*arch_pre_time_init)(void);
	int (*arch_time_init)(void);
	int (*arch_pre_intr_init)(void);
	int (*arch_intr_init)(void);
	int (*arch_trap_init)(void);
	char * (*arch_memory_setup)(void);
	int (*mach_get_smp_config)(unsigned int early);
	int (*mach_find_smp_config)(unsigned int reserve);

	int *mpc_record;
	int (*mpc_apic_id)(struct mpc_cpu *m);
	void (*mpc_oem_bus_info)(struct mpc_bus *m, char *name);
	void (*mpc_oem_pci_bus)(struct mpc_bus *m);
	void (*smp_read_mpc_oem)(struct mpc_oemtable *oemtable,
                                    unsigned short oemsize);
	int (*setup_ioapic_ids)(void);
	int (*update_genapic)(void);
};

extern struct x86_quirks *x86_quirks;
extern unsigned long saved_video_mode;

#ifndef CONFIG_PARAVIRT
#define paravirt_post_allocator_init()	do {} while (0)
#endif
#endif /* __ASSEMBLY__ */

#ifdef __KERNEL__

#ifdef __i386__

#include <linux/pfn.h>
/*
 * Reserved space for vmalloc and iomap - defined in asm/page.h
 */
#define MAXMEM_PFN	PFN_DOWN(MAXMEM)
#define MAX_NONPAE_PFN	(1 << 20)

#endif /* __i386__ */

#define PARAM_SIZE 4096		/* sizeof(struct boot_params) */

#define OLD_CL_MAGIC		0xA33F
#define OLD_CL_ADDRESS		0x020	/* Relative to real mode data */
#define NEW_CL_POINTER		0x228	/* Relative to real mode data */

#ifndef __ASSEMBLY__
#include <asm/bootparam.h>

#ifndef _SETUP

/*
 * This is set up by the setup-routine at boot-time
 */
extern struct boot_params boot_params;

/*
 * Do NOT EVER look at the BIOS memory size location.
 * It does not work on many machines.
 */
#define LOWMEMSIZE()	(0x9f000)

#ifdef __i386__

void __init i386_start_kernel(void);
extern void probe_roms(void);

extern unsigned long init_pg_tables_start;
extern unsigned long init_pg_tables_end;

#else
void __init x86_64_init_pda(void);
void __init x86_64_start_kernel(char *real_mode);
void __init x86_64_start_reservations(char *real_mode_data);

#endif /* __i386__ */
#endif /* _SETUP */
#endif /* __ASSEMBLY__ */
#endif  /*  __KERNEL__  */

#endif /* _ASM_X86_SETUP_H */

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

#ifndef __ASMSPARC_ELF_H
#define __ASMSPARC_ELF_H

/*
 * ELF register definitions..
 */

#include <asm/ptrace.h>

/*
 * Sparc section types
 */
#define STT_REGISTER		13

/*
 * Sparc ELF relocation types
 */
#define	R_SPARC_NONE		0
#define	R_SPARC_8		1
#define	R_SPARC_16		2
#define	R_SPARC_32		3
#define	R_SPARC_DISP8		4
#define	R_SPARC_DISP16		5
#define	R_SPARC_DISP32		6
#define	R_SPARC_WDISP30		7
#define	R_SPARC_WDISP22		8
#define	R_SPARC_HI22		9
#define	R_SPARC_22		10
#define	R_SPARC_13		11
#define	R_SPARC_LO10		12
#define	R_SPARC_GOT10		13
#define	R_SPARC_GOT13		14
#define	R_SPARC_GOT22		15
#define	R_SPARC_PC10		16
#define	R_SPARC_PC22		17
#define	R_SPARC_WPLT30		18
#define	R_SPARC_COPY		19
#define	R_SPARC_GLOB_DAT	20
#define	R_SPARC_JMP_SLOT	21
#define	R_SPARC_RELATIVE	22
#define	R_SPARC_UA32		23
#define R_SPARC_PLT32		24
#define R_SPARC_HIPLT22		25
#define R_SPARC_LOPLT10		26
#define R_SPARC_PCPLT32		27
#define R_SPARC_PCPLT22		28
#define R_SPARC_PCPLT10		29
#define R_SPARC_10		30
#define R_SPARC_11		31
#define R_SPARC_64		32
#define R_SPARC_OLO10		33
#define R_SPARC_WDISP16		40
#define R_SPARC_WDISP19		41
#define R_SPARC_7		43
#define R_SPARC_5		44
#define R_SPARC_6		45

/* Bits present in AT_HWCAP, primarily for Sparc32.  */

#define HWCAP_SPARC_FLUSH       1    /* CPU supports flush instruction. */
#define HWCAP_SPARC_STBAR       2
#define HWCAP_SPARC_SWAP        4
#define HWCAP_SPARC_MULDIV      8
#define HWCAP_SPARC_V9		16
#define HWCAP_SPARC_ULTRA3	32

#define CORE_DUMP_USE_REGSET

/* Format is:
 * 	G0 --> G7
 *	O0 --> O7
 *	L0 --> L7
 *	I0 --> I7
 *	PSR, PC, nPC, Y, WIM, TBR
 */
typedef unsigned long elf_greg_t;
#define ELF_NGREG 38
typedef elf_greg_t elf_gregset_t[ELF_NGREG];

typedef struct {
	union {
		unsigned long	pr_regs[32];
		double		pr_dregs[16];
	} pr_fr;
	unsigned long __unused;
	unsigned long	pr_fsr;
	unsigned char	pr_qcnt;
	unsigned char	pr_q_entrysize;
	unsigned char	pr_en;
	unsigned int	pr_q[64];
} elf_fpregset_t;

#include <asm/mbus.h>

/*
 * This is used to ensure we don't load something for the wrong architecture.
 */
#define elf_check_arch(x) ((x)->e_machine == EM_SPARC)

/*
 * These are used to set parameters in the core dumps.
 */
#define ELF_ARCH	EM_SPARC
#define ELF_CLASS	ELFCLASS32
#define ELF_DATA	ELFDATA2MSB

#define USE_ELF_CORE_DUMP

#define ELF_EXEC_PAGESIZE	4096


/* This is the location that an ET_DYN program is loaded if exec'ed.  Typical
   use of this is to invoke "./ld.so someprog" to test out a new version of
   the loader.  We need to make sure that it is out of the way of the program
   that it will "exec", and that there is sufficient room for the brk.  */

#define ELF_ET_DYN_BASE         (TASK_UNMAPPED_BASE)

/* This yields a mask that user programs can use to figure out what
   instruction set this cpu supports.  This can NOT be done in userspace
   on Sparc.  */

/* Sun4c has none of the capabilities, most sun4m's have them all.
 * XXX This is gross, set some global variable at boot time. -DaveM
 */
#define ELF_HWCAP	((ARCH_SUN4C) ? 0 : \
			 (HWCAP_SPARC_FLUSH | HWCAP_SPARC_STBAR | \
			  HWCAP_SPARC_SWAP | \
			  ((srmmu_modtype != Cypress && \
			    srmmu_modtype != Cypress_vE && \
			    srmmu_modtype != Cypress_vD) ? \
			   HWCAP_SPARC_MULDIV : 0)))

/* This yields a string that ld.so will use to load implementation
   specific libraries for optimization.  This is more specific in
   intent than poking at uname or /proc/cpuinfo. */

#define ELF_PLATFORM	(NULL)

#define SET_PERSONALITY(ex) set_personality(PER_LINUX)

#endif /* !(__ASMSPARC_ELF_H) */

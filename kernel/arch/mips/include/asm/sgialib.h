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
 * SGI ARCS firmware interface library for the Linux kernel.
 *
 * Copyright (C) 1996 David S. Miller (dm@engr.sgi.com)
 * Copyright (C) 2001, 2002 Ralf Baechle (ralf@gnu.org)
 */
#ifndef _ASM_SGIALIB_H
#define _ASM_SGIALIB_H

#include <asm/sgiarcs.h>

extern struct linux_romvec *romvec;
extern int prom_argc;

extern LONG *_prom_argv, *_prom_envp;

/* A 32-bit ARC PROM pass arguments and environment as 32-bit pointer.
   These macros take care of sign extension.  */
#define prom_argv(index) ((char *) (long) _prom_argv[(index)])
#define prom_argc(index) ((char *) (long) _prom_argc[(index)])

extern int prom_flags;

#define PROM_FLAG_ARCS			1
#define PROM_FLAG_USE_AS_CONSOLE	2
#define PROM_FLAG_DONT_FREE_TEMP	4

/* Simple char-by-char console I/O. */
extern void prom_putchar(char c);
extern char prom_getchar(void);

/* Memory descriptor management. */
#define PROM_MAX_PMEMBLOCKS    32
struct prom_pmemblock {
	LONG	base;		/* Within KSEG0 or XKPHYS. */
	ULONG	size;		/* In bytes. */
	ULONG	type;		/* free or prom memory */
};

/* Get next memory descriptor after CURR, returns first descriptor
 * in chain is CURR is NULL.
 */
extern struct linux_mdesc *prom_getmdesc(struct linux_mdesc *curr);
#define PROM_NULL_MDESC   ((struct linux_mdesc *) 0)

/* Called by prom_init to setup the physical memory pmemblock
 * array.
 */
extern void prom_meminit(void);
extern void prom_fixup_mem_map(unsigned long start_mem, unsigned long end_mem);

/* PROM device tree library routines. */
#define PROM_NULL_COMPONENT ((pcomponent *) 0)

/* Get sibling component of THIS. */
extern pcomponent *ArcGetPeer(pcomponent *this);

/* Get child component of THIS. */
extern pcomponent *ArcGetChild(pcomponent *this);

/* Get parent component of CHILD. */
extern pcomponent *prom_getparent(pcomponent *child);

/* Copy component opaque data of component THIS into BUFFER
 * if component THIS has opaque data.  Returns success or
 * failure status.
 */
extern long prom_getcdata(void *buffer, pcomponent *this);

/* Other misc. component routines. */
extern pcomponent *prom_childadd(pcomponent *this, pcomponent *tmp, void *data);
extern long prom_delcomponent(pcomponent *this);
extern pcomponent *prom_componentbypath(char *path);

/* This is called at prom_init time to identify the
 * ARC architecture we are running on
 */
extern void prom_identify_arch(void);

/* Environment variable routines. */
extern PCHAR ArcGetEnvironmentVariable(PCHAR name);
extern LONG ArcSetEnvironmentVariable(PCHAR name, PCHAR value);

/* ARCS command line acquisition and parsing. */
extern char *prom_getcmdline(void);
extern void prom_init_cmdline(void);

/* Acquiring info about the current time, etc. */
extern struct linux_tinfo *prom_gettinfo(void);
extern unsigned long prom_getrtime(void);

/* File operations. */
extern long prom_getvdirent(unsigned long fd, struct linux_vdirent *ent, unsigned long num, unsigned long *cnt);
extern long prom_open(char *name, enum linux_omode md, unsigned long *fd);
extern long prom_close(unsigned long fd);
extern LONG ArcRead(ULONG fd, PVOID buf, ULONG num, PULONG cnt);
extern long prom_getrstatus(unsigned long fd);
extern LONG ArcWrite(ULONG fd, PVOID buf, ULONG num, PULONG cnt);
extern long prom_seek(unsigned long fd, struct linux_bigint *off, enum linux_seekmode sm);
extern long prom_mount(char *name, enum linux_mountops op);
extern long prom_getfinfo(unsigned long fd, struct linux_finfo *buf);
extern long prom_setfinfo(unsigned long fd, unsigned long flags, unsigned long msk);

/* Running stand-along programs. */
extern long prom_load(char *name, unsigned long end, unsigned long *pc, unsigned long *eaddr);
extern long prom_invoke(unsigned long pc, unsigned long sp, long argc, char **argv, char **envp);
extern long prom_exec(char *name, long argc, char **argv, char **envp);

/* Misc. routines. */
extern VOID prom_halt(VOID) __attribute__((noreturn));
extern VOID prom_powerdown(VOID) __attribute__((noreturn));
extern VOID prom_restart(VOID) __attribute__((noreturn));
extern VOID ArcReboot(VOID) __attribute__((noreturn));
extern VOID ArcEnterInteractiveMode(VOID) __attribute__((noreturn));
extern long prom_cfgsave(VOID);
extern struct linux_sysid *prom_getsysid(VOID);
extern VOID ArcFlushAllCaches(VOID);
extern DISPLAY_STATUS *ArcGetDisplayStatus(ULONG FileID);

#endif /* _ASM_SGIALIB_H */

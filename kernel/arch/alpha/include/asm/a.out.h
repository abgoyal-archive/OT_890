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

#ifndef __ALPHA_A_OUT_H__
#define __ALPHA_A_OUT_H__

#include <linux/types.h>

/*
 * OSF/1 ECOFF header structs.  ECOFF files consist of:
 * 	- a file header (struct filehdr),
 *	- an a.out header (struct aouthdr),
 *	- one or more section headers (struct scnhdr). 
 *	  The filhdr's "f_nscns" field contains the
 *	  number of section headers.
 */

struct filehdr
{
	/* OSF/1 "file" header */
	__u16 f_magic, f_nscns;
	__u32 f_timdat;
	__u64 f_symptr;
	__u32 f_nsyms;
	__u16 f_opthdr, f_flags;
};

struct aouthdr
{
	__u64 info;		/* after that it looks quite normal.. */
	__u64 tsize;
	__u64 dsize;
	__u64 bsize;
	__u64 entry;
	__u64 text_start;	/* with a few additions that actually make sense */
	__u64 data_start;
	__u64 bss_start;
	__u32 gprmask, fprmask;	/* bitmask of general & floating point regs used in binary */
	__u64 gpvalue;
};

struct scnhdr
{
	char	s_name[8];
	__u64	s_paddr;
	__u64	s_vaddr;
	__u64	s_size;
	__u64	s_scnptr;
	__u64	s_relptr;
	__u64	s_lnnoptr;
	__u16	s_nreloc;
	__u16	s_nlnno;
	__u32	s_flags;
};

struct exec
{
	/* OSF/1 "file" header */
	struct filehdr		fh;
	struct aouthdr		ah;
};

/*
 * Define's so that the kernel exec code can access the a.out header
 * fields...
 */
#define	a_info		ah.info
#define	a_text		ah.tsize
#define a_data		ah.dsize
#define a_bss		ah.bsize
#define a_entry		ah.entry
#define a_textstart	ah.text_start
#define	a_datastart	ah.data_start
#define	a_bssstart	ah.bss_start
#define	a_gprmask	ah.gprmask
#define a_fprmask	ah.fprmask
#define a_gpvalue	ah.gpvalue

#define N_TXTADDR(x) ((x).a_textstart)
#define N_DATADDR(x) ((x).a_datastart)
#define N_BSSADDR(x) ((x).a_bssstart)
#define N_DRSIZE(x) 0
#define N_TRSIZE(x) 0
#define N_SYMSIZE(x) 0

#define AOUTHSZ		sizeof(struct aouthdr)
#define SCNHSZ		sizeof(struct scnhdr)
#define SCNROUND	16

#define N_TXTOFF(x) \
  ((long) N_MAGIC(x) == ZMAGIC ? 0 : \
   (sizeof(struct exec) + (x).fh.f_nscns*SCNHSZ + SCNROUND - 1) & ~(SCNROUND - 1))

#ifdef __KERNEL__

/* Assume that start addresses below 4G belong to a TASO application.
   Unfortunately, there is no proper bit in the exec header to check.
   Worse, we have to notice the start address before swapping to use
   /sbin/loader, which of course is _not_ a TASO application.  */
#define SET_AOUT_PERSONALITY(BFPM, EX) \
	set_personality (((BFPM->taso || EX.ah.entry < 0x100000000L \
			   ? ADDR_LIMIT_32BIT : 0) | PER_OSF4))

#endif /* __KERNEL__ */
#endif /* __A_OUT_GNU_H__ */

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
 * Some ECOFF definitions.
 */
typedef struct filehdr {
        unsigned short  f_magic;        /* magic number */
        unsigned short  f_nscns;        /* number of sections */
        long            f_timdat;       /* time & date stamp */
        long            f_symptr;       /* file pointer to symbolic header */
        long            f_nsyms;        /* sizeof(symbolic hdr) */
        unsigned short  f_opthdr;       /* sizeof(optional hdr) */
        unsigned short  f_flags;        /* flags */
} FILHDR;
#define FILHSZ  sizeof(FILHDR)

#define OMAGIC		0407
#define MIPSEBMAGIC	0x160
#define MIPSELMAGIC	0x162

typedef struct scnhdr {
        char            s_name[8];      /* section name */
        long            s_paddr;        /* physical address, aliased s_nlib */
        long            s_vaddr;        /* virtual address */
        long            s_size;         /* section size */
        long            s_scnptr;       /* file ptr to raw data for section */
        long            s_relptr;       /* file ptr to relocation */
        long            s_lnnoptr;      /* file ptr to gp histogram */
        unsigned short  s_nreloc;       /* number of relocation entries */
        unsigned short  s_nlnno;        /* number of gp histogram entries */
        long            s_flags;        /* flags */
} SCNHDR;
#define SCNHSZ		sizeof(SCNHDR)
#define SCNROUND	((long)16)

typedef struct aouthdr {
        short   magic;          /* see above                            */
        short   vstamp;         /* version stamp                        */
        long    tsize;          /* text size in bytes, padded to DW bdry*/
        long    dsize;          /* initialized data "  "                */
        long    bsize;          /* uninitialized data "   "             */
        long    entry;          /* entry pt.                            */
        long    text_start;     /* base of text used for this file      */
        long    data_start;     /* base of data used for this file      */
        long    bss_start;      /* base of bss used for this file       */
        long    gprmask;        /* general purpose register mask        */
        long    cprmask[4];     /* co-processor register masks          */
        long    gp_value;       /* the gp value used for this object    */
} AOUTHDR;
#define AOUTHSZ sizeof(AOUTHDR)

#define OMAGIC		0407
#define NMAGIC		0410
#define ZMAGIC		0413
#define SMAGIC		0411
#define LIBMAGIC        0443

#define N_TXTOFF(f, a) \
 ((a).magic == ZMAGIC || (a).magic == LIBMAGIC ? 0 : \
  ((a).vstamp < 23 ? \
   ((FILHSZ + AOUTHSZ + (f).f_nscns * SCNHSZ + 7) & 0xfffffff8) : \
   ((FILHSZ + AOUTHSZ + (f).f_nscns * SCNHSZ + SCNROUND-1) & ~(SCNROUND-1)) ) )
#define N_DATOFF(f, a) \
  N_TXTOFF(f, a) + (a).tsize;

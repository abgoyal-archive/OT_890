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
 * include/asm-blackfin/flat.h -- uClinux flat-format executables
 *
 * Copyright (C) 2003,
 *
 */

#ifndef __BLACKFIN_FLAT_H__
#define __BLACKFIN_FLAT_H__

#include <asm/unaligned.h>

#define	flat_stack_align(sp)	/* nothing needed */
#define	flat_argvp_envp_on_stack()		0
#define	flat_old_ram_flag(flags)		(flags)

extern unsigned long bfin_get_addr_from_rp (unsigned long *ptr,
					unsigned long relval,
					unsigned long flags,
					unsigned long *persistent);

extern void bfin_put_addr_at_rp(unsigned long *ptr, unsigned long addr,
		                unsigned long relval);

/* The amount by which a relocation can exceed the program image limits
   without being regarded as an error.  */

#define	flat_reloc_valid(reloc, size)	((reloc) <= (size))

#define	flat_get_addr_from_rp(rp, relval, flags, persistent)	\
	bfin_get_addr_from_rp(rp, relval, flags, persistent)
#define	flat_put_addr_at_rp(rp, val, relval)	\
	bfin_put_addr_at_rp(rp, val, relval)

/* Convert a relocation entry into an address.  */
static inline unsigned long
flat_get_relocate_addr (unsigned long relval)
{
	return relval & 0x03ffffff; /* Mask out top 6 bits */
}

static inline int flat_set_persistent(unsigned long relval,
				      unsigned long *persistent)
{
	int type = (relval >> 26) & 7;
	if (type == 3) {
		*persistent = relval << 16;
		return 1;
	}
	return 0;
}

static inline int flat_addr_absolute(unsigned long relval)
{
	return (relval & (1 << 29)) != 0;
}

#endif				/* __BLACKFIN_FLAT_H__ */

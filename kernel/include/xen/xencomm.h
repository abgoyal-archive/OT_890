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
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * Copyright (C) IBM Corp. 2006
 *
 * Authors: Hollis Blanchard <hollisb@us.ibm.com>
 *          Jerone Young <jyoung5@us.ibm.com>
 */

#ifndef _LINUX_XENCOMM_H_
#define _LINUX_XENCOMM_H_

#include <xen/interface/xencomm.h>

#define XENCOMM_MINI_ADDRS 3
struct xencomm_mini {
	struct xencomm_desc _desc;
	uint64_t address[XENCOMM_MINI_ADDRS];
};

/* To avoid additionnal virt to phys conversion, an opaque structure is
   presented.  */
struct xencomm_handle;

extern void xencomm_free(struct xencomm_handle *desc);
extern struct xencomm_handle *xencomm_map(void *ptr, unsigned long bytes);
extern struct xencomm_handle *__xencomm_map_no_alloc(void *ptr,
			unsigned long bytes,  struct xencomm_mini *xc_area);

#if 0
#define XENCOMM_MINI_ALIGNED(xc_desc, n)				\
	struct xencomm_mini xc_desc ## _base[(n)]			\
	__attribute__((__aligned__(sizeof(struct xencomm_mini))));	\
	struct xencomm_mini *xc_desc = &xc_desc ## _base[0];
#else
/*
 * gcc bug workaround:
 * http://gcc.gnu.org/bugzilla/show_bug.cgi?id=16660
 * gcc doesn't handle properly stack variable with
 * __attribute__((__align__(sizeof(struct xencomm_mini))))
 */
#define XENCOMM_MINI_ALIGNED(xc_desc, n)				\
	unsigned char xc_desc ## _base[((n) + 1 ) *			\
				       sizeof(struct xencomm_mini)];	\
	struct xencomm_mini *xc_desc = (struct xencomm_mini *)		\
		((unsigned long)xc_desc ## _base +			\
		 (sizeof(struct xencomm_mini) -				\
		  ((unsigned long)xc_desc ## _base) %			\
		  sizeof(struct xencomm_mini)));
#endif
#define xencomm_map_no_alloc(ptr, bytes)			\
	({ XENCOMM_MINI_ALIGNED(xc_desc, 1);			\
		__xencomm_map_no_alloc(ptr, bytes, xc_desc); })

/* provided by architecture code: */
extern unsigned long xencomm_vtop(unsigned long vaddr);

static inline void *xencomm_pa(void *ptr)
{
	return (void *)xencomm_vtop((unsigned long)ptr);
}

#define xen_guest_handle(hnd)  ((hnd).p)

#endif /* _LINUX_XENCOMM_H_ */

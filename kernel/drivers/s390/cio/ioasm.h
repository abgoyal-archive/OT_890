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

#ifndef S390_CIO_IOASM_H
#define S390_CIO_IOASM_H

#include <asm/chpid.h>
#include <asm/schid.h>

/*
 * TPI info structure
 */
struct tpi_info {
	struct subchannel_id schid;
	__u32 intparm;		 /* interruption parameter */
	__u32 adapter_IO : 1;
	__u32 reserved2	 : 1;
	__u32 isc	 : 3;
	__u32 reserved3	 : 12;
	__u32 int_type	 : 3;
	__u32 reserved4	 : 12;
} __attribute__ ((packed));


/*
 * Some S390 specific IO instructions as inline
 */

static inline int stsch(struct subchannel_id schid, struct schib *addr)
{
	register struct subchannel_id reg1 asm ("1") = schid;
	int ccode;

	asm volatile(
		"	stsch	0(%3)\n"
		"	ipm	%0\n"
		"	srl	%0,28"
		: "=d" (ccode), "=m" (*addr)
		: "d" (reg1), "a" (addr)
		: "cc");
	return ccode;
}

static inline int stsch_err(struct subchannel_id schid, struct schib *addr)
{
	register struct subchannel_id reg1 asm ("1") = schid;
	int ccode = -EIO;

	asm volatile(
		"	stsch	0(%3)\n"
		"0:	ipm	%0\n"
		"	srl	%0,28\n"
		"1:\n"
		EX_TABLE(0b,1b)
		: "+d" (ccode), "=m" (*addr)
		: "d" (reg1), "a" (addr)
		: "cc");
	return ccode;
}

static inline int msch(struct subchannel_id schid, struct schib *addr)
{
	register struct subchannel_id reg1 asm ("1") = schid;
	int ccode;

	asm volatile(
		"	msch	0(%2)\n"
		"	ipm	%0\n"
		"	srl	%0,28"
		: "=d" (ccode)
		: "d" (reg1), "a" (addr), "m" (*addr)
		: "cc");
	return ccode;
}

static inline int msch_err(struct subchannel_id schid, struct schib *addr)
{
	register struct subchannel_id reg1 asm ("1") = schid;
	int ccode = -EIO;

	asm volatile(
		"	msch	0(%2)\n"
		"0:	ipm	%0\n"
		"	srl	%0,28\n"
		"1:\n"
		EX_TABLE(0b,1b)
		: "+d" (ccode)
		: "d" (reg1), "a" (addr), "m" (*addr)
		: "cc");
	return ccode;
}

static inline int tsch(struct subchannel_id schid, struct irb *addr)
{
	register struct subchannel_id reg1 asm ("1") = schid;
	int ccode;

	asm volatile(
		"	tsch	0(%3)\n"
		"	ipm	%0\n"
		"	srl	%0,28"
		: "=d" (ccode), "=m" (*addr)
		: "d" (reg1), "a" (addr)
		: "cc");
	return ccode;
}

static inline int tpi(struct tpi_info *addr)
{
	int ccode;

	asm volatile(
		"	tpi	0(%2)\n"
		"	ipm	%0\n"
		"	srl	%0,28"
		: "=d" (ccode), "=m" (*addr)
		: "a" (addr)
		: "cc");
	return ccode;
}

static inline int chsc(void *chsc_area)
{
	typedef struct { char _[4096]; } addr_type;
	int cc;

	asm volatile(
		"	.insn	rre,0xb25f0000,%2,0\n"
		"	ipm	%0\n"
		"	srl	%0,28\n"
		: "=d" (cc), "=m" (*(addr_type *) chsc_area)
		: "d" (chsc_area), "m" (*(addr_type *) chsc_area)
		: "cc");
	return cc;
}

static inline int rchp(struct chp_id chpid)
{
	register struct chp_id reg1 asm ("1") = chpid;
	int ccode;

	asm volatile(
		"	lr	1,%1\n"
		"	rchp\n"
		"	ipm	%0\n"
		"	srl	%0,28"
		: "=d" (ccode) : "d" (reg1) : "cc");
	return ccode;
}

#endif

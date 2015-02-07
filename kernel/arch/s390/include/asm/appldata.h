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
 * include/asm-s390/appldata.h
 *
 * Copyright (C) IBM Corp. 2006
 *
 * Author(s): Melissa Howland <melissah@us.ibm.com>
 */

#ifndef _ASM_S390_APPLDATA_H
#define _ASM_S390_APPLDATA_H

#include <asm/io.h>

#ifndef CONFIG_64BIT

#define APPLDATA_START_INTERVAL_REC	0x00	/* Function codes for */
#define APPLDATA_STOP_REC		0x01	/* DIAG 0xDC	      */
#define APPLDATA_GEN_EVENT_REC		0x02
#define APPLDATA_START_CONFIG_REC	0x03

/*
 * Parameter list for DIAGNOSE X'DC'
 */
struct appldata_parameter_list {
	u16 diag;		/* The DIAGNOSE code X'00DC'	      */
	u8  function;		/* The function code for the DIAGNOSE */
	u8  parlist_length;	/* Length of the parameter list       */
	u32 product_id_addr;	/* Address of the 16-byte product ID  */
	u16 reserved;
	u16 buffer_length;	/* Length of the application data buffer  */
	u32 buffer_addr;	/* Address of the application data buffer */
} __attribute__ ((packed));

#else /* CONFIG_64BIT */

#define APPLDATA_START_INTERVAL_REC	0x80
#define APPLDATA_STOP_REC		0x81
#define APPLDATA_GEN_EVENT_REC		0x82
#define APPLDATA_START_CONFIG_REC	0x83

/*
 * Parameter list for DIAGNOSE X'DC'
 */
struct appldata_parameter_list {
	u16 diag;
	u8  function;
	u8  parlist_length;
	u32 unused01;
	u16 reserved;
	u16 buffer_length;
	u32 unused02;
	u64 product_id_addr;
	u64 buffer_addr;
} __attribute__ ((packed));

#endif /* CONFIG_64BIT */

struct appldata_product_id {
	char prod_nr[7];	/* product number */
	u16  prod_fn;		/* product function */
	u8   record_nr; 	/* record number */
	u16  version_nr;	/* version */
	u16  release_nr;	/* release */
	u16  mod_lvl;		/* modification level */
} __attribute__ ((packed));

static inline int appldata_asm(struct appldata_product_id *id,
			       unsigned short fn, void *buffer,
			       unsigned short length)
{
	struct appldata_parameter_list parm_list;
	int ry;

	if (!MACHINE_IS_VM)
		return -ENOSYS;
	parm_list.diag = 0xdc;
	parm_list.function = fn;
	parm_list.parlist_length = sizeof(parm_list);
	parm_list.buffer_length = length;
	parm_list.product_id_addr = (unsigned long) id;
	parm_list.buffer_addr = virt_to_phys(buffer);
	asm volatile(
		"	diag	%1,%0,0xdc"
		: "=d" (ry)
		: "d" (&parm_list), "m" (parm_list), "m" (*id)
		: "cc");
	return ry;
}

#endif /* _ASM_S390_APPLDATA_H */

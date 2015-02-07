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
 * Copyright (C) 1992 - 1997, 2000-2006 Silicon Graphics, Inc. All rights reserved.
 */

#ifndef _ASM_IA64_SN_INTR_H
#define _ASM_IA64_SN_INTR_H

#include <linux/rcupdate.h>
#include <asm/sn/types.h>

#define SGI_UART_VECTOR		0xe9

/* Reserved IRQs : Note, not to exceed IA64_SN2_FIRST_DEVICE_VECTOR */
#define SGI_XPC_ACTIVATE	0x30
#define SGI_II_ERROR		0x31
#define SGI_XBOW_ERROR		0x32
#define SGI_PCIASIC_ERROR	0x33
#define SGI_ACPI_SCI_INT	0x34
#define SGI_TIOCA_ERROR		0x35
#define SGI_TIO_ERROR		0x36
#define SGI_TIOCX_ERROR		0x37
#define SGI_MMTIMER_VECTOR	0x38
#define SGI_XPC_NOTIFY		0xe7

#define IA64_SN2_FIRST_DEVICE_VECTOR	0x3c
#define IA64_SN2_LAST_DEVICE_VECTOR	0xe6

#define SN2_IRQ_RESERVED	0x1
#define SN2_IRQ_CONNECTED	0x2
#define SN2_IRQ_SHARED		0x4

// The SN PROM irq struct
struct sn_irq_info {
	struct sn_irq_info *irq_next;	/* deprecated DO NOT USE     */
	short		irq_nasid;	/* Nasid IRQ is assigned to  */
	int		irq_slice;	/* slice IRQ is assigned to  */
	int		irq_cpuid;	/* kernel logical cpuid	     */
	int		irq_irq;	/* the IRQ number */
	int		irq_int_bit;	/* Bridge interrupt pin */
					/* <0 means MSI */
	u64	irq_xtalkaddr;	/* xtalkaddr IRQ is sent to  */
	int		irq_bridge_type;/* pciio asic type (pciio.h) */
	void	       *irq_bridge;	/* bridge generating irq     */
	void	       *irq_pciioinfo;	/* associated pciio_info_t   */
	int		irq_last_intr;	/* For Shub lb lost intr WAR */
	int		irq_cookie;	/* unique cookie 	     */
	int		irq_flags;	/* flags */
	int		irq_share_cnt;	/* num devices sharing IRQ   */
	struct list_head	list;	/* list of sn_irq_info structs */
	struct rcu_head		rcu;	/* rcu callback list */
};

extern void sn_send_IPI_phys(int, long, int, int);
extern u64 sn_intr_alloc(nasid_t, int,
			      struct sn_irq_info *,
			      int, nasid_t, int);
extern void sn_intr_free(nasid_t, int, struct sn_irq_info *);
extern struct sn_irq_info *sn_retarget_vector(struct sn_irq_info *, nasid_t, int);
extern void sn_set_err_irq_affinity(unsigned int);
extern struct list_head **sn_irq_lh;

#define CPU_VECTOR_TO_IRQ(cpuid,vector) (vector)

#endif /* _ASM_IA64_SN_INTR_H */

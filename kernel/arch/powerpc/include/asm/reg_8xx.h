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
 * Contains register definitions common to PowerPC 8xx CPUs.  Notice
 */
#ifndef _ASM_POWERPC_REG_8xx_H
#define _ASM_POWERPC_REG_8xx_H

/* Cache control on the MPC8xx is provided through some additional
 * special purpose registers.
 */
#define SPRN_IC_CST	560	/* Instruction cache control/status */
#define SPRN_IC_ADR	561	/* Address needed for some commands */
#define SPRN_IC_DAT	562	/* Read-only data register */
#define SPRN_DC_CST	568	/* Data cache control/status */
#define SPRN_DC_ADR	569	/* Address needed for some commands */
#define SPRN_DC_DAT	570	/* Read-only data register */

/* Commands.  Only the first few are available to the instruction cache.
*/
#define	IDC_ENABLE	0x02000000	/* Cache enable */
#define IDC_DISABLE	0x04000000	/* Cache disable */
#define IDC_LDLCK	0x06000000	/* Load and lock */
#define IDC_UNLINE	0x08000000	/* Unlock line */
#define IDC_UNALL	0x0a000000	/* Unlock all */
#define IDC_INVALL	0x0c000000	/* Invalidate all */

#define DC_FLINE	0x0e000000	/* Flush data cache line */
#define DC_SFWT		0x01000000	/* Set forced writethrough mode */
#define DC_CFWT		0x03000000	/* Clear forced writethrough mode */
#define DC_SLES		0x05000000	/* Set little endian swap mode */
#define DC_CLES		0x07000000	/* Clear little endian swap mode */

/* Status.
*/
#define IDC_ENABLED	0x80000000	/* Cache is enabled */
#define IDC_CERR1	0x00200000	/* Cache error 1 */
#define IDC_CERR2	0x00100000	/* Cache error 2 */
#define IDC_CERR3	0x00080000	/* Cache error 3 */

#define DC_DFWT		0x40000000	/* Data cache is forced write through */
#define DC_LES		0x20000000	/* Caches are little endian mode */

#endif /* _ASM_POWERPC_REG_8xx_H */

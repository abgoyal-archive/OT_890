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

#ifndef __SUPP_REG_H__
#define __SUPP_REG_H__

/* Macros for reading and writing support/special registers. */

#ifndef STRINGIFYFY
#define STRINGIFYFY(i) #i
#endif

#ifndef STRINGIFY
#define STRINGIFY(i) STRINGIFYFY(i)
#endif

#define SPEC_REG_BZ     "BZ"
#define SPEC_REG_VR     "VR"
#define SPEC_REG_PID    "PID"
#define SPEC_REG_SRS    "SRS"
#define SPEC_REG_WZ     "WZ"
#define SPEC_REG_EXS    "EXS"
#define SPEC_REG_EDA    "EDA"
#define SPEC_REG_MOF    "MOF"
#define SPEC_REG_DZ     "DZ"
#define SPEC_REG_EBP    "EBP"
#define SPEC_REG_ERP    "ERP"
#define SPEC_REG_SRP    "SRP"
#define SPEC_REG_NRP    "NRP"
#define SPEC_REG_CCS    "CCS"
#define SPEC_REG_USP    "USP"
#define SPEC_REG_SPC    "SPC"

#define RW_MM_CFG       0
#define RW_MM_KBASE_LO  1
#define RW_MM_KBASE_HI  2
#define RW_MM_CAUSE     3
#define RW_MM_TLB_SEL   4
#define RW_MM_TLB_LO    5
#define RW_MM_TLB_HI    6
#define RW_MM_TLB_PGD   7

#define BANK_GC		0
#define BANK_IM		1
#define BANK_DM		2
#define BANK_BP		3

#define RW_GC_CFG       0
#define RW_GC_CCS       1
#define RW_GC_SRS       2
#define RW_GC_NRP       3
#define RW_GC_EXS       4
#define RW_GC_R0        8
#define RW_GC_R1        9

#define SPEC_REG_WR(r,v) \
__asm__ __volatile__ ("move %0, $" r : : "r" (v));

#define SPEC_REG_RD(r,v) \
__asm__ __volatile__ ("move $" r ",%0" : "=r" (v));

#define NOP() \
	__asm__ __volatile__ ("nop");

#define SUPP_BANK_SEL(b) 		\
	SPEC_REG_WR(SPEC_REG_SRS,b);	\
	NOP();				\
	NOP();				\
	NOP();

#define SUPP_REG_WR(r,v) \
__asm__ __volatile__ ("move %0, $S" STRINGIFYFY(r) "\n\t"	\
		      "nop\n\t"					\
		      "nop\n\t"					\
		      "nop\n\t"					\
		      : : "r" (v));

#define SUPP_REG_RD(r,v) \
__asm__ __volatile__ ("move $S" STRINGIFYFY(r) ",%0" : "=r" (v));

#endif /* __SUPP_REG_H__ */

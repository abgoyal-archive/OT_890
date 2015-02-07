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

#ifndef _ASM_BLACKFIN_SIGCONTEXT_H
#define _ASM_BLACKFIN_SIGCONTEXT_H

/* Add new entries at the end of the structure only.  */
struct sigcontext {
	unsigned long sc_r0;
	unsigned long sc_r1;
	unsigned long sc_r2;
	unsigned long sc_r3;
	unsigned long sc_r4;
	unsigned long sc_r5;
	unsigned long sc_r6;
	unsigned long sc_r7;
	unsigned long sc_p0;
	unsigned long sc_p1;
	unsigned long sc_p2;
	unsigned long sc_p3;
	unsigned long sc_p4;
	unsigned long sc_p5;
	unsigned long sc_usp;
	unsigned long sc_a0w;
	unsigned long sc_a1w;
	unsigned long sc_a0x;
	unsigned long sc_a1x;
	unsigned long sc_astat;
	unsigned long sc_rets;
	unsigned long sc_pc;
	unsigned long sc_retx;
	unsigned long sc_fp;
	unsigned long sc_i0;
	unsigned long sc_i1;
	unsigned long sc_i2;
	unsigned long sc_i3;
	unsigned long sc_m0;
	unsigned long sc_m1;
	unsigned long sc_m2;
	unsigned long sc_m3;
	unsigned long sc_l0;
	unsigned long sc_l1;
	unsigned long sc_l2;
	unsigned long sc_l3;
	unsigned long sc_b0;
	unsigned long sc_b1;
	unsigned long sc_b2;
	unsigned long sc_b3;
	unsigned long sc_lc0;
	unsigned long sc_lc1;
	unsigned long sc_lt0;
	unsigned long sc_lt1;
	unsigned long sc_lb0;
	unsigned long sc_lb1;
	unsigned long sc_seqstat;
};

#endif

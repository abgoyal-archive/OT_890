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

/* -*- mode: c; c-basic-offset: 8 -*- */

/* Platform specific MCA defines */
#ifndef _ASM_X86_MCA_H
#define _ASM_X86_MCA_H

/* Maximal number of MCA slots - actually, some machines have less, but
 * they all have sufficient number of POS registers to cover 8.
 */
#define MCA_MAX_SLOT_NR  8

/* Most machines have only one MCA bus.  The only multiple bus machines
 * I know have at most two */
#define MAX_MCA_BUSSES 2

#define MCA_PRIMARY_BUS		0
#define MCA_SECONDARY_BUS	1

/* Dummy slot numbers on primary MCA for integrated functions */
#define MCA_INTEGSCSI	(MCA_MAX_SLOT_NR)
#define MCA_INTEGVIDEO	(MCA_MAX_SLOT_NR+1)
#define MCA_MOTHERBOARD (MCA_MAX_SLOT_NR+2)

/* Dummy POS values for integrated functions */
#define MCA_DUMMY_POS_START	0x10000
#define MCA_INTEGSCSI_POS	(MCA_DUMMY_POS_START+1)
#define MCA_INTEGVIDEO_POS	(MCA_DUMMY_POS_START+2)
#define MCA_MOTHERBOARD_POS	(MCA_DUMMY_POS_START+3)

/* MCA registers */

#define MCA_MOTHERBOARD_SETUP_REG	0x94
#define MCA_ADAPTER_SETUP_REG		0x96
#define MCA_POS_REG(n)			(0x100+(n))

#define MCA_ENABLED	0x01	/* POS 2, set if adapter enabled */

/* Max number of adapters, including both slots and various integrated
 * things.
 */
#define MCA_NUMADAPTERS (MCA_MAX_SLOT_NR+3)

#endif /* _ASM_X86_MCA_H */

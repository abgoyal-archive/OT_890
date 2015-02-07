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

/* $Id$
 *
 * ICC specific routines
 *
 * Author       Matt Henderson & Guy Ellis
 * Copyright    by Traverse Technologies Pty Ltd, www.travers.com.au
 * 
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 * 1999.7.14 Initial implementation of routines for Siemens ISDN 
 * Communication Controller PEB 2070 based on the ISAC routines 
 * written by Karsten Keil.
 */

/* All Registers original Siemens Spec  */

#define ICC_MASK 0x20
#define ICC_ISTA 0x20
#define ICC_STAR 0x21
#define ICC_CMDR 0x21
#define ICC_EXIR 0x24
#define ICC_ADF2 0x39
#define ICC_SPCR 0x30
#define ICC_ADF1 0x38
#define ICC_CIR0 0x31
#define ICC_CIX0 0x31
#define ICC_CIR1 0x33
#define ICC_CIX1 0x33
#define ICC_STCR 0x37
#define ICC_MODE 0x22
#define ICC_RSTA 0x27
#define ICC_RBCL 0x25
#define ICC_RBCH 0x2A
#define ICC_TIMR 0x23
#define ICC_SQXR 0x3b
#define ICC_MOSR 0x3a
#define ICC_MOCR 0x3a
#define ICC_MOR0 0x32
#define ICC_MOX0 0x32
#define ICC_MOR1 0x34
#define ICC_MOX1 0x34

#define ICC_RBCH_XAC 0x80

#define ICC_CMD_TIM    0x0
#define ICC_CMD_RES    0x1
#define ICC_CMD_DU     0x3
#define ICC_CMD_EI1    0x4
#define ICC_CMD_SSP    0x5
#define ICC_CMD_DT     0x6
#define ICC_CMD_AR     0x8
#define ICC_CMD_ARL    0xA
#define ICC_CMD_AI     0xC
#define ICC_CMD_DI     0xF

#define ICC_IND_DR     0x0
#define ICC_IND_FJ     0x2
#define ICC_IND_EI1    0x4
#define ICC_IND_INT    0x6
#define ICC_IND_PU     0x7
#define ICC_IND_AR     0x8
#define ICC_IND_ARL    0xA
#define ICC_IND_AI     0xC
#define ICC_IND_AIL    0xE
#define ICC_IND_DC     0xF

extern void ICCVersion(struct IsdnCardState *cs, char *s);
extern void initicc(struct IsdnCardState *cs);
extern void icc_interrupt(struct IsdnCardState *cs, u_char val);
extern void clear_pending_icc_ints(struct IsdnCardState *cs);
extern void setup_icc(struct IsdnCardState *);

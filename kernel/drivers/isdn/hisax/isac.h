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
 * ISAC specific defines
 *
 * Author       Karsten Keil
 * Copyright    by Karsten Keil      <keil@isdn4linux.de>
 * 
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

/* All Registers original Siemens Spec  */

#define ISAC_MASK 0x20
#define ISAC_ISTA 0x20
#define ISAC_STAR 0x21
#define ISAC_CMDR 0x21
#define ISAC_EXIR 0x24
#define ISAC_ADF2 0x39
#define ISAC_SPCR 0x30
#define ISAC_ADF1 0x38
#define ISAC_CIR0 0x31
#define ISAC_CIX0 0x31
#define ISAC_CIR1 0x33
#define ISAC_CIX1 0x33
#define ISAC_STCR 0x37
#define ISAC_MODE 0x22
#define ISAC_RSTA 0x27
#define ISAC_RBCL 0x25
#define ISAC_RBCH 0x2A
#define ISAC_TIMR 0x23
#define ISAC_SQXR 0x3b
#define ISAC_MOSR 0x3a
#define ISAC_MOCR 0x3a
#define ISAC_MOR0 0x32
#define ISAC_MOX0 0x32
#define ISAC_MOR1 0x34
#define ISAC_MOX1 0x34

#define ISAC_RBCH_XAC 0x80

#define ISAC_CMD_TIM	0x0
#define ISAC_CMD_RS	0x1
#define ISAC_CMD_SCZ	0x4
#define ISAC_CMD_SSZ	0x2
#define ISAC_CMD_AR8	0x8
#define ISAC_CMD_AR10	0x9
#define ISAC_CMD_ARL	0xA
#define ISAC_CMD_DUI	0xF

#define ISAC_IND_RS	0x1
#define ISAC_IND_PU	0x7
#define ISAC_IND_DR	0x0
#define ISAC_IND_SD	0x2
#define ISAC_IND_DIS	0x3
#define ISAC_IND_EI	0x6
#define ISAC_IND_RSY	0x4
#define ISAC_IND_ARD	0x8
#define ISAC_IND_TI	0xA
#define ISAC_IND_ATI	0xB
#define ISAC_IND_AI8	0xC
#define ISAC_IND_AI10	0xD
#define ISAC_IND_DID	0xF

extern void ISACVersion(struct IsdnCardState *, char *);
extern void setup_isac(struct IsdnCardState *);
extern void initisac(struct IsdnCardState *);
extern void isac_interrupt(struct IsdnCardState *, u_char);
extern void clear_pending_isac_ints(struct IsdnCardState *);

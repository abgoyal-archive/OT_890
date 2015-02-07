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
 * specific defines for CCD's HFC 2BS0
 *
 * Author       Karsten Keil
 * Copyright    by Karsten Keil      <keil@isdn4linux.de>
 * 
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#define HFC_CTMT	0xe0
#define HFC_CIRM  	0xc0
#define HFC_CIP		0x80
#define HFC_Z1		0x00
#define HFC_Z2		0x08
#define HFC_Z_LOW	0x00
#define HFC_Z_HIGH	0x04
#define HFC_F1_INC	0x28
#define HFC_FIFO_IN	0x2c
#define HFC_F1		0x30
#define HFC_F2		0x34
#define HFC_F2_INC	0x38
#define HFC_FIFO_OUT	0x3c
#define HFC_B1          0x00
#define HFC_B2		0x02
#define HFC_REC		0x01
#define HFC_SEND	0x00
#define HFC_CHANNEL(ch) (ch ? HFC_B2 : HFC_B1)

#define HFC_STATUS	0
#define HFC_DATA	1
#define HFC_DATA_NODEB	2

/* Status (READ) */
#define HFC_BUSY	0x01
#define HFC_TIMINT	0x02
#define HFC_EXTINT	0x04

/* CTMT (Write) */
#define HFC_CLTIMER 0x10
#define HFC_TIM50MS 0x08
#define HFC_TIMIRQE 0x04
#define HFC_TRANSB2 0x02
#define HFC_TRANSB1 0x01

/* CIRM (Write) */
#define HFC_RESET  	0x08
#define HFC_MEM8K	0x10
#define HFC_INTA	0x01
#define HFC_INTB	0x02
#define HFC_INTC	0x03
#define HFC_INTD	0x04
#define HFC_INTE	0x05
#define HFC_INTF	0x06

extern void main_irq_hfc(struct BCState *bcs);
extern void inithfc(struct IsdnCardState *cs);
extern void releasehfc(struct IsdnCardState *cs);

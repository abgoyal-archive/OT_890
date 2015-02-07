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
 * specific defines for CCD's HFC 2BDS0
 *
 * Author       Karsten Keil
 * Copyright    by Karsten Keil      <keil@isdn4linux.de>
 * 
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#define HFCD_CIRM  	0x18
#define HFCD_CTMT	0x19
#define HFCD_INT_M1  	0x1A
#define HFCD_INT_M2  	0x1B
#define HFCD_INT_S1  	0x1E
#define HFCD_STAT  	0x1C
#define HFCD_STAT_DISB  0x1D
#define HFCD_STATES  	0x30
#define HFCD_SCTRL  	0x31
#define HFCD_TEST  	0x32
#define HFCD_SQ  	0x34
#define HFCD_CLKDEL  	0x37
#define HFCD_MST_MODE	0x2E
#define HFCD_CONN  	0x2F

#define HFCD_FIFO	0x80
#define HFCD_Z1		0x10
#define HFCD_Z2		0x18
#define HFCD_Z_LOW	0x00
#define HFCD_Z_HIGH	0x04
#define HFCD_F1_INC	0x12
#define HFCD_FIFO_IN	0x16
#define HFCD_F1		0x1a
#define HFCD_F2		0x1e
#define HFCD_F2_INC	0x22
#define HFCD_FIFO_OUT	0x26
#define HFCD_REC	0x01
#define HFCD_SEND	0x00

#define HFCB_FIFO	0x80
#define HFCB_Z1		0x00
#define HFCB_Z2		0x08
#define HFCB_Z_LOW	0x00
#define HFCB_Z_HIGH	0x04
#define HFCB_F1_INC	0x28
#define HFCB_FIFO_IN	0x2c
#define HFCB_F1		0x30
#define HFCB_F2		0x34
#define HFCB_F2_INC	0x38
#define HFCB_FIFO_OUT	0x3c
#define HFCB_REC	0x01
#define HFCB_SEND	0x00
#define HFCB_B1		0x00
#define HFCB_B2		0x02
#define HFCB_CHANNEL(ch) (ch ? HFCB_B2 : HFCB_B1)

#define HFCD_STATUS	0
#define HFCD_DATA	1
#define HFCD_DATA_NODEB	2

/* Status (READ) */
#define HFCD_BUSY	0x01
#define HFCD_BUSY_NBUSY	0x04
#define HFCD_TIMER_ELAP	0x10
#define HFCD_STATINT	0x20
#define HFCD_FRAMEINT	0x40
#define HFCD_ANYINT	0x80

/* CTMT (Write) */
#define HFCD_CLTIMER 0x80
#define HFCD_TIM25  0x00
#define HFCD_TIM50  0x08
#define HFCD_TIM400 0x10
#define HFCD_TIM800 0x18
#define HFCD_AUTO_TIMER 0x20
#define HFCD_TRANSB2 0x02
#define HFCD_TRANSB1 0x01

/* CIRM (Write) */
#define HFCD_RESET  	0x08
#define HFCD_MEM8K	0x10
#define HFCD_INTA	0x01
#define HFCD_INTB	0x02
#define HFCD_INTC	0x03
#define HFCD_INTD	0x04
#define HFCD_INTE	0x05
#define HFCD_INTF	0x06

/* INT_M1;INT_S1 */
#define HFCD_INTS_B1TRANS	0x01
#define HFCD_INTS_B2TRANS	0x02
#define HFCD_INTS_DTRANS	0x04
#define HFCD_INTS_B1REC		0x08
#define HFCD_INTS_B2REC		0x10
#define HFCD_INTS_DREC		0x20
#define HFCD_INTS_L1STATE	0x40
#define HFCD_INTS_TIMER		0x80

/* INT_M2 */
#define HFCD_IRQ_ENABLE		0x08

/* STATES */
#define HFCD_LOAD_STATE		0x10
#define HFCD_ACTIVATE		0x20
#define HFCD_DO_ACTION		0x40

/* HFCD_MST_MODE */
#define HFCD_MASTER		0x01

/* HFCD_SCTRL */
#define SCTRL_B1_ENA		0x01
#define SCTRL_B2_ENA		0x02
#define SCTRL_LOW_PRIO		0x08
#define SCTRL_SQ_ENA		0x10
#define SCTRL_TEST		0x20
#define SCTRL_NONE_CAP		0x40
#define SCTRL_PWR_DOWN		0x80

/* HFCD_TEST */
#define HFCD_AUTO_AWAKE		0x01

extern void main_irq_2bds0(struct BCState *bcs);
extern void init2bds0(struct IsdnCardState *cs);
extern void release2bds0(struct IsdnCardState *cs);
extern void hfc2bds0_interrupt(struct IsdnCardState *cs, u_char val);
extern void set_cs_func(struct IsdnCardState *cs);

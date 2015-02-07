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

#ifndef _MC6821_H_
#define _MC6821_H_

/*
 * This file describes the memery mapping of the MC6821 PIA.
 * The unions describe overlayed registers. Which of them is used is
 * determined by bit 2 of the corresponding control register.
 * this files expects the PIA_REG_PADWIDTH to be defined the numeric
 * value of the register spacing.
 *
 * Data came from MFC-31-Developer Kit (from Ralph Seidel,
 * zodiac@darkness.gun.de) and Motorola Data Sheet (from 
 * Richard Hirst, srh@gpt.co.uk)
 *
 * 6.11.95 copyright Joerg Dorchain (dorchain@mpi-sb.mpg.de)
 *
 */

#ifndef PIA_REG_PADWIDTH
#define PIA_REG_PADWIDTH 255
#endif

struct pia {
	union {
		volatile u_char pra;
		volatile u_char ddra;
	} ua;
	u_char pad1[PIA_REG_PADWIDTH];
	volatile u_char cra;
	u_char pad2[PIA_REG_PADWIDTH];
	union {
		volatile u_char prb;
		volatile u_char ddrb;
	} ub;
	u_char pad3[PIA_REG_PADWIDTH];
	volatile u_char crb;
	u_char pad4[PIA_REG_PADWIDTH];
};

#define ppra ua.pra
#define pddra ua.ddra
#define pprb ub.prb
#define pddrb ub.ddrb

#define PIA_C1_ENABLE_IRQ (1<<0)
#define PIA_C1_LOW_TO_HIGH (1<<1)
#define PIA_DDR (1<<2)
#define PIA_IRQ2 (1<<6)
#define PIA_IRQ1 (1<<7)

#endif

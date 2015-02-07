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

#ifndef AU88X0_EQ_H
#define AU88X0_EQ_H

/***************************************************************************
 *            au88x0_eq.h
 *
 *  Definitions and constant data for the Aureal Hardware EQ.
 *
 *  Sun Jun  8 18:23:38 2003
 *  Author: Manuel Jander (mjander@users.sourceforge.net)
 ****************************************************************************/

typedef struct {
	u16 LeftCoefs[50];	//0x4
	u16 RightCoefs[50];	// 0x68
	u16 LeftGains[10];	//0xd0
	u16 RightGains[10];	//0xe4
} auxxEqCoeffSet_t;

typedef struct {
	s32 this04;		/* How many filters for each side (default = 10) */
	s32 this08;		/* inited to cero. Stereo flag? */
} eqhw_t;

typedef struct {
	eqhw_t this04;		/* CHwEq */
	u16 this08;		/* Bad codec flag ? SetBypassGain: bypass gain */
	u16 this0a;
	u16 this0c;		/* SetBypassGain: bypass gain when this28 is not set. */
	u16 this0e;

	s32 this10;		/* How many gains are used for each side (right or left). */
	u16 this14_array[10];	/* SetLeftGainsTarget: Left (and right?) EQ gains  */
	s32 this28;		/* flag related to EQ enabled or not. Gang flag ? */
	s32 this54;		/* SetBypass */
	s32 this58;
	s32 this5c;
	/*0x60 */ auxxEqCoeffSet_t coefset;
	/* 50 u16 word each channel. */
	u16 this130[20];	/* Left and Right gains */
} eqlzr_t;

#endif

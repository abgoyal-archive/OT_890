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

/* z0194a.h Sharp z0194a tuner support
*
* Copyright (C) 2008 Igor M. Liplianin (liplianin@me.by)
*
*	This program is free software; you can redistribute it and/or modify it
*	under the terms of the GNU General Public License as published by the
*	Free Software Foundation, version 2.
*
* see Documentation/dvb/README.dvb-usb for more information
*/

#ifndef Z0194A
#define Z0194A

static int sharp_z0194a_set_symbol_rate(struct dvb_frontend *fe,
					 u32 srate, u32 ratio)
{
	u8 aclk = 0;
	u8 bclk = 0;

	if (srate < 1500000) {
		aclk = 0xb7; bclk = 0x47; }
	else if (srate < 3000000) {
		aclk = 0xb7; bclk = 0x4b; }
	else if (srate < 7000000) {
		aclk = 0xb7; bclk = 0x4f; }
	else if (srate < 14000000) {
		aclk = 0xb7; bclk = 0x53; }
	else if (srate < 30000000) {
		aclk = 0xb6; bclk = 0x53; }
	else if (srate < 45000000) {
		aclk = 0xb4; bclk = 0x51; }

	stv0299_writereg(fe, 0x13, aclk);
	stv0299_writereg(fe, 0x14, bclk);
	stv0299_writereg(fe, 0x1f, (ratio >> 16) & 0xff);
	stv0299_writereg(fe, 0x20, (ratio >> 8) & 0xff);
	stv0299_writereg(fe, 0x21, (ratio) & 0xf0);

	return 0;
}

static u8 sharp_z0194a_inittab[] = {
	0x01, 0x15,
	0x02, 0x00,
	0x03, 0x00,
	0x04, 0x7d,   /* F22FR = 0x7d, F22 = f_VCO / 128 / 0x7d = 22 kHz */
	0x05, 0x35,   /* I2CT = 0, SCLT = 1, SDAT = 1 */
	0x06, 0x40,   /* DAC not used, set to high impendance mode */
	0x07, 0x00,   /* DAC LSB */
	0x08, 0x40,   /* DiSEqC off, LNB power on OP2/LOCK pin on */
	0x09, 0x00,   /* FIFO */
	0x0c, 0x51,   /* OP1 ctl = Normal, OP1 val = 1 (LNB Power ON) */
	0x0d, 0x82,   /* DC offset compensation = ON, beta_agc1 = 2 */
	0x0e, 0x23,   /* alpha_tmg = 2, beta_tmg = 3 */
	0x10, 0x3f,   /* AGC2  0x3d */
	0x11, 0x84,
	0x12, 0xb9,
	0x15, 0xc9,   /* lock detector threshold */
	0x16, 0x00,
	0x17, 0x00,
	0x18, 0x00,
	0x19, 0x00,
	0x1a, 0x00,
	0x1f, 0x50,
	0x20, 0x00,
	0x21, 0x00,
	0x22, 0x00,
	0x23, 0x00,
	0x28, 0x00,  /* out imp: normal  out type: parallel FEC mode:0 */
	0x29, 0x1e,  /* 1/2 threshold */
	0x2a, 0x14,  /* 2/3 threshold */
	0x2b, 0x0f,  /* 3/4 threshold */
	0x2c, 0x09,  /* 5/6 threshold */
	0x2d, 0x05,  /* 7/8 threshold */
	0x2e, 0x01,
	0x31, 0x1f,  /* test all FECs */
	0x32, 0x19,  /* viterbi and synchro search */
	0x33, 0xfc,  /* rs control */
	0x34, 0x93,  /* error control */
	0x0f, 0x52,
	0xff, 0xff
};

#endif

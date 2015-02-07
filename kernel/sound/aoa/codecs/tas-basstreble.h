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

/*
 * This file is only included exactly once!
 *
 * The tables here are derived from the tas3004 datasheet,
 * modulo typo corrections and some smoothing...
 */

#define TAS3004_TREBLE_MIN	0
#define TAS3004_TREBLE_MAX	72
#define TAS3004_BASS_MIN	0
#define TAS3004_BASS_MAX	72
#define TAS3004_TREBLE_ZERO	36
#define TAS3004_BASS_ZERO	36

static u8 tas3004_treble_table[] = {
	150, /* -18 dB */
	149,
	148,
	147,
	146,
	145,
	144,
	143,
	142,
	141,
	140,
	139,
	138,
	137,
	136,
	135,
	134,
	133,
	132,
	131,
	130,
	129,
	128,
	127,
	126,
	125,
	124,
	123,
	122,
	121,
	120,
	119,
	118,
	117,
	116,
	115,
	114, /* 0 dB */
	113,
	112,
	111,
	109,
	108,
	107,
	105,
	104,
	103,
	101,
	99,
	98,
	96,
	93,
	91,
	89,
	86,
	83,
	81,
	77,
	74,
	71,
	67,
	63,
	59,
	54,
	49,
	44,
	38,
	32,
	26,
	19,
	10,
	4,
	2,
	1, /* +18 dB */
};

static inline u8 tas3004_treble(int idx)
{
	return tas3004_treble_table[idx];
}

/* I only save the difference here to the treble table
 * so that the binary is smaller...
 * I have also ignored completely differences of
 * +/- 1
 */
static s8 tas3004_bass_diff_to_treble[] = {
	2, /* 7 dB, offset 50 */
	2,
	2,
	2,
	2,
	1,
	2,
	2,
	2,
	3,
	4,
	4,
	5,
	6,
	7,
	8,
	9,
	10,
	11,
	14,
	13,
	8,
	1, /* 18 dB */
};

static inline u8 tas3004_bass(int idx)
{
	u8 result = tas3004_treble_table[idx];

	if (idx >= 50)
		result += tas3004_bass_diff_to_treble[idx-50];
	return result;
}

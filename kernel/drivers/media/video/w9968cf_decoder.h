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

/***************************************************************************
 * Video decoder for the W996[87]CF driver for Linux.                      *
 *                                                                         *
 * Copyright (C) 2003 2004 by Luca Risolia <luca.risolia@studio.unibo.it>  *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * This program is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with this program; if not, write to the Free Software             *
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.               *
 ***************************************************************************/

#ifndef _W9968CF_DECODER_H_
#define _W9968CF_DECODER_H_

/* Comment/uncomment this for high/low quality of compressed video */
#define W9968CF_DEC_FAST_LOWQUALITY_VIDEO

#ifdef W9968CF_DEC_FAST_LOWQUALITY_VIDEO
static const unsigned char Y_QUANTABLE[64] = {
	16,  11,  10,  16,  24,  40,  51,  61,
	12,  12,  14,  19,  26,  58,  60,  55,
	14,  13,  16,  24,  40,  57,  69,  56,
	14,  17,  22,  29,  51,  87,  80,  62,
	18,  22,  37,  56,  68, 109, 103,  77,
	24,  35,  55,  64,  81, 104, 113,  92,
	49,  64,  78,  87, 103, 121, 120, 101,
	72,  92,  95,  98, 112, 100, 103,  99
};

static const unsigned char UV_QUANTABLE[64] = {
	17,  18,  24,  47,  99,  99,  99,  99,
	18,  21,  26,  66,  99,  99,  99,  99,
	24,  26,  56,  99,  99,  99,  99,  99,
	47,  66,  99,  99,  99,  99,  99,  99,
	99,  99,  99,  99,  99,  99,  99,  99,
	99,  99,  99,  99,  99,  99,  99,  99,
	99,  99,  99,  99,  99,  99,  99,  99,
	99,  99,  99,  99,  99,  99,  99,  99
};
#else
static const unsigned char Y_QUANTABLE[64] = {
	 8,   5,   5,   8,  12,  20,  25,  30,
	 6,   6,   7,   9,  13,  29,  30,  27,
	 7,   6,   8,  12,  20,  28,  34,  28,
	 7,   8,  11,  14,  25,  43,  40,  31,
	 9,  11,  18,  28,  34,  54,  51,  38,
	12,  17,  27,  32,  40,  52,  56,  46,
	24,  32,  39,  43,  51,  60,  60,  50,
	36,  46,  47,  49,  56,  50,  51,  49
};

static const unsigned char UV_QUANTABLE[64] = {
	 8,   9,  12,  23,  49,  49,  49,  49,
	 9,  10,  13,  33,  49,  49,  49,  49,
	12,  13,  28,  49,  49,  49,  49,  49,
	23,  33,  49,  49,  49,  49,  49,  49,
	49,  49,  49,  49,  49,  49,  49,  49,
	49,  49,  49,  49,  49,  49,  49,  49,
	49,  49,  49,  49,  49,  49,  49,  49,
	49,  49,  49,  49,  49,  49,  49,  49
};
#endif

#define W9968CF_DEC_ERR_CORRUPTED_DATA  -1
#define W9968CF_DEC_ERR_BUF_OVERFLOW    -2
#define W9968CF_DEC_ERR_NO_SOI          -3
#define W9968CF_DEC_ERR_NO_SOF0         -4
#define W9968CF_DEC_ERR_NO_SOS          -5
#define W9968CF_DEC_ERR_NO_EOI          -6

extern void w9968cf_init_decoder(void);
extern int w9968cf_check_headers(const unsigned char* Pin,
				 const unsigned long BUF_SIZE);
extern int w9968cf_decode(const char* Pin, const unsigned long BUF_SIZE,
			  const unsigned W, const unsigned H, char* Pout);

#endif /* _W9968CF_DECODER_H_ */

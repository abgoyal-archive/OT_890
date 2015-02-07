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
 *
 * display7seg - Driver interface for the 7-segment display
 * present on Sun Microsystems CP1400 and CP1500
 *
 * Copyright (c) 2000 Eric Brower <ebrower@usa.net>
 *
 */

#ifndef __display7seg_h__
#define __display7seg_h__

#define D7S_IOC	'p'

#define D7SIOCRD _IOR(D7S_IOC, 0x45, int)	/* Read device state	*/
#define D7SIOCWR _IOW(D7S_IOC, 0x46, int)	/* Write device state	*/
#define D7SIOCTM _IO (D7S_IOC, 0x47)		/* Translate mode (FLIP)*/

/*
 * ioctl flag definitions
 *
 * POINT	- Toggle decimal point	(0=absent 1=present)
 * ALARM	- Toggle alarm LED 		(0=green  1=red)
 * FLIP		- Toggle inverted mode 	(0=normal 1=flipped)
 * bits 0-4	- Character displayed	(see definitions below)
 *
 * Display segments are defined as follows,
 * subject to D7S_FLIP register state:
 *
 *    a
 *   ---
 * f|   |b
 *   -g-
 * e|   |c
 *   ---
 *    d
 */

#define D7S_POINT	(1 << 7)	/* Decimal point*/
#define D7S_ALARM	(1 << 6)	/* Alarm LED 	*/
#define D7S_FLIP	(1 << 5)	/* Flip display */

#define D7S_0		0x00		/* Numerals 0-9 */
#define D7S_1		0x01
#define D7S_2		0x02
#define D7S_3		0x03
#define D7S_4		0x04
#define D7S_5		0x05
#define D7S_6		0x06
#define D7S_7		0x07
#define D7S_8		0x08
#define D7S_9		0x09
#define D7S_A		0x0A		/* Letters A-F, H, L, P */
#define D7S_B		0x0B
#define D7S_C		0x0C
#define D7S_D		0x0D
#define D7S_E		0x0E
#define D7S_F		0x0F
#define D7S_H		0x10
#define D7S_E2		0x11
#define D7S_L		0x12
#define D7S_P		0x13
#define D7S_SEGA	0x14		/* Individual segments */
#define D7S_SEGB	0x15
#define D7S_SEGC	0x16
#define D7S_SEGD	0x17
#define D7S_SEGE	0x18
#define D7S_SEGF	0x19
#define D7S_SEGG	0x1A
#define D7S_SEGABFG 0x1B		/* Segment groupings */
#define D7S_SEGCDEG	0x1C
#define D7S_SEGBCEF 0x1D
#define D7S_SEGADG	0x1E
#define D7S_BLANK	0x1F		/* Clear all segments */

#define D7S_MIN_VAL	0x0
#define D7S_MAX_VAL	0x1F

#endif /* ifndef __display7seg_h__ */

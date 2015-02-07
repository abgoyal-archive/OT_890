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

/* cs8420.h - cs8420 initializations
   Copyright (C) 1999 Nathan Laredo (laredo@gnu.org)

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

 */
#ifndef __CS8420_H__
#define __CS8420_H__

/* Initialization Sequence */

static __u8 init8420[] = {
	1, 0x01,	2, 0x02,	3, 0x00,	4, 0x46,
	5, 0x24,	6, 0x84,	18, 0x18,	19, 0x13,
};

#define INIT8420LEN	(sizeof(init8420)/2)

static __u8 mode8420pro[] = {	/* professional output mode */
	32, 0xa1,	33, 0x00,	34, 0x00,	35, 0x00,
	36, 0x00,	37, 0x00,	38, 0x00,	39, 0x00,
	40, 0x00,	41, 0x00,	42, 0x00,	43, 0x00,
	44, 0x00,	45, 0x00,	46, 0x00,	47, 0x00,
	48, 0x00,	49, 0x00,	50, 0x00,	51, 0x00,
	52, 0x00,	53, 0x00,	54, 0x00,	55, 0x00,
};
#define MODE8420LEN	(sizeof(mode8420pro)/2)

static __u8 mode8420con[] = {	/* consumer output mode */
	32, 0x20,	33, 0x00,	34, 0x00,	35, 0x48,
	36, 0x00,	37, 0x00,	38, 0x00,	39, 0x00,
	40, 0x00,	41, 0x00,	42, 0x00,	43, 0x00,
	44, 0x00,	45, 0x00,	46, 0x00,	47, 0x00,
	48, 0x00,	49, 0x00,	50, 0x00,	51, 0x00,
	52, 0x00,	53, 0x00,	54, 0x00,	55, 0x00,
};

#endif

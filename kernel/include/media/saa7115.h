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
    saa7115.h - definition for saa7111/3/4/5 inputs and frequency flags

    Copyright (C) 2006 Hans Verkuil (hverkuil@xs4all.nl)

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

#ifndef _SAA7115_H_
#define _SAA7115_H_

/* SAA7111/3/4/5 HW inputs */
#define SAA7115_COMPOSITE0 0
#define SAA7115_COMPOSITE1 1
#define SAA7115_COMPOSITE2 2
#define SAA7115_COMPOSITE3 3
#define SAA7115_COMPOSITE4 4 /* not available for the saa7111/3 */
#define SAA7115_COMPOSITE5 5 /* not available for the saa7111/3 */
#define SAA7115_SVIDEO0    6
#define SAA7115_SVIDEO1    7
#define SAA7115_SVIDEO2    8
#define SAA7115_SVIDEO3    9

/* SAA7115 v4l2_crystal_freq frequency values */
#define SAA7115_FREQ_32_11_MHZ  32110000   /* 32.11 MHz crystal, SAA7114/5 only */
#define SAA7115_FREQ_24_576_MHZ 24576000   /* 24.576 MHz crystal */

/* SAA7115 v4l2_crystal_freq audio clock control flags */
#define SAA7115_FREQ_FL_UCGC   (1 << 0)	   /* SA 3A[7], UCGC, SAA7115 only */
#define SAA7115_FREQ_FL_CGCDIV (1 << 1)	   /* SA 3A[6], CGCDIV, SAA7115 only */
#define SAA7115_FREQ_FL_APLL   (1 << 2)	   /* SA 3A[3], APLL, SAA7114/5 only */

#define SAA7115_IPORT_ON    	1
#define SAA7115_IPORT_OFF   	0

/* SAA7111 specific output flags */
#define SAA7111_VBI_BYPASS 	2
#define SAA7111_FMT_YUV422      0x00
#define SAA7111_FMT_RGB 	0x40
#define SAA7111_FMT_CCIR 	0x80
#define SAA7111_FMT_YUV411 	0xc0

#endif


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
 *
 *  Copyright (C) 2005 Mike Isely <isely@pobox.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#ifndef __PVRUSB2_STD_H
#define __PVRUSB2_STD_H

#include <linux/videodev2.h>

// Convert string describing one or more video standards into a mask of V4L
// standard bits.  Return true if conversion succeeds otherwise return
// false.  String is expected to be of the form: C1-x/y;C2-a/b where C1 and
// C2 are color system names (e.g. "PAL", "NTSC") and x, y, a, and b are
// modulation schemes (e.g. "M", "B", "G", etc).
int pvr2_std_str_to_id(v4l2_std_id *idPtr,const char *bufPtr,
		       unsigned int bufSize);

// Convert any arbitrary set of video standard bits into an unambiguous
// readable string.  Return value is the number of bytes consumed in the
// buffer.  The formatted string is of a form that can be parsed by our
// sibling std_std_to_id() function.
unsigned int pvr2_std_id_to_str(char *bufPtr, unsigned int bufSize,
				v4l2_std_id id);

// Create an array of suitable v4l2_standard structures given a bit mask of
// video standards to support.  The array is allocated from the heap, and
// the number of elements is returned in the first argument.
struct v4l2_standard *pvr2_std_create_enum(unsigned int *countptr,
					   v4l2_std_id id);

// Return mask of which video standard bits are valid
v4l2_std_id pvr2_std_get_usable(void);

#endif /* __PVRUSB2_STD_H */

/*
  Stuff for Emacs to see, in order to encourage consistent editing style:
  *** Local Variables: ***
  *** mode: c ***
  *** fill-column: 75 ***
  *** tab-width: 8 ***
  *** c-basic-offset: 8 ***
  *** End: ***
  */

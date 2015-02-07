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

#ifndef _VIDEO_ATAFB_H
#define _VIDEO_ATAFB_H

void atafb_mfb_copyarea(struct fb_info *info, u_long next_line, int sy, int sx, int dy,
			int dx, int height, int width);
void atafb_mfb_fillrect(struct fb_info *info, u_long next_line, u32 color,
			int sy, int sx, int height, int width);
void atafb_mfb_linefill(struct fb_info *info, u_long next_line,
			int dy, int dx, u32 width,
			const u8 *data, u32 bgcolor, u32 fgcolor);

void atafb_iplan2p2_copyarea(struct fb_info *info, u_long next_line, int sy, int sx, int dy,
			     int dx, int height, int width);
void atafb_iplan2p2_fillrect(struct fb_info *info, u_long next_line, u32 color,
			     int sy, int sx, int height, int width);
void atafb_iplan2p2_linefill(struct fb_info *info, u_long next_line,
			     int dy, int dx, u32 width,
			     const u8 *data, u32 bgcolor, u32 fgcolor);

void atafb_iplan2p4_copyarea(struct fb_info *info, u_long next_line, int sy, int sx, int dy,
			     int dx, int height, int width);
void atafb_iplan2p4_fillrect(struct fb_info *info, u_long next_line, u32 color,
			     int sy, int sx, int height, int width);
void atafb_iplan2p4_linefill(struct fb_info *info, u_long next_line,
			     int dy, int dx, u32 width,
			     const u8 *data, u32 bgcolor, u32 fgcolor);

void atafb_iplan2p8_copyarea(struct fb_info *info, u_long next_line, int sy, int sx, int dy,
			     int dx, int height, int width);
void atafb_iplan2p8_fillrect(struct fb_info *info, u_long next_line, u32 color,
			     int sy, int sx, int height, int width);
void atafb_iplan2p8_linefill(struct fb_info *info, u_long next_line,
			     int dy, int dx, u32 width,
			     const u8 *data, u32 bgcolor, u32 fgcolor);

#endif /* _VIDEO_ATAFB_H */

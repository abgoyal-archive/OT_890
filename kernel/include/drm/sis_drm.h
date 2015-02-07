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

/* sis_drv.h -- Private header for sis driver -*- linux-c -*- */
/*
 * Copyright 2005 Eric Anholt
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef __SIS_DRM_H__
#define __SIS_DRM_H__

/* SiS specific ioctls */
#define NOT_USED_0_3
#define DRM_SIS_FB_ALLOC	0x04
#define DRM_SIS_FB_FREE	        0x05
#define NOT_USED_6_12
#define DRM_SIS_AGP_INIT	0x13
#define DRM_SIS_AGP_ALLOC	0x14
#define DRM_SIS_AGP_FREE	0x15
#define DRM_SIS_FB_INIT	        0x16

#define DRM_IOCTL_SIS_FB_ALLOC		DRM_IOWR(DRM_COMMAND_BASE + DRM_SIS_FB_ALLOC, drm_sis_mem_t)
#define DRM_IOCTL_SIS_FB_FREE		DRM_IOW( DRM_COMMAND_BASE + DRM_SIS_FB_FREE, drm_sis_mem_t)
#define DRM_IOCTL_SIS_AGP_INIT		DRM_IOWR(DRM_COMMAND_BASE + DRM_SIS_AGP_INIT, drm_sis_agp_t)
#define DRM_IOCTL_SIS_AGP_ALLOC		DRM_IOWR(DRM_COMMAND_BASE + DRM_SIS_AGP_ALLOC, drm_sis_mem_t)
#define DRM_IOCTL_SIS_AGP_FREE		DRM_IOW( DRM_COMMAND_BASE + DRM_SIS_AGP_FREE, drm_sis_mem_t)
#define DRM_IOCTL_SIS_FB_INIT		DRM_IOW( DRM_COMMAND_BASE + DRM_SIS_FB_INIT, drm_sis_fb_t)
/*
#define DRM_IOCTL_SIS_FLIP		DRM_IOW( 0x48, drm_sis_flip_t)
#define DRM_IOCTL_SIS_FLIP_INIT		DRM_IO(  0x49)
#define DRM_IOCTL_SIS_FLIP_FINAL	DRM_IO(  0x50)
*/

typedef struct {
	int context;
	unsigned int offset;
	unsigned int size;
	unsigned long free;
} drm_sis_mem_t;

typedef struct {
	unsigned int offset, size;
} drm_sis_agp_t;

typedef struct {
	unsigned int offset, size;
} drm_sis_fb_t;

#endif				/* __SIS_DRM_H__ */

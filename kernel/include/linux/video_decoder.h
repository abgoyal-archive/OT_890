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

#ifndef _LINUX_VIDEO_DECODER_H
#define _LINUX_VIDEO_DECODER_H

#include <linux/types.h>

#define HAVE_VIDEO_DECODER 1

struct video_decoder_capability { /* this name is too long */
	__u32	flags;
#define	VIDEO_DECODER_PAL	1	/* can decode PAL signal */
#define	VIDEO_DECODER_NTSC	2	/* can decode NTSC */
#define	VIDEO_DECODER_SECAM	4	/* can decode SECAM */
#define	VIDEO_DECODER_AUTO	8	/* can autosense norm */
#define	VIDEO_DECODER_CCIR	16	/* CCIR-601 pixel rate (720 pixels per line) instead of square pixel rate */
	int	inputs;			/* number of inputs */
	int	outputs;		/* number of outputs */
};

/*
DECODER_GET_STATUS returns the following flags.  The only one you need is
DECODER_STATUS_GOOD, the others are just nice things to know.
*/
#define	DECODER_STATUS_GOOD	1	/* receiving acceptable input */
#define	DECODER_STATUS_COLOR	2	/* receiving color information */
#define	DECODER_STATUS_PAL	4	/* auto detected */
#define	DECODER_STATUS_NTSC	8	/* auto detected */
#define	DECODER_STATUS_SECAM	16	/* auto detected */

struct video_decoder_init {
	unsigned char len;
	const unsigned char *data;
};

#define	DECODER_GET_CAPABILITIES _IOR('d', 1, struct video_decoder_capability)
#define	DECODER_GET_STATUS    	_IOR('d', 2, int)
#define	DECODER_SET_NORM	_IOW('d', 3, int)
#define	DECODER_SET_INPUT	_IOW('d', 4, int)	/* 0 <= input < #inputs */
#define	DECODER_SET_OUTPUT	_IOW('d', 5, int)	/* 0 <= output < #outputs */
#define	DECODER_ENABLE_OUTPUT	_IOW('d', 6, int)	/* boolean output enable control */
#define	DECODER_SET_PICTURE   	_IOW('d', 7, struct video_picture)
#define	DECODER_SET_GPIO	_IOW('d', 8, int)	/* switch general purpose pin */
#define	DECODER_INIT		_IOW('d', 9, struct video_decoder_init)	/* init internal registers at once */
#define	DECODER_SET_VBI_BYPASS	_IOW('d', 10, int)	/* switch vbi bypass */

#define	DECODER_DUMP		_IO('d', 192)		/* debug hook */


#endif

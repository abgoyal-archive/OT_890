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
 * Decompression convenience functions
 *
 * Copyright 2007 David Gibson, IBM Corporation.
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
#ifndef _PPC_BOOT_GUNZIP_UTIL_H_
#define _PPC_BOOT_GUNZIP_UTIL_H_

#include "zlib.h"

/*
 * These functions are designed to make life easy for decompressing
 * kernel images, initrd images or any other gzip compressed image,
 * particularly if its useful to decompress part of the image (e.g. to
 * examine headers) before decompressing the remainder.
 *
 * To use:
 *     - declare a gunzip_state structure
 *     - use gunzip_start() to initialize the state, associating it
 *       with a stream of compressed data
 *     - use gunzip_partial(), gunzip_exactly() and gunzip_discard()
 *       in any combination to extract pieces of data from the stream
 *     - Finally use gunzip_finish() to extract the tail of the
 *       compressed stream and wind up zlib
 */

/* scratch space for gunzip; 46912 is from zlib_inflate_workspacesize() */
#define GUNZIP_SCRATCH_SIZE	46912

struct gunzip_state {
	z_stream s;
	char scratch[46912];
};

void gunzip_start(struct gunzip_state *state, void *src, int srclen);
int gunzip_partial(struct gunzip_state *state, void *dst, int dstlen);
void gunzip_exactly(struct gunzip_state *state, void *dst, int len);
void gunzip_discard(struct gunzip_state *state, int len);
int gunzip_finish(struct gunzip_state *state, void *dst, int len);

#endif /* _PPC_BOOT_GUNZIP_UTIL_H_ */

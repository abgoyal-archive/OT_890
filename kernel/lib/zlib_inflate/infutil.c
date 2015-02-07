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

#include <linux/zutil.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

/* Utility function: initialize zlib, unpack binary blob, clean up zlib,
 * return len or negative error code.
 */
int zlib_inflate_blob(void *gunzip_buf, unsigned int sz,
		      const void *buf, unsigned int len)
{
	const u8 *zbuf = buf;
	struct z_stream_s *strm;
	int rc;

	rc = -ENOMEM;
	strm = kmalloc(sizeof(*strm), GFP_KERNEL);
	if (strm == NULL)
		goto gunzip_nomem1;
	strm->workspace = kmalloc(zlib_inflate_workspacesize(), GFP_KERNEL);
	if (strm->workspace == NULL)
		goto gunzip_nomem2;

	/* gzip header (1f,8b,08... 10 bytes total + possible asciz filename)
	 * expected to be stripped from input
	 */
	strm->next_in = zbuf;
	strm->avail_in = len;
	strm->next_out = gunzip_buf;
	strm->avail_out = sz;

	rc = zlib_inflateInit2(strm, -MAX_WBITS);
	if (rc == Z_OK) {
		rc = zlib_inflate(strm, Z_FINISH);
		/* after Z_FINISH, only Z_STREAM_END is "we unpacked it all" */
		if (rc == Z_STREAM_END)
			rc = sz - strm->avail_out;
		else
			rc = -EINVAL;
		zlib_inflateEnd(strm);
	} else
		rc = -EINVAL;

	kfree(strm->workspace);
gunzip_nomem2:
	kfree(strm);
gunzip_nomem1:
	return rc; /* returns Z_OK (0) if successful */
}

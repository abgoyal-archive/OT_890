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

#ifndef _XT_HASHLIMIT_H
#define _XT_HASHLIMIT_H

/* timings are in milliseconds. */
#define XT_HASHLIMIT_SCALE 10000
/* 1/10,000 sec period => max of 10,000/sec.  Min rate is then 429490
   seconds, or one every 59 hours. */

/* details of this structure hidden by the implementation */
struct xt_hashlimit_htable;

enum {
	XT_HASHLIMIT_HASH_DIP = 1 << 0,
	XT_HASHLIMIT_HASH_DPT = 1 << 1,
	XT_HASHLIMIT_HASH_SIP = 1 << 2,
	XT_HASHLIMIT_HASH_SPT = 1 << 3,
	XT_HASHLIMIT_INVERT   = 1 << 4,
};

struct hashlimit_cfg {
	u_int32_t mode;	  /* bitmask of XT_HASHLIMIT_HASH_* */
	u_int32_t avg;    /* Average secs between packets * scale */
	u_int32_t burst;  /* Period multiplier for upper limit. */

	/* user specified */
	u_int32_t size;		/* how many buckets */
	u_int32_t max;		/* max number of entries */
	u_int32_t gc_interval;	/* gc interval */
	u_int32_t expire;	/* when do entries expire? */
};

struct xt_hashlimit_info {
	char name [IFNAMSIZ];		/* name */
	struct hashlimit_cfg cfg;

	/* Used internally by the kernel */
	struct xt_hashlimit_htable *hinfo;
	union {
		void *ptr;
		struct xt_hashlimit_info *master;
	} u;
};

struct hashlimit_cfg1 {
	u_int32_t mode;	  /* bitmask of XT_HASHLIMIT_HASH_* */
	u_int32_t avg;    /* Average secs between packets * scale */
	u_int32_t burst;  /* Period multiplier for upper limit. */

	/* user specified */
	u_int32_t size;		/* how many buckets */
	u_int32_t max;		/* max number of entries */
	u_int32_t gc_interval;	/* gc interval */
	u_int32_t expire;	/* when do entries expire? */

	u_int8_t srcmask, dstmask;
};

struct xt_hashlimit_mtinfo1 {
	char name[IFNAMSIZ];
	struct hashlimit_cfg1 cfg;

	/* Used internally by the kernel */
	struct xt_hashlimit_htable *hinfo __attribute__((aligned(8)));
};

#endif /*_XT_HASHLIMIT_H*/

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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#if !defined(__AEE_IPANIC_H__)
#define __AEE_IPANIC_H__

#include <linux/autoconf.h>
#include <linux/kallsyms.h>


#define IPANIC_OOPS_BLOCK_COUNT 64

#define AEE_IPANIC_MAGIC 0xaee0dead
#define AEE_IPANIC_PHDR_VERSION   0x02
#define AEE_IPANIC_DATALENGTH_MAX (64 * 1024)

struct ipanic_header {
	u32 magic;

	u32 version;

	u32 console_offset;
	u32 console_length;

	u32 oops_header_offset;
	u32 oops_header_length;

	u32 oops_detail_offset;
	u32 oops_detail_length;
};

#define PANICLOG_BACKTRACE_NUM 4
#define PANICLOG_SOURCE_LENGTH 16

#define IPANIC_OOPS_HEADER_BACKTRACE_LENGTH 3072
#define IPANIC_OOPS_HEADER_PROCESS_NAME_LENGTH 256

struct ipanic_oops_header 
{
	char backtrace[IPANIC_OOPS_HEADER_BACKTRACE_LENGTH];
	char process_path[IPANIC_OOPS_HEADER_PROCESS_NAME_LENGTH];
};

struct ipanic_oops
{
	struct ipanic_oops_header header;

	char *detail;
	int detail_len;

	char *console;
	int console_len;
};

struct ipanic_data {
	struct mtd_info		*mtd;
	struct ipanic_header	curr;
	void			*bounce;
	u32 blk_offset[IPANIC_OOPS_BLOCK_COUNT];

	struct proc_dir_entry	*oops;
};

struct ipanic_oops *ipanic_oops_copy(void);

void ipanic_oops_free(struct ipanic_oops *oops, int erase);

#endif

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

#ifndef __COW_H__
#define __COW_H__

#include <asm/types.h>

#if defined(__KERNEL__)

# include <asm/byteorder.h>

# if defined(__BIG_ENDIAN)
#	define ntohll(x) (x)
#	define htonll(x) (x)
# elif defined(__LITTLE_ENDIAN)
#	define ntohll(x)  be64_to_cpu(x)
#	define htonll(x)  cpu_to_be64(x)
# else
#	error "Could not determine byte order"
# endif

#else
/* For the definition of ntohl, htonl and __BYTE_ORDER */
#include <endian.h>
#include <netinet/in.h>
#if defined(__BYTE_ORDER)

#  if __BYTE_ORDER == __BIG_ENDIAN
#	define ntohll(x) (x)
#	define htonll(x) (x)
#  elif __BYTE_ORDER == __LITTLE_ENDIAN
#	define ntohll(x)  bswap_64(x)
#	define htonll(x)  bswap_64(x)
#  else
#	error "Could not determine byte order: __BYTE_ORDER uncorrectly defined"
#  endif

#else  /* ! defined(__BYTE_ORDER) */
#	error "Could not determine byte order: __BYTE_ORDER not defined"
#endif
#endif /* ! defined(__KERNEL__) */

extern int init_cow_file(int fd, char *cow_file, char *backing_file,
			 int sectorsize, int alignment, int *bitmap_offset_out,
			 unsigned long *bitmap_len_out, int *data_offset_out);

extern int file_reader(__u64 offset, char *buf, int len, void *arg);
extern int read_cow_header(int (*reader)(__u64, char *, int, void *),
			   void *arg, __u32 *version_out,
			   char **backing_file_out, time_t *mtime_out,
			   unsigned long long *size_out, int *sectorsize_out,
			   __u32 *align_out, int *bitmap_offset_out);

extern int write_cow_header(char *cow_file, int fd, char *backing_file,
			    int sectorsize, int alignment,
			    unsigned long long *size);

extern void cow_sizes(int version, __u64 size, int sectorsize, int align,
		      int bitmap_offset, unsigned long *bitmap_len_out,
		      int *data_offset_out);

#endif

/*
 * ---------------------------------------------------------------------------
 * Local variables:
 * c-file-style: "linux"
 * End:
 */

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

#ifndef __LINUX_ROMFS_FS_H
#define __LINUX_ROMFS_FS_H

/* The basic structures of the romfs filesystem */

#define ROMBSIZE BLOCK_SIZE
#define ROMBSBITS BLOCK_SIZE_BITS
#define ROMBMASK (ROMBSIZE-1)
#define ROMFS_MAGIC 0x7275

#define ROMFS_MAXFN 128

#define __mkw(h,l) (((h)&0x00ff)<< 8|((l)&0x00ff))
#define __mkl(h,l) (((h)&0xffff)<<16|((l)&0xffff))
#define __mk4(a,b,c,d) cpu_to_be32(__mkl(__mkw(a,b),__mkw(c,d)))
#define ROMSB_WORD0 __mk4('-','r','o','m')
#define ROMSB_WORD1 __mk4('1','f','s','-')

/* On-disk "super block" */

struct romfs_super_block {
	__be32 word0;
	__be32 word1;
	__be32 size;
	__be32 checksum;
	char name[0];		/* volume name */
};

/* On disk inode */

struct romfs_inode {
	__be32 next;		/* low 4 bits see ROMFH_ */
	__be32 spec;
	__be32 size;
	__be32 checksum;
	char name[0];
};

#define ROMFH_TYPE 7
#define ROMFH_HRD 0
#define ROMFH_DIR 1
#define ROMFH_REG 2
#define ROMFH_SYM 3
#define ROMFH_BLK 4
#define ROMFH_CHR 5
#define ROMFH_SCK 6
#define ROMFH_FIF 7
#define ROMFH_EXEC 8

/* Alignment */

#define ROMFH_SIZE 16
#define ROMFH_PAD (ROMFH_SIZE-1)
#define ROMFH_MASK (~ROMFH_PAD)

#ifdef __KERNEL__

/* Not much now */

#endif /* __KERNEL__ */
#endif

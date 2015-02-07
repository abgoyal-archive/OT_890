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

#ifndef __ASM_AXISFLASHMAP_H
#define __ASM_AXISFLASHMAP_H

/* Bootblock parameters are stored at 0xc000 and has the FLASH_BOOT_MAGIC 
 * as start, it ends with 0xFFFFFFFF */
#define FLASH_BOOT_MAGIC 0xbeefcace
#define BOOTPARAM_OFFSET 0xc000
/* apps/bootblocktool is used to read and write the parameters,
 * and it has nothing to do with the partition table. 
 */

#define PARTITION_TABLE_OFFSET 10
#define PARTITION_TABLE_MAGIC 0xbeef	/* Not a good magic */

/* The partitiontable_head is located at offset +10: */
struct partitiontable_head {
	__u16 magic;	/* PARTITION_TABLE_MAGIC */
	__u16 size;	/* Length of ptable block (entries + end marker) */
	__u32 checksum;	/* simple longword sum, over entries + end marker  */
};

/* And followed by partition table entries */
struct partitiontable_entry {
	__u32 offset;		/* relative to the sector the ptable is in */
	__u32 size;		/* in bytes */
	__u32 checksum;		/* simple longword sum */
	__u16 type;		/* see type codes below */
	__u16 flags;		/* bit 0: ro/rw = 1/0 */
	__u32 future0;		/* 16 bytes reserved for future use */
	__u32 future1;
	__u32 future2;
	__u32 future3;
};
/* ended by an end marker: */
#define PARTITIONTABLE_END_MARKER 0xFFFFFFFF
#define PARTITIONTABLE_END_MARKER_SIZE 4

#define PARTITIONTABLE_END_PAD	10

/* Complete structure for whole partition table */
/* note that table may end before CONFIG_ETRAX_PTABLE_ENTRIES by setting
 * offset of the last entry + 1 to PARTITIONTABLE_END_MARKER.
 */
struct partitiontable {
	__u8 skip[PARTITION_TABLE_OFFSET];
	struct partitiontable_head head;
	struct partitiontable_entry entries[];
};

#define PARTITION_TYPE_PARAM  0x0001
#define PARTITION_TYPE_KERNEL 0x0002
#define PARTITION_TYPE_JFFS   0x0003
#define PARTITION_TYPE_JFFS2  0x0000

#define	PARTITION_FLAGS_READONLY_MASK	0x0001
#define	PARTITION_FLAGS_READONLY	0x0001

/* The master mtd for the entire flash. */
extern struct mtd_info *axisflash_mtd;

#endif

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

#ifndef	AFFS_HARDBLOCKS_H
#define	AFFS_HARDBLOCKS_H

#include <linux/types.h>

/* Just the needed definitions for the RDB of an Amiga HD. */

struct RigidDiskBlock {
	__u32	rdb_ID;
	__be32	rdb_SummedLongs;
	__s32	rdb_ChkSum;
	__u32	rdb_HostID;
	__be32	rdb_BlockBytes;
	__u32	rdb_Flags;
	__u32	rdb_BadBlockList;
	__be32	rdb_PartitionList;
	__u32	rdb_FileSysHeaderList;
	__u32	rdb_DriveInit;
	__u32	rdb_Reserved1[6];
	__u32	rdb_Cylinders;
	__u32	rdb_Sectors;
	__u32	rdb_Heads;
	__u32	rdb_Interleave;
	__u32	rdb_Park;
	__u32	rdb_Reserved2[3];
	__u32	rdb_WritePreComp;
	__u32	rdb_ReducedWrite;
	__u32	rdb_StepRate;
	__u32	rdb_Reserved3[5];
	__u32	rdb_RDBBlocksLo;
	__u32	rdb_RDBBlocksHi;
	__u32	rdb_LoCylinder;
	__u32	rdb_HiCylinder;
	__u32	rdb_CylBlocks;
	__u32	rdb_AutoParkSeconds;
	__u32	rdb_HighRDSKBlock;
	__u32	rdb_Reserved4;
	char	rdb_DiskVendor[8];
	char	rdb_DiskProduct[16];
	char	rdb_DiskRevision[4];
	char	rdb_ControllerVendor[8];
	char	rdb_ControllerProduct[16];
	char	rdb_ControllerRevision[4];
	__u32	rdb_Reserved5[10];
};

#define	IDNAME_RIGIDDISK	0x5244534B	/* "RDSK" */

struct PartitionBlock {
	__be32	pb_ID;
	__be32	pb_SummedLongs;
	__s32	pb_ChkSum;
	__u32	pb_HostID;
	__be32	pb_Next;
	__u32	pb_Flags;
	__u32	pb_Reserved1[2];
	__u32	pb_DevFlags;
	__u8	pb_DriveName[32];
	__u32	pb_Reserved2[15];
	__be32	pb_Environment[17];
	__u32	pb_EReserved[15];
};

#define	IDNAME_PARTITION	0x50415254	/* "PART" */

#define RDB_ALLOCATION_LIMIT	16

#endif	/* AFFS_HARDBLOCKS_H */

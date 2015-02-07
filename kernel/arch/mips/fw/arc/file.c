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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * ARC firmware interface.
 *
 * Copyright (C) 1994, 1995, 1996, 1999 Ralf Baechle
 * Copyright (C) 1999 Silicon Graphics, Inc.
 */
#include <linux/init.h>

#include <asm/fw/arc/types.h>
#include <asm/sgialib.h>

LONG
ArcGetDirectoryEntry(ULONG FileID, struct linux_vdirent *Buffer,
                     ULONG N, ULONG *Count)
{
	return ARC_CALL4(get_vdirent, FileID, Buffer, N, Count);
}

LONG
ArcOpen(CHAR *Path, enum linux_omode OpenMode, ULONG *FileID)
{
	return ARC_CALL3(open, Path, OpenMode, FileID);
}

LONG
ArcClose(ULONG FileID)
{
	return ARC_CALL1(close, FileID);
}

LONG
ArcRead(ULONG FileID, VOID *Buffer, ULONG N, ULONG *Count)
{
	return ARC_CALL4(read, FileID, Buffer, N, Count);
}

LONG
ArcGetReadStatus(ULONG FileID)
{
	return ARC_CALL1(get_rstatus, FileID);
}

LONG
ArcWrite(ULONG FileID, PVOID Buffer, ULONG N, PULONG Count)
{
	return ARC_CALL4(write, FileID, Buffer, N, Count);
}

LONG
ArcSeek(ULONG FileID, struct linux_bigint *Position, enum linux_seekmode SeekMode)
{
	return ARC_CALL3(seek, FileID, Position, SeekMode);
}

LONG
ArcMount(char *name, enum linux_mountops op)
{
	return ARC_CALL2(mount, name, op);
}

LONG
ArcGetFileInformation(ULONG FileID, struct linux_finfo *Information)
{
	return ARC_CALL2(get_finfo, FileID, Information);
}

LONG ArcSetFileInformation(ULONG FileID, ULONG AttributeFlags,
                           ULONG AttributeMask)
{
	return ARC_CALL3(set_finfo, FileID, AttributeFlags, AttributeMask);
}

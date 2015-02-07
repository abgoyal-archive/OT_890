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

#ifndef __IDE_FLOPPY_H
#define __IDE_FLOPPY_H

#include "ide-gd.h"

#ifdef CONFIG_IDE_GD_ATAPI
/*
 * Pages of the SELECT SENSE / MODE SENSE packet commands.
 * See SFF-8070i spec.
 */
#define	IDEFLOPPY_CAPABILITIES_PAGE	0x1b
#define IDEFLOPPY_FLEXIBLE_DISK_PAGE	0x05

/* IOCTLs used in low-level formatting. */
#define	IDEFLOPPY_IOCTL_FORMAT_SUPPORTED	0x4600
#define	IDEFLOPPY_IOCTL_FORMAT_GET_CAPACITY	0x4601
#define	IDEFLOPPY_IOCTL_FORMAT_START		0x4602
#define IDEFLOPPY_IOCTL_FORMAT_GET_PROGRESS	0x4603

/* ide-floppy.c */
extern const struct ide_disk_ops ide_atapi_disk_ops;
void ide_floppy_create_mode_sense_cmd(struct ide_atapi_pc *, u8);
void ide_floppy_create_read_capacity_cmd(struct ide_atapi_pc *);

/* ide-floppy_ioctl.c */
int ide_floppy_ioctl(ide_drive_t *, struct block_device *, fmode_t,
		     unsigned int, unsigned long);

#ifdef CONFIG_IDE_PROC_FS
/* ide-floppy_proc.c */
extern ide_proc_entry_t ide_floppy_proc[];
extern const struct ide_proc_devset ide_floppy_settings[];
#endif
#else
#define ide_floppy_proc		NULL
#define ide_floppy_settings	NULL
#endif

#endif /*__IDE_FLOPPY_H */

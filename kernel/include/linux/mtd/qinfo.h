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

#ifndef __LINUX_MTD_QINFO_H
#define __LINUX_MTD_QINFO_H

#include <linux/mtd/map.h>
#include <linux/wait.h>
#include <linux/spinlock.h>
#include <linux/delay.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/flashchip.h>
#include <linux/mtd/partitions.h>

/* lpddr_private describes lpddr flash chip in memory map
 * @ManufactId - Chip Manufacture ID
 * @DevId - Chip Device ID
 * @qinfo - pointer to qinfo records describing the chip
 * @numchips - number of chips including virual RWW partitions
 * @chipshift - Chip/partiton size 2^chipshift
 * @chips - per-chip data structure
 */
struct lpddr_private {
	uint16_t ManufactId;
	uint16_t DevId;
	struct qinfo_chip *qinfo;
	int numchips;
	unsigned long chipshift;
	struct flchip chips[0];
};

/* qinfo_query_info structure contains request information for
 * each qinfo record
 * @major - major number of qinfo record
 * @major - minor number of qinfo record
 * @id_str - descriptive string to access the record
 * @desc - detailed description for the qinfo record
 */
struct qinfo_query_info {
	uint8_t	major;
	uint8_t	minor;
	char *id_str;
	char *desc;
};

/*
 * qinfo_chip structure contains necessary qinfo records data
 * @DevSizeShift - Device size 2^n bytes
 * @BufSizeShift - Program buffer size 2^n bytes
 * @TotalBlocksNum - Total number of blocks
 * @UniformBlockSizeShift - Uniform block size 2^UniformBlockSizeShift bytes
 * @HWPartsNum - Number of hardware partitions
 * @SuspEraseSupp - Suspend erase supported
 * @SingleWordProgTime - Single word program 2^SingleWordProgTime u-sec
 * @ProgBufferTime - Program buffer write 2^ProgBufferTime u-sec
 * @BlockEraseTime - Block erase 2^BlockEraseTime m-sec
 */
struct qinfo_chip {
	/* General device info */
	uint16_t DevSizeShift;
	uint16_t BufSizeShift;
	/* Erase block information */
	uint16_t TotalBlocksNum;
	uint16_t UniformBlockSizeShift;
	/* Partition information */
	uint16_t HWPartsNum;
	/* Optional features */
	uint16_t SuspEraseSupp;
	/* Operation typical time */
	uint16_t SingleWordProgTime;
	uint16_t ProgBufferTime;
	uint16_t BlockEraseTime;
};

/* defines for fixup usage */
#define LPDDR_MFR_ANY		0xffff
#define LPDDR_ID_ANY		0xffff
#define NUMONYX_MFGR_ID		0x0089
#define R18_DEVICE_ID_1G	0x893c

static inline map_word lpddr_build_cmd(u_long cmd, struct map_info *map)
{
	map_word val = { {0} };
	val.x[0] = cmd;
	return val;
}

#define CMD(x) lpddr_build_cmd(x, map)
#define CMDVAL(cmd) cmd.x[0]

struct mtd_info *lpddr_cmdset(struct map_info *);

#endif


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
 *	inftl.h -- defines to support the Inverse NAND Flash Translation Layer
 *
 *	(C) Copyright 2002, Greg Ungerer (gerg@snapgear.com)
 */

#ifndef __MTD_INFTL_H__
#define __MTD_INFTL_H__

#ifndef __KERNEL__
#error This is a kernel header. Perhaps include nftl-user.h instead?
#endif

#include <linux/mtd/blktrans.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nftl.h>

#include <mtd/inftl-user.h>

#ifndef INFTL_MAJOR
#define INFTL_MAJOR 96
#endif
#define INFTL_PARTN_BITS 4

#ifdef __KERNEL__

struct INFTLrecord {
	struct mtd_blktrans_dev mbd;
	__u16 MediaUnit;
	__u32 EraseSize;
	struct INFTLMediaHeader MediaHdr;
	int usecount;
	unsigned char heads;
	unsigned char sectors;
	unsigned short cylinders;
	__u16 numvunits;
	__u16 firstEUN;
	__u16 lastEUN;
	__u16 numfreeEUNs;
	__u16 LastFreeEUN; 		/* To speed up finding a free EUN */
	int head,sect,cyl;
	__u16 *PUtable;	 		/* Physical Unit Table  */
	__u16 *VUtable; 		/* Virtual Unit Table */
        unsigned int nb_blocks;		/* number of physical blocks */
        unsigned int nb_boot_blocks;	/* number of blocks used by the bios */
        struct erase_info instr;
        struct nand_ecclayout oobinfo;
};

int INFTL_mount(struct INFTLrecord *s);
int INFTL_formatblock(struct INFTLrecord *s, int block);

void INFTL_dumptables(struct INFTLrecord *s);
void INFTL_dumpVUchains(struct INFTLrecord *s);

int inftl_read_oob(struct mtd_info *mtd, loff_t offs, size_t len,
		   size_t *retlen, uint8_t *buf);
int inftl_write_oob(struct mtd_info *mtd, loff_t offs, size_t len,
		    size_t *retlen, uint8_t *buf);

#endif /* __KERNEL__ */

#endif /* __MTD_INFTL_H__ */

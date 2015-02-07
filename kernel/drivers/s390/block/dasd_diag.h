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
 * File...........: linux/drivers/s390/block/dasd_diag.h
 * Author(s)......: Holger Smolinski <Holger.Smolinski@de.ibm.com>
 * Based on.......: linux/drivers/s390/block/mdisk.h
 * ...............: by Hartmunt Penner <hpenner@de.ibm.com>
 * Bugreports.to..: <Linux390@de.ibm.com>
 * (C) IBM Corporation, IBM Deutschland Entwicklung GmbH, 1999,2000
 *
 */

#define MDSK_WRITE_REQ 0x01
#define MDSK_READ_REQ  0x02

#define INIT_BIO	0x00
#define RW_BIO		0x01
#define TERM_BIO	0x02

#define DEV_CLASS_FBA	0x01
#define DEV_CLASS_ECKD	0x04

#define DASD_DIAG_LC_INT_CODE		132
#define DASD_DIAG_LC_INT_STATUS		133
#define DASD_DIAG_LC_INT_PARM_31BIT	128
#define DASD_DIAG_LC_INT_PARM_64BIT	4536
#define DASD_DIAG_CODE_31BIT		0x03
#define DASD_DIAG_CODE_64BIT		0x07

#define DASD_DIAG_RWFLAG_ASYNC		0x02
#define DASD_DIAG_RWFLAG_NOCACHE	0x01

#define DASD_DIAG_FLAGA_FORMAT_64BIT	0x80

struct dasd_diag_characteristics {
	u16 dev_nr;
	u16 rdc_len;
	u8 vdev_class;
	u8 vdev_type;
	u8 vdev_status;
	u8 vdev_flags;
	u8 rdev_class;
	u8 rdev_type;
	u8 rdev_model;
	u8 rdev_features;
} __attribute__ ((packed, aligned(4)));


#ifdef CONFIG_64BIT
#define DASD_DIAG_FLAGA_DEFAULT		DASD_DIAG_FLAGA_FORMAT_64BIT

typedef u64 blocknum_t;
typedef s64 sblocknum_t;

struct dasd_diag_bio {
	u8 type;
	u8 status;
	u8 spare1[2];
	u32 alet;
	blocknum_t block_number;
	void *buffer;
} __attribute__ ((packed, aligned(8)));

struct dasd_diag_init_io {
	u16 dev_nr;
	u8 flaga;
	u8 spare1[21];
	u32 block_size;
	u8 spare2[4];
	blocknum_t offset;
	sblocknum_t start_block;
	blocknum_t end_block;
	u8  spare3[8];
} __attribute__ ((packed, aligned(8)));

struct dasd_diag_rw_io {
	u16 dev_nr;
	u8  flaga;
	u8  spare1[21];
	u8  key;
	u8  flags;
	u8  spare2[2];
	u32 block_count;
	u32 alet;
	u8  spare3[4];
	u64 interrupt_params;
	struct dasd_diag_bio *bio_list;
	u8  spare4[8];
} __attribute__ ((packed, aligned(8)));
#else /* CONFIG_64BIT */
#define DASD_DIAG_FLAGA_DEFAULT		0x0

typedef u32 blocknum_t;
typedef s32 sblocknum_t;

struct dasd_diag_bio {
	u8 type;
	u8 status;
	u16 spare1;
	blocknum_t block_number;
	u32 alet;
	void *buffer;
} __attribute__ ((packed, aligned(8)));

struct dasd_diag_init_io {
	u16 dev_nr;
	u8 flaga;
	u8 spare1[21];
	u32 block_size;
	blocknum_t offset;
	sblocknum_t start_block;
	blocknum_t end_block;
	u8 spare2[24];
} __attribute__ ((packed, aligned(8)));

struct dasd_diag_rw_io {
	u16 dev_nr;
	u8 flaga;
	u8 spare1[21];
	u8 key;
	u8 flags;
	u8 spare2[2];
	u32 block_count;
	u32 alet;
	struct dasd_diag_bio *bio_list;
	u32 interrupt_params;
	u8 spare3[20];
} __attribute__ ((packed, aligned(8)));
#endif /* CONFIG_64BIT */

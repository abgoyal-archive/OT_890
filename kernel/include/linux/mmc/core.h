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
 *  linux/include/linux/mmc/core.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef LINUX_MMC_CORE_H
#define LINUX_MMC_CORE_H

#include <linux/interrupt.h>
#include <linux/device.h>

struct request;
struct mmc_data;
struct mmc_request;

struct mmc_command {
	u32			opcode;
	u32			arg;
	u32			resp[4];
	unsigned int		flags;		/* expected response type */
#define MMC_RSP_PRESENT	(1 << 0)
#define MMC_RSP_136	(1 << 1)		/* 136 bit response */
#define MMC_RSP_CRC	(1 << 2)		/* expect valid crc */
#define MMC_RSP_BUSY	(1 << 3)		/* card may send busy */
#define MMC_RSP_OPCODE	(1 << 4)		/* response contains opcode */

#define MMC_CMD_MASK	(3 << 5)		/* non-SPI command type */
#define MMC_CMD_AC	(0 << 5)
#define MMC_CMD_ADTC	(1 << 5)
#define MMC_CMD_BC	(2 << 5)
#define MMC_CMD_BCR	(3 << 5)

#define MMC_RSP_SPI_S1	(1 << 7)		/* one status byte */
#define MMC_RSP_SPI_S2	(1 << 8)		/* second byte */
#define MMC_RSP_SPI_B4	(1 << 9)		/* four data bytes */
#define MMC_RSP_SPI_BUSY (1 << 10)		/* card may send busy */

/*
 * These are the native response types, and correspond to valid bit
 * patterns of the above flags.  One additional valid pattern
 * is all zeros, which means we don't expect a response.
 */
#define MMC_RSP_NONE	(0)
#define MMC_RSP_R1	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R1B	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE|MMC_RSP_BUSY)
#define MMC_RSP_R2	(MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC)
#define MMC_RSP_R3	(MMC_RSP_PRESENT)
#define MMC_RSP_R4	(MMC_RSP_PRESENT)
#define MMC_RSP_R5	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R6	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R7	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)

#define mmc_resp_type(cmd)	((cmd)->flags & (MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC|MMC_RSP_BUSY|MMC_RSP_OPCODE))

/*
 * These are the SPI response types for MMC, SD, and SDIO cards.
 * Commands return R1, with maybe more info.  Zero is an error type;
 * callers must always provide the appropriate MMC_RSP_SPI_Rx flags.
 */
#define MMC_RSP_SPI_R1	(MMC_RSP_SPI_S1)
#define MMC_RSP_SPI_R1B	(MMC_RSP_SPI_S1|MMC_RSP_SPI_BUSY)
#define MMC_RSP_SPI_R2	(MMC_RSP_SPI_S1|MMC_RSP_SPI_S2)
#define MMC_RSP_SPI_R3	(MMC_RSP_SPI_S1|MMC_RSP_SPI_B4)
#define MMC_RSP_SPI_R4	(MMC_RSP_SPI_S1|MMC_RSP_SPI_B4)
#define MMC_RSP_SPI_R5	(MMC_RSP_SPI_S1|MMC_RSP_SPI_S2)
#define MMC_RSP_SPI_R7	(MMC_RSP_SPI_S1|MMC_RSP_SPI_B4)

#define mmc_spi_resp_type(cmd)	((cmd)->flags & \
		(MMC_RSP_SPI_S1|MMC_RSP_SPI_BUSY|MMC_RSP_SPI_S2|MMC_RSP_SPI_B4))

/*
 * These are the command types.
 */
#define mmc_cmd_type(cmd)	((cmd)->flags & MMC_CMD_MASK)

	unsigned int		retries;	/* max number of retries */
	unsigned int		error;		/* command error */

/*
 * Standard errno values are used for errors, but some have specific
 * meaning in the MMC layer:
 *
 * ETIMEDOUT    Card took too long to respond
 * EILSEQ       Basic format problem with the received or sent data
 *              (e.g. CRC check failed, incorrect opcode in response
 *              or bad end bit)
 * EINVAL       Request cannot be performed because of restrictions
 *              in hardware and/or the driver
 * ENOMEDIUM    Host can determine that the slot is empty and is
 *              actively failing requests
 */

	struct mmc_data		*data;		/* data segment associated with cmd */
	struct mmc_request	*mrq;		/* associated request */
};

struct mmc_data {
	unsigned int		timeout_ns;	/* data timeout (in ns, max 80ms) */
	unsigned int		timeout_clks;	/* data timeout (in clocks) */
	unsigned int		blksz;		/* data block size */
	unsigned int		blocks;		/* number of blocks */
	unsigned int		error;		/* data error */
	unsigned int		flags;

#define MMC_DATA_WRITE	(1 << 8)
#define MMC_DATA_READ	(1 << 9)
#define MMC_DATA_STREAM	(1 << 10)

	unsigned int		bytes_xfered;

	struct mmc_command	*stop;		/* stop command */
	struct mmc_request	*mrq;		/* associated request */

	unsigned int		sg_len;		/* size of scatter list */
	struct scatterlist	*sg;		/* I/O scatter list */
};

struct mmc_request {
	struct mmc_command	*cmd;
	struct mmc_data		*data;
	struct mmc_command	*stop;

	void			*done_data;	/* completion data */
	void			(*done)(struct mmc_request *);/* completion function */
};

struct mmc_host;
struct mmc_card;

extern void mmc_wait_for_req(struct mmc_host *, struct mmc_request *);
extern int mmc_wait_for_cmd(struct mmc_host *, struct mmc_command *, int);
extern int mmc_wait_for_app_cmd(struct mmc_host *, struct mmc_card *,
	struct mmc_command *, int);

extern void mmc_set_data_timeout(struct mmc_data *, const struct mmc_card *);
extern unsigned int mmc_align_data_size(struct mmc_card *, unsigned int);

extern int __mmc_claim_host(struct mmc_host *host, atomic_t *abort);
extern void mmc_release_host(struct mmc_host *host);

/**
 *	mmc_claim_host - exclusively claim a host
 *	@host: mmc host to claim
 *
 *	Claim a host for a set of operations.
 */
static inline void mmc_claim_host(struct mmc_host *host)
{
	__mmc_claim_host(host, NULL);
}

extern u32 mmc_vddrange_to_ocrmask(int vdd_min, int vdd_max);

#endif

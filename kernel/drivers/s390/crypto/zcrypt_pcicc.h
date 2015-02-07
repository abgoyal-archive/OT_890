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
 *  linux/drivers/s390/crypto/zcrypt_pcicc.h
 *
 *  zcrypt 2.1.0
 *
 *  Copyright (C)  2001, 2006 IBM Corporation
 *  Author(s): Robert Burroughs
 *	       Eric Rossman (edrossma@us.ibm.com)
 *
 *  Hotplug & misc device support: Jochen Roehrig (roehrig@de.ibm.com)
 *  Major cleanup & driver split: Martin Schwidefsky <schwidefsky@de.ibm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _ZCRYPT_PCICC_H_
#define _ZCRYPT_PCICC_H_

/**
 * The type 6 message family is associated with PCICC or PCIXCC cards.
 *
 * It contains a message header followed by a CPRB, both of which
 * are described below.
 *
 * Note that all reserved fields must be zeroes.
 */
struct type6_hdr {
	unsigned char reserved1;	/* 0x00				*/
	unsigned char type;		/* 0x06				*/
	unsigned char reserved2[2];	/* 0x0000			*/
	unsigned char right[4];		/* 0x00000000			*/
	unsigned char reserved3[2];	/* 0x0000			*/
	unsigned char reserved4[2];	/* 0x0000			*/
	unsigned char apfs[4];		/* 0x00000000			*/
	unsigned int  offset1;		/* 0x00000058 (offset to CPRB)	*/
	unsigned int  offset2;		/* 0x00000000			*/
	unsigned int  offset3;		/* 0x00000000			*/
	unsigned int  offset4;		/* 0x00000000			*/
	unsigned char agent_id[16];	/* PCICC:			*/
					/*    0x0100			*/
					/*    0x4343412d4150504c202020	*/
					/*    0x010101			*/
					/* PCIXCC:			*/
					/*    0x4341000000000000	*/
					/*    0x0000000000000000	*/
	unsigned char rqid[2];		/* rqid.  internal to 603	*/
	unsigned char reserved5[2];	/* 0x0000			*/
	unsigned char function_code[2];	/* for PKD, 0x5044 (ascii 'PD')	*/
	unsigned char reserved6[2];	/* 0x0000			*/
	unsigned int  ToCardLen1;	/* (request CPRB len + 3) & -4	*/
	unsigned int  ToCardLen2;	/* db len 0x00000000 for PKD	*/
	unsigned int  ToCardLen3;	/* 0x00000000			*/
	unsigned int  ToCardLen4;	/* 0x00000000			*/
	unsigned int  FromCardLen1;	/* response buffer length	*/
	unsigned int  FromCardLen2;	/* db len 0x00000000 for PKD	*/
	unsigned int  FromCardLen3;	/* 0x00000000			*/
	unsigned int  FromCardLen4;	/* 0x00000000			*/
} __attribute__((packed));

/**
 * CPRB
 *	  Note that all shorts, ints and longs are little-endian.
 *	  All pointer fields are 32-bits long, and mean nothing
 *
 *	  A request CPRB is followed by a request_parameter_block.
 *
 *	  The request (or reply) parameter block is organized thus:
 *	    function code
 *	    VUD block
 *	    key block
 */
struct CPRB {
	unsigned short cprb_len;	/* CPRB length			 */
	unsigned char cprb_ver_id;	/* CPRB version id.		 */
	unsigned char pad_000;		/* Alignment pad byte.		 */
	unsigned char srpi_rtcode[4];	/* SRPI return code LELONG	 */
	unsigned char srpi_verb;	/* SRPI verb type		 */
	unsigned char flags;		/* flags			 */
	unsigned char func_id[2];	/* function id			 */
	unsigned char checkpoint_flag;	/*				 */
	unsigned char resv2;		/* reserved			 */
	unsigned short req_parml;	/* request parameter buffer	 */
					/* length 16-bit little endian	 */
	unsigned char req_parmp[4];	/* request parameter buffer	 *
					 * pointer (means nothing: the	 *
					 * parameter buffer follows	 *
					 * the CPRB).			 */
	unsigned char req_datal[4];	/* request data buffer		 */
					/* length	  ULELONG	 */
	unsigned char req_datap[4];	/* request data buffer		 */
					/* pointer			 */
	unsigned short rpl_parml;	/* reply  parameter buffer	 */
					/* length 16-bit little endian	 */
	unsigned char pad_001[2];	/* Alignment pad bytes. ULESHORT */
	unsigned char rpl_parmp[4];	/* reply parameter buffer	 *
					 * pointer (means nothing: the	 *
					 * parameter buffer follows	 *
					 * the CPRB).			 */
	unsigned char rpl_datal[4];	/* reply data buffer len ULELONG */
	unsigned char rpl_datap[4];	/* reply data buffer		 */
					/* pointer			 */
	unsigned short ccp_rscode;	/* server reason code	ULESHORT */
	unsigned short ccp_rtcode;	/* server return code	ULESHORT */
	unsigned char repd_parml[2];	/* replied parameter len ULESHORT*/
	unsigned char mac_data_len[2];	/* Mac Data Length	ULESHORT */
	unsigned char repd_datal[4];	/* replied data length	ULELONG	 */
	unsigned char req_pc[2];	/* PC identifier		 */
	unsigned char res_origin[8];	/* resource origin		 */
	unsigned char mac_value[8];	/* Mac Value			 */
	unsigned char logon_id[8];	/* Logon Identifier		 */
	unsigned char usage_domain[2];	/* cdx				 */
	unsigned char resv3[18];	/* reserved for requestor	 */
	unsigned short svr_namel;	/* server name length  ULESHORT	 */
	unsigned char svr_name[8];	/* server name			 */
} __attribute__((packed));

/**
 * The type 86 message family is associated with PCICC and PCIXCC cards.
 *
 * It contains a message header followed by a CPRB.  The CPRB is
 * the same as the request CPRB, which is described above.
 *
 * If format is 1, an error condition exists and no data beyond
 * the 8-byte message header is of interest.
 *
 * The non-error message is shown below.
 *
 * Note that all reserved fields must be zeroes.
 */
struct type86_hdr {
	unsigned char reserved1;	/* 0x00				*/
	unsigned char type;		/* 0x86				*/
	unsigned char format;		/* 0x01 (error) or 0x02 (ok)	*/
	unsigned char reserved2;	/* 0x00				*/
	unsigned char reply_code;	/* reply code (see above)	*/
	unsigned char reserved3[3];	/* 0x000000			*/
} __attribute__((packed));

#define TYPE86_RSP_CODE 0x86
#define TYPE86_FMT2	0x02

struct type86_fmt2_ext {
	unsigned char	  reserved[4];	/* 0x00000000			*/
	unsigned char	  apfs[4];	/* final status			*/
	unsigned int	  count1;	/* length of CPRB + parameters	*/
	unsigned int	  offset1;	/* offset to CPRB		*/
	unsigned int	  count2;	/* 0x00000000			*/
	unsigned int	  offset2;	/* db offset 0x00000000 for PKD	*/
	unsigned int	  count3;	/* 0x00000000			*/
	unsigned int	  offset3;	/* 0x00000000			*/
	unsigned int	  count4;	/* 0x00000000			*/
	unsigned int	  offset4;	/* 0x00000000			*/
} __attribute__((packed));

struct function_and_rules_block {
	unsigned char function_code[2];
	unsigned short ulen;
	unsigned char only_rule[8];
} __attribute__((packed));

int zcrypt_pcicc_init(void);
void zcrypt_pcicc_exit(void);

#endif /* _ZCRYPT_PCICC_H_ */

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

#ifndef _LINUX_ISICOM_H
#define _LINUX_ISICOM_H

#define		YES	1
#define		NO	0

/*
 *  ISICOM Driver definitions ...
 *
 */

#define		ISICOM_NAME	"ISICom"

/*
 *      PCI definitions
 */

#define		DEVID_COUNT	9
#define		VENDOR_ID	0x10b5

/*
 *	These are now officially allocated numbers
 */

#define		ISICOM_NMAJOR	112	/* normal  */
#define		ISICOM_CMAJOR	113	/* callout */
#define		ISICOM_MAGIC	(('M' << 8) | 'T')

#define		WAKEUP_CHARS	256	/* hard coded for now	*/
#define		TX_SIZE		254

#define		BOARD_COUNT	4
#define		PORT_COUNT	(BOARD_COUNT*16)

/*   character sizes  */

#define		ISICOM_CS5		0x0000
#define		ISICOM_CS6		0x0001
#define		ISICOM_CS7		0x0002
#define		ISICOM_CS8		0x0003

/* stop bits */

#define		ISICOM_1SB		0x0000
#define		ISICOM_2SB		0x0004

/* parity */

#define		ISICOM_NOPAR		0x0000
#define		ISICOM_ODPAR		0x0008
#define		ISICOM_EVPAR		0x0018

/* flow control */

#define		ISICOM_CTSRTS		0x03
#define		ISICOM_INITIATE_XONXOFF	0x04
#define		ISICOM_RESPOND_XONXOFF	0x08

#define	BOARD(line)  (((line) >> 4) & 0x3)

	/*	isi kill queue bitmap	*/

#define		ISICOM_KILLTX		0x01
#define		ISICOM_KILLRX		0x02

	/* isi_board status bitmap */

#define		FIRMWARE_LOADED		0x0001
#define		BOARD_ACTIVE		0x0002

 	/* isi_port status bitmap  */

#define		ISI_CTS			0x1000
#define		ISI_DSR			0x2000
#define		ISI_RI			0x4000
#define		ISI_DCD			0x8000
#define		ISI_DTR			0x0100
#define		ISI_RTS			0x0200


#define		ISI_TXOK		0x0001

#endif	/*	ISICOM_H	*/

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

/******************************************************************************
 *
 *	(C)Copyright 1998,1999 SysKonnect,
 *	a business unit of Schneider & Koch & Co. Datensysteme GmbH.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	The information in this file is provided "AS IS" without warranty.
 *
 ******************************************************************************/

#define PHYS			0		/* physical addr */
#define PERM_ADDR		0x80		/* permanet address */
#define SB_STATIC		0x00000001
#define MAX_PAYLOAD		1562
#define PRIMARY_RING		0x00000001
#ifndef NULL
#define NULL			0x00
#endif

/*********************** SB_Input Variable Values ***********************/
/*	 may be needed when ever the SBA state machine is called	*/

#define UNKNOWN_SYNC_SOURCE	0x0001
#define REQ_ALLOCATION		0x0002
#define REPORT_RESP		0x0003
#define CHANGE_RESP		0x0004
#define TNEG			0x0005
#define NIF			0x0006
#define SB_STOP			0x0007
#define SB_START		0x0008
#define REPORT_TIMER		0x0009
#define CHANGE_REQUIRED		0x000A

#define DEFAULT_OV		50

#ifdef SBA
/**************************** SBA STATES *****************************/

#define SBA_STANDBY		0x00000000
#define SBA_ACTIVE		0x00000001
#define SBA_RECOVERY		0x00000002
#define SBA_REPORT		0x00000003
#define SBA_CHANGE		0x00000004

/**************************** OTHERS *********************************/

#define FIFTY_PERCENT		50		/* bytes per second */
#define MAX_SESSIONS		150	
#define TWO_MINUTES		13079		/* 9.175 ms/tick */
#define FIFTY_BYTES		50
#define SBA_DENIED		0x0000000D
#define I_NEED_ONE		0x00000000
#define MAX_NODES		50
/*#define T_REPORT		0x59682F00L*/	/* 120s/80ns in Hex */
#define	TWO_MIN			120		/* seconds */
#define SBA_ST_UNKNOWN		0x00000002
#define SBA_ST_ACTIVE		0x00000001
#define S_CLEAR			0x00000000L
#define ZERO			0x00000000
#define FULL			0x00000000	/* old: 0xFFFFFFFFF */
#define S_SET			0x00000001L
#define LOW_PRIO		0x02		/* ??????? */
#define OK			0x01		/* ??????? */
#define NOT_OK			0x00		/* ??????? */

/****************************************/
/* deallocate_status[ni][si] values	*/
/****************************************/
#define TX_CHANGE		0X00000001L
#define PENDING			0x00000002L
#define NONE			0X00000000L
#endif

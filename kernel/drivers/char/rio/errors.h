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
** -----------------------------------------------------------------------------
**
**  Perle Specialix driver for Linux
**  Ported from existing RIO Driver for SCO sources.
 *
 *  (C) 1990 - 2000 Specialix International Ltd., Byfleet, Surrey, UK.
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
**	Module		: errors.h
**	SID		: 1.2
**	Last Modified	: 11/6/98 11:34:10
**	Retrieved	: 11/6/98 11:34:21
**
**  ident @(#)errors.h	1.2
**
** -----------------------------------------------------------------------------
*/

#ifndef	__rio_errors_h__
#define	__rio_errors_h__

/*
** error codes
*/

#define	NOTHING_WRONG_AT_ALL		0
#define	BAD_CHARACTER_IN_NAME		1
#define	TABLE_ENTRY_ISNT_PROPERLY_NULL	2
#define	UNKNOWN_HOST_NUMBER		3
#define	ZERO_RTA_ID			4
#define	BAD_RTA_ID			5
#define	DUPLICATED_RTA_ID		6
#define	DUPLICATE_UNIQUE_NUMBER		7
#define	BAD_TTY_NUMBER			8
#define	TTY_NUMBER_IN_USE		9
#define	NAME_USED_TWICE			10
#define	HOST_ID_NOT_ZERO		11
#define	BOOT_IN_PROGRESS		12
#define	COPYIN_FAILED			13
#define	HOST_FILE_TOO_LARGE		14
#define	COPYOUT_FAILED			15
#define	NOT_SUPER_USER			16
#define	RIO_ALREADY_POLLING		17

#define	ID_NUMBER_OUT_OF_RANGE		18
#define PORT_NUMBER_OUT_OF_RANGE	19
#define	HOST_NUMBER_OUT_OF_RANGE	20
#define	RUP_NUMBER_OUT_OF_RANGE		21
#define	TTY_NUMBER_OUT_OF_RANGE		22
#define	LINK_NUMBER_OUT_OF_RANGE	23

#define	HOST_NOT_RUNNING		24
#define	IOCTL_COMMAND_UNKNOWN		25
#define	RIO_SYSTEM_HALTED		26
#define	WAIT_FOR_DRAIN_BROKEN		27
#define	PORT_NOT_MAPPED_INTO_SYSTEM	28
#define	EXCLUSIVE_USE_SET		29
#define	WAIT_FOR_NOT_CLOSING_BROKEN	30
#define	WAIT_FOR_PORT_TO_OPEN_BROKEN	31
#define	WAIT_FOR_CARRIER_BROKEN		32
#define	WAIT_FOR_NOT_IN_USE_BROKEN	33
#define	WAIT_FOR_CAN_ADD_COMMAND_BROKEN	34
#define	WAIT_FOR_ADD_COMMAND_BROKEN	35
#define	WAIT_FOR_NOT_PARAM_BROKEN	36
#define	WAIT_FOR_RETRY_BROKEN		37
#define	HOST_HAS_ALREADY_BEEN_BOOTED	38
#define	UNIT_IS_IN_USE			39
#define	COULDNT_FIND_ENTRY		40
#define	RTA_UNIQUE_NUMBER_ZERO		41
#define	CLOSE_COMMAND_FAILED		42
#define	WAIT_FOR_CLOSE_BROKEN		43
#define	CPS_VALUE_OUT_OF_RANGE		44
#define	ID_ALREADY_IN_USE		45
#define	SIGNALS_ALREADY_SET		46
#define	NOT_RECEIVING_PROCESS		47
#define	RTA_NUMBER_WRONG		48
#define NO_SUCH_PRODUCT			49
#define	HOST_SYSPORT_BAD		50
#define	ID_NOT_TENTATIVE		51
#define XPRINT_CPS_OUT_OF_RANGE		52
#define	NOT_ENOUGH_CORE_FOR_PCI_COPY	53


#endif				/* __rio_errors_h__ */

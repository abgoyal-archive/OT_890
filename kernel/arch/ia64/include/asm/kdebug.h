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

#ifndef _IA64_KDEBUG_H
#define _IA64_KDEBUG_H 1
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Copyright (C) Intel Corporation, 2005
 *
 * 2005-Apr     Rusty Lynch <rusty.lynch@intel.com> and Anil S Keshavamurthy
 *              <anil.s.keshavamurthy@intel.com> adopted from
 *              include/asm-x86_64/kdebug.h
 *
 * 2005-Oct	Keith Owens <kaos@sgi.com>.  Expand notify_die to cover more
 *		events.
 */

enum die_val {
	DIE_BREAK = 1,
	DIE_FAULT,
	DIE_OOPS,
	DIE_MACHINE_HALT,
	DIE_MACHINE_RESTART,
	DIE_MCA_MONARCH_ENTER,
	DIE_MCA_MONARCH_PROCESS,
	DIE_MCA_MONARCH_LEAVE,
	DIE_MCA_SLAVE_ENTER,
	DIE_MCA_SLAVE_PROCESS,
	DIE_MCA_SLAVE_LEAVE,
	DIE_MCA_RENDZVOUS_ENTER,
	DIE_MCA_RENDZVOUS_PROCESS,
	DIE_MCA_RENDZVOUS_LEAVE,
	DIE_MCA_NEW_TIMEOUT,
	DIE_INIT_ENTER,
	DIE_INIT_MONARCH_ENTER,
	DIE_INIT_MONARCH_PROCESS,
	DIE_INIT_MONARCH_LEAVE,
	DIE_INIT_SLAVE_ENTER,
	DIE_INIT_SLAVE_PROCESS,
	DIE_INIT_SLAVE_LEAVE,
	DIE_KDEBUG_ENTER,
	DIE_KDEBUG_LEAVE,
	DIE_KDUMP_ENTER,
	DIE_KDUMP_LEAVE,
};

#endif

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

/**************************************************************************
 *
 * Copyright (c) 2000-2002 Alacritech, Inc.  All rights reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY ALACRITECH, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL ALACRITECH, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation
 * are those of the authors and should not be interpreted as representing
 * official policies, either expressed or implied, of Alacritech, Inc.
 *
 **************************************************************************/

/*
 * FILENAME: slicbuild.h
 *
 * The following contains the compiler directive switches used for
 * different SLIC build options.  They can all be set in the Makefile
 * but the defaults are defined here.
 */
#ifndef _SLIC_BUILD_H_
#define _SLIC_BUILD_H_

#ifndef SLIC_PRODUCTION_BUILD
#define SLIC_PRODUCTION_BUILD			1
#endif
#ifndef SLIC_FAILURE_RESET
#define SLIC_FAILURE_RESET			1
#endif
#define DBG                         1
#ifndef SLIC_ASSERT_ENABLED
#define SLIC_ASSERT_ENABLED			1
#endif
#ifndef SLIC_MCAST_ENABLED
#define SLIC_MCAST_ENABLED			1
#endif
#ifndef SLIC_GET_STATS_ENABLED
#define SLIC_GET_STATS_ENABLED			1
#endif
#ifndef SLIC_GET_STATS_TIMER_ENABLED
#define SLIC_GET_STATS_TIMER_ENABLED		0
#endif
#ifndef SLIC_PING_TIMER_ENABLED
#define SLIC_PING_TIMER_ENABLED		1
#endif
#ifndef SLIC_IOCTL_SUPPORT_ENABLED
#define SLIC_IOCTL_SUPPORT_ENABLED		1
#endif
#ifndef ATK_DEBUG
#define ATK_DEBUG				1
#endif
#ifndef SLIC_POWER_MANAGEMENT_ENABLED
#define SLIC_POWER_MANAGEMENT_ENABLED		0
#endif
#ifndef SLIC_INTERRUPT_PROCESS_LIMIT
#define SLIC_INTERRUPT_PROCESS_LIMIT		1
#endif
#ifndef LINUX_FREES_ADAPTER_RESOURCES
#define LINUX_FREES_ADAPTER_RESOURCES		1
#endif
#ifndef SLIC_OFFLOAD_IP_CHECKSUM
#define SLIC_OFFLOAD_IP_CHECKSUM		1
#endif
#ifndef SLIC_POWER_MANAGEMENT_ENABLED
#define SLIC_POWER_MANAGEMENT_ENABLED		0
#endif
#ifndef STATS_TIMER_INTERVAL
#define STATS_TIMER_INTERVAL			2
#endif
#ifndef PING_TIMER_INTERVAL
#define PING_TIMER_INTERVAL			1
#endif

#endif   /* _SLIC_BUILD_H_  */

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
 * RT-Mutexes: blocking mutual exclusion locks with PI support
 *
 * started by Ingo Molnar and Thomas Gleixner:
 *
 *  Copyright (C) 2004-2006 Red Hat, Inc., Ingo Molnar <mingo@redhat.com>
 *  Copyright (C) 2006, Timesys Corp., Thomas Gleixner <tglx@timesys.com>
 *
 * This file contains macros used solely by rtmutex.c.
 * Non-debug version.
 */

#define rt_mutex_deadlock_check(l)			(0)
#define rt_mutex_deadlock_account_lock(m, t)		do { } while (0)
#define rt_mutex_deadlock_account_unlock(l)		do { } while (0)
#define debug_rt_mutex_init_waiter(w)			do { } while (0)
#define debug_rt_mutex_free_waiter(w)			do { } while (0)
#define debug_rt_mutex_lock(l)				do { } while (0)
#define debug_rt_mutex_proxy_lock(l,p)			do { } while (0)
#define debug_rt_mutex_proxy_unlock(l)			do { } while (0)
#define debug_rt_mutex_unlock(l)			do { } while (0)
#define debug_rt_mutex_init(m, n)			do { } while (0)
#define debug_rt_mutex_deadlock(d, a ,l)		do { } while (0)
#define debug_rt_mutex_print_deadlock(w)		do { } while (0)
#define debug_rt_mutex_detect_deadlock(w,d)		(d)
#define debug_rt_mutex_reset_waiter(w)			do { } while (0)

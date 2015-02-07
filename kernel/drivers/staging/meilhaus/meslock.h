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

/**
 * @file meslock.h
 *
 * @brief Provides the subdevice lock class.
 * @note Copyright (C) 2006 Meilhaus Electronic GmbH (support@meilhaus.de)
 * @author Guenter Gebhardt
 */

#ifndef _MESLOCK_H_
#define _MESLOCK_H_

#include <linux/spinlock.h>

#ifdef __KERNEL__

/**
 * @brief The subdevice lock class.
 */
typedef struct me_slock {
	struct file *filep;		/**< Pointer to file structure holding the subdevice. */
	int count;			/**< Number of tasks which are inside the subdevice. */
	spinlock_t spin_lock;		/**< Spin lock protecting the attributes from concurrent access. */
} me_slock_t;

/**
 * @brief Tries to enter a subdevice.
 *
 * @param slock The subdevice lock instance.
 * @param filep The file structure identifying the calling process.
 *
 * @return 0 on success.
 */
int me_slock_enter(struct me_slock *slock, struct file *filep);

/**
 * @brief Exits a subdevice.
 *
 * @param slock The subdevice lock instance.
 * @param filep The file structure identifying the calling process.
 *
 * @return 0 on success.
 */
int me_slock_exit(struct me_slock *slock, struct file *filep);

/**
 * @brief Tries to perform a locking action on a subdevice.
 *
 * @param slock The subdevice lock instance.
 * @param filep The file structure identifying the calling process.
 * @param The action to be done.
 *
 * @return 0 on success.
 */
int me_slock_lock(struct me_slock *slock, struct file *filep, int lock);

/**
 * @brief Initializes a lock structure.
 *
 * @param slock The lock structure to initialize.
 * @return 0 on success.
 */
int me_slock_init(me_slock_t * slock);

/**
 * @brief Deinitializes a lock structure.
 *
 * @param slock The lock structure to deinitialize.
 * @return 0 on success.
 */
void me_slock_deinit(me_slock_t * slock);

#endif
#endif

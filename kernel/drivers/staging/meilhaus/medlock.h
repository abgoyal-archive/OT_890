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
 * @file medlock.h
 *
 * @brief Provides the device lock class.
 * @note Copyright (C) 2006 Meilhaus Electronic GmbH (support@meilhaus.de)
 * @author Guenter Gebhardt
 */

#ifndef _MEDLOCK_H_
#define _MEDLOCK_H_

#include <linux/spinlock.h>

#ifdef __KERNEL__

/**
 * @brief The device lock class.
 */
typedef struct me_dlock {
	struct file *filep;		/**< Pointer to file structure holding the device. */
	int count;				/**< Number of tasks which are inside the device. */
	spinlock_t spin_lock;	/**< Spin lock protecting the attributes from concurrent access. */
} me_dlock_t;

/**
 * @brief Tries to enter a device.
 *
 * @param dlock The device lock instance.
 * @param filep The file structure identifying the calling process.
 *
 * @return 0 on success.
 */
int me_dlock_enter(struct me_dlock *dlock, struct file *filep);

/**
 * @brief Exits a device.
 *
 * @param dlock The device lock instance.
 * @param filep The file structure identifying the calling process.
 *
 * @return 0 on success.
 */
int me_dlock_exit(struct me_dlock *dlock, struct file *filep);

/**
 * @brief Tries to perform a locking action on a device.
 *
 * @param dlock The device lock instance.
 * @param filep The file structure identifying the calling process.
 * @param The action to be done.
 * @param flags Flags from user space.
 * @param slist The subdevice list of the device.
 *
 * @return 0 on success.
 */
int me_dlock_lock(struct me_dlock *dlock,
		  struct file *filep, int lock, int flags, me_slist_t * slist);

/**
 * @brief Initializes a lock structure.
 *
 * @param dlock The lock structure to initialize.
 * @return 0 on success.
 */
int me_dlock_init(me_dlock_t * dlock);

/**
 * @brief Deinitializes a lock structure.
 *
 * @param dlock The lock structure to deinitialize.
 * @return 0 on success.
 */
void me_dlock_deinit(me_dlock_t * dlock);

#endif
#endif

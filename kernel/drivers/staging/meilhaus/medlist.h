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
 * @file me_dlist.h
 *
 * @brief Provides the device list class.
 * @note Copyright (C) 2007 Meilhaus Electronic GmbH (support@meilhaus.de)
 * @author Guenter Gebhardt
 */

#ifndef _ME_DLIST_H_
#define _ME_DLIST_H_

#include <linux/list.h>

#include "medevice.h"

#ifdef __KERNEL__

/**
 * @brief The device list container.
 */
typedef struct me_dlist {
	struct list_head head;		/**< The head of the internal list. */
	unsigned int n;			/**< The number of devices in the list. */
} me_dlist_t;

/**
 * @brief Queries the number of devices currently inside the list.
 *
 * @param dlist The device list to query.
 * @param[out] number The number of devices.
 *
 * @return ME-iDS error code.
 */
int me_dlist_query_number_devices(struct me_dlist *dlist, int *number);

/**
 * @brief Returns the number of devices currently inside the list.
 *
 * @param dlist The device list to query.
 *
 * @return The number of devices in the list.
 */
unsigned int me_dlist_get_number_devices(struct me_dlist *dlist);

/**
 * @brief Get a device by index.
 *
 * @param dlist The device list to query.
 * @param index The index of the device to get in the list.
 *
 * @return The device at index if available.\n
 *         NULL if the index is out of range.
 */
me_device_t *me_dlist_get_device(struct me_dlist *dlist, unsigned int index);

/**
 * @brief Adds a device to the tail of the list.
 *
 * @param dlist The device list to add a device to.
 * @param device The device to add to the list.
 */
void me_dlist_add_device_tail(struct me_dlist *dlist, me_device_t * device);

/**
 * @brief Removes a device from the tail of the list.
 *
 * @param dlist The device list.
 *
 * @return Pointer to the removed subdeivce.\n
 *         NULL in cases where the list was empty.
 */
me_device_t *me_dlist_del_device_tail(struct me_dlist *dlist);

/**
 * @brief Initializes a device list structure.
 *
 * @param lock The device list structure to initialize.
 * @return 0 on success.
 */
int me_dlist_init(me_dlist_t * dlist);

/**
 * @brief Deinitializes a device list structure and destructs every device in it.
 *
 * @param dlist The device list structure to deinitialize.
 * @return 0 on success.
 */
void me_dlist_deinit(me_dlist_t * dlist);

#endif
#endif

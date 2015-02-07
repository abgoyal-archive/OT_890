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
 * @file me_slist.h
 *
 * @brief Provides the subdevice list class.
 * @note Copyright (C) 2007 Meilhaus Electronic GmbH (support@meilhaus.de)
 * @author Guenter Gebhardt
 */

#ifndef _ME_SLIST_H_
#define _ME_SLIST_H_

#include <linux/list.h>

#include "mesubdevice.h"

#ifdef __KERNEL__

/**
 * @brief The subdevice list container.
 */
typedef struct me_slist {
	struct list_head head;		/**< The head of the internal list. */
	unsigned int n;			/**< The number of subdevices in the list. */
} me_slist_t;

/**
 * @brief Queries the number of subdevices currently inside the list.
 *
 * @param slist The subdevice list to query.
 * @param[out] number The number of subdevices of the device.
 *
 * @return ME-iDS error code.
 */
int me_slist_query_number_subdevices(struct me_slist *slist, int *number);

/**
 * @brief Returns the number of subdevices currently inside the list.
 *
 * @param slist The subdevice list to query.
 *
 * @return The number of subdevices in the list.
 */
unsigned int me_slist_get_number_subdevices(struct me_slist *slist);

/**
 * @brief Get a subdevice by index.
 *
 * @param slist The subdevice list to query.
 * @param index The index of the subdevice to get in the list.
 *
 * @return The subdevice at index if available.\n
 *         NULL if the index is out of range.
 */
me_subdevice_t *me_slist_get_subdevice(struct me_slist *slist,
				       unsigned int index);

/**
 * @brief Get a subdevice index by type and subtype.
 *
 * @param slist The subdevice list to query.
 * @param start_subdevice The subdevice index at which the start shall begin.
 * @param type The type of the subdevice to query.
 * @param subtype The subtype of the subdevice to query.
 * @param[out] subdevice On success this parameter returns the index of the subdevice matching the requested type.
 *
 * @return ME_ERRNO_SUCCESS on success.
 */
int me_slist_get_subdevice_by_type(struct me_slist *slist,
				   unsigned int start_subdevice,
				   int type, int subtype, int *subdevice);

/**
 * @brief Adds a subdevice to the tail of the list.
 *
 * @param slist The subdevice list to add a subdevice to.
 * @param subdevice The subdevice to add to the list.
 */
void me_slist_add_subdevice_tail(struct me_slist *slist,
				 me_subdevice_t * subdevice);

/**
 * @brief Removes a subdevice from the tail of the list.
 *
 * @param slist The subdevice list.
 *
 * @return Pointer to the removed subdeivce.\n
 *         NULL in cases where the list was empty.
 */
me_subdevice_t *me_slist_del_subdevice_tail(struct me_slist *slist);

/**
 * @brief Initializes a subdevice list structure.
 *
 * @param lock The subdevice list structure to initialize.
 * @return 0 on success.
 */
int me_slist_init(me_slist_t * slist);

/**
 * @brief Deinitializes a subdevice list structure and destructs every subdevice in it.
 *
 * @param slist The subdevice list structure to deinitialize.
 * @return 0 on success.
 */
void me_slist_deinit(me_slist_t * slist);

#endif
#endif

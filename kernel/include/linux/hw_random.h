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
	Hardware Random Number Generator

	Please read Documentation/hw_random.txt for details on use.

	----------------------------------------------------------
	This software may be used and distributed according to the terms
        of the GNU General Public License, incorporated herein by reference.

 */

#ifndef LINUX_HWRANDOM_H_
#define LINUX_HWRANDOM_H_

#include <linux/types.h>
#include <linux/list.h>

/**
 * struct hwrng - Hardware Random Number Generator driver
 * @name:		Unique RNG name.
 * @init:		Initialization callback (can be NULL).
 * @cleanup:		Cleanup callback (can be NULL).
 * @data_present:	Callback to determine if data is available
 *			on the RNG. If NULL, it is assumed that
 *			there is always data available.
 * @data_read:		Read data from the RNG device.
 *			Returns the number of lower random bytes in "data".
 *			Must not be NULL.
 * @priv:		Private data, for use by the RNG driver.
 */
struct hwrng {
	const char *name;
	int (*init)(struct hwrng *rng);
	void (*cleanup)(struct hwrng *rng);
	int (*data_present)(struct hwrng *rng, int wait);
	int (*data_read)(struct hwrng *rng, u32 *data);
	unsigned long priv;

	/* internal. */
	struct list_head list;
};

/** Register a new Hardware Random Number Generator driver. */
extern int hwrng_register(struct hwrng *rng);
/** Unregister a Hardware Random Number Generator driver. */
extern void hwrng_unregister(struct hwrng *rng);

#endif /* LINUX_HWRANDOM_H_ */

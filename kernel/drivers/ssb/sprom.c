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
 * Sonics Silicon Backplane
 * Common SPROM support routines
 *
 * Copyright (C) 2005-2008 Michael Buesch <mb@bu3sch.de>
 * Copyright (C) 2005 Martin Langer <martin-langer@gmx.de>
 * Copyright (C) 2005 Stefano Brivio <st3@riseup.net>
 * Copyright (C) 2005 Danny van Dyk <kugelfang@gentoo.org>
 * Copyright (C) 2005 Andreas Jaggi <andreas.jaggi@waterwave.ch>
 *
 * Licensed under the GNU/GPL. See COPYING for details.
 */

#include "ssb_private.h"


static int sprom2hex(const u16 *sprom, char *buf, size_t buf_len,
		     size_t sprom_size_words)
{
	int i, pos = 0;

	for (i = 0; i < sprom_size_words; i++)
		pos += snprintf(buf + pos, buf_len - pos - 1,
				"%04X", swab16(sprom[i]) & 0xFFFF);
	pos += snprintf(buf + pos, buf_len - pos - 1, "\n");

	return pos + 1;
}

static int hex2sprom(u16 *sprom, const char *dump, size_t len,
		     size_t sprom_size_words)
{
	char tmp[5] = { 0 };
	int cnt = 0;
	unsigned long parsed;

	if (len < sprom_size_words * 2)
		return -EINVAL;

	while (cnt < sprom_size_words) {
		memcpy(tmp, dump, 4);
		dump += 4;
		parsed = simple_strtoul(tmp, NULL, 16);
		sprom[cnt++] = swab16((u16)parsed);
	}

	return 0;
}

/* Common sprom device-attribute show-handler */
ssize_t ssb_attr_sprom_show(struct ssb_bus *bus, char *buf,
			    int (*sprom_read)(struct ssb_bus *bus, u16 *sprom))
{
	u16 *sprom;
	int err = -ENOMEM;
	ssize_t count = 0;
	size_t sprom_size_words = bus->sprom_size;

	sprom = kcalloc(sprom_size_words, sizeof(u16), GFP_KERNEL);
	if (!sprom)
		goto out;

	/* Use interruptible locking, as the SPROM write might
	 * be holding the lock for several seconds. So allow userspace
	 * to cancel operation. */
	err = -ERESTARTSYS;
	if (mutex_lock_interruptible(&bus->sprom_mutex))
		goto out_kfree;
	err = sprom_read(bus, sprom);
	mutex_unlock(&bus->sprom_mutex);

	if (!err)
		count = sprom2hex(sprom, buf, PAGE_SIZE, sprom_size_words);

out_kfree:
	kfree(sprom);
out:
	return err ? err : count;
}

/* Common sprom device-attribute store-handler */
ssize_t ssb_attr_sprom_store(struct ssb_bus *bus,
			     const char *buf, size_t count,
			     int (*sprom_check_crc)(const u16 *sprom, size_t size),
			     int (*sprom_write)(struct ssb_bus *bus, const u16 *sprom))
{
	u16 *sprom;
	int res = 0, err = -ENOMEM;
	size_t sprom_size_words = bus->sprom_size;

	sprom = kcalloc(bus->sprom_size, sizeof(u16), GFP_KERNEL);
	if (!sprom)
		goto out;
	err = hex2sprom(sprom, buf, count, sprom_size_words);
	if (err) {
		err = -EINVAL;
		goto out_kfree;
	}
	err = sprom_check_crc(sprom, sprom_size_words);
	if (err) {
		err = -EINVAL;
		goto out_kfree;
	}

	/* Use interruptible locking, as the SPROM write might
	 * be holding the lock for several seconds. So allow userspace
	 * to cancel operation. */
	err = -ERESTARTSYS;
	if (mutex_lock_interruptible(&bus->sprom_mutex))
		goto out_kfree;
	err = ssb_devices_freeze(bus);
	if (err == -EOPNOTSUPP) {
		ssb_printk(KERN_ERR PFX "SPROM write: Could not freeze devices. "
			   "No suspend support. Is CONFIG_PM enabled?\n");
		goto out_unlock;
	}
	if (err) {
		ssb_printk(KERN_ERR PFX "SPROM write: Could not freeze all devices\n");
		goto out_unlock;
	}
	res = sprom_write(bus, sprom);
	err = ssb_devices_thaw(bus);
	if (err)
		ssb_printk(KERN_ERR PFX "SPROM write: Could not thaw all devices\n");
out_unlock:
	mutex_unlock(&bus->sprom_mutex);
out_kfree:
	kfree(sprom);
out:
	if (res)
		return res;
	return err ? err : count;
}

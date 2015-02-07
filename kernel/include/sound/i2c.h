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

#ifndef __SOUND_I2C_H
#define __SOUND_I2C_H

/*
 *
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 *
 */

#define SND_I2C_DEVICE_ADDRTEN	(1<<0)	/* 10-bit I2C address */

struct snd_i2c_device {
	struct list_head list;
	struct snd_i2c_bus *bus;	/* I2C bus */
	char name[32];		/* some useful device name */
	unsigned short flags;	/* device flags */
	unsigned short addr;	/* device address (might be 10-bit) */
	unsigned long private_value;
	void *private_data;
	void (*private_free)(struct snd_i2c_device *device);
};

#define snd_i2c_device(n) list_entry(n, struct snd_i2c_device, list)

struct snd_i2c_bit_ops {
	void (*start)(struct snd_i2c_bus *bus);	/* transfer start */
	void (*stop)(struct snd_i2c_bus *bus);	/* transfer stop */
	void (*direction)(struct snd_i2c_bus *bus, int clock, int data);  /* set line direction (0 = write, 1 = read) */
	void (*setlines)(struct snd_i2c_bus *bus, int clock, int data);
	int (*getclock)(struct snd_i2c_bus *bus);
	int (*getdata)(struct snd_i2c_bus *bus, int ack);
};

struct snd_i2c_ops {
	int (*sendbytes)(struct snd_i2c_device *device, unsigned char *bytes, int count);
	int (*readbytes)(struct snd_i2c_device *device, unsigned char *bytes, int count);
	int (*probeaddr)(struct snd_i2c_bus *bus, unsigned short addr);
};

struct snd_i2c_bus {
	struct snd_card *card;	/* card which I2C belongs to */
	char name[32];		/* some useful label */

	struct mutex lock_mutex;

	struct snd_i2c_bus *master;	/* master bus when SCK/SCL is shared */
	struct list_head buses;	/* master: slave buses sharing SCK/SCL, slave: link list */

	struct list_head devices; /* attached devices to this bus */

	union {
		struct snd_i2c_bit_ops *bit;
		void *ops;
	} hw_ops;		/* lowlevel operations */
	struct snd_i2c_ops *ops;	/* midlevel operations */

	unsigned long private_value;
	void *private_data;
	void (*private_free)(struct snd_i2c_bus *bus);
};

#define snd_i2c_slave_bus(n) list_entry(n, struct snd_i2c_bus, buses)

int snd_i2c_bus_create(struct snd_card *card, const char *name,
		       struct snd_i2c_bus *master, struct snd_i2c_bus **ri2c);
int snd_i2c_device_create(struct snd_i2c_bus *bus, const char *name,
			  unsigned char addr, struct snd_i2c_device **rdevice);
int snd_i2c_device_free(struct snd_i2c_device *device);

static inline void snd_i2c_lock(struct snd_i2c_bus *bus)
{
	if (bus->master)
		mutex_lock(&bus->master->lock_mutex);
	else
		mutex_lock(&bus->lock_mutex);
}

static inline void snd_i2c_unlock(struct snd_i2c_bus *bus)
{
	if (bus->master)
		mutex_unlock(&bus->master->lock_mutex);
	else
		mutex_unlock(&bus->lock_mutex);
}

int snd_i2c_sendbytes(struct snd_i2c_device *device, unsigned char *bytes, int count);
int snd_i2c_readbytes(struct snd_i2c_device *device, unsigned char *bytes, int count);
int snd_i2c_probeaddr(struct snd_i2c_bus *bus, unsigned short addr);

#endif /* __SOUND_I2C_H */

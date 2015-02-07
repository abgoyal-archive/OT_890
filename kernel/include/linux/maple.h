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

#ifndef __LINUX_MAPLE_H
#define __LINUX_MAPLE_H

#include <linux/device.h>
#include <mach/maple.h>

extern struct bus_type maple_bus_type;

/* Maple Bus command and response codes */
enum maple_code {
	MAPLE_RESPONSE_FILEERR = -5,
	MAPLE_RESPONSE_AGAIN = -4,	/* request should be retransmitted */
	MAPLE_RESPONSE_BADCMD = -3,
	MAPLE_RESPONSE_BADFUNC = -2,
	MAPLE_RESPONSE_NONE = -1,	/* unit didn't respond at all */
	MAPLE_COMMAND_DEVINFO = 1,
	MAPLE_COMMAND_ALLINFO = 2,
	MAPLE_COMMAND_RESET = 3,
	MAPLE_COMMAND_KILL = 4,
	MAPLE_RESPONSE_DEVINFO = 5,
	MAPLE_RESPONSE_ALLINFO = 6,
	MAPLE_RESPONSE_OK = 7,
	MAPLE_RESPONSE_DATATRF = 8,
	MAPLE_COMMAND_GETCOND = 9,
	MAPLE_COMMAND_GETMINFO = 10,
	MAPLE_COMMAND_BREAD = 11,
	MAPLE_COMMAND_BWRITE = 12,
	MAPLE_COMMAND_SETCOND = 14
};

struct mapleq {
	struct list_head list;
	struct maple_device *dev;
	void *sendbuf, *recvbuf, *recvbufdcsp;
	unsigned char length;
	enum maple_code command;
	struct mutex mutex;
};

struct maple_devinfo {
	unsigned long function;
	unsigned long function_data[3];
	unsigned char area_code;
	unsigned char connector_direction;
	char product_name[31];
	char product_licence[61];
	unsigned short standby_power;
	unsigned short max_power;
};

struct maple_device {
	struct maple_driver *driver;
	struct mapleq *mq;
	void (*callback) (struct mapleq * mq);
	unsigned long when, interval, function;
	struct maple_devinfo devinfo;
	unsigned char port, unit;
	char product_name[32];
	char product_licence[64];
	struct device dev;
};

struct maple_driver {
	unsigned long function;
	struct device_driver drv;
};

void maple_getcond_callback(struct maple_device *dev,
			    void (*callback) (struct mapleq * mq),
			    unsigned long interval,
			    unsigned long function);
int maple_driver_register(struct maple_driver *);
void maple_driver_unregister(struct maple_driver *);

int maple_add_packet_sleeps(struct maple_device *mdev, u32 function,
	u32 command, u32 length, void *data);
void maple_clear_dev(struct maple_device *mdev);

#define to_maple_dev(n) container_of(n, struct maple_device, dev)
#define to_maple_driver(n) container_of(n, struct maple_driver, drv)

#define maple_get_drvdata(d)		dev_get_drvdata(&(d)->dev)
#define maple_set_drvdata(d,p)		dev_set_drvdata(&(d)->dev, (p))

#endif				/* __LINUX_MAPLE_H */

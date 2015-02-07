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

#ifndef S390_CCWGROUP_H
#define S390_CCWGROUP_H

struct ccw_device;
struct ccw_driver;

/**
 * struct ccwgroup_device - ccw group device
 * @creator_id: unique number of the driver
 * @state: online/offline state
 * @count: number of attached slave devices
 * @dev: embedded device structure
 * @cdev: variable number of slave devices, allocated as needed
 */
struct ccwgroup_device {
	unsigned long creator_id;
	enum {
		CCWGROUP_OFFLINE,
		CCWGROUP_ONLINE,
	} state;
/* private: */
	atomic_t onoff;
	struct mutex reg_mutex;
/* public: */
	unsigned int count;
	struct device	dev;
	struct ccw_device *cdev[0];
};

/**
 * struct ccwgroup_driver - driver for ccw group devices
 * @owner: driver owner
 * @name: driver name
 * @max_slaves: maximum number of slave devices
 * @driver_id: unique id
 * @probe: function called on probe
 * @remove: function called on remove
 * @set_online: function called when device is set online
 * @set_offline: function called when device is set offline
 * @shutdown: function called when device is shut down
 * @driver: embedded driver structure
 */
struct ccwgroup_driver {
	struct module *owner;
	char *name;
	int max_slaves;
	unsigned long driver_id;

	int (*probe) (struct ccwgroup_device *);
	void (*remove) (struct ccwgroup_device *);
	int (*set_online) (struct ccwgroup_device *);
	int (*set_offline) (struct ccwgroup_device *);
	void (*shutdown)(struct ccwgroup_device *);

	struct device_driver driver;
};

extern int  ccwgroup_driver_register   (struct ccwgroup_driver *cdriver);
extern void ccwgroup_driver_unregister (struct ccwgroup_driver *cdriver);
int ccwgroup_create_from_string(struct device *root, unsigned int creator_id,
				struct ccw_driver *cdrv, int num_devices,
				const char *buf);

extern int ccwgroup_probe_ccwdev(struct ccw_device *cdev);
extern void ccwgroup_remove_ccwdev(struct ccw_device *cdev);

#define to_ccwgroupdev(x) container_of((x), struct ccwgroup_device, dev)
#define to_ccwgroupdrv(x) container_of((x), struct ccwgroup_driver, driver)
#endif

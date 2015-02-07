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

#ifndef S390_CMB_H
#define S390_CMB_H

#include <linux/types.h>

/**
 * struct cmbdata - channel measurement block data for user space
 * @size: size of the stored data
 * @elapsed_time: time since last sampling
 * @ssch_rsch_count: number of ssch and rsch
 * @sample_count: number of samples
 * @device_connect_time: time of device connect
 * @function_pending_time: time of function pending
 * @device_disconnect_time: time of device disconnect
 * @control_unit_queuing_time: time of control unit queuing
 * @device_active_only_time: time of device active only
 * @device_busy_time: time of device busy (ext. format)
 * @initial_command_response_time: initial command response time (ext. format)
 *
 * All values are stored as 64 bit for simplicity, especially
 * in 32 bit emulation mode. All time values are normalized to
 * nanoseconds.
 * Currently, two formats are known, which differ by the size of
 * this structure, i.e. the last two members are only set when
 * the extended channel measurement facility (first shipped in
 * z990 machines) is activated.
 * Potentially, more fields could be added, which would result in a
 * new ioctl number.
 */
struct cmbdata {
	__u64 size;
	__u64 elapsed_time;
 /* basic and exended format: */
	__u64 ssch_rsch_count;
	__u64 sample_count;
	__u64 device_connect_time;
	__u64 function_pending_time;
	__u64 device_disconnect_time;
	__u64 control_unit_queuing_time;
	__u64 device_active_only_time;
 /* extended format only: */
	__u64 device_busy_time;
	__u64 initial_command_response_time;
};

/* enable channel measurement */
#define BIODASDCMFENABLE	_IO(DASD_IOCTL_LETTER, 32)
/* enable channel measurement */
#define BIODASDCMFDISABLE	_IO(DASD_IOCTL_LETTER, 33)
/* read channel measurement data */
#define BIODASDREADALLCMB	_IOWR(DASD_IOCTL_LETTER, 33, struct cmbdata)

#ifdef __KERNEL__
struct ccw_device;
extern int enable_cmf(struct ccw_device *cdev);
extern int disable_cmf(struct ccw_device *cdev);
extern u64 cmf_read(struct ccw_device *cdev, int index);
extern int cmf_readall(struct ccw_device *cdev, struct cmbdata *data);

#endif /* __KERNEL__ */
#endif /* S390_CMB_H */

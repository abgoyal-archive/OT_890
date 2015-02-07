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

#ifndef BSG_H
#define BSG_H

#define BSG_PROTOCOL_SCSI		0

#define BSG_SUB_PROTOCOL_SCSI_CMD	0
#define BSG_SUB_PROTOCOL_SCSI_TMF	1
#define BSG_SUB_PROTOCOL_SCSI_TRANSPORT	2

struct sg_io_v4 {
	__s32 guard;		/* [i] 'Q' to differentiate from v3 */
	__u32 protocol;		/* [i] 0 -> SCSI , .... */
	__u32 subprotocol;	/* [i] 0 -> SCSI command, 1 -> SCSI task
				   management function, .... */

	__u32 request_len;	/* [i] in bytes */
	__u64 request;		/* [i], [*i] {SCSI: cdb} */
	__u64 request_tag;	/* [i] {SCSI: task tag (only if flagged)} */
	__u32 request_attr;	/* [i] {SCSI: task attribute} */
	__u32 request_priority;	/* [i] {SCSI: task priority} */
	__u32 request_extra;	/* [i] {spare, for padding} */
	__u32 max_response_len;	/* [i] in bytes */
	__u64 response;		/* [i], [*o] {SCSI: (auto)sense data} */

        /* "dout_": data out (to device); "din_": data in (from device) */
	__u32 dout_iovec_count;	/* [i] 0 -> "flat" dout transfer else
				   dout_xfer points to array of iovec */
	__u32 dout_xfer_len;	/* [i] bytes to be transferred to device */
	__u32 din_iovec_count;	/* [i] 0 -> "flat" din transfer */
	__u32 din_xfer_len;	/* [i] bytes to be transferred from device */
	__u64 dout_xferp;	/* [i], [*i] */
	__u64 din_xferp;	/* [i], [*o] */

	__u32 timeout;		/* [i] units: millisecond */
	__u32 flags;		/* [i] bit mask */
	__u64 usr_ptr;		/* [i->o] unused internally */
	__u32 spare_in;		/* [i] */

	__u32 driver_status;	/* [o] 0 -> ok */
	__u32 transport_status;	/* [o] 0 -> ok */
	__u32 device_status;	/* [o] {SCSI: command completion status} */
	__u32 retry_delay;	/* [o] {SCSI: status auxiliary information} */
	__u32 info;		/* [o] additional information */
	__u32 duration;		/* [o] time to complete, in milliseconds */
	__u32 response_len;	/* [o] bytes of response actually written */
	__s32 din_resid;	/* [o] din_xfer_len - actual_din_xfer_len */
	__s32 dout_resid;	/* [o] dout_xfer_len - actual_dout_xfer_len */
	__u64 generated_tag;	/* [o] {SCSI: transport generated task tag} */
	__u32 spare_out;	/* [o] */

	__u32 padding;
};

#ifdef __KERNEL__

#if defined(CONFIG_BLK_DEV_BSG)
struct bsg_class_device {
	struct device *class_dev;
	struct device *parent;
	int minor;
	struct request_queue *queue;
	struct kref ref;
	void (*release)(struct device *);
};

extern int bsg_register_queue(struct request_queue *q,
			      struct device *parent, const char *name,
			      void (*release)(struct device *));
extern void bsg_unregister_queue(struct request_queue *);
#else
static inline int bsg_register_queue(struct request_queue *q,
				     struct device *parent, const char *name,
				     void (*release)(struct device *))
{
	return 0;
}
static inline void bsg_unregister_queue(struct request_queue *q)
{
}
#endif

#endif /* __KERNEL__ */

#endif

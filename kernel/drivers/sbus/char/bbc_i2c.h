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

#ifndef _BBC_I2C_H
#define _BBC_I2C_H

#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/list.h>

struct bbc_i2c_client {
	struct bbc_i2c_bus		*bp;
	struct of_device		*op;
	int				bus;
	int				address;
};

enum fan_action { FAN_SLOWER, FAN_SAME, FAN_FASTER, FAN_FULLBLAST, FAN_STATE_MAX };

struct bbc_cpu_temperature {
	struct list_head		bp_list;
	struct list_head		glob_list;

	struct bbc_i2c_client		*client;
	int				index;

	/* Current readings, and history. */
	s8				curr_cpu_temp;
	s8				curr_amb_temp;
	s8				prev_cpu_temp;
	s8				prev_amb_temp;
	s8				avg_cpu_temp;
	s8				avg_amb_temp;

	int				sample_tick;

	enum fan_action			fan_todo[2];
#define FAN_AMBIENT	0
#define FAN_CPU		1
};

struct bbc_fan_control {
	struct list_head		bp_list;
	struct list_head		glob_list;

	struct bbc_i2c_client 		*client;
	int 				index;

	int				psupply_fan_on;
	int				cpu_fan_speed;
	int				system_fan_speed;
};

#define NUM_CHILDREN	8

struct bbc_i2c_bus {
	struct bbc_i2c_bus		*next;
	int				index;
	spinlock_t			lock;
	void				__iomem *i2c_bussel_reg;
	void				__iomem *i2c_control_regs;
	unsigned char			own, clock;

	wait_queue_head_t		wq;
	volatile int			waiting;

	struct list_head		temps;
	struct list_head		fans;

	struct of_device		*op;
	struct {
		struct of_device	*device;
		int			client_claimed;
	} devs[NUM_CHILDREN];
};

/* Probing and attachment. */
extern struct of_device *bbc_i2c_getdev(struct bbc_i2c_bus *, int);
extern struct bbc_i2c_client *bbc_i2c_attach(struct bbc_i2c_bus *bp, struct of_device *);
extern void bbc_i2c_detach(struct bbc_i2c_client *);

/* Register read/write.  NOTE: Blocking! */
extern int bbc_i2c_writeb(struct bbc_i2c_client *, unsigned char val, int off);
extern int bbc_i2c_readb(struct bbc_i2c_client *, unsigned char *byte, int off);
extern int bbc_i2c_write_buf(struct bbc_i2c_client *, char *buf, int len, int off);
extern int bbc_i2c_read_buf(struct bbc_i2c_client *, char *buf, int len, int off);

#endif /* _BBC_I2C_H */

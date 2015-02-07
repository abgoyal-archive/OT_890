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
   nsc_gpio.c

   National Semiconductor GPIO common access methods.

   struct nsc_gpio_ops abstracts the low-level access
   operations for the GPIO units on 2 NSC chip families; the GEODE
   integrated CPU, and the PC-8736[03456] integrated PC-peripheral
   chips.

   The GPIO units on these chips have the same pin architecture, but
   the access methods differ.  Thus, scx200_gpio and pc8736x_gpio
   implement their own versions of these routines; and use the common
   file-operations routines implemented in nsc_gpio module.

   Copyright (c) 2005 Jim Cromie <jim.cromie@gmail.com>

   NB: this work was tested on the Geode SC-1100 and PC-87366 chips.
   NSC sold the GEODE line to AMD, and the PC-8736x line to Winbond.
*/

struct nsc_gpio_ops {
	struct module*	owner;
	u32	(*gpio_config)	(unsigned iminor, u32 mask, u32 bits);
	void	(*gpio_dump)	(struct nsc_gpio_ops *amp, unsigned iminor);
	int	(*gpio_get)	(unsigned iminor);
	void	(*gpio_set)	(unsigned iminor, int state);
	void	(*gpio_change)	(unsigned iminor);
	int	(*gpio_current)	(unsigned iminor);
	struct device*	dev;	/* for dev_dbg() support, set in init  */
};

extern ssize_t nsc_gpio_write(struct file *file, const char __user *data,
			      size_t len, loff_t *ppos);

extern ssize_t nsc_gpio_read(struct file *file, char __user *buf,
			     size_t len, loff_t *ppos);

extern void nsc_gpio_dump(struct nsc_gpio_ops *amp, unsigned index);


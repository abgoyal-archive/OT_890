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
 * Zoran zr36057/zr36067 PCI controller driver, for the
 * Pinnacle/Miro DC10/DC10+/DC30/DC30+, Iomega Buz, Linux
 * Media Labs LML33/LML33R10.
 *
 * This part handles card-specific data and detection
 *
 * Copyright (C) 2000 Serguei Miridonov <mirsev@cicese.mx>
 *
 * Currently maintained by:
 *   Ronald Bultje    <rbultje@ronald.bitfreak.net>
 *   Laurent Pinchart <laurent.pinchart@skynet.be>
 *   Mailinglist      <mjpeg-users@lists.sf.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __ZORAN_DEVICE_H__
#define __ZORAN_DEVICE_H__

/* general purpose I/O */
extern void GPIO(struct zoran *zr,
		 int bit,
		 unsigned int value);

/* codec (or actually: guest bus) access */
extern int post_office_wait(struct zoran *zr);
extern int post_office_write(struct zoran *zr,
			     unsigned guest,
			     unsigned reg,
			     unsigned value);
extern int post_office_read(struct zoran *zr,
			    unsigned guest,
			    unsigned reg);

extern void detect_guest_activity(struct zoran *zr);

extern void jpeg_codec_sleep(struct zoran *zr,
			     int sleep);
extern int jpeg_codec_reset(struct zoran *zr);

/* zr360x7 access to raw capture */
extern void zr36057_overlay(struct zoran *zr,
			    int on);
extern void write_overlay_mask(struct file *file,
			       struct video_clip *vp,
			       int count);
extern void zr36057_set_memgrab(struct zoran *zr,
				int mode);
extern int wait_grab_pending(struct zoran *zr);

/* interrupts */
extern void print_interrupts(struct zoran *zr);
extern void clear_interrupt_counters(struct zoran *zr);
extern irqreturn_t zoran_irq(int irq, void *dev_id);

/* JPEG codec access */
extern void jpeg_start(struct zoran *zr);
extern void zr36057_enable_jpg(struct zoran *zr,
			       enum zoran_codec_mode mode);
extern void zoran_feed_stat_com(struct zoran *zr);

/* general */
extern void zoran_set_pci_master(struct zoran *zr,
				 int set_master);
extern void zoran_init_hardware(struct zoran *zr);
extern void zr36057_restart(struct zoran *zr);

extern const struct zoran_format zoran_formats[];

extern int v4l_nbufs;
extern int v4l_bufsize;
extern int jpg_nbufs;
extern int jpg_bufsize;
extern int pass_through;

/* i2c */
extern int decoder_command(struct zoran *zr,
			   int cmd,
			   void *data);
extern int encoder_command(struct zoran *zr,
			   int cmd,
			   void *data);

#endif				/* __ZORAN_DEVICE_H__ */

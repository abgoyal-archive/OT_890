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
 *      sr.h by David Giller
 *      CD-ROM disk driver header file
 *      
 *      adapted from:
 *      sd.h Copyright (C) 1992 Drew Eckhardt 
 *      SCSI disk driver header file by
 *              Drew Eckhardt 
 *
 *      <drew@colorado.edu>
 *
 *       Modified by Eric Youngdale eric@andante.org to
 *       add scatter-gather, multiple outstanding request, and other
 *       enhancements.
 */

#ifndef _SR_H
#define _SR_H

#include <linux/genhd.h>
#include <linux/kref.h>

#define MAX_RETRIES	3
#define SR_TIMEOUT	(30 * HZ)

struct scsi_device;

/* The CDROM is fairly slow, so we need a little extra time */
/* In fact, it is very slow if it has to spin up first */
#define IOCTL_TIMEOUT 30*HZ


typedef struct scsi_cd {
	struct scsi_driver *driver;
	unsigned capacity;	/* size in blocks                       */
	struct scsi_device *device;
	unsigned int vendor;	/* vendor code, see sr_vendor.c         */
	unsigned long ms_offset;	/* for reading multisession-CD's        */
	unsigned use:1;		/* is this device still supportable     */
	unsigned xa_flag:1;	/* CD has XA sectors ? */
	unsigned readcd_known:1;	/* drive supports READ_CD (0xbe) */
	unsigned readcd_cdda:1;	/* reading audio data using READ_CD */
	unsigned previous_state:1;	/* media has changed */
	struct cdrom_device_info cdi;
	/* We hold gendisk and scsi_device references on probe and use
	 * the refs on this kref to decide when to release them */
	struct kref kref;
	struct gendisk *disk;
} Scsi_CD;

int sr_do_ioctl(Scsi_CD *, struct packet_command *);

int sr_lock_door(struct cdrom_device_info *, int);
int sr_tray_move(struct cdrom_device_info *, int);
int sr_drive_status(struct cdrom_device_info *, int);
int sr_disk_status(struct cdrom_device_info *);
int sr_get_last_session(struct cdrom_device_info *, struct cdrom_multisession *);
int sr_get_mcn(struct cdrom_device_info *, struct cdrom_mcn *);
int sr_reset(struct cdrom_device_info *);
int sr_select_speed(struct cdrom_device_info *cdi, int speed);
int sr_audio_ioctl(struct cdrom_device_info *, unsigned int, void *);

int sr_is_xa(Scsi_CD *);
int sr_test_unit_ready(struct scsi_device *sdev, struct scsi_sense_hdr *sshdr);

/* sr_vendor.c */
void sr_vendor_init(Scsi_CD *);
int sr_cd_check(struct cdrom_device_info *);
int sr_set_blocklength(Scsi_CD *, int blocklength);

#endif

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

#include <linux/kernel.h>
#include <linux/ide.h>
#include <linux/hdreg.h>

#include "ide-disk.h"

static int smart_enable(ide_drive_t *drive)
{
	ide_task_t args;
	struct ide_taskfile *tf = &args.tf;

	memset(&args, 0, sizeof(ide_task_t));
	tf->feature = ATA_SMART_ENABLE;
	tf->lbam    = ATA_SMART_LBAM_PASS;
	tf->lbah    = ATA_SMART_LBAH_PASS;
	tf->command = ATA_CMD_SMART;
	args.tf_flags = IDE_TFLAG_TF | IDE_TFLAG_DEVICE;
	return ide_no_data_taskfile(drive, &args);
}

static int get_smart_data(ide_drive_t *drive, u8 *buf, u8 sub_cmd)
{
	ide_task_t args;
	struct ide_taskfile *tf = &args.tf;

	memset(&args, 0, sizeof(ide_task_t));
	tf->feature = sub_cmd;
	tf->nsect   = 0x01;
	tf->lbam    = ATA_SMART_LBAM_PASS;
	tf->lbah    = ATA_SMART_LBAH_PASS;
	tf->command = ATA_CMD_SMART;
	args.tf_flags	= IDE_TFLAG_TF | IDE_TFLAG_DEVICE;
	args.data_phase	= TASKFILE_IN;
	(void) smart_enable(drive);
	return ide_raw_taskfile(drive, &args, buf, 1);
}

static int proc_idedisk_read_cache
	(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	ide_drive_t	*drive = (ide_drive_t *) data;
	char		*out = page;
	int		len;

	if (drive->dev_flags & IDE_DFLAG_ID_READ)
		len = sprintf(out, "%i\n", drive->id[ATA_ID_BUF_SIZE] / 2);
	else
		len = sprintf(out, "(none)\n");

	PROC_IDE_READ_RETURN(page, start, off, count, eof, len);
}

static int proc_idedisk_read_capacity
	(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	ide_drive_t*drive = (ide_drive_t *)data;
	int len;

	len = sprintf(page, "%llu\n", (long long)ide_gd_capacity(drive));

	PROC_IDE_READ_RETURN(page, start, off, count, eof, len);
}

static int proc_idedisk_read_smart(char *page, char **start, off_t off,
				   int count, int *eof, void *data, u8 sub_cmd)
{
	ide_drive_t	*drive = (ide_drive_t *)data;
	int		len = 0, i = 0;

	if (get_smart_data(drive, page, sub_cmd) == 0) {
		unsigned short *val = (unsigned short *) page;
		char *out = (char *)val + SECTOR_SIZE;

		page = out;
		do {
			out += sprintf(out, "%04x%c", le16_to_cpu(*val),
				       (++i & 7) ? ' ' : '\n');
			val += 1;
		} while (i < SECTOR_SIZE / 2);
		len = out - page;
	}

	PROC_IDE_READ_RETURN(page, start, off, count, eof, len);
}

static int proc_idedisk_read_sv
	(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	return proc_idedisk_read_smart(page, start, off, count, eof, data,
				       ATA_SMART_READ_VALUES);
}

static int proc_idedisk_read_st
	(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	return proc_idedisk_read_smart(page, start, off, count, eof, data,
				       ATA_SMART_READ_THRESHOLDS);
}

ide_proc_entry_t ide_disk_proc[] = {
	{ "cache",	  S_IFREG|S_IRUGO, proc_idedisk_read_cache,    NULL },
	{ "capacity",	  S_IFREG|S_IRUGO, proc_idedisk_read_capacity, NULL },
	{ "geometry",	  S_IFREG|S_IRUGO, proc_ide_read_geometry,     NULL },
	{ "smart_values", S_IFREG|S_IRUSR, proc_idedisk_read_sv,       NULL },
	{ "smart_thresholds", S_IFREG|S_IRUSR, proc_idedisk_read_st,   NULL },
	{ NULL, 0, NULL, NULL }
};

ide_devset_rw_field(bios_cyl, bios_cyl);
ide_devset_rw_field(bios_head, bios_head);
ide_devset_rw_field(bios_sect, bios_sect);
ide_devset_rw_field(failures, failures);
ide_devset_rw_field(lun, lun);
ide_devset_rw_field(max_failures, max_failures);

const struct ide_proc_devset ide_disk_settings[] = {
	IDE_PROC_DEVSET(acoustic,	0,   254),
	IDE_PROC_DEVSET(address,	0,     2),
	IDE_PROC_DEVSET(bios_cyl,	0, 65535),
	IDE_PROC_DEVSET(bios_head,	0,   255),
	IDE_PROC_DEVSET(bios_sect,	0,    63),
	IDE_PROC_DEVSET(failures,	0, 65535),
	IDE_PROC_DEVSET(lun,		0,     7),
	IDE_PROC_DEVSET(max_failures,	0, 65535),
	IDE_PROC_DEVSET(multcount,	0,    16),
	IDE_PROC_DEVSET(nowerr,		0,     1),
	IDE_PROC_DEVSET(wcache,		0,     1),
	{ NULL },
};

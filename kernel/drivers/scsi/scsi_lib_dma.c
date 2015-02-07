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
 * SCSI library functions depending on DMA
 */

#include <linux/blkdev.h>
#include <linux/device.h>
#include <linux/kernel.h>

#include <scsi/scsi.h>
#include <scsi/scsi_cmnd.h>
#include <scsi/scsi_device.h>
#include <scsi/scsi_host.h>

/**
 * scsi_dma_map - perform DMA mapping against command's sg lists
 * @cmd:	scsi command
 *
 * Returns the number of sg lists actually used, zero if the sg lists
 * is NULL, or -ENOMEM if the mapping failed.
 */
int scsi_dma_map(struct scsi_cmnd *cmd)
{
	int nseg = 0;

	if (scsi_sg_count(cmd)) {
		struct device *dev = cmd->device->host->shost_gendev.parent;

		nseg = dma_map_sg(dev, scsi_sglist(cmd), scsi_sg_count(cmd),
				  cmd->sc_data_direction);
		if (unlikely(!nseg))
			return -ENOMEM;
	}
	return nseg;
}
EXPORT_SYMBOL(scsi_dma_map);

/**
 * scsi_dma_unmap - unmap command's sg lists mapped by scsi_dma_map
 * @cmd:	scsi command
 */
void scsi_dma_unmap(struct scsi_cmnd *cmd)
{
	if (scsi_sg_count(cmd)) {
		struct device *dev = cmd->device->host->shost_gendev.parent;

		dma_unmap_sg(dev, scsi_sglist(cmd), scsi_sg_count(cmd),
			     cmd->sc_data_direction);
	}
}
EXPORT_SYMBOL(scsi_dma_unmap);

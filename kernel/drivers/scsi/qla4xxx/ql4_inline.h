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
 * QLogic iSCSI HBA Driver
 * Copyright (c)  2003-2006 QLogic Corporation
 *
 * See LICENSE.qla4xxx for copyright and licensing details.
 */

/*
 *
 * qla4xxx_lookup_ddb_by_fw_index
 *	This routine locates a device handle given the firmware device
 *	database index.	 If device doesn't exist, returns NULL.
 *
 * Input:
 *	ha - Pointer to host adapter structure.
 *	fw_ddb_index - Firmware's device database index
 *
 * Returns:
 *	Pointer to the corresponding internal device database structure
 */
static inline struct ddb_entry *
qla4xxx_lookup_ddb_by_fw_index(struct scsi_qla_host *ha, uint32_t fw_ddb_index)
{
	struct ddb_entry *ddb_entry = NULL;

	if ((fw_ddb_index < MAX_DDB_ENTRIES) &&
	    (ha->fw_ddb_index_map[fw_ddb_index] !=
		(struct ddb_entry *) INVALID_ENTRY)) {
		ddb_entry = ha->fw_ddb_index_map[fw_ddb_index];
	}

	DEBUG3(printk("scsi%d: %s: index [%d], ddb_entry = %p\n",
	    ha->host_no, __func__, fw_ddb_index, ddb_entry));

	return ddb_entry;
}

static inline void
__qla4xxx_enable_intrs(struct scsi_qla_host *ha)
{
	if (is_qla4022(ha) | is_qla4032(ha)) {
		writel(set_rmask(IMR_SCSI_INTR_ENABLE),
		       &ha->reg->u1.isp4022.intr_mask);
		readl(&ha->reg->u1.isp4022.intr_mask);
	} else {
		writel(set_rmask(CSR_SCSI_INTR_ENABLE), &ha->reg->ctrl_status);
		readl(&ha->reg->ctrl_status);
	}
	set_bit(AF_INTERRUPTS_ON, &ha->flags);
}

static inline void
__qla4xxx_disable_intrs(struct scsi_qla_host *ha)
{
	if (is_qla4022(ha) | is_qla4032(ha)) {
		writel(clr_rmask(IMR_SCSI_INTR_ENABLE),
		       &ha->reg->u1.isp4022.intr_mask);
		readl(&ha->reg->u1.isp4022.intr_mask);
	} else {
		writel(clr_rmask(CSR_SCSI_INTR_ENABLE), &ha->reg->ctrl_status);
		readl(&ha->reg->ctrl_status);
	}
	clear_bit(AF_INTERRUPTS_ON, &ha->flags);
}

static inline void
qla4xxx_enable_intrs(struct scsi_qla_host *ha)
{
	unsigned long flags;

	spin_lock_irqsave(&ha->hardware_lock, flags);
	__qla4xxx_enable_intrs(ha);
	spin_unlock_irqrestore(&ha->hardware_lock, flags);
}

static inline void
qla4xxx_disable_intrs(struct scsi_qla_host *ha)
{
	unsigned long flags;

	spin_lock_irqsave(&ha->hardware_lock, flags);
	__qla4xxx_disable_intrs(ha);
	spin_unlock_irqrestore(&ha->hardware_lock, flags);
}

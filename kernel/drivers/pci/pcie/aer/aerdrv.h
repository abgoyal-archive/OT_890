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
 * Copyright (C) 2006 Intel Corp.
 *	Tom Long Nguyen (tom.l.nguyen@intel.com)
 *	Zhang Yanmin (yanmin.zhang@intel.com)
 *
 */

#ifndef _AERDRV_H_
#define _AERDRV_H_

#include <linux/workqueue.h>
#include <linux/pcieport_if.h>
#include <linux/aer.h>

#define AER_NONFATAL			0
#define AER_FATAL			1
#define AER_CORRECTABLE			2
#define AER_UNCORRECTABLE		4
#define AER_ERROR_MASK			0x001fffff
#define AER_ERROR(d)			(d & AER_ERROR_MASK)

/* Root Error Status Register Bits */
#define ROOT_ERR_STATUS_MASKS			0x0f

#define SYSTEM_ERROR_INTR_ON_MESG_MASK	(PCI_EXP_RTCTL_SECEE|	\
					PCI_EXP_RTCTL_SENFEE|	\
					PCI_EXP_RTCTL_SEFEE)
#define ROOT_PORT_INTR_ON_MESG_MASK	(PCI_ERR_ROOT_CMD_COR_EN|	\
					PCI_ERR_ROOT_CMD_NONFATAL_EN|	\
					PCI_ERR_ROOT_CMD_FATAL_EN)
#define ERR_COR_ID(d)			(d & 0xffff)
#define ERR_UNCOR_ID(d)			(d >> 16)

#define AER_SUCCESS			0
#define AER_UNSUCCESS			1
#define AER_ERROR_SOURCES_MAX		100

#define AER_LOG_TLP_MASKS		(PCI_ERR_UNC_POISON_TLP|	\
					PCI_ERR_UNC_ECRC|		\
					PCI_ERR_UNC_UNSUP|		\
					PCI_ERR_UNC_COMP_ABORT|		\
					PCI_ERR_UNC_UNX_COMP|		\
					PCI_ERR_UNC_MALF_TLP)

/* AER Error Info Flags */
#define AER_TLP_HEADER_VALID_FLAG	0x00000001
#define AER_MULTI_ERROR_VALID_FLAG	0x00000002

#define ERR_CORRECTABLE_ERROR_MASK	0x000031c1
#define ERR_UNCORRECTABLE_ERROR_MASK	0x001ff010

struct header_log_regs {
	unsigned int dw0;
	unsigned int dw1;
	unsigned int dw2;
	unsigned int dw3;
};

struct aer_err_info {
	int severity;			/* 0:NONFATAL | 1:FATAL | 2:COR */
	int flags;
	unsigned int status;		/* COR/UNCOR Error Status */
	struct header_log_regs tlp; 	/* TLP Header */
};

struct aer_err_source {
	unsigned int status;
	unsigned int id;
};

struct aer_rpc {
	struct pcie_device *rpd;	/* Root Port device */
	struct work_struct dpc_handler;
	struct aer_err_source e_sources[AER_ERROR_SOURCES_MAX];
	unsigned short prod_idx;	/* Error Producer Index */
	unsigned short cons_idx;	/* Error Consumer Index */
	int isr;
	spinlock_t e_lock;		/*
					 * Lock access to Error Status/ID Regs
					 * and error producer/consumer index
					 */
	struct mutex rpc_mutex;		/*
					 * only one thread could do
					 * recovery on the same
					 * root port hierarchy
					 */
	wait_queue_head_t wait_release;
};

struct aer_broadcast_data {
	enum pci_channel_state state;
	enum pci_ers_result result;
};

static inline pci_ers_result_t merge_result(enum pci_ers_result orig,
		enum pci_ers_result new)
{
	switch (orig) {
	case PCI_ERS_RESULT_CAN_RECOVER:
	case PCI_ERS_RESULT_RECOVERED:
		orig = new;
		break;
	case PCI_ERS_RESULT_DISCONNECT:
		if (new == PCI_ERS_RESULT_NEED_RESET)
			orig = new;
		break;
	default:
		break;
	}

	return orig;
}

extern struct bus_type pcie_port_bus_type;
extern void aer_enable_rootport(struct aer_rpc *rpc);
extern void aer_delete_rootport(struct aer_rpc *rpc);
extern int aer_init(struct pcie_device *dev);
extern void aer_isr(struct work_struct *work);
extern void aer_print_error(struct pci_dev *dev, struct aer_err_info *info);

#ifdef CONFIG_ACPI
extern int aer_osc_setup(struct pcie_device *pciedev);
#else
static inline int aer_osc_setup(struct pcie_device *pciedev)
{
	return 0;
}
#endif

#endif //_AERDRV_H_

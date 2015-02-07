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

/* $Id$ */

#ifndef __DIVA_OS_XDI_ADAPTER_H__
#define __DIVA_OS_XDI_ADAPTER_H__

#define DIVAS_XDI_ADAPTER_BUS_PCI  0
#define DIVAS_XDI_ADAPTER_BUS_ISA  1

typedef struct _divas_pci_card_resources {
	byte bus;
	byte func;
	void *hdev;

	dword bar[8];		/* contains context of appropriate BAR Register */
	void __iomem *addr[8];		/* same bar, but mapped into memory */
	dword length[8];	/* bar length */
	int mem_type_id[MAX_MEM_TYPE];
	unsigned int qoffset;
	byte irq;
} divas_pci_card_resources_t;

typedef union _divas_card_resources {
	divas_pci_card_resources_t pci;
} divas_card_resources_t;

struct _diva_os_xdi_adapter;
typedef int (*diva_init_card_proc_t) (struct _diva_os_xdi_adapter * a);
typedef int (*diva_cmd_card_proc_t) (struct _diva_os_xdi_adapter * a,
				     diva_xdi_um_cfg_cmd_t * data,
				     int length);
typedef void (*diva_xdi_clear_interrupts_proc_t) (struct
						  _diva_os_xdi_adapter *);

#define DIVA_XDI_MBOX_BUSY			1
#define DIVA_XDI_MBOX_WAIT_XLOG	2

typedef struct _xdi_mbox_t {
	dword status;
	diva_xdi_um_cfg_cmd_data_t cmd_data;
	dword data_length;
	void *data;
} xdi_mbox_t;

typedef struct _diva_os_idi_adapter_interface {
	diva_init_card_proc_t cleanup_adapter_proc;
	diva_cmd_card_proc_t cmd_proc;
} diva_os_idi_adapter_interface_t;

typedef struct _diva_os_xdi_adapter {
	struct list_head link;
	int CardIndex;
	int CardOrdinal;
	int controller;		/* number of this controller */
	int Bus;		/* PCI, ISA, ... */
	divas_card_resources_t resources;
	char port_name[24];
	ISDN_ADAPTER xdi_adapter;
	xdi_mbox_t xdi_mbox;
	diva_os_idi_adapter_interface_t interface;
	struct _diva_os_xdi_adapter *slave_adapters[3];
	void *slave_list;
	void *proc_adapter_dir;	/* adapterX proc entry */
	void *proc_info;	/* info proc entry     */
	void *proc_grp_opt;	/* group_optimization  */
	void *proc_d_l1_down;	/* dynamic_l1_down     */
	volatile diva_xdi_clear_interrupts_proc_t clear_interrupts_proc;
	dword dsp_mask;
} diva_os_xdi_adapter_t;

#endif

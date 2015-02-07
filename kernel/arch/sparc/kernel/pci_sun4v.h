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

/* pci_sun4v.h: SUN4V specific PCI controller support.
 *
 * Copyright (C) 2006 David S. Miller (davem@davemloft.net)
 */

#ifndef _PCI_SUN4V_H
#define _PCI_SUN4V_H

extern long pci_sun4v_iommu_map(unsigned long devhandle,
				unsigned long tsbid,
				unsigned long num_ttes,
				unsigned long io_attributes,
				unsigned long io_page_list_pa);
extern unsigned long pci_sun4v_iommu_demap(unsigned long devhandle,
					   unsigned long tsbid,
					   unsigned long num_ttes);
extern unsigned long pci_sun4v_iommu_getmap(unsigned long devhandle,
					    unsigned long tsbid,
					    unsigned long *io_attributes,
					    unsigned long *real_address);
extern unsigned long pci_sun4v_config_get(unsigned long devhandle,
					  unsigned long pci_device,
					  unsigned long config_offset,
					  unsigned long size);
extern int pci_sun4v_config_put(unsigned long devhandle,
				unsigned long pci_device,
				unsigned long config_offset,
				unsigned long size,
				unsigned long data);

extern unsigned long pci_sun4v_msiq_conf(unsigned long devhandle,
					 unsigned long msiqid,
					 unsigned long msiq_paddr,
					 unsigned long num_entries);
extern unsigned long pci_sun4v_msiq_info(unsigned long devhandle,
					 unsigned long msiqid,
					 unsigned long *msiq_paddr,
					 unsigned long *num_entries);
extern unsigned long pci_sun4v_msiq_getvalid(unsigned long devhandle,
					     unsigned long msiqid,
					     unsigned long *valid);
extern unsigned long pci_sun4v_msiq_setvalid(unsigned long devhandle,
					     unsigned long msiqid,
					     unsigned long valid);
extern unsigned long pci_sun4v_msiq_getstate(unsigned long devhandle,
					     unsigned long msiqid,
					     unsigned long *state);
extern unsigned long pci_sun4v_msiq_setstate(unsigned long devhandle,
					     unsigned long msiqid,
					     unsigned long state);
extern unsigned long pci_sun4v_msiq_gethead(unsigned long devhandle,
					     unsigned long msiqid,
					     unsigned long *head);
extern unsigned long pci_sun4v_msiq_sethead(unsigned long devhandle,
					     unsigned long msiqid,
					     unsigned long head);
extern unsigned long pci_sun4v_msiq_gettail(unsigned long devhandle,
					     unsigned long msiqid,
					     unsigned long *head);
extern unsigned long pci_sun4v_msi_getvalid(unsigned long devhandle,
					    unsigned long msinum,
					    unsigned long *valid);
extern unsigned long pci_sun4v_msi_setvalid(unsigned long devhandle,
					    unsigned long msinum,
					    unsigned long valid);
extern unsigned long pci_sun4v_msi_getmsiq(unsigned long devhandle,
					   unsigned long msinum,
					   unsigned long *msiq);
extern unsigned long pci_sun4v_msi_setmsiq(unsigned long devhandle,
					   unsigned long msinum,
					   unsigned long msiq,
					   unsigned long msitype);
extern unsigned long pci_sun4v_msi_getstate(unsigned long devhandle,
					    unsigned long msinum,
					    unsigned long *state);
extern unsigned long pci_sun4v_msi_setstate(unsigned long devhandle,
					    unsigned long msinum,
					    unsigned long state);
extern unsigned long pci_sun4v_msg_getmsiq(unsigned long devhandle,
					   unsigned long msinum,
					   unsigned long *msiq);
extern unsigned long pci_sun4v_msg_setmsiq(unsigned long devhandle,
					   unsigned long msinum,
					   unsigned long msiq);
extern unsigned long pci_sun4v_msg_getvalid(unsigned long devhandle,
					    unsigned long msinum,
					    unsigned long *valid);
extern unsigned long pci_sun4v_msg_setvalid(unsigned long devhandle,
					    unsigned long msinum,
					    unsigned long valid);

#endif /* !(_PCI_SUN4V_H) */

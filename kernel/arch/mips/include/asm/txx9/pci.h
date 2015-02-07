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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#ifndef __ASM_TXX9_PCI_H
#define __ASM_TXX9_PCI_H

#include <linux/pci.h>

extern struct pci_controller txx9_primary_pcic;
struct pci_controller *
txx9_alloc_pci_controller(struct pci_controller *pcic,
			  unsigned long mem_base, unsigned long mem_size,
			  unsigned long io_base, unsigned long io_size);

int txx9_pci66_check(struct pci_controller *hose, int top_bus,
			int current_bus);
extern int txx9_pci_mem_high __initdata;

extern int txx9_pci_option;
#define TXX9_PCI_OPT_PICMG	0x0002
#define TXX9_PCI_OPT_CLK_33	0x0008
#define TXX9_PCI_OPT_CLK_66	0x0010
#define TXX9_PCI_OPT_CLK_MASK	\
	(TXX9_PCI_OPT_CLK_33 | TXX9_PCI_OPT_CLK_66)
#define TXX9_PCI_OPT_CLK_AUTO	TXX9_PCI_OPT_CLK_MASK

enum txx9_pci_err_action {
	TXX9_PCI_ERR_REPORT,
	TXX9_PCI_ERR_IGNORE,
	TXX9_PCI_ERR_PANIC,
};
extern enum txx9_pci_err_action txx9_pci_err_action;

extern char * (*txx9_board_pcibios_setup)(char *str);
char *txx9_pcibios_setup(char *str);

#endif /* __ASM_TXX9_PCI_H */

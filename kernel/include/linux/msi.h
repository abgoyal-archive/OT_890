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

#ifndef LINUX_MSI_H
#define LINUX_MSI_H

#include <linux/list.h>

struct msi_msg {
	u32	address_lo;	/* low 32 bits of msi message address */
	u32	address_hi;	/* high 32 bits of msi message address */
	u32	data;		/* 16 bits of msi message data */
};

/* Helper functions */
struct irq_desc;
extern void mask_msi_irq(unsigned int irq);
extern void unmask_msi_irq(unsigned int irq);
extern void read_msi_msg_desc(struct irq_desc *desc, struct msi_msg *msg);
extern void write_msi_msg_desc(struct irq_desc *desc, struct msi_msg *msg);
extern void read_msi_msg(unsigned int irq, struct msi_msg *msg);
extern void write_msi_msg(unsigned int irq, struct msi_msg *msg);

struct msi_desc {
	struct {
		__u8	type	: 5; 	/* {0: unused, 5h:MSI, 11h:MSI-X} */
		__u8	maskbit	: 1; 	/* mask-pending bit supported ?   */
		__u8	masked	: 1;
		__u8	is_64	: 1;	/* Address size: 0=32bit 1=64bit  */
		__u8	pos;	 	/* Location of the msi capability */
		__u32	maskbits_mask;  /* mask bits mask */
		__u16	entry_nr;    	/* specific enabled entry 	  */
		unsigned default_irq;	/* default pre-assigned irq	  */
	}msi_attrib;

	unsigned int irq;
	struct list_head list;

	void __iomem *mask_base;
	struct pci_dev *dev;

	/* Last set MSI message */
	struct msi_msg msg;
};

/*
 * The arch hook for setup up msi irqs
 */
int arch_setup_msi_irq(struct pci_dev *dev, struct msi_desc *desc);
void arch_teardown_msi_irq(unsigned int irq);
extern int arch_setup_msi_irqs(struct pci_dev *dev, int nvec, int type);
extern void arch_teardown_msi_irqs(struct pci_dev *dev);
extern int arch_msi_check_device(struct pci_dev* dev, int nvec, int type);


#endif /* LINUX_MSI_H */

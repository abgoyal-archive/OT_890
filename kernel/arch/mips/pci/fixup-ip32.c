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

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <asm/ip32/ip32_ints.h>
/*
 * O2 has up to 5 PCI devices connected into the MACE bridge.  The device
 * map looks like this:
 *
 * 0  aic7xxx 0
 * 1  aic7xxx 1
 * 2  expansion slot
 * 3  N/C
 * 4  N/C
 */

#define SCSI0  MACEPCI_SCSI0_IRQ
#define SCSI1  MACEPCI_SCSI1_IRQ
#define INTA0  MACEPCI_SLOT0_IRQ
#define INTA1  MACEPCI_SLOT1_IRQ
#define INTA2  MACEPCI_SLOT2_IRQ
#define INTB   MACEPCI_SHARED0_IRQ
#define INTC   MACEPCI_SHARED1_IRQ
#define INTD   MACEPCI_SHARED2_IRQ
static char irq_tab_mace[][5] __initdata = {
      /* Dummy  INT#A  INT#B  INT#C  INT#D */
	{0,         0,     0,     0,     0}, /* This is placeholder row - never used */
	{0,     SCSI0, SCSI0, SCSI0, SCSI0},
	{0,     SCSI1, SCSI1, SCSI1, SCSI1},
	{0,     INTA0,  INTB,  INTC,  INTD},
	{0,     INTA1,  INTC,  INTD,  INTB},
	{0,     INTA2,  INTD,  INTB,  INTC},
};


/*
 * Given a PCI slot number (a la PCI_SLOT(...)) and the interrupt pin of
 * the device (1-4 => A-D), tell what irq to use.  Note that we don't
 * in theory have slots 4 and 5, and we never normally use the shared
 * irqs.  I suppose a device without a pin A will thank us for doing it
 * right if there exists such a broken piece of crap.
 */
int __init pcibios_map_irq(const struct pci_dev *dev, u8 slot, u8 pin)
{
	return irq_tab_mace[slot][pin];
}

/* Do platform specific device initialization at pci_enable_device() time */
int pcibios_plat_dev_init(struct pci_dev *dev)
{
	return 0;
}

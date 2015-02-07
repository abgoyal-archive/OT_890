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
 *	pci_syscall.c
 *
 * For architectures where we want to allow direct access
 * to the PCI config stuff - it would probably be preferable
 * on PCs too, but there people just do it by hand with the
 * magic northbridge registers..
 */

#include <linux/errno.h>
#include <linux/pci.h>
#include <linux/smp_lock.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>
#include "pci.h"

SYSCALL_DEFINE5(pciconfig_read, unsigned long, bus, unsigned long, dfn,
		unsigned long, off, unsigned long, len, void __user *, buf)
{
	struct pci_dev *dev;
	u8 byte;
	u16 word;
	u32 dword;
	long err;
	long cfg_ret;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	err = -ENODEV;
	dev = pci_get_bus_and_slot(bus, dfn);
	if (!dev)
		goto error;

	switch (len) {
	case 1:
		cfg_ret = pci_user_read_config_byte(dev, off, &byte);
		break;
	case 2:
		cfg_ret = pci_user_read_config_word(dev, off, &word);
		break;
	case 4:
		cfg_ret = pci_user_read_config_dword(dev, off, &dword);
		break;
	default:
		err = -EINVAL;
		goto error;
	};

	err = -EIO;
	if (cfg_ret != PCIBIOS_SUCCESSFUL)
		goto error;

	switch (len) {
	case 1:
		err = put_user(byte, (unsigned char __user *)buf);
		break;
	case 2:
		err = put_user(word, (unsigned short __user *)buf);
		break;
	case 4:
		err = put_user(dword, (unsigned int __user *)buf);
		break;
	}
	pci_dev_put(dev);
	return err;

error:
	/* ??? XFree86 doesn't even check the return value.  They
	   just look for 0xffffffff in the output, since that's what
	   they get instead of a machine check on x86.  */
	switch (len) {
	case 1:
		put_user(-1, (unsigned char __user *)buf);
		break;
	case 2:
		put_user(-1, (unsigned short __user *)buf);
		break;
	case 4:
		put_user(-1, (unsigned int __user *)buf);
		break;
	}
	pci_dev_put(dev);
	return err;
}

SYSCALL_DEFINE5(pciconfig_write, unsigned long, bus, unsigned long, dfn,
		unsigned long, off, unsigned long, len, void __user *, buf)
{
	struct pci_dev *dev;
	u8 byte;
	u16 word;
	u32 dword;
	int err = 0;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	dev = pci_get_bus_and_slot(bus, dfn);
	if (!dev)
		return -ENODEV;

	switch(len) {
	case 1:
		err = get_user(byte, (u8 __user *)buf);
		if (err)
			break;
		err = pci_user_write_config_byte(dev, off, byte);
		if (err != PCIBIOS_SUCCESSFUL)
			err = -EIO;
		break;

	case 2:
		err = get_user(word, (u16 __user *)buf);
		if (err)
			break;
		err = pci_user_write_config_word(dev, off, word);
		if (err != PCIBIOS_SUCCESSFUL)
			err = -EIO;
		break;

	case 4:
		err = get_user(dword, (u32 __user *)buf);
		if (err)
			break;
		err = pci_user_write_config_dword(dev, off, dword);
		if (err != PCIBIOS_SUCCESSFUL)
			err = -EIO;
		break;

	default:
		err = -EINVAL;
		break;
	}
	pci_dev_put(dev);
	return err;
}

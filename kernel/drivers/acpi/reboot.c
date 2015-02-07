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


#include <linux/pci.h>
#include <linux/acpi.h>
#include <acpi/reboot.h>

void acpi_reboot(void)
{
	struct acpi_generic_address *rr;
	struct pci_bus *bus0;
	u8 reset_value;
	unsigned int devfn;

	if (acpi_disabled)
		return;

	rr = &acpi_gbl_FADT.reset_register;

	/* Is the reset register supported? */
	if (!(acpi_gbl_FADT.flags & ACPI_FADT_RESET_REGISTER) ||
	    rr->bit_width != 8 || rr->bit_offset != 0)
		return;

	reset_value = acpi_gbl_FADT.reset_value;

	/* The reset register can only exist in I/O, Memory or PCI config space
	 * on a device on bus 0. */
	switch (rr->space_id) {
	case ACPI_ADR_SPACE_PCI_CONFIG:
		/* The reset register can only live on bus 0. */
		bus0 = pci_find_bus(0, 0);
		if (!bus0)
			return;
		/* Form PCI device/function pair. */
		devfn = PCI_DEVFN((rr->address >> 32) & 0xffff,
				  (rr->address >> 16) & 0xffff);
		printk(KERN_DEBUG "Resetting with ACPI PCI RESET_REG.");
		/* Write the value that resets us. */
		pci_bus_write_config_byte(bus0, devfn,
				(rr->address & 0xffff), reset_value);
		break;

	case ACPI_ADR_SPACE_SYSTEM_MEMORY:
	case ACPI_ADR_SPACE_SYSTEM_IO:
		printk(KERN_DEBUG "ACPI MEMORY or I/O RESET_REG.\n");
		acpi_reset();
		break;
	}
	/* Wait ten seconds */
	acpi_os_stall(10000000);
}

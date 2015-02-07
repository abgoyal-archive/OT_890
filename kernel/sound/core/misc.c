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
 *  Misc and compatibility things
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>
 *
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

#include <linux/init.h>
#include <linux/time.h>
#include <linux/ioport.h>
#include <sound/core.h>

void release_and_free_resource(struct resource *res)
{
	if (res) {
		release_resource(res);
		kfree(res);
	}
}

EXPORT_SYMBOL(release_and_free_resource);

#ifdef CONFIG_SND_VERBOSE_PRINTK
void snd_verbose_printk(const char *file, int line, const char *format, ...)
{
	va_list args;
	
	if (format[0] == '<' && format[1] >= '0' && format[1] <= '7' && format[2] == '>') {
		char tmp[] = "<0>";
		tmp[1] = format[1];
		printk("%sALSA %s:%d: ", tmp, file, line);
		format += 3;
	} else {
		printk("ALSA %s:%d: ", file, line);
	}
	va_start(args, format);
	vprintk(format, args);
	va_end(args);
}

EXPORT_SYMBOL(snd_verbose_printk);
#endif

#if defined(CONFIG_SND_DEBUG) && defined(CONFIG_SND_VERBOSE_PRINTK)
void snd_verbose_printd(const char *file, int line, const char *format, ...)
{
	va_list args;
	
	if (format[0] == '<' && format[1] >= '0' && format[1] <= '7' && format[2] == '>') {
		char tmp[] = "<0>";
		tmp[1] = format[1];
		printk("%sALSA %s:%d: ", tmp, file, line);
		format += 3;
	} else {
		printk(KERN_DEBUG "ALSA %s:%d: ", file, line);
	}
	va_start(args, format);
	vprintk(format, args);
	va_end(args);

}

EXPORT_SYMBOL(snd_verbose_printd);
#endif

#ifdef CONFIG_PCI
#include <linux/pci.h>
/**
 * snd_pci_quirk_lookup - look up a PCI SSID quirk list
 * @pci: pci_dev handle
 * @list: quirk list, terminated by a null entry
 *
 * Look through the given quirk list and finds a matching entry
 * with the same PCI SSID.  When subdevice is 0, all subdevice
 * values may match.
 *
 * Returns the matched entry pointer, or NULL if nothing matched.
 */
const struct snd_pci_quirk *
snd_pci_quirk_lookup(struct pci_dev *pci, const struct snd_pci_quirk *list)
{
	const struct snd_pci_quirk *q;

	for (q = list; q->subvendor; q++)
		if (q->subvendor == pci->subsystem_vendor &&
		    (!q->subdevice || q->subdevice == pci->subsystem_device))
			return q;
	return NULL;
}

EXPORT_SYMBOL(snd_pci_quirk_lookup);
#endif

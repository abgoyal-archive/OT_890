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
 * Mapping of the rootfs in a physical region of memory
 *
 * Copyright (C) 2005-2007 PMC-Sierra Inc.
 * Author: Andrew Hughes, Andrew_Hughes@pmc-sierra.com
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *  THIS  SOFTWARE  IS PROVIDED   ``AS  IS'' AND   ANY  EXPRESS OR IMPLIED
 *  WARRANTIES,   INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO  EVENT  SHALL   THE AUTHOR  BE    LIABLE FOR ANY   DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED   TO, PROCUREMENT OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF
 *  USE, DATA,  OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/root_dev.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>

#include <asm/io.h>

#include <msp_prom.h>

static struct mtd_info *rr_mtd;

struct map_info rr_map = {
	.name = "ramroot",
	.bankwidth = 4,
};

static int __init init_rrmap(void)
{
	void *ramroot_start;
	unsigned long ramroot_size;

	/* Check for supported rootfs types */
	if (get_ramroot(&ramroot_start, &ramroot_size)) {
		rr_map.phys = CPHYSADDR(ramroot_start);
		rr_map.size = ramroot_size;

		printk(KERN_NOTICE
			"PMC embedded root device: 0x%08lx @ 0x%08lx\n",
			rr_map.size, (unsigned long)rr_map.phys);
	} else {
		printk(KERN_ERR
			"init_rrmap: no supported embedded rootfs detected!\n");
		return -ENXIO;
	}

	/* Map rootfs to I/O space for block device driver */
	rr_map.virt = ioremap(rr_map.phys, rr_map.size);
	if (!rr_map.virt) {
		printk(KERN_ERR "Failed to ioremap\n");
		return -EIO;
	}

	simple_map_init(&rr_map);

	rr_mtd = do_map_probe("map_ram", &rr_map);
	if (rr_mtd) {
		rr_mtd->owner = THIS_MODULE;

		add_mtd_device(rr_mtd);

		return 0;
	}

	iounmap(rr_map.virt);
	return -ENXIO;
}

static void __exit cleanup_rrmap(void)
{
	del_mtd_device(rr_mtd);
	map_destroy(rr_mtd);

	iounmap(rr_map.virt);
	rr_map.virt = NULL;
}

MODULE_AUTHOR("PMC-Sierra, Inc");
MODULE_DESCRIPTION("MTD map driver for embedded PMC-Sierra MSP filesystem");
MODULE_LICENSE("GPL");

module_init(init_rrmap);
module_exit(cleanup_rrmap);

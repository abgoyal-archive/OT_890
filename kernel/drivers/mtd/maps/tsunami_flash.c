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
 * tsunami_flash.c
 *
 * flash chip on alpha ds10...
 */
#include <asm/io.h>
#include <asm/core_tsunami.h>
#include <linux/init.h>
#include <linux/mtd/map.h>
#include <linux/mtd/mtd.h>

#define FLASH_ENABLE_PORT 0x00C00001
#define FLASH_ENABLE_BYTE 0x01
#define FLASH_DISABLE_BYTE 0x00

#define MAX_TIG_FLASH_SIZE (12*1024*1024)
static inline map_word tsunami_flash_read8(struct map_info *map, unsigned long offset)
{
	map_word val;
	val.x[0] = tsunami_tig_readb(offset);
	return val;
}

static void tsunami_flash_write8(struct map_info *map, map_word value, unsigned long offset)
{
	tsunami_tig_writeb(value.x[0], offset);
}

static void tsunami_flash_copy_from(
	struct map_info *map, void *addr, unsigned long offset, ssize_t len)
{
	unsigned char *dest;
	dest = addr;
	while(len && (offset < MAX_TIG_FLASH_SIZE)) {
		*dest = tsunami_tig_readb(offset);
		offset++;
		dest++;
		len--;
	}
}

static void tsunami_flash_copy_to(
	struct map_info *map, unsigned long offset,
	const void *addr, ssize_t len)
{
	const unsigned char *src;
	src = addr;
	while(len && (offset < MAX_TIG_FLASH_SIZE)) {
		tsunami_tig_writeb(*src, offset);
		offset++;
		src++;
		len--;
	}
}

/*
 * Deliberately don't provide operations wider than 8 bits.  I don't
 * have then and it scares me to think how you could mess up if
 * you tried to use them.   Buswidth is correctly so I'm safe.
 */
static struct map_info tsunami_flash_map = {
	.name = "flash chip on the Tsunami TIG bus",
	.size = MAX_TIG_FLASH_SIZE,
	.phys = NO_XIP,
	.bankwidth = 1,
	.read = tsunami_flash_read8,
	.copy_from = tsunami_flash_copy_from,
	.write = tsunami_flash_write8,
	.copy_to = tsunami_flash_copy_to,
};

static struct mtd_info *tsunami_flash_mtd;

static void __exit  cleanup_tsunami_flash(void)
{
	struct mtd_info *mtd;
	mtd = tsunami_flash_mtd;
	if (mtd) {
		del_mtd_device(mtd);
		map_destroy(mtd);
	}
	tsunami_flash_mtd = 0;
}


static int __init init_tsunami_flash(void)
{
	static const char *rom_probe_types[] = { "cfi_probe", "jedec_probe", "map_rom", NULL };
	char **type;

	tsunami_tig_writeb(FLASH_ENABLE_BYTE, FLASH_ENABLE_PORT);

	tsunami_flash_mtd = 0;
	type = rom_probe_types;
	for(; !tsunami_flash_mtd && *type; type++) {
		tsunami_flash_mtd = do_map_probe(*type, &tsunami_flash_map);
	}
	if (tsunami_flash_mtd) {
		tsunami_flash_mtd->owner = THIS_MODULE;
		add_mtd_device(tsunami_flash_mtd);
		return 0;
	}
	return -ENXIO;
}

module_init(init_tsunami_flash);
module_exit(cleanup_tsunami_flash);

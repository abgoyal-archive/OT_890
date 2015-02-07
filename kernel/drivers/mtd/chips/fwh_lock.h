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

#ifndef FWH_LOCK_H
#define FWH_LOCK_H


enum fwh_lock_state {
        FWH_UNLOCKED   = 0,
	FWH_DENY_WRITE = 1,
	FWH_IMMUTABLE  = 2,
	FWH_DENY_READ  = 4,
};

struct fwh_xxlock_thunk {
	enum fwh_lock_state val;
	flstate_t state;
};


#define FWH_XXLOCK_ONEBLOCK_LOCK   ((struct fwh_xxlock_thunk){ FWH_DENY_WRITE, FL_LOCKING})
#define FWH_XXLOCK_ONEBLOCK_UNLOCK ((struct fwh_xxlock_thunk){ FWH_UNLOCKED,   FL_UNLOCKING})

/*
 * This locking/unlock is specific to firmware hub parts.  Only one
 * is known that supports the Intel command set.    Firmware
 * hub parts cannot be interleaved as they are on the LPC bus
 * so this code has not been tested with interleaved chips,
 * and will likely fail in that context.
 */
static int fwh_xxlock_oneblock(struct map_info *map, struct flchip *chip,
	unsigned long adr, int len, void *thunk)
{
	struct cfi_private *cfi = map->fldrv_priv;
	struct fwh_xxlock_thunk *xxlt = (struct fwh_xxlock_thunk *)thunk;
	int ret;

	/* Refuse the operation if the we cannot look behind the chip */
	if (chip->start < 0x400000) {
		DEBUG( MTD_DEBUG_LEVEL3,
			"MTD %s(): chip->start: %lx wanted >= 0x400000\n",
			__func__, chip->start );
		return -EIO;
	}
	/*
	 * lock block registers:
	 * - on 64k boundariesand
	 * - bit 1 set high
	 * - block lock registers are 4MiB lower - overflow subtract (danger)
	 *
	 * The address manipulation is first done on the logical address
	 * which is 0 at the start of the chip, and then the offset of
	 * the individual chip is addted to it.  Any other order a weird
	 * map offset could cause problems.
	 */
	adr = (adr & ~0xffffUL) | 0x2;
	adr += chip->start - 0x400000;

	/*
	 * This is easy because these are writes to registers and not writes
	 * to flash memory - that means that we don't have to check status
	 * and timeout.
	 */
	spin_lock(chip->mutex);
	ret = get_chip(map, chip, adr, FL_LOCKING);
	if (ret) {
		spin_unlock(chip->mutex);
		return ret;
	}

	chip->oldstate = chip->state;
	chip->state = xxlt->state;
	map_write(map, CMD(xxlt->val), adr);

	/* Done and happy. */
	chip->state = chip->oldstate;
	put_chip(map, chip, adr);
	spin_unlock(chip->mutex);
	return 0;
}


static int fwh_lock_varsize(struct mtd_info *mtd, loff_t ofs, uint64_t len)
{
	int ret;

	ret = cfi_varsize_frob(mtd, fwh_xxlock_oneblock, ofs, len,
		(void *)&FWH_XXLOCK_ONEBLOCK_LOCK);

	return ret;
}


static int fwh_unlock_varsize(struct mtd_info *mtd, loff_t ofs, uint64_t len)
{
	int ret;

	ret = cfi_varsize_frob(mtd, fwh_xxlock_oneblock, ofs, len,
		(void *)&FWH_XXLOCK_ONEBLOCK_UNLOCK);

	return ret;
}

static void fixup_use_fwh_lock(struct mtd_info *mtd, void *param)
{
	printk(KERN_NOTICE "using fwh lock/unlock method\n");
	/* Setup for the chips with the fwh lock method */
	mtd->lock   = fwh_lock_varsize;
	mtd->unlock = fwh_unlock_varsize;
}
#endif /* FWH_LOCK_H */

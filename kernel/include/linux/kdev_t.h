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

#ifndef _LINUX_KDEV_T_H
#define _LINUX_KDEV_T_H
#ifdef __KERNEL__
#define MINORBITS	20
#define MINORMASK	((1U << MINORBITS) - 1)

#define MAJOR(dev)	((unsigned int) ((dev) >> MINORBITS))
#define MINOR(dev)	((unsigned int) ((dev) & MINORMASK))
#define MKDEV(ma,mi)	(((ma) << MINORBITS) | (mi))

#define print_dev_t(buffer, dev)					\
	sprintf((buffer), "%u:%u\n", MAJOR(dev), MINOR(dev))

#define format_dev_t(buffer, dev)					\
	({								\
		sprintf(buffer, "%u:%u", MAJOR(dev), MINOR(dev));	\
		buffer;							\
	})

/* acceptable for old filesystems */
static inline int old_valid_dev(dev_t dev)
{
	return MAJOR(dev) < 256 && MINOR(dev) < 256;
}

static inline u16 old_encode_dev(dev_t dev)
{
	return (MAJOR(dev) << 8) | MINOR(dev);
}

static inline dev_t old_decode_dev(u16 val)
{
	return MKDEV((val >> 8) & 255, val & 255);
}

static inline int new_valid_dev(dev_t dev)
{
	return 1;
}

static inline u32 new_encode_dev(dev_t dev)
{
	unsigned major = MAJOR(dev);
	unsigned minor = MINOR(dev);
	return (minor & 0xff) | (major << 8) | ((minor & ~0xff) << 12);
}

static inline dev_t new_decode_dev(u32 dev)
{
	unsigned major = (dev & 0xfff00) >> 8;
	unsigned minor = (dev & 0xff) | ((dev >> 12) & 0xfff00);
	return MKDEV(major, minor);
}

static inline int huge_valid_dev(dev_t dev)
{
	return 1;
}

static inline u64 huge_encode_dev(dev_t dev)
{
	return new_encode_dev(dev);
}

static inline dev_t huge_decode_dev(u64 dev)
{
	return new_decode_dev(dev);
}

static inline int sysv_valid_dev(dev_t dev)
{
	return MAJOR(dev) < (1<<14) && MINOR(dev) < (1<<18);
}

static inline u32 sysv_encode_dev(dev_t dev)
{
	return MINOR(dev) | (MAJOR(dev) << 18);
}

static inline unsigned sysv_major(u32 dev)
{
	return (dev >> 18) & 0x3fff;
}

static inline unsigned sysv_minor(u32 dev)
{
	return dev & 0x3ffff;
}

#else /* __KERNEL__ */

/*
Some programs want their definitions of MAJOR and MINOR and MKDEV
from the kernel sources. These must be the externally visible ones.
*/
#define MAJOR(dev)	((dev)>>8)
#define MINOR(dev)	((dev) & 0xff)
#define MKDEV(ma,mi)	((ma)<<8 | (mi))
#endif /* __KERNEL__ */
#endif

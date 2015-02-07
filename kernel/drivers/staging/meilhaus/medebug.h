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

/**
 * @file medebug.h
 *
 * @brief Debugging defines.
 * @note Copyright (C) 2006 Meilhaus Electronic GmbH (support@meilhaus.de)
 * @author Guenter Gebhardt
 * @author Krzysztof Gantzke	(k.gantzke@meilhaus.de)
 */

#ifndef _MEDEBUG_H_
#define _MEDEBUG_H_

#ifdef __KERNEL__

#include <linux/kernel.h>

//Messages control.

#ifdef MEDEBUG_TEST_ALL		/* Switch to enable all info messages. */
# ifndef MEDEBUG_TEST
#  define MEDEBUG_TEST
# endif
# ifndef MEDEBUG_TEST_INFO
#  define MEDEBUG_TEST_INFO
# endif
# ifndef MEDEBUG_DEBUG_REG
#  define MEDEBUG_DEBUG_REG	/* Switch to enable registry access debuging messages. */
# endif
# ifndef MEDEBUG_DEBUG_LOCKS
#  define MEDEBUG_DEBUG_LOCKS	/* Switch to enable locking messages. */
# endif
#endif

#ifdef MEDEBUG_TEST_INFO	/* Switch to enable info  and test messages. */
# ifndef MEDEBUG_INFO
#  define MEDEBUG_INFO		/* Switch to enable info messages. */
# endif
# ifndef MEDEBUG_TEST
#  define MEDEBUG_TEST
# endif
#endif

#ifdef MEDEBUG_TEST		/* Switch to enable debug test messages. */
# ifndef MEDEBUG_DEBUG
#  define MEDEBUG_DEBUG		/* Switch to enable debug messages. */
# endif
# ifndef MEDEBUG_ERROR
#  define MEDEBUG_ERROR		/* Switch to enable error messages. */
# endif
#endif

#ifdef MEDEBUG_ERROR		/* Switch to enable error messages. */
# ifndef MEDEBUG_ERROR_CRITICAL	/* Also critical error messages. */
#  define MEDEBUG_ERROR_CRITICAL	/* Switch to enable high importance error messages. */
# endif
#endif

#undef PDEBUG			/* Only to be sure. */
#undef PINFO			/* Only to be sure. */
#undef PERROR			/* Only to be sure. */
#undef PERROR_CRITICAL		/* Only to be sure. */
#undef PDEBUG_REG		/* Only to be sure. */
#undef PDEBUG_LOCKS		/* Only to be sure. */
#undef PSECURITY		/* Only to be sure. */
#undef PLOG			/* Only to be sure. */

#ifdef MEDEBUG_DEBUG
# define PDEBUG(fmt, args...) \
	printk(KERN_DEBUG"ME_DRV D: <%s> " fmt, __func__, ##args)
#else
# define PDEBUG(fmt, args...)
#endif

#ifdef MEDEBUG_DEBUG_LOCKS
# define PDEBUG_LOCKS(fmt, args...) \
	printk(KERN_DEBUG"ME_DRV L: <%s> " fmt, __func__, ##args)
#else
# define PDEBUG_LOCKS(fmt, args...)
#endif

#ifdef MEDEBUG_DEBUG_REG
# define PDEBUG_REG(fmt, args...) \
	printk(KERN_DEBUG"ME_DRV R: <%s:%d> REG:" fmt, __func__, __LINE__, ##args)
#else
# define PDEBUG_REG(fmt, args...)
#endif

#ifdef MEDEBUG_INFO
# define PINFO(fmt, args...) \
	printk(KERN_INFO"ME_DRV I: " fmt, ##args)
#else
# define PINFO(fmt, args...)
#endif

#ifdef MEDEBUG_ERROR
# define PERROR(fmt, args...) \
	printk(KERN_ERR"ME_DRV E: <%s:%i> " fmt, __FILE__, __LINE__, ##args)
#else
# define PERROR(fmt, args...)
#endif

#ifdef MEDEBUG_ERROR_CRITICAL
# define PERROR_CRITICAL(fmt, args...) \
	printk(KERN_CRIT"ME_DRV C: <%s:%i> " fmt, __FILE__, __LINE__, ##args)
#else
# define PERROR_CRITICAL(fmt, args...)
#endif

//This debug is only to detect logical errors!
# define PSECURITY(fmt, args...) \
	printk(KERN_CRIT"ME_DRV SECURITY: <%s:%s:%i> " fmt, __FILE__, __func__, __LINE__, ##args)
//This debug is to keep track in customers' system
# define PLOG(fmt, args...) \
	printk(KERN_INFO"ME_DRV: " fmt, ##args)

//This debug is to check new parts during development
#ifdef MEDEBUG_DEVELOP
# define PDEVELOP(fmt, args...) \
	printk(KERN_CRIT"ME_DRV: <%s:%s:%i> " fmt, __FILE__, __func__, __LINE__, ##args)
#else
# define PDEVELOP(fmt, args...)
#endif

#endif //__KERNEL__
#endif //_MEDEBUG_H_

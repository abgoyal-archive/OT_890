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
 * 1. load vmlinuz
 *
 * CONFIG_MEMORY_START  	+-----------------------+
 *				|        vmlinuz	|
 *				+-----------------------+
 * 2. decompressed
 *
 * CONFIG_MEMORY_START  	+-----------------------+
 *				|        vmlinuz	|
 *				+-----------------------+
 *				|			|
 * BOOT_RELOC_ADDR		+-----------------------+
 *				|		 	|
 * KERNEL_DECOMPRESS_ADDR 	+-----------------------+
 *				|  	vmlinux		|
 *				+-----------------------+
 *
 * 3. relocate copy & jump code
 *
 * CONFIG_MEMORY_START  	+-----------------------+
 *				|        vmlinuz	|
 *				+-----------------------+
 *				|			|
 * BOOT_RELOC_ADDR		+-----------------------+
 *				|    boot(copy&jump)	|
 * KERNEL_DECOMPRESS_ADDR 	+-----------------------+
 *				|  	vmlinux		|
 *				+-----------------------+
 *
 * 4. relocate decompressed kernel
 *
 * CONFIG_MEMORY_START  	+-----------------------+
 *				|        vmlinux	|
 *				+-----------------------+
 *				|			|
 * BOOT_RELOC_ADDR		+-----------------------+
 *				|     boot(copy&jump) 	|
 * KERNEL_DECOMPRESS_ADDR 	+-----------------------+
 *				|  			|
 *				+-----------------------+
 *
 */
#ifdef __ASSEMBLY__
#define __val(x)	x
#else
#define __val(x)	(x)
#endif

#define DECOMPRESS_OFFSET_BASE	__val(0x00900000)
#define BOOT_RELOC_SIZE		__val(0x00001000)

#define KERNEL_EXEC_ADDR	__val(CONFIG_MEMORY_START)
#define KERNEL_DECOMPRESS_ADDR	__val(CONFIG_MEMORY_START + \
				      DECOMPRESS_OFFSET_BASE + BOOT_RELOC_SIZE)
#define KERNEL_ENTRY		__val(CONFIG_MEMORY_START + 0x1000)

#define BOOT_EXEC_ADDR		__val(CONFIG_MEMORY_START)
#define BOOT_RELOC_ADDR		__val(CONFIG_MEMORY_START + DECOMPRESS_OFFSET_BASE)

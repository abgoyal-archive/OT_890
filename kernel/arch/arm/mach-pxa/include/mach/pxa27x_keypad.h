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

#ifndef __ASM_ARCH_PXA27x_KEYPAD_H
#define __ASM_ARCH_PXA27x_KEYPAD_H

#include <linux/input.h>

#define MAX_MATRIX_KEY_ROWS	(8)
#define MAX_MATRIX_KEY_COLS	(8)

/* pxa3xx keypad platform specific parameters
 *
 * NOTE:
 * 1. direct_key_num indicates the number of keys in the direct keypad
 *    _plus_ the number of rotary-encoder sensor inputs,  this can be
 *    left as 0 if only rotary encoders are enabled,  the driver will
 *    automatically calculate this
 *
 * 2. direct_key_map is the key code map for the direct keys, if rotary
 *    encoder(s) are enabled, direct key 0/1(2/3) will be ignored
 *
 * 3. rotary can be either interpreted as a relative input event (e.g.
 *    REL_WHEEL/REL_HWHEEL) or specific keys (e.g. UP/DOWN/LEFT/RIGHT)
 *
 * 4. matrix key and direct key will use the same debounce_interval by
 *    default, which should be sufficient in most cases
 */
struct pxa27x_keypad_platform_data {

	/* code map for the matrix keys */
	unsigned int	matrix_key_rows;
	unsigned int	matrix_key_cols;
	unsigned int	*matrix_key_map;
	int		matrix_key_map_size;

	/* direct keys */
	int		direct_key_num;
	unsigned int	direct_key_map[8];

	/* rotary encoders 0 */
	int		enable_rotary0;
	int		rotary0_rel_code;
	int		rotary0_up_key;
	int		rotary0_down_key;

	/* rotary encoders 1 */
	int		enable_rotary1;
	int		rotary1_rel_code;
	int		rotary1_up_key;
	int		rotary1_down_key;

	/* key debounce interval */
	unsigned int	debounce_interval;
};

#define KEY(row, col, val)	(((row) << 28) | ((col) << 24) | (val))

extern void pxa_set_keypad_info(struct pxa27x_keypad_platform_data *info);

#endif /* __ASM_ARCH_PXA27x_KEYPAD_H */

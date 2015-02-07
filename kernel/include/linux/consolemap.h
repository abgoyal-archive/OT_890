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
 * consolemap.h
 *
 * Interface between console.c, selection.c  and consolemap.c
 */
#ifndef __LINUX_CONSOLEMAP_H__
#define __LINUX_CONSOLEMAP_H__

#define LAT1_MAP 0
#define GRAF_MAP 1
#define IBMPC_MAP 2
#define USER_MAP 3

#include <linux/types.h>

#ifdef CONFIG_CONSOLE_TRANSLATIONS
struct vc_data;

extern u16 inverse_translate(struct vc_data *conp, int glyph, int use_unicode);
extern unsigned short *set_translate(int m, struct vc_data *vc);
extern int conv_uni_to_pc(struct vc_data *conp, long ucs);
extern u32 conv_8bit_to_uni(unsigned char c);
extern int conv_uni_to_8bit(u32 uni);
void console_map_init(void);
#else
#define inverse_translate(conp, glyph, uni) ((uint16_t)glyph)
#define set_translate(m, vc) ((unsigned short *)NULL)
#define conv_uni_to_pc(conp, ucs) ((int) (ucs > 0xff ? -1: ucs))
#define conv_8bit_to_uni(c) ((uint32_t)(c))
#define conv_uni_to_8bit(c) ((int) ((c) & 0xff))
#define console_map_init(c) do { ; } while (0)
#endif /* CONFIG_CONSOLE_TRANSLATIONS */

#endif /* __LINUX_CONSOLEMAP_H__ */

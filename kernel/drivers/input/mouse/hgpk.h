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
 * OLPC HGPK (XO-1) touchpad PS/2 mouse driver
 */

#ifndef _HGPK_H
#define _HGPK_H

enum hgpk_model_t {
	HGPK_MODEL_PREA = 0x0a,	/* pre-B1s */
	HGPK_MODEL_A = 0x14,	/* found on B1s, PT disabled in hardware */
	HGPK_MODEL_B = 0x28,	/* B2s, has capacitance issues */
	HGPK_MODEL_C = 0x3c,
	HGPK_MODEL_D = 0x50,	/* C1, mass production */
};

struct hgpk_data {
	struct psmouse *psmouse;
	int powered;
	int count, x_tally, y_tally;	/* hardware workaround stuff */
	unsigned long recalib_window;
	struct delayed_work recalib_wq;
};

#define hgpk_dbg(psmouse, format, arg...)		\
	dev_dbg(&(psmouse)->ps2dev.serio->dev, format, ## arg)
#define hgpk_err(psmouse, format, arg...)		\
	dev_err(&(psmouse)->ps2dev.serio->dev, format, ## arg)
#define hgpk_info(psmouse, format, arg...)		\
	dev_info(&(psmouse)->ps2dev.serio->dev, format, ## arg)
#define hgpk_warn(psmouse, format, arg...)		\
	dev_warn(&(psmouse)->ps2dev.serio->dev, format, ## arg)
#define hgpk_notice(psmouse, format, arg...)		\
	dev_notice(&(psmouse)->ps2dev.serio->dev, format, ## arg)

#ifdef CONFIG_MOUSE_PS2_OLPC
int hgpk_detect(struct psmouse *psmouse, int set_properties);
int hgpk_init(struct psmouse *psmouse);
#else
static inline int hgpk_detect(struct psmouse *psmouse, int set_properties)
{
	return -ENODEV;
}
static inline int hgpk_init(struct psmouse *psmouse)
{
	return -ENODEV;
}
#endif

#endif

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

/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/nv/nv_proto.h,v 1.10 2003/07/31 20:24:29 mvojkovi Exp $ */

#ifndef __NV_PROTO_H__
#define __NV_PROTO_H__

/* in nv_setup.c */
int NVCommonSetup(struct fb_info *info);
void NVWriteCrtc(struct nvidia_par *par, u8 index, u8 value);
u8 NVReadCrtc(struct nvidia_par *par, u8 index);
void NVWriteGr(struct nvidia_par *par, u8 index, u8 value);
u8 NVReadGr(struct nvidia_par *par, u8 index);
void NVWriteSeq(struct nvidia_par *par, u8 index, u8 value);
u8 NVReadSeq(struct nvidia_par *par, u8 index);
void NVWriteAttr(struct nvidia_par *par, u8 index, u8 value);
u8 NVReadAttr(struct nvidia_par *par, u8 index);
void NVWriteMiscOut(struct nvidia_par *par, u8 value);
u8 NVReadMiscOut(struct nvidia_par *par);
void NVWriteDacMask(struct nvidia_par *par, u8 value);
void NVWriteDacReadAddr(struct nvidia_par *par, u8 value);
void NVWriteDacWriteAddr(struct nvidia_par *par, u8 value);
void NVWriteDacData(struct nvidia_par *par, u8 value);
u8 NVReadDacData(struct nvidia_par *par);

/* in nv_hw.c */
void NVCalcStateExt(struct nvidia_par *par, struct _riva_hw_state *,
		    int, int, int, int, int, int);
void NVLoadStateExt(struct nvidia_par *par, struct _riva_hw_state *);
void NVUnloadStateExt(struct nvidia_par *par, struct _riva_hw_state *);
void NVSetStartAddress(struct nvidia_par *par, u32);
int NVShowHideCursor(struct nvidia_par *par, int);
void NVLockUnlock(struct nvidia_par *par, int);

/* in nvidia-i2c.c */
#ifdef CONFIG_FB_NVIDIA_I2C
void nvidia_create_i2c_busses(struct nvidia_par *par);
void nvidia_delete_i2c_busses(struct nvidia_par *par);
int nvidia_probe_i2c_connector(struct fb_info *info, int conn,
			       u8 ** out_edid);
#else
#define nvidia_create_i2c_busses(...)
#define nvidia_delete_i2c_busses(...)
#define nvidia_probe_i2c_connector(p, c, edid) (-1)
#endif

#ifdef CONFIG_PPC_OF
int nvidia_probe_of_connector(struct fb_info *info, int conn,
			      u8 ** out_edid);
#else
static inline int nvidia_probe_of_connector(struct fb_info *info, int conn,
				      u8 ** out_edid)
{
	return -1;
}
#endif

/* in nv_accel.c */
extern void NVResetGraphics(struct fb_info *info);
extern void nvidiafb_copyarea(struct fb_info *info,
			      const struct fb_copyarea *region);
extern void nvidiafb_fillrect(struct fb_info *info,
			      const struct fb_fillrect *rect);
extern void nvidiafb_imageblit(struct fb_info *info,
			       const struct fb_image *image);
extern int nvidiafb_sync(struct fb_info *info);

/* in nv_backlight.h */
#ifdef CONFIG_FB_NVIDIA_BACKLIGHT
extern void nvidia_bl_init(struct nvidia_par *par);
extern void nvidia_bl_exit(struct nvidia_par *par);
#else
static inline void nvidia_bl_init(struct nvidia_par *par) {}
static inline void nvidia_bl_exit(struct nvidia_par *par) {}
#endif

#endif				/* __NV_PROTO_H__ */

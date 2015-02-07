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

#ifndef _LINUX_SVGA_H
#define _LINUX_SVGA_H

#include <linux/pci.h>
#include <video/vga.h>

/* Terminator for register set */

#define VGA_REGSET_END_VAL	0xFF
#define VGA_REGSET_END		{VGA_REGSET_END_VAL, 0, 0}

struct vga_regset {
	u8 regnum;
	u8 lowbit;
	u8 highbit;
};

/* ------------------------------------------------------------------------- */

#define SVGA_FORMAT_END_VAL	0xFFFF
#define SVGA_FORMAT_END		{SVGA_FORMAT_END_VAL, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 0, 0, 0, 0, 0}

struct svga_fb_format {
	/* var part */
	u32 bits_per_pixel;
	struct fb_bitfield red;
	struct fb_bitfield green;
	struct fb_bitfield blue;
	struct fb_bitfield transp;
	u32 nonstd;
	/* fix part */
	u32 type;
	u32 type_aux;
	u32 visual;
	u32 xpanstep;
	u32 xresstep;
};

struct svga_timing_regs {
	const struct vga_regset *h_total_regs;
	const struct vga_regset *h_display_regs;
	const struct vga_regset *h_blank_start_regs;
	const struct vga_regset *h_blank_end_regs;
	const struct vga_regset *h_sync_start_regs;
	const struct vga_regset *h_sync_end_regs;

	const struct vga_regset *v_total_regs;
	const struct vga_regset *v_display_regs;
	const struct vga_regset *v_blank_start_regs;
	const struct vga_regset *v_blank_end_regs;
	const struct vga_regset *v_sync_start_regs;
	const struct vga_regset *v_sync_end_regs;
};

struct svga_pll {
	u16 m_min;
	u16 m_max;
	u16 n_min;
	u16 n_max;
	u16 r_min;
	u16 r_max;  /* r_max < 32 */
	u32 f_vco_min;
	u32 f_vco_max;
	u32 f_base;
};


/* Write a value to the attribute register */

static inline void svga_wattr(u8 index, u8 data)
{
	inb(0x3DA);
	outb(index, 0x3C0);
	outb(data, 0x3C0);
}

/* Write a value to a sequence register with a mask */

static inline void svga_wseq_mask(u8 index, u8 data, u8 mask)
{
	vga_wseq(NULL, index, (data & mask) | (vga_rseq(NULL, index) & ~mask));
}

/* Write a value to a CRT register with a mask */

static inline void svga_wcrt_mask(u8 index, u8 data, u8 mask)
{
	vga_wcrt(NULL, index, (data & mask) | (vga_rcrt(NULL, index) & ~mask));
}

static inline int svga_primary_device(struct pci_dev *dev)
{
	u16 flags;
	pci_read_config_word(dev, PCI_COMMAND, &flags);
	return (flags & PCI_COMMAND_IO);
}


void svga_wcrt_multi(const struct vga_regset *regset, u32 value);
void svga_wseq_multi(const struct vga_regset *regset, u32 value);

void svga_set_default_gfx_regs(void);
void svga_set_default_atc_regs(void);
void svga_set_default_seq_regs(void);
void svga_set_default_crt_regs(void);
void svga_set_textmode_vga_regs(void);

void svga_settile(struct fb_info *info, struct fb_tilemap *map);
void svga_tilecopy(struct fb_info *info, struct fb_tilearea *area);
void svga_tilefill(struct fb_info *info, struct fb_tilerect *rect);
void svga_tileblit(struct fb_info *info, struct fb_tileblit *blit);
void svga_tilecursor(struct fb_info *info, struct fb_tilecursor *cursor);
int svga_get_tilemax(struct fb_info *info);
void svga_get_caps(struct fb_info *info, struct fb_blit_caps *caps,
		   struct fb_var_screeninfo *var);

int svga_compute_pll(const struct svga_pll *pll, u32 f_wanted, u16 *m, u16 *n, u16 *r, int node);
int svga_check_timings(const struct svga_timing_regs *tm, struct fb_var_screeninfo *var, int node);
void svga_set_timings(const struct svga_timing_regs *tm, struct fb_var_screeninfo *var, u32 hmul, u32 hdiv, u32 vmul, u32 vdiv, u32 hborder, int node);

int svga_match_format(const struct svga_fb_format *frm, struct fb_var_screeninfo *var, struct fb_fix_screeninfo *fix);

#endif /* _LINUX_SVGA_H */


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
 *  The driver for the Cirrus Logic's Sound Fusion CS46XX based soundcards
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>
 *
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

#ifndef __CS46XX_LIB_H__
#define __CS46XX_LIB_H__

/*
 *  constants
 */

#define CS46XX_BA0_SIZE		  0x1000
#define CS46XX_BA1_DATA0_SIZE 0x3000
#define CS46XX_BA1_DATA1_SIZE 0x3800
#define CS46XX_BA1_PRG_SIZE	  0x7000
#define CS46XX_BA1_REG_SIZE	  0x0100



#ifdef CONFIG_SND_CS46XX_NEW_DSP
#define CS46XX_MIN_PERIOD_SIZE 1
#define CS46XX_MAX_PERIOD_SIZE 1024*1024
#else
#define CS46XX_MIN_PERIOD_SIZE 2048
#define CS46XX_MAX_PERIOD_SIZE 2048
#endif

#define CS46XX_FRAGS 2
/* #define CS46XX_BUFFER_SIZE CS46XX_MAX_PERIOD_SIZE * CS46XX_FRAGS */

#define SCB_NO_PARENT             0
#define SCB_ON_PARENT_NEXT_SCB    1
#define SCB_ON_PARENT_SUBLIST_SCB 2

/* 3*1024 parameter, 3.5*1024 sample, 2*3.5*1024 code */
#define BA1_DWORD_SIZE		(13 * 1024 + 512)
#define BA1_MEMORY_COUNT	3

/*
 *  common I/O routines
 */

static inline void snd_cs46xx_poke(struct snd_cs46xx *chip, unsigned long reg, unsigned int val)
{
	unsigned int bank = reg >> 16;
	unsigned int offset = reg & 0xffff;

	/*if (bank == 0) printk("snd_cs46xx_poke: %04X - %08X\n",reg >> 2,val); */
	writel(val, chip->region.idx[bank+1].remap_addr + offset);
}

static inline unsigned int snd_cs46xx_peek(struct snd_cs46xx *chip, unsigned long reg)
{
	unsigned int bank = reg >> 16;
	unsigned int offset = reg & 0xffff;
	return readl(chip->region.idx[bank+1].remap_addr + offset);
}

static inline void snd_cs46xx_pokeBA0(struct snd_cs46xx *chip, unsigned long offset, unsigned int val)
{
	writel(val, chip->region.name.ba0.remap_addr + offset);
}

static inline unsigned int snd_cs46xx_peekBA0(struct snd_cs46xx *chip, unsigned long offset)
{
	return readl(chip->region.name.ba0.remap_addr + offset);
}

struct dsp_spos_instance *cs46xx_dsp_spos_create (struct snd_cs46xx * chip);
void cs46xx_dsp_spos_destroy (struct snd_cs46xx * chip);
int cs46xx_dsp_load_module (struct snd_cs46xx * chip, struct dsp_module_desc * module);
#ifdef CONFIG_PM
int cs46xx_dsp_resume(struct snd_cs46xx * chip);
#endif
struct dsp_symbol_entry *cs46xx_dsp_lookup_symbol (struct snd_cs46xx * chip, char * symbol_name,
						   int symbol_type);
#ifdef CONFIG_PROC_FS
int cs46xx_dsp_proc_init (struct snd_card *card, struct snd_cs46xx *chip);
int cs46xx_dsp_proc_done (struct snd_cs46xx *chip);
#else
#define cs46xx_dsp_proc_init(card, chip)
#define cs46xx_dsp_proc_done(chip)
#endif
int cs46xx_dsp_scb_and_task_init (struct snd_cs46xx *chip);
int snd_cs46xx_download (struct snd_cs46xx *chip, u32 *src, unsigned long offset,
			 unsigned long len);
int snd_cs46xx_clear_BA1(struct snd_cs46xx *chip, unsigned long offset, unsigned long len);
int cs46xx_dsp_enable_spdif_out (struct snd_cs46xx *chip);
int cs46xx_dsp_enable_spdif_hw (struct snd_cs46xx *chip);
int cs46xx_dsp_disable_spdif_out (struct snd_cs46xx *chip);
int cs46xx_dsp_enable_spdif_in (struct snd_cs46xx *chip);
int cs46xx_dsp_disable_spdif_in (struct snd_cs46xx *chip);
int cs46xx_dsp_enable_pcm_capture (struct snd_cs46xx *chip);
int cs46xx_dsp_disable_pcm_capture (struct snd_cs46xx *chip);
int cs46xx_dsp_enable_adc_capture (struct snd_cs46xx *chip);
int cs46xx_dsp_disable_adc_capture (struct snd_cs46xx *chip);
int cs46xx_poke_via_dsp (struct snd_cs46xx *chip, u32 address, u32 data);
struct dsp_scb_descriptor * cs46xx_dsp_create_scb (struct snd_cs46xx *chip, char * name,
						   u32 * scb_data, u32 dest);
#ifdef CONFIG_PROC_FS
void cs46xx_dsp_proc_free_scb_desc (struct dsp_scb_descriptor * scb);
void cs46xx_dsp_proc_register_scb_desc (struct snd_cs46xx *chip,
					struct dsp_scb_descriptor * scb);
#else
#define cs46xx_dsp_proc_free_scb_desc(scb)
#define cs46xx_dsp_proc_register_scb_desc(chip, scb)
#endif
struct dsp_scb_descriptor * cs46xx_dsp_create_timing_master_scb (struct snd_cs46xx *chip);
struct dsp_scb_descriptor *
cs46xx_dsp_create_codec_out_scb(struct snd_cs46xx * chip,
				char * codec_name, u16 channel_disp, u16 fifo_addr,
				u16 child_scb_addr, u32 dest,
				struct dsp_scb_descriptor * parent_scb,
				int scb_child_type);
struct dsp_scb_descriptor *
cs46xx_dsp_create_codec_in_scb(struct snd_cs46xx * chip, char * codec_name,
			       u16 channel_disp, u16 fifo_addr,
			       u16 sample_buffer_addr, u32 dest,
			       struct dsp_scb_descriptor * parent_scb,
			       int scb_child_type);
void cs46xx_dsp_remove_scb (struct snd_cs46xx *chip,
			    struct dsp_scb_descriptor * scb);
struct dsp_scb_descriptor *
cs46xx_dsp_create_codec_in_scb(struct snd_cs46xx * chip, char * codec_name,
			       u16 channel_disp, u16 fifo_addr,
			       u16 sample_buffer_addr, u32 dest,
			       struct dsp_scb_descriptor * parent_scb,
			       int scb_child_type);
struct dsp_scb_descriptor *
cs46xx_dsp_create_src_task_scb(struct snd_cs46xx * chip, char * scb_name,
			       int sample_rate, u16 src_buffer_addr,
			       u16 src_delay_buffer_addr, u32 dest,
			       struct dsp_scb_descriptor * parent_scb,
			       int scb_child_type, int pass_through);
struct dsp_scb_descriptor *
cs46xx_dsp_create_mix_only_scb(struct snd_cs46xx * chip, char * scb_name,
			       u16 mix_buffer_addr, u32 dest,
			       struct dsp_scb_descriptor * parent_scb,
			       int scb_child_type);

struct dsp_scb_descriptor *
cs46xx_dsp_create_vari_decimate_scb(struct snd_cs46xx * chip, char * scb_name,
				    u16 vari_buffer_addr0, u16 vari_buffer_addr1, u32 dest,
				    struct dsp_scb_descriptor * parent_scb,
				    int scb_child_type);
struct dsp_scb_descriptor *
cs46xx_dsp_create_asynch_fg_rx_scb(struct snd_cs46xx * chip, char * scb_name,
				   u32 dest, u16 hfg_scb_address, u16 asynch_buffer_address,
				   struct dsp_scb_descriptor * parent_scb,
				   int scb_child_type);
struct dsp_scb_descriptor *
cs46xx_dsp_create_spio_write_scb(struct snd_cs46xx * chip, char * scb_name, u32 dest,
				 struct dsp_scb_descriptor * parent_scb,
				 int scb_child_type);
struct dsp_scb_descriptor *
cs46xx_dsp_create_mix_to_ostream_scb(struct snd_cs46xx * chip, char * scb_name,
				     u16 mix_buffer_addr, u16 writeback_spb, u32 dest,
				     struct dsp_scb_descriptor * parent_scb,
				     int scb_child_type);
struct dsp_scb_descriptor *
cs46xx_dsp_create_magic_snoop_scb(struct snd_cs46xx * chip, char * scb_name,
				  u32 dest, u16 snoop_buffer_address,
				  struct dsp_scb_descriptor * snoop_scb,
				  struct dsp_scb_descriptor * parent_scb,
				  int scb_child_type);
struct dsp_pcm_channel_descriptor *
cs46xx_dsp_create_pcm_channel (struct snd_cs46xx * chip, u32 sample_rate,
			       void * private_data, u32 hw_dma_addr,
			       int pcm_channel_id);
void cs46xx_dsp_destroy_pcm_channel (struct snd_cs46xx * chip,
				     struct dsp_pcm_channel_descriptor * pcm_channel);
int cs46xx_dsp_pcm_unlink (struct snd_cs46xx * chip,
			   struct dsp_pcm_channel_descriptor * pcm_channel);
int cs46xx_dsp_pcm_link (struct snd_cs46xx * chip,
			 struct dsp_pcm_channel_descriptor * pcm_channel);
struct dsp_scb_descriptor *
cs46xx_add_record_source (struct snd_cs46xx *chip, struct dsp_scb_descriptor * source,
			  u16 addr, char * scb_name);
int cs46xx_src_unlink(struct snd_cs46xx *chip, struct dsp_scb_descriptor * src);
int cs46xx_src_link(struct snd_cs46xx *chip, struct dsp_scb_descriptor * src);
int cs46xx_iec958_pre_open (struct snd_cs46xx *chip);
int cs46xx_iec958_post_close (struct snd_cs46xx *chip);
int cs46xx_dsp_pcm_channel_set_period (struct snd_cs46xx * chip,
				       struct dsp_pcm_channel_descriptor * pcm_channel,
				       int period_size);
int cs46xx_dsp_pcm_ostream_set_period (struct snd_cs46xx * chip, int period_size);
int cs46xx_dsp_set_dac_volume (struct snd_cs46xx * chip, u16 left, u16 right);
int cs46xx_dsp_set_iec958_volume (struct snd_cs46xx * chip, u16 left, u16 right);
#endif /* __CS46XX_LIB_H__ */

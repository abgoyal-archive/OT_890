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
 * Copyright 1998-2008 VIA Technologies, Inc. All Rights Reserved.
 * Copyright 2001-2008 S3 Graphics, Inc. All Rights Reserved.

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTIES OR REPRESENTATIONS; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.See the GNU General Public License
 * for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include "global.h"

static void tmds_register_write(int index, u8 data);
static int tmds_register_read(int index);
static int tmds_register_read_bytes(int index, u8 *buff, int buff_len);
static int check_reduce_blanking_mode(int mode_index,
	int refresh_rate);
static int dvi_get_panel_size_from_DDCv1(void);
static int dvi_get_panel_size_from_DDCv2(void);
static unsigned char dvi_get_panel_info(void);
static int viafb_dvi_query_EDID(void);

static int check_tmds_chip(int device_id_subaddr, int device_id)
{
	if (tmds_register_read(device_id_subaddr) == device_id)
		return OK;
	else
		return FAIL;
}

void viafb_init_dvi_size(void)
{
	DEBUG_MSG(KERN_INFO "viafb_init_dvi_size()\n");
	DEBUG_MSG(KERN_INFO
		"viaparinfo->tmds_setting_info->get_dvi_size_method %d\n",
		  viaparinfo->tmds_setting_info->get_dvi_size_method);

	switch (viaparinfo->tmds_setting_info->get_dvi_size_method) {
	case GET_DVI_SIZE_BY_SYSTEM_BIOS:
		break;
	case GET_DVI_SZIE_BY_HW_STRAPPING:
		break;
	case GET_DVI_SIZE_BY_VGA_BIOS:
	default:
		dvi_get_panel_info();
		break;
	}
	return;
}

int viafb_tmds_trasmitter_identify(void)
{
	unsigned char sr2a = 0, sr1e = 0, sr3e = 0;

	/* Turn on ouputting pad */
	switch (viaparinfo->chip_info->gfx_chip_name) {
	case UNICHROME_K8M890:
	    /*=* DFP Low Pad on *=*/
		sr2a = viafb_read_reg(VIASR, SR2A);
		viafb_write_reg_mask(SR2A, VIASR, 0x03, BIT0 + BIT1);
		break;

	case UNICHROME_P4M900:
	case UNICHROME_P4M890:
		/* DFP Low Pad on */
		sr2a = viafb_read_reg(VIASR, SR2A);
		viafb_write_reg_mask(SR2A, VIASR, 0x03, BIT0 + BIT1);
		/* DVP0 Pad on */
		sr1e = viafb_read_reg(VIASR, SR1E);
		viafb_write_reg_mask(SR1E, VIASR, 0xC0, BIT6 + BIT7);
		break;

	default:
	    /* DVP0/DVP1 Pad on */
		sr1e = viafb_read_reg(VIASR, SR1E);
		viafb_write_reg_mask(SR1E, VIASR, 0xF0, BIT4 +
			BIT5 + BIT6 + BIT7);
	    /* SR3E[1]Multi-function selection:
	    0 = Emulate I2C and DDC bus by GPIO2/3/4. */
		sr3e = viafb_read_reg(VIASR, SR3E);
		viafb_write_reg_mask(SR3E, VIASR, 0x0, BIT5);
		break;
	}

	/* Check for VT1632: */
	viaparinfo->chip_info->tmds_chip_info.tmds_chip_name = VT1632_TMDS;
	viaparinfo->chip_info->
		tmds_chip_info.tmds_chip_slave_addr = VT1632_TMDS_I2C_ADDR;
	viaparinfo->chip_info->tmds_chip_info.i2c_port = I2CPORTINDEX;
	if (check_tmds_chip(VT1632_DEVICE_ID_REG, VT1632_DEVICE_ID) != FAIL) {
		/*
		 * Currently only support 12bits,dual edge,add 24bits mode later
		 */
		tmds_register_write(0x08, 0x3b);

		DEBUG_MSG(KERN_INFO "\n VT1632 TMDS ! \n");
		DEBUG_MSG(KERN_INFO "\n %2d",
			  viaparinfo->chip_info->tmds_chip_info.tmds_chip_name);
		DEBUG_MSG(KERN_INFO "\n %2d",
			  viaparinfo->chip_info->tmds_chip_info.i2c_port);
		return OK;
	} else {
		viaparinfo->chip_info->tmds_chip_info.i2c_port = GPIOPORTINDEX;
		if (check_tmds_chip(VT1632_DEVICE_ID_REG, VT1632_DEVICE_ID)
		    != FAIL) {
			tmds_register_write(0x08, 0x3b);
			DEBUG_MSG(KERN_INFO "\n VT1632 TMDS ! \n");
			DEBUG_MSG(KERN_INFO "\n %2d",
				  viaparinfo->chip_info->
				  tmds_chip_info.tmds_chip_name);
			DEBUG_MSG(KERN_INFO "\n %2d",
				  viaparinfo->chip_info->
				  tmds_chip_info.i2c_port);
			return OK;
		}
	}

	viaparinfo->chip_info->tmds_chip_info.tmds_chip_name = INTEGRATED_TMDS;

	if ((viaparinfo->chip_info->gfx_chip_name == UNICHROME_CX700) &&
	    ((viafb_display_hardware_layout == HW_LAYOUT_DVI_ONLY) ||
	     (viafb_display_hardware_layout == HW_LAYOUT_LCD_DVI))) {
		DEBUG_MSG(KERN_INFO "\n Integrated TMDS ! \n");
		return OK;
	}

	switch (viaparinfo->chip_info->gfx_chip_name) {
	case UNICHROME_K8M890:
		viafb_write_reg(SR2A, VIASR, sr2a);
		break;

	case UNICHROME_P4M900:
	case UNICHROME_P4M890:
		viafb_write_reg(SR2A, VIASR, sr2a);
		viafb_write_reg(SR1E, VIASR, sr1e);
		break;

	default:
		viafb_write_reg(SR1E, VIASR, sr1e);
		viafb_write_reg(SR3E, VIASR, sr3e);
		break;
	}

	viaparinfo->chip_info->
		tmds_chip_info.tmds_chip_name = NON_TMDS_TRANSMITTER;
	viaparinfo->chip_info->tmds_chip_info.
		tmds_chip_slave_addr = VT1632_TMDS_I2C_ADDR;
	return FAIL;
}

static void tmds_register_write(int index, u8 data)
{
	viaparinfo->i2c_stuff.i2c_port =
		viaparinfo->chip_info->tmds_chip_info.i2c_port;

	viafb_i2c_writebyte(viaparinfo->chip_info->tmds_chip_info.
		tmds_chip_slave_addr, index,
		     data);
}

static int tmds_register_read(int index)
{
	u8 data;

	viaparinfo->i2c_stuff.i2c_port =
		viaparinfo->chip_info->tmds_chip_info.i2c_port;
	viafb_i2c_readbyte((u8) viaparinfo->chip_info->
	    tmds_chip_info.tmds_chip_slave_addr,
			(u8) index, &data);
	return data;
}

static int tmds_register_read_bytes(int index, u8 *buff, int buff_len)
{
	viaparinfo->i2c_stuff.i2c_port =
		viaparinfo->chip_info->tmds_chip_info.i2c_port;
	viafb_i2c_readbytes((u8) viaparinfo->chip_info->tmds_chip_info.
			 tmds_chip_slave_addr, (u8) index, buff, buff_len);
	return 0;
}

static int check_reduce_blanking_mode(int mode_index,
	int refresh_rate)
{
	if (refresh_rate != 60)
		return false;

	switch (mode_index) {
		/* Following modes have reduce blanking mode. */
	case VIA_RES_1360X768:
	case VIA_RES_1400X1050:
	case VIA_RES_1440X900:
	case VIA_RES_1600X900:
	case VIA_RES_1680X1050:
	case VIA_RES_1920X1080:
	case VIA_RES_1920X1200:
		break;

	default:
		DEBUG_MSG(KERN_INFO
			  "This dvi mode %d have no reduce blanking mode!\n",
			  mode_index);
		return false;
	}

	return true;
}

/* DVI Set Mode */
void viafb_dvi_set_mode(int video_index, int mode_bpp, int set_iga)
{
	struct VideoModeTable *videoMode = NULL;
	struct crt_mode_table *pDviTiming;
	unsigned long desirePixelClock, maxPixelClock;
	int status = 0;
	videoMode = viafb_get_modetbl_pointer(video_index);
	pDviTiming = videoMode->crtc;
	desirePixelClock = pDviTiming->clk / 1000000;
	maxPixelClock = (unsigned long)viaparinfo->
		tmds_setting_info->max_pixel_clock;

	DEBUG_MSG(KERN_INFO "\nDVI_set_mode!!\n");

	if ((maxPixelClock != 0) && (desirePixelClock > maxPixelClock)) {
		/*Check if reduce-blanking mode is exist */
		status =
		    check_reduce_blanking_mode(video_index,
					       pDviTiming->refresh_rate);
		if (status) {
			video_index += 100;	/*Use reduce-blanking mode */
			videoMode = viafb_get_modetbl_pointer(video_index);
			pDviTiming = videoMode->crtc;
			DEBUG_MSG(KERN_INFO
				  "DVI use reduce blanking mode %d!!\n",
				  video_index);
		}
	}
	viafb_fill_crtc_timing(pDviTiming, video_index, mode_bpp / 8, set_iga);
	viafb_set_output_path(DEVICE_DVI, set_iga,
			viaparinfo->chip_info->tmds_chip_info.output_interface);
}

/* Sense DVI Connector */
int viafb_dvi_sense(void)
{
	u8 RegSR1E = 0, RegSR3E = 0, RegCR6B = 0, RegCR91 = 0,
		RegCR93 = 0, RegCR9B = 0, data;
	int ret = false;

	DEBUG_MSG(KERN_INFO "viafb_dvi_sense!!\n");

	if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_CLE266) {
		/* DI1 Pad on */
		RegSR1E = viafb_read_reg(VIASR, SR1E);
		viafb_write_reg(SR1E, VIASR, RegSR1E | 0x30);

		/* CR6B[0]VCK Input Selection: 1 = External clock. */
		RegCR6B = viafb_read_reg(VIACR, CR6B);
		viafb_write_reg(CR6B, VIACR, RegCR6B | 0x08);

		/* CR91[4] VDD On [3] Data On [2] VEE On [1] Back Light Off
		   [0] Software Control Power Sequence */
		RegCR91 = viafb_read_reg(VIACR, CR91);
		viafb_write_reg(CR91, VIACR, 0x1D);

		/* CR93[7] DI1 Data Source Selection: 1 = DSP2.
		   CR93[5] DI1 Clock Source: 1 = internal.
		   CR93[4] DI1 Clock Polarity.
		   CR93[3:1] DI1 Clock Adjust. CR93[0] DI1 enable */
		RegCR93 = viafb_read_reg(VIACR, CR93);
		viafb_write_reg(CR93, VIACR, 0x01);
	} else {
		/* DVP0/DVP1 Pad on */
		RegSR1E = viafb_read_reg(VIASR, SR1E);
		viafb_write_reg(SR1E, VIASR, RegSR1E | 0xF0);

		/* SR3E[1]Multi-function selection:
		   0 = Emulate I2C and DDC bus by GPIO2/3/4. */
		RegSR3E = viafb_read_reg(VIASR, SR3E);
		viafb_write_reg(SR3E, VIASR, RegSR3E & (~0x20));

		/* CR91[4] VDD On [3] Data On [2] VEE On [1] Back Light Off
		   [0] Software Control Power Sequence */
		RegCR91 = viafb_read_reg(VIACR, CR91);
		viafb_write_reg(CR91, VIACR, 0x1D);

		/*CR9B[4] DVP1 Data Source Selection: 1 = From secondary
		display.CR9B[2:0] DVP1 Clock Adjust */
		RegCR9B = viafb_read_reg(VIACR, CR9B);
		viafb_write_reg(CR9B, VIACR, 0x01);
	}

	data = (u8) tmds_register_read(0x09);
	if (data & 0x04)
		ret = true;

	if (ret == false) {
		if (viafb_dvi_query_EDID())
			ret = true;
	}

	/* Restore status */
	viafb_write_reg(SR1E, VIASR, RegSR1E);
	viafb_write_reg(CR91, VIACR, RegCR91);
	if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_CLE266) {
		viafb_write_reg(CR6B, VIACR, RegCR6B);
		viafb_write_reg(CR93, VIACR, RegCR93);
	} else {
		viafb_write_reg(SR3E, VIASR, RegSR3E);
		viafb_write_reg(CR9B, VIACR, RegCR9B);
	}

	return ret;
}

/* Query Flat Panel's EDID Table Version Through DVI Connector */
static int viafb_dvi_query_EDID(void)
{
	u8 data0, data1;
	int restore;

	DEBUG_MSG(KERN_INFO "viafb_dvi_query_EDID!!\n");

	restore = viaparinfo->chip_info->tmds_chip_info.tmds_chip_slave_addr;
	viaparinfo->chip_info->tmds_chip_info.tmds_chip_slave_addr = 0xA0;

	data0 = (u8) tmds_register_read(0x00);
	data1 = (u8) tmds_register_read(0x01);
	if ((data0 == 0) && (data1 == 0xFF)) {
		viaparinfo->chip_info->
			tmds_chip_info.tmds_chip_slave_addr = restore;
		return EDID_VERSION_1;	/* Found EDID1 Table */
	}

	data0 = (u8) tmds_register_read(0x00);
	viaparinfo->chip_info->tmds_chip_info.tmds_chip_slave_addr = restore;
	if (data0 == 0x20)
		return EDID_VERSION_2;	/* Found EDID2 Table */
	else
		return false;
}

/*
 *
 * int dvi_get_panel_size_from_DDCv1(void)
 *
 *     - Get Panel Size Using EDID1 Table
 *
 * Return Type:    int
 *
 */
static int dvi_get_panel_size_from_DDCv1(void)
{
	int i, max_h = 0, max_v = 0, tmp, restore;
	unsigned char rData;
	unsigned char EDID_DATA[18];

	DEBUG_MSG(KERN_INFO "\n dvi_get_panel_size_from_DDCv1 \n");

	restore = viaparinfo->chip_info->tmds_chip_info.tmds_chip_slave_addr;
	viaparinfo->chip_info->tmds_chip_info.tmds_chip_slave_addr = 0xA0;

	rData = tmds_register_read(0x23);
	if (rData & 0x3C)
		max_h = 640;
	if (rData & 0xC0)
		max_h = 720;
	if (rData & 0x03)
		max_h = 800;

	rData = tmds_register_read(0x24);
	if (rData & 0xC0)
		max_h = 800;
	if (rData & 0x1E)
		max_h = 1024;
	if (rData & 0x01)
		max_h = 1280;

	for (i = 0x25; i < 0x6D; i++) {
		switch (i) {
		case 0x26:
		case 0x28:
		case 0x2A:
		case 0x2C:
		case 0x2E:
		case 0x30:
		case 0x32:
		case 0x34:
			rData = tmds_register_read(i);
			if (rData == 1)
				break;
			/* data = (data + 31) * 8 */
			tmp = (rData + 31) << 3;
			if (tmp > max_h)
				max_h = tmp;
			break;

		case 0x36:
		case 0x48:
		case 0x5A:
		case 0x6C:
			tmds_register_read_bytes(i, EDID_DATA, 10);
			if (!(EDID_DATA[0] || EDID_DATA[1])) {
				/* The first two byte must be zero. */
				if (EDID_DATA[3] == 0xFD) {
					/* To get max pixel clock. */
					viaparinfo->tmds_setting_info->
					max_pixel_clock = EDID_DATA[9] * 10;
				}
			}
			break;

		default:
			break;
		}
	}

	switch (max_h) {
	case 640:
		viaparinfo->tmds_setting_info->dvi_panel_size =
			VIA_RES_640X480;
		break;
	case 800:
		viaparinfo->tmds_setting_info->dvi_panel_size =
			VIA_RES_800X600;
		break;
	case 1024:
		viaparinfo->tmds_setting_info->dvi_panel_size =
			VIA_RES_1024X768;
		break;
	case 1280:
		viaparinfo->tmds_setting_info->dvi_panel_size =
			VIA_RES_1280X1024;
		break;
	case 1400:
		viaparinfo->tmds_setting_info->dvi_panel_size =
			VIA_RES_1400X1050;
		break;
	case 1440:
		viaparinfo->tmds_setting_info->dvi_panel_size =
			VIA_RES_1440X1050;
		break;
	case 1600:
		viaparinfo->tmds_setting_info->dvi_panel_size =
			VIA_RES_1600X1200;
		break;
	case 1920:
		if (max_v == 1200) {
			viaparinfo->tmds_setting_info->dvi_panel_size =
				VIA_RES_1920X1200;
		} else {
			viaparinfo->tmds_setting_info->dvi_panel_size =
				VIA_RES_1920X1080;
		}

		break;
	default:
		viaparinfo->tmds_setting_info->dvi_panel_size =
			VIA_RES_1024X768;
		DEBUG_MSG(KERN_INFO "Unknow panel size max resolution = %d !\
					 set default panel size.\n", max_h);
		break;
	}

	DEBUG_MSG(KERN_INFO "DVI max pixelclock = %d\n",
		  viaparinfo->tmds_setting_info->max_pixel_clock);
	viaparinfo->chip_info->tmds_chip_info.tmds_chip_slave_addr = restore;
	return viaparinfo->tmds_setting_info->dvi_panel_size;
}

/*
 *
 * int dvi_get_panel_size_from_DDCv2(void)
 *
 *     - Get Panel Size Using EDID2 Table
 *
 * Return Type:    int
 *
 */
static int dvi_get_panel_size_from_DDCv2(void)
{
	int HSize = 0, restore;
	unsigned char R_Buffer[2];

	DEBUG_MSG(KERN_INFO "\n dvi_get_panel_size_from_DDCv2 \n");

	restore = viaparinfo->chip_info->tmds_chip_info.tmds_chip_slave_addr;
	viaparinfo->chip_info->tmds_chip_info.tmds_chip_slave_addr = 0xA2;

	/* Horizontal: 0x76, 0x77 */
	tmds_register_read_bytes(0x76, R_Buffer, 2);
	HSize = R_Buffer[0];
	HSize += R_Buffer[1] << 8;

	switch (HSize) {
	case 640:
		viaparinfo->tmds_setting_info->dvi_panel_size =
			VIA_RES_640X480;
		break;
	case 800:
		viaparinfo->tmds_setting_info->dvi_panel_size =
			VIA_RES_800X600;
		break;
	case 1024:
		viaparinfo->tmds_setting_info->dvi_panel_size =
			VIA_RES_1024X768;
		break;
	case 1280:
		viaparinfo->tmds_setting_info->dvi_panel_size =
			VIA_RES_1280X1024;
		break;
	case 1400:
		viaparinfo->tmds_setting_info->dvi_panel_size =
			VIA_RES_1400X1050;
		break;
	case 1440:
		viaparinfo->tmds_setting_info->dvi_panel_size =
			VIA_RES_1440X1050;
		break;
	case 1600:
		viaparinfo->tmds_setting_info->dvi_panel_size =
			VIA_RES_1600X1200;
		break;
	default:
		viaparinfo->tmds_setting_info->dvi_panel_size =
			VIA_RES_1024X768;
		DEBUG_MSG(KERN_INFO "Unknow panel size max resolution = %d!\
					set default panel size.\n", HSize);
		break;
	}

	viaparinfo->chip_info->tmds_chip_info.tmds_chip_slave_addr = restore;
	return viaparinfo->tmds_setting_info->dvi_panel_size;
}

/*
 *
 * unsigned char dvi_get_panel_info(void)
 *
 *     - Get Panel Size
 *
 * Return Type:    unsigned char
 */
static unsigned char dvi_get_panel_info(void)
{
	unsigned char dvipanelsize;
	DEBUG_MSG(KERN_INFO "dvi_get_panel_info! \n");

	viafb_dvi_sense();
	switch (viafb_dvi_query_EDID()) {
	case 1:
		dvi_get_panel_size_from_DDCv1();
		break;
	case 2:
		dvi_get_panel_size_from_DDCv2();
		break;
	default:
		break;
	}

	DEBUG_MSG(KERN_INFO "dvi panel size is %2d \n",
		  viaparinfo->tmds_setting_info->dvi_panel_size);
	dvipanelsize = (unsigned char)(viaparinfo->
		tmds_setting_info->dvi_panel_size);
	return dvipanelsize;
}

/* If Disable DVI, turn off pad */
void viafb_dvi_disable(void)
{
	if (viaparinfo->chip_info->
		tmds_chip_info.output_interface == INTERFACE_DVP0)
		viafb_write_reg(SR1E, VIASR,
		viafb_read_reg(VIASR, SR1E) & (~0xC0));

	if (viaparinfo->chip_info->
		tmds_chip_info.output_interface == INTERFACE_DVP1)
		viafb_write_reg(SR1E, VIASR,
		viafb_read_reg(VIASR, SR1E) & (~0x30));

	if (viaparinfo->chip_info->
		tmds_chip_info.output_interface == INTERFACE_DFP_HIGH)
		viafb_write_reg(SR2A, VIASR,
		viafb_read_reg(VIASR, SR2A) & (~0x0C));

	if (viaparinfo->chip_info->
		tmds_chip_info.output_interface == INTERFACE_DFP_LOW)
		viafb_write_reg(SR2A, VIASR,
		viafb_read_reg(VIASR, SR2A) & (~0x03));

	if (viaparinfo->chip_info->
		tmds_chip_info.output_interface == INTERFACE_TMDS)
		/* Turn off TMDS power. */
		viafb_write_reg(CRD2, VIACR,
		viafb_read_reg(VIACR, CRD2) | 0x08);
}

/* If Enable DVI, turn off pad */
void viafb_dvi_enable(void)
{
	u8 data;

	if (viaparinfo->chip_info->
		tmds_chip_info.output_interface == INTERFACE_DVP0) {
		viafb_write_reg(SR1E, VIASR,
			viafb_read_reg(VIASR, SR1E) | 0xC0);
		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_CLE266)
			tmds_register_write(0x88, 0x3b);
		else
			/*clear CR91[5] to direct on display period
			   in the secondary diplay path */
			viafb_write_reg(CR91, VIACR,
			viafb_read_reg(VIACR, CR91) & 0xDF);
	}

	if (viaparinfo->chip_info->
		tmds_chip_info.output_interface == INTERFACE_DVP1) {
		viafb_write_reg(SR1E, VIASR,
			viafb_read_reg(VIASR, SR1E) | 0x30);

		/*fix dvi cann't be enabled with MB VT5718C4 - Al Zhang */
		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_CLE266) {
			tmds_register_write(0x88, 0x3b);
		} else {
			/*clear CR91[5] to direct on display period
			  in the secondary diplay path */
			viafb_write_reg(CR91, VIACR,
			viafb_read_reg(VIACR, CR91) & 0xDF);
		}

		/*fix DVI cannot enable on EPIA-M board */
		if (viafb_platform_epia_dvi == 1) {
			viafb_write_reg_mask(CR91, VIACR, 0x1f, 0x1f);
			viafb_write_reg_mask(CR88, VIACR, 0x00, BIT6 + BIT0);
			if (viafb_bus_width == 24) {
				if (viafb_device_lcd_dualedge == 1)
					data = 0x3F;
				else
					data = 0x37;
				viafb_i2c_writebyte(viaparinfo->chip_info->
					     tmds_chip_info.
					     tmds_chip_slave_addr,
					     0x08, data);
			}
		}
	}

	if (viaparinfo->chip_info->
		tmds_chip_info.output_interface == INTERFACE_DFP_HIGH) {
		viafb_write_reg(SR2A, VIASR,
			viafb_read_reg(VIASR, SR2A) | 0x0C);
		viafb_write_reg(CR91, VIACR,
			viafb_read_reg(VIACR, CR91) & 0xDF);
	}

	if (viaparinfo->chip_info->
		tmds_chip_info.output_interface == INTERFACE_DFP_LOW) {
		viafb_write_reg(SR2A, VIASR,
			viafb_read_reg(VIASR, SR2A) | 0x03);
		viafb_write_reg(CR91, VIACR,
			viafb_read_reg(VIACR, CR91) & 0xDF);
	}
	if (viaparinfo->chip_info->
		tmds_chip_info.output_interface == INTERFACE_TMDS) {
		/* Turn on Display period in the panel path. */
		viafb_write_reg_mask(CR91, VIACR, 0, BIT7);

		/* Turn on TMDS power. */
		viafb_write_reg_mask(CRD2, VIACR, 0, BIT3);
	}
}

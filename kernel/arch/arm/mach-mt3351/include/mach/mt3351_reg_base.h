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

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (c) 2008 MediaTek Inc.
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
;
;/*****************************************************************************
; *
; * Filename:
; * ---------
; *   reg_base.h
; *
; * Project:
; * --------
; *   MT3351 FPGA
; *
; * Description:
; * ------------
; *   This Module defines register base memory map
; *
; * Author:
; * -------
; *   Kelvin Yang (mtk01638)
; *
; ****************************************************************************/
;

#ifndef __MT3351_REG_BASE_H__
#define __REG_BASE_H__
// 0xF0000000
#define MDM_TM_BASE      	(0x10f00000)//DSP cosim
#define EFUSE_BASE      	(0xF0000000)
#define CONFIG_BASE      	(0xF0001000)     
#define GPIO_BASE        	(0xF0002000)     
#define RGU_BASE         	(0xF0003000)
#define DVFS_BASE         	(0xF0004000)
// 0xF0020000
#define EMI_BASE         	(0xF0020000)     
#define EMI2_BASE         	(0xF0021000)     
#define CIRQ_BASE        	(0xF0022000)     
#define DMA_BASE         	(0xF0023000)     
#define UART1_BASE       	(0xF0025000)     
#define UART2_BASE       	(0xF0026000)     
#define UART3_BASE       	(0xF0027000)     
#define UART4_BASE       	(0xF0028000)     
#define UART5_BASE       	(0xF0029000)     
#define GPT_BASE         	(0xF002A000)     
#define PWM_BASE         	(0xF002B000)     
#define RTC_base         	(0xF002C000)     
#define I2C_BASE         	(0xF002D000)     

// 0xF0030000
#define MSDC1_BASE        	(0xF002E000)     
#define MSDC2_BASE        	(0xF002F000)
#define MSDC3_BASE        	(0xF0030000)
#define NFI_BASE         	(0xF0031000)     
#define NFIECC_BASE	        (0xF0032000)     
#define AUXADC_BASE         (0xF0034000)     
#define SLPCTRL_BASE        (0xF0035000)     
//#define APVFE_BASE       	(0xF0035000)     //Removed
#define KP_BASE       		(0xF0036000)     
#define IRDA_BASE       	(0xF0037000)     
#define SPI_GPS_BASE     	(0xF0033000)     

// 0xF0040000
#define GMC_BASE         	(0xF0040000)     
#define G2D_BASE         	(0xF0041000)     
#define GCMQ_BASE        	(0xF0042000)     
#define GIFDEC_BASE         (0xF0043000)     
#define JPEG_BASE       	(0xF0044000)     
#define IMGDMA_BASE       	(0xF0045000)     
#define CAM_BASE_1       	(0xF0046000)     
#define IMGPROC_BASE  		(0xF0047000)     
#define PRZ_BASE       		(0xF0048000)     
#define CRZ_BASE         	(0xF0049000)     
#define DRZ_BASE         	(0xF004A000)     
#define ASM_BASE         	(0xF004B000)     
//#define AFE_BASE         	(0xF004C000)     // Removed
#define VFE_BASE        	(0xF004D000)     
#define SPI_BASE        	(0xF004E000) 
#define SHARE_BASE        	(0xF004F000)         	

// 0xF0050000
#define BLS_BASE            (0xF0050000)
#define PATCH_BASE        	(0xF0051000) 
#define INFLATE_BASE       	(0xF0052000)         	

// 0xF0060000
#define PLL_BASE        	(0xF0060000)     
#define PMU_BASE        	(0xF0061300) 
#define ABI_BASE        	(0xF0061000) 

#define USB_BASE            (0xF0100000)

// 0x80120000
#define LCD_BASE            (0xF0120000)

// 0x80130000
#define DPI_BASE            (0xF0130000)




/*****************
 * EMI Registers *
 *****************/
#define EMI_CON0 			((UINT16P)(EMI_BASE+0x0000)) /* Bank 0 configuration */
#define EMI_CON1 			((UINT16P)(EMI_BASE+0x0004)) /* Bank 1 configuration */
#define EMI_CON2 			((UINT16P)(EMI_BASE+0x0008)) /* Bank 2 configuration */
#define EMI_CON3 			((UINT16P)(EMI_BASE+0x000C)) /* Bank 3 configuration */
#define EMI_CON4 			((UINT16P)(EMI_BASE+0x0010)) /* Boot Mapping config  */
#define	EMI_CON5 			((UINT16P)(EMI_BASE+0x0014))
#define SDRAM_MODE 			((UINT16P)(EMI_BASE+0x0020))
#define SDRAM_COMD 			((UINT16P)(EMI_BASE+0x0024))
#define SDRAM_SET 			((UINT16P)(EMI_BASE+0x0028))
#define SDRAM_BASE			0x20000000


/******************************
 * Audio Front End Registers *
 ******************************/
#define VCSR  				((UINT16P)(AFE_BASE+0x0000))
#define VGCR  				((UINT16P)(AFE_BASE+0x0004))
#define VACCR   			((UINT16P)(AFE_BASE+0x0008))
#define VPDN   				((UINT16P)(AFE_BASE+0x000C))
#define VBTDAICR 			((UINT16P)(AFE_BASE+0x0010))
#define VLBCR  				((UINT16P)(AFE_BASE+0x0014))


/******************************
 * AHB MCU-DSP SHARE RAM      *
 ******************************/
#define MCU_DSP_SHARE_RAM_BASE			(0xF0600000)
#define MCU_DSP_SHARE_RAM_SIZE      	(0x01A0)        // 208*2
#define IDMA_PORT_BASE              	(0xF0700000)

#define dbg_print

#endif /*ifndef __REG_BASE_H__ */

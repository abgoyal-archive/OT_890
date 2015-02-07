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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
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

/*****************************************************************************
; *
; * Filename:
; * ---------
; *   reg_base.h
; *
; * Project:
; * --------
; *   MT6516 
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

#ifndef __MT6516_REG_BASE_H__
#define __REG_BASE_H__

// 0xF0000000
#define EFUSE_BASE      	(0xF0000000)
#define CONFIG_BASE      	(0xF0001000)     
#define GPIO_BASE        	(0xF0002000)     
#define RGU_BASE         	(0xF0003000)

// 0xF0020000
#define EMI_BASE         	(0xF0020000)     
#define CIRQ_BASE        	(0xF0021000)     
#define DMA_BASE         	(0xF0022000)     
#define UART1_BASE       	(0xF0023000)     
#define UART2_BASE       	(0xF0024000)     
#define UART3_BASE       	(0xF0025000)     
#define GPT_BASE         	(0xF0026000)     
#define HDQ_BASE         	(0xF0027000)     
#define KP_BASE         	(0xF0028000)     
#define PWM_BASE         	(0xF0029000)     
#define UART4_BASE       	(0xF002B000)     
#define RTC_BASE         	(0xF002C000)     
#define SEJ_base         	(0xF002D000)     
#define I2C3_BASE        	(0xF002E000)     
#define IRDA_BASE       	(0xF002F000)     

// 0xF0030000
#define I2C_BASE        	(0xF0030000)
#define MSDC1_BASE       	(0xF0031000)
#define NFI_BASE 	      	(0xF0032000)
#define SIM_BASE 	      	(0xF0033000)
#define MSDC2_BASE       	(0xF0034000)
#define I2C2_BASE        	(0xF0035000)
#define CCIF_BASE        	(0xF0036000)
#define NFIECC_BASE      	(0xF0038000)
#define AMCONFG_BASE     	(0xF0039000)
#define AP2MD_BASE	     	(0xF003A000)
#define APVFE_BASE	     	(0xF003B000)
#define APSLP_BASE	     	(0xF003C000)
#define AUXADC_BASE	     	(0xF003D000)
#define APXGPT_BASE	     	(0xF003E000)
#define MSDC3_BASE       	(0xF003F000)

// 0xF0040000
#define CSDBG_BASE				(0xF0040000)

// 0xF0060000
#define PLL_BASE				(0xF0060000)
#define DSI_PHY_BASE            (0xF0060B00)

// 0xF0080000
#define GMC1_BASE				(0xF0080000)
#define G2D_BASE				(0xF0081000)
#define GCMQ_BASE				(0xF0082000)
#define GIFDEC_BASE				(0xF0083000)
#define IMGDMA_BASE				(0xF0084000)
#define PNGDEC_BASE				(0xF0085000)
#define MTVSPI_BASE				(0xF0087000)
#define TVCON_BASE				(0xF0088000)
#define TVENC_BASE				(0xF0089000)
#define CAM_BASE				(0xF008A000)
#define CAM_ISP_BASE				(0xF008B000)
#define BLS_BASE				(0xF008C000)
#define CRZ_BASE				(0xF008D000)
#define DRZ_BASE				(0xF008E000)
#define ASM_BASE				(0xF008F000)

// 0xF0090000
#define WT_BASE					(0xF0090000)
#define IMG_BASE				(0xF0091000)
#define GRAPH1SYS_CONFG_BASE			(0xF0092000)

// 0xF00A0000
#define GMC2_BASE				(0xF00A0000)
#define JPEG_BASE				(0xF00A1000)
#define M3D_BASE				(0xF00A2000)
#define PRZ_BASE				(0xF00A3000)
#define IMGDMA1_BASE				(0xF00A4000)
#define MP4_DEBLK_BASE		    		(0xF00A5000)
#define FAKE_ENG2_BASE				(0xF00A6000)
#define GRAPH2SYS_BASE		    		(0xF00A7000)

// 0xF00C0000
#define MP4_BASE				(0xF00C0000)
#define H264_BASE				(0xF00C1000)

// 0xF0100000
#define USB_BASE            			(0xF0100000)

// 0xF0120000
#define LCD_BASE            			(0xF0120000)

// 0xF0130000
#define DPI_BASE            			(0xF0130000)


#define CEVA_BASE        			(0xF1000000)

#define dbg_print
#endif /*ifndef __REG_BASE_H__ */

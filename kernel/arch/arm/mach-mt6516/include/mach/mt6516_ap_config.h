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
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
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
 *
 * Filename:
 * ---------
 *   mt6516_ap_config.h
 *
 * Project:
 * --------
 *   MT6516
 *
 * Description:
 * ------------
 *   MT6516 AP Config setting (0x80001000) 
 *
 * Author:
 * -------
 *   Kelvin Yang   (mtk01638)
 *
 ****************************************************************************/

#ifndef __MT6516_AP_CONFIG__
#define __MT6516_AP_CONFIG__

#include    <mach/mt6516_typedefs.h>
#include    <mach/mt6516_reg_base.h>


/*******************************************************************************
 * Constant definition 
 *******************************************************************************/

// clock and PDN register

#define HW_VER              (CONFIG_BASE+0x0000)
#define SW_VER              (CONFIG_BASE+0x0004)
#define HW_CODE             (CONFIG_BASE+0x0008) 
#define SW_MISC_L			(CONFIG_BASE+0x0010)
#define SW_MISC_H           (CONFIG_BASE+0x0014)
#define HW_MISC             (CONFIG_BASE+0x0020)
#define ARM9_FREQ_DIV       (CONFIG_BASE+0x0100)
#define SLEEP_CON           (CONFIG_BASE+0x0204)
#define MCUCLK_CON          (CONFIG_BASE+0x0208)
#define EMICLK_CON          (CONFIG_BASE+0x020c)
#define ISO_EN              (CONFIG_BASE+0x0300)
#define PWR_OFF             (CONFIG_BASE+0x0304)
#define MCU_MEM_PDN         (CONFIG_BASE+0x0308)
#define G1_MEM_PDN          (CONFIG_BASE+0x030c)
#define G2_MEM_PDN          (CONFIG_BASE+0x0310)
#define CEVA_MEM_PDN        (CONFIG_BASE+0x0314)
#define IN_ISO_EN           (CONFIG_BASE+0x0318)
#define PWR_ACK             (CONFIG_BASE+0x031c)
#define ACK_CLR             (CONFIG_BASE+0x0320)
#define APB_CON             (CONFIG_BASE+0x0404)
#define SECURITY_REG        (CONFIG_BASE+0x0408)
#define IO_DRV0             (CONFIG_BASE+0x0500)
#define IO_DRV1             (CONFIG_BASE+0x0504)
#define IC_SIZE             (CONFIG_BASE+0x0600)
#define DC_SIZE             (CONFIG_BASE+0x0604)
#define MDVCXO_OFF          (CONFIG_BASE+0x0608)

/*******************************************************************************
 * offset definition
 *******************************************************************************/

//MCU_MEM_PDN
typedef enum
{
    PDN_MCU_IC            =       0,
    PDN_MCU_IC_16KB  =       1,
    PDN_MCU_DC           =       2,
    PDN_MCU_DC_16KB =       3,
    PDN_MCU_MMU        =       4,
    PDN_MCU_ITCM       =       5,
    PDN_MCU_DTCM      =       9,
    PDN_MCU_AP_SYSROM   = 13,
    PDN_MCU_USB        =        14,
    PDN_MCU_CCIF       =       15,
    PDN_MCU_ETB        =        16,
    PDN_MCU_MD_SYSROM  = 17,
    PDN_MCU_L1_CACHE     = 18,
    PDN_MCU_L1_TCM         = 19
    
} MCUMEM_PDNCONA_MODE;


//G1_MEM_PDN     
typedef enum
{
    PDN_G1_DSI            =       0,
    PDN_G1_DPI            =       1,
    PDN_G1_AFE           =       2,
    PDN_G1_ASM           =       3,
    PDN_G1_WAVE        =       4,
    PDN_G1_LCD           =       5,
    PDN_G1_TVC           =       6,
    PDN_G1_IMGDMA    =       7,
    PDN_G1_RESZ         =        8,
    PDN_G1_CAM          =        9,
    
} G1_PDNCONA_MODE;

//G2_MEM_PDN     
typedef enum
{
    PDN_G2_M3D            =       0,
    
} G2_PDNCONA_MODE;

//CEVA_MEM_PDN   
typedef enum
{
    PDN_CEVA_L1_MEM_DN    =       0,
    PDN_CEVA_L2_MEM_DN    =       3,
    PDN_CEVA_CCIF               =        5,
    
} CEVA_PDNCONA_MODE;



/*******************************************************************************
 * Command definition
 *******************************************************************************/

//HW_VER         
//SW_VER         
//HW_CODE        
//SW_MISC_L		
//SW_MISC_H      

//HW_MISC       
#define USB_SEL                0x00000001
#define GMC_AUTOCG        0x00000004
#define UART1_SEL           0x00000008
#define UART2_SEL           0x00000010
#define UART3_SEL           0x00000020
#define UART4_SEL           0x00000040
#define SIM2_SEL             0x00000080
#define NFI_SEL               0x00000100
#define CEVADBG_EN        0x00000200
#define MASK_GMC1         0x00000400
#define MASK_GMC2         0x00000800
#define NIRQ_MASK          0x00001000
#define MD_BOOT_ONLY    0x00004000

//ARM9_FREQ_DIV  
typedef enum
{
	DIV_1_416 = 0,
	Reserved  = 1,
	DIV_2_208 = 2,
	DIV_4_104 = 3,
}ARM9_FREQ_DIV_ENUM;

//SLEEP_CON      
#define APB_CLK_EN        0x00000002
#define DDR_CLK_EN        0x00000010
#define CEVA_CLK_EN       0x00000020
#define MD_ACT_B          0x00000040
#define F48M              0x00000080
#define FMCU_DIV_EN       0x00000100
#define FMCU_2x_DIV_EN    0x00000200

//MCUCLK_CON
typedef enum
{
	AHB_13M = 0,
	AHB_26M = 1,
	RESERVED0 = 2,
	AHB_52M = 3,
	RESERVED1 = 4,
	RESERVED2 = 5,
	RESERVED3 = 6,
	AHB_104M = 7,
}MCU_FSEL_ENUM;

//EMICLK_CON     
typedef enum
{
	EMI_3250000 = 0x0,
	EMI_6500000 = 0x1,
	EMI_13000000 = 0x3,
	EMI_26000000 = 0x7,
	EMI_52000000 = 0xF,
	EMI_104000000 = 0x1F,
}EMI_FREQ;

//ISO_EN
#define GRAPH1_ISO_EN   0x0008
#define GRAPH2_ISO_EN   0x0010
#define CEVA_ISO_EN     0x0020

//PWR_OFF        
#define GRAPH1_PDN   0x0008
#define GRAPH2_PDN   0x0010
#define CEVA_PDN     0x0020

//IN_ISO_EN      
#define GRAPH1_IN_EN   0x00000008
#define GRAPH2_IN_EN   0x00000010 
#define CEVA_IN_EN       0x00000020

//PWR_ACK        


//ACK_CLR        
#define CEVA_PWR_ACK           0x00000001
#define G2_PWR_ACK               0x00000002 
#define G1_PWR_ACK               0x00000004
#define MD2G_PWR_PWR_ACK 0x00000008

//APB_CON      
typedef enum
{
	APBR_1CYCLE = 0x1,
	APBR_2CYCLE = 0x2,
	APBR_3CYCLE = 0x4,
	APBR_4CYCLE = 0x8,
	APBR_5CYCLE = 0x10,
	APBR_6CYCLE = 0x20,
	APBR_7CYCLE = 0x40,
}APBR;

typedef enum
{
	APBW_1CYCLE = 0x100,
	APBW_2CYCLE = 0x200,
	APBW_3CYCLE = 0x400,
	APBW_4CYCLE = 0x800,
	APBW_5CYCLE = 0x1000,
	APBW_6CYCLE = 0x2000,
	APBW_7CYCLE = 0x4000,
}APBW;

//SECURITY_REG   
//IO_DRV0        
//IO_DRV1        

//IC_SIZE        
#define IC_SIZE_16KB 0x00000001
 
//DC_SIZE        
#define DC_SIZE_16KB 0x00000001

//MDVCXO_OFF     

#define CHIP_VER_ECO_1 (0x8a00)
#define CHIP_VER_ECO_2 (0x8a01)


#endif /* __PDN_HW_H__ */


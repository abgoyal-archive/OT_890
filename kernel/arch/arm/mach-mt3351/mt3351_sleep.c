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
#include    <mach/mt3351_sleep.h>
#include    <mach/mt3351_typedefs.h>
#include    <mach/mt3351_reg_base.h>
#include    <linux/spinlock.h>
#include    <asm/io.h>
#include    <linux/delay.h>
#include    <linux/pm.h>

BOOL
SLPCTL_Set_Sleep_Pause_Duration(UINT32 millisec)
{
	UINT32 uints, u4Val;
    //20090204, The original is list below, we remove floating operation
    //uints = (UINT32)((millisec * 1000) / 30.5);
	uints = (UINT32)((millisec * 10000) / 305);
	if (uints > 0x7FFFF)
		uints = 0x7FFFF;
	DRV_WriteReg(SLP_PAUSE_H, (uints >> 16));	
	DRV_WriteReg(SLP_PAUSE_L, (uints & SLP_PAUSE_L_MASK));
	
	if (millisec == 0) 
	{
    	u4Val = DRV_Reg(SLP_CFG);
    	u4Val &= ~SLEEP_CNT_EN;
    	DRV_WriteReg(SLP_CFG, u4Val);	    
	}
	else 
	{
    	u4Val = DRV_Reg(SLP_CFG);
    	u4Val |= SLEEP_CNT_EN;
    	DRV_WriteReg(SLP_CFG, u4Val);	    
	}

	return TRUE;
}

BOOL
SLPCTL_Set_Ext_CLK_Settle_Time(UINT32 units)
{
	if (units > SLP_ECLK_SETTLE_MASK)
		units = SLP_ECLK_SETTLE_MASK;

	DRV_WriteReg(SLP_ECLK_SETTLE, units);

	return TRUE;
}

BOOL
SLPCTL_Set_Final_Sleep_Pause_Counter(UINT32 uints)
{
	uints &= (SLP_FPAUSE_L_MASK  | (SLP_FPAUSE_H_MASK << 16) );

	DRV_WriteReg(SLP_FPAUSE_H, (uints >> 16));
	DRV_WriteReg(SLP_FPAUSE_L, (uints & SLP_FPAUSE_L));
	
	return TRUE;
}

void SLPCTL_Set_Pause_Start(BOOL start)
{  
	if (start)
		DRV_WriteReg(SLP_CNTL, SLP_CNTL_START);
	else
		DRV_WriteReg(SLP_CNTL, ~SLP_CNTL_START);		

	return;
}


UINT16
SLPCTL_Get_Pause_Status()
{
	UINT16 status;	
  	status = DRV_Reg(SLP_STAT);	
	return status;
}

void
SLPCTL_Reset_WakeUp_Source()
{
	if (DRV_Reg(SLP_CFG) & SLEEP_CNT_EN) 
	{
		DRV_WriteReg(SLP_CFG, 0);
		DRV_WriteReg(SLP_CFG, SLEEP_CNT_EN);
	}
	else 
	{
		DRV_WriteReg(SLP_CFG, 0);				
	}
}

void
SLPCTL_Set_WakeUp_Source(WAKE_SOURCE Source)
{	
    UINT16 status;
    status = DRV_Reg(SLP_CFG);
    DRV_WriteReg(SLP_CFG, status|Source);    
}

UINT16
SLPCTL_Get_WakeUp_Source()
{	
	return (UINT16)DRV_Reg(SLP_CFG);
}

void
SLPCTL_Clear_WakeUp_Source(WAKE_SOURCE Source)
{	
    UINT16 status;
    status = DRV_Reg(SLP_CFG);
    status &= ~Source;
    DRV_WriteReg(SLP_CFG, status);    
}

void 
SLPCTL_Set_PLL_WakeUp_Reset(BOOL enable, UINT32 units)
{
	UINT16 config;
	
	if (units > PLL_RESET_WIDTH_MASK)
		units = PLL_RESET_WIDTH_MASK;

	config = units;
	if(enable)
		config |= PLL_RESET_EN;
	else
		config &= ~ PLL_RESET_EN;
    DRV_WriteReg(WAKE_PLL, config);    
	
}

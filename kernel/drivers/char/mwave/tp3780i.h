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
*
* tp3780i.h -- declarations for tp3780i.c
*
*
* Written By: Mike Sullivan IBM Corporation
*
* Copyright (C) 1999 IBM Corporation
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* NO WARRANTY
* THE PROGRAM IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR
* CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED INCLUDING, WITHOUT
* LIMITATION, ANY WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT,
* MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. Each Recipient is
* solely responsible for determining the appropriateness of using and
* distributing the Program and assumes all risks associated with its
* exercise of rights under this Agreement, including but not limited to
* the risks and costs of program errors, damage to or loss of data,
* programs or equipment, and unavailability or interruption of operations.
*
* DISCLAIMER OF LIABILITY
* NEITHER RECIPIENT NOR ANY CONTRIBUTORS SHALL HAVE ANY LIABILITY FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING WITHOUT LIMITATION LOST PROFITS), HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
* USE OR DISTRIBUTION OF THE PROGRAM OR THE EXERCISE OF ANY RIGHTS GRANTED
* HEREUNDER, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*
* 10/23/2000 - Alpha Release
*	First release to the public
*/

#ifndef _LINUX_TP3780I_H
#define _LINUX_TP3780I_H

#include <asm/io.h>
#include "mwavepub.h"


/* DSP abilities constants for 3780i based Thinkpads */
#define TP_ABILITIES_INTS_PER_SEC       39160800
#define TP_ABILITIES_DATA_SIZE          32768
#define TP_ABILITIES_INST_SIZE          32768
#define TP_ABILITIES_MWAVEOS_NAME       "mwaveos0700.dsp"
#define TP_ABILITIES_BIOSTASK_NAME      "mwbio701.dsp"


/* DSP configuration values for 3780i based Thinkpads */
#define TP_CFG_NumTransfers     3	/* 16 transfers */
#define TP_CFG_RerequestTimer   1	/* 2 usec */
#define TP_CFG_MEMCS16          0	/* Disabled, 16-bit memory assumed */
#define TP_CFG_IsaMemCmdWidth   3	/* 295 nsec (16-bit) */
#define TP_CFG_GateIOCHRDY      0	/* No IOCHRDY gating */
#define TP_CFG_EnablePwrMgmt    1	/* Enable low poser suspend/resume */
#define TP_CFG_HBusTimerValue 255	/* HBus timer load value */
#define TP_CFG_DisableLBusTimeout 0	/* Enable LBus timeout */
#define TP_CFG_N_Divisor       32	/* Clock = 39.1608 Mhz */
#define TP_CFG_M_Multiplier    37	/* " */
#define TP_CFG_PllBypass        0	/* don't bypass */
#define TP_CFG_ChipletEnable 0xFFFF	/* Enable all chiplets */

typedef struct {
	int bDSPEnabled;
	int bShareDspIrq;
	int bShareUartIrq;
	DSP_3780I_CONFIG_SETTINGS rDspSettings;
} THINKPAD_BD_DATA;

int tp3780I_InitializeBoardData(THINKPAD_BD_DATA * pBDData);
int tp3780I_CalcResources(THINKPAD_BD_DATA * pBDData);
int tp3780I_ClaimResources(THINKPAD_BD_DATA * pBDData);
int tp3780I_ReleaseResources(THINKPAD_BD_DATA * pBDData);
int tp3780I_EnableDSP(THINKPAD_BD_DATA * pBDData);
int tp3780I_DisableDSP(THINKPAD_BD_DATA * pBDData);
int tp3780I_ResetDSP(THINKPAD_BD_DATA * pBDData);
int tp3780I_StartDSP(THINKPAD_BD_DATA * pBDData);
int tp3780I_QueryAbilities(THINKPAD_BD_DATA * pBDData, MW_ABILITIES * pAbilities);
int tp3780I_Cleanup(THINKPAD_BD_DATA * pBDData);
int tp3780I_ReadWriteDspDStore(THINKPAD_BD_DATA * pBDData, unsigned int uOpcode,
                               void __user *pvBuffer, unsigned int uCount,
                               unsigned long ulDSPAddr);
int tp3780I_ReadWriteDspIStore(THINKPAD_BD_DATA * pBDData, unsigned int uOpcode,
                               void __user *pvBuffer, unsigned int uCount,
                               unsigned long ulDSPAddr);


#endif

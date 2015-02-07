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

#include <linux/autoconf.h>
#include <linux/linkage.h>
#include <asm/assembler.h>
#include <mach/hardware.h>

#define MT3351_DVFS_ITCM_PA	 0xF0400000
#define MT3351_EMI_CONN_PA	 0xF0020068
#define MT3351_CON_REGS_PA	 0xF0001208
#define MT3351_PMU_REGS_PA	 0xF0061300

	.text

ENTRY(DVFS_CLKSwitch_4x)  
		stmdb   r13!,{r4-r7,r14}

//	pITCM_Base = OALPAtoVA(0x50000000, FALSE)//
		ldr     r4,=MT3351_DVFS_ITCM_PA

//	pEMI_CONN =  OALPAtoVA(MT3351_EMI_CONN_PA, FALSE)//
		ldr     r5,=MT3351_EMI_CONN_PA

//	pCONFIG_REGS =  OALPAtoVA(MT3351_CON_REGS_PA, FALSE)//
		ldr     r6,=MT3351_CON_REGS_PA

//	pPMU_REGS =  OALPAtoVA(MT3351_PMU_REGS_PA, FALSE)//
		ldr     r7,=MT3351_PMU_REGS_PA

//	copy suspend code to ITCM
		mov     r0,r4					// destination address
		ldr     r1,=clk_4x_switch_start
		mov     r2,#0x300				// copy 300 byte length 
		bl      memcpy
		mov     pc, r4					// set Program counter to ITCM0
		nop

clk_4x_switch_start:
//	*pEMI_CONN |= 0x20//				//// 2. set DRAM into self refresh mode
		ldr     r3,[r5]
		orr     r3,r3,#0x20
		str     r3,[r5]

//									//// 3. count over 32 bus clk cycle
		mov		r0,#0x40
dvfs_delay1_4x:
		sub		r0,r0,#0x1
		cmp     r0,#0x0
		bgt     dvfs_delay1_4x


// 441  : 	s_pConforDVFSRegs->CLKCON &= 0xFFFCFFFF//

        ldr     r3, [r6, #0]        //// 4. switch clock frequency
        mvn     r4, #3, 16
        and     r3, r3, r4
        str		r3,[r6,#0]
        
// s_pConforDVFSRegs->CLKCON |= ( (ARM_DIV_1 << 16) ) //
// no assembly code

//									
		mov		r6,#0x10            //// 5. count over 8 clk cycle
dvfs_delay2_4x:
		sub		r6,r6,#0x1
		cmp     r6,#0x0
		bgt     dvfs_delay2_4x

		ldr     r3,[r5]             //// 6. disable DRAM 
		bic     r3,r3,#0x1
		str     r3,[r5]

//	*pEMI_CONN &= ~0x20//			//// 7. leave self refresh mode 
		ldr     r3,[r5]
		bic     r3,r3,#0x20
		str     r3,[r5]

		mov		r6,#0xFF            //// 8. count over 200 clk cycle
dvfs_delay3_4x:
		sub		r6,r6,#0x1
		cmp     r6,#0x0
		bgt     dvfs_delay3_4x

		ldr     r3,[r5]             //// 9. enable DRAM 
		orr     r3,r3,#0x1
		str     r3,[r5]


		ldmia   r13!,{r4-r7,r14}
		bx      r14



ENTRY(DVFS_CLKSwitch_2x)  

		stmdb   r13!,{r4-r7,r14}

//	pITCM_Base = OALPAtoVA(0x50000000, FALSE)//
		ldr     r4,=MT3351_DVFS_ITCM_PA

//	pEMI_CONN =  OALPAtoVA(MT3351_EMI_CONN_PA, FALSE)//
		ldr     r5,=MT3351_EMI_CONN_PA

//	pCONFIG_REGS =  OALPAtoVA(MT3351_CON_REGS_PA, FALSE)//
		ldr     r6,=MT3351_CON_REGS_PA

//	pPMU_REGS =  OALPAtoVA(MT3351_PMU_REGS_PA, FALSE)//
		ldr     r7,=MT3351_PMU_REGS_PA

//	copy suspend code to ITCM
		mov     r0,r4					// destination address
		ldr     r1,=clk_2x_switch_start
		mov     r2,#0x300				// copy 300 byte length 
		bl      memcpy
		mov     pc, r4					// set Program counter to ITCM0
		nop

clk_2x_switch_start:
//	*pEMI_CONN |= 0x20//				//// 2. set DRAM into self refresh mode
		ldr     r3,[r5]
		orr     r3,r3,#0x20
		str     r3,[r5]

//									//// 3. count over 32 bus clk cycle
		mov		r0,#0x40
dvfs_delay1_2x:
		sub		r0,r0,#0x1
		cmp     r0,#0x0
		bgt     dvfs_delay1_2x


// 441  : 	s_pConforDVFSRegs->CLKCON &= 0xFFFCFFFF//

        ldr     r3, [r6, #0]        //// 4. switch clock frequency
        mvn     r4, #3, 16
        and     r3, r3, r4
        orr     r3, r3, #1, 16
        str		r3,[r6,#0]
        
// s_pConforDVFSRegs->CLKCON |= ( (ARM_DIV_2 << 16) ) //

		mov		r6,#0x10            //// 5. count over 8 clk cycle
dvfs_delay2_2x:
		sub		r6,r6,#0x1
		cmp     r6,#0x0
		bgt     dvfs_delay2_2x

		ldr     r3,[r5]             //// 6. disable DRAM 
		bic     r3,r3,#0x1
		str     r3,[r5]		

//	*pEMI_CONN &= ~0x20//			//// 7. leave self refresh mode 
		ldr     r3,[r5]
		bic     r3,r3,#0x20
		str     r3,[r5]

		mov		r6,#0xFF            //// 8. count over 200 clk cycle
dvfs_delay3_2x:
		sub		r6,r6,#0x1
		cmp     r6,#0x0
		bgt     dvfs_delay3_2x

		ldr     r3,[r5]             //// 9. enable DRAM 
		orr     r3,r3,#0x1
		str     r3,[r5]


		ldmia   r13!,{r4-r7,r14}
		bx      r14



ENTRY(DVFS_CLKSwitch_1x)  

		stmdb   r13!,{r4-r7,r14}

//	pITCM_Base = OALPAtoVA(0x50000000, FALSE)//
		ldr     r4,=MT3351_DVFS_ITCM_PA

//	pEMI_CONN =  OALPAtoVA(MT3351_EMI_CONN_PA, FALSE)//
		ldr     r5,=MT3351_EMI_CONN_PA

//	pCONFIG_REGS =  OALPAtoVA(MT3351_CON_REGS_PA, FALSE)//
		ldr     r6,=MT3351_CON_REGS_PA

//	pPMU_REGS =  OALPAtoVA(MT3351_PMU_REGS_PA, FALSE)//
		ldr     r7,=MT3351_PMU_REGS_PA

//	copy suspend code to ITCM
		mov     r0,r4					// destination address
		ldr     r1,=clk_1x_switch_start
		mov     r2,#0x300				// copy 300 byte length 
		bl      memcpy
		mov     pc, r4					// set Program counter to ITCM0
		nop

clk_1x_switch_start:
//	*pEMI_CONN |= 0x20//				//// 2. set DRAM into self refresh mode
		ldr     r3,[r5]
		orr     r3,r3,#0x20
		str     r3,[r5]

//									//// 3. count over 32 bus clk cycle
		mov		r0,#0x40
dvfs_delay1_1x:
		sub		r0,r0,#0x1
		cmp     r0,#0x0
		bgt     dvfs_delay1_1x


// 441  : 	s_pConforDVFSRegs->CLKCON &= 0xFFFCFFFF//
// s_pConforDVFSRegs->CLKCON |= ( (ARM_DIV_2 << 16) ) //
        ldr     r3, [r6, #0]        //// 4. switch clock frequency
        mvn     r4, #3, 16
        and     r3, r3, r4
        orr     r3, r3, #3, 16
        str		r3,[r6,#0]
        
		mov		r6,#0xFF            //// 5. count over 8 clk cycle
dvfs_delay2_1x:
		sub		r6,r6,#0x1
		cmp     r6,#0x0
		bgt     dvfs_delay2_1x

		ldr     r3,[r5]             //// 6. disable DRAM 
		bic     r3,r3,#0x1
		str     r3,[r5]

//	*pEMI_CONN &= ~0x20//			//// 7. leave self refresh mode 
		ldr     r3,[r5]
		bic     r3,r3,#0x20
		str     r3,[r5]

		mov		r6,#0xFF            //// 8. count over 200 clk cycle
dvfs_delay3_1x:
		sub		r6,r6,#0x1
		cmp     r6,#0x0
		bgt     dvfs_delay3_1x

		ldr     r3,[r5]             //// 9. enable DRAM 
		orr     r3,r3,#0x1
		str     r3,[r5]


		ldmia   r13!,{r4-r7,r14}
		bx      r14



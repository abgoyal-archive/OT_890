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

#define ENABLE_DPI_INTERRUPT        0
#define ENABLE_DPI_REFRESH_RATE_LOG 0

#if ENABLE_DPI_REFRESH_RATE_LOG && !ENABLE_DPI_INTERRUPT
#error "ENABLE_DPI_REFRESH_RATE_LOG should be also ENABLE_DPI_INTERRUPT"
#endif

#include <mach/mt6516_typedefs.h>
#include <mach/mt6516_reg_base.h>
#include <mach/mt6516_pll.h>

#include "dpi_reg.h"
#include "dpi_drv.h"

#if ENABLE_DPI_INTERRUPT
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <asm/tcm.h>
#include <mach/irqs.h>
#include "mtkfb.h"
#endif


PDPI_REGS const DPI_REG = (PDPI_REGS)(DPI_BASE);

static BOOL s_isDpiPowerOn = FALSE;
static DPI_REGS regBackup;

#define DPI_REG_OFFSET(r)       offsetof(DPI_REGS, r)
#define REG_ADDR(base, offset)  (((BYTE *)(base)) + (offset))

const UINT32 BACKUP_DPI_REG_OFFSETS[] =
{
    DPI_REG_OFFSET(INT_ENABLE),
    DPI_REG_OFFSET(SIZE),
    DPI_REG_OFFSET(CLK_CNTL),
    DPI_REG_OFFSET(DITHER),

    DPI_REG_OFFSET(FB[0].ADDR),
    DPI_REG_OFFSET(FB[0].STEP),
    DPI_REG_OFFSET(FB[1].ADDR),
    DPI_REG_OFFSET(FB[1].STEP),
    DPI_REG_OFFSET(FB[2].ADDR),
    DPI_REG_OFFSET(FB[2].STEP),

    DPI_REG_OFFSET(OVL_CON),
    DPI_REG_OFFSET(FBCD_LINE_W),
    DPI_REG_OFFSET(FIFO_TH),
    DPI_REG_OFFSET(FIFO_INC),
    DPI_REG_OFFSET(FIFO_TH_MAX),
    DPI_REG_OFFSET(FIFO_MAX),

    DPI_REG_OFFSET(TGEN_HCNTL),
    DPI_REG_OFFSET(TGEN_VCNTL),

    DPI_REG_OFFSET(CUSR_CNTL),
    DPI_REG_OFFSET(CUSR_COORD),
    DPI_REG_OFFSET(CUSR_ADDR),

    DPI_REG_OFFSET(TMODE),

    DPI_REG_OFFSET(CNTL),
};

static void _BackupDPIRegisters(void)
{
    DPI_REGS *reg = &regBackup;
    UINT32 i;
    
    for (i = 0; i < ARY_SIZE(BACKUP_DPI_REG_OFFSETS); ++ i)
    {
        OUTREG32(REG_ADDR(reg, BACKUP_DPI_REG_OFFSETS[i]),
                 AS_UINT32(REG_ADDR(DPI_REG, BACKUP_DPI_REG_OFFSETS[i])));
    }
}

static void _RestoreDPIRegisters(void)
{
    DPI_REGS *reg = &regBackup;
    UINT32 i;

    for (i = 0; i < ARY_SIZE(BACKUP_DPI_REG_OFFSETS); ++ i)
    {
        OUTREG32(REG_ADDR(DPI_REG, BACKUP_DPI_REG_OFFSETS[i]),
                 AS_UINT32(REG_ADDR(reg, BACKUP_DPI_REG_OFFSETS[i])));
    }
}

static void _ResetBackupedDPIRegisterValues(void)
{
    DPI_REGS *regs = &regBackup;
    memset((void*)regs, 0, sizeof(DPI_REGS));

    OUTREG32(&regs->CLK_CNTL, 0x00000101);
    OUTREG32(&regs->FIFO_MAX, 0x00000200);
}


#if ENABLE_DPI_REFRESH_RATE_LOG
static void _DPI_LogRefreshRate(DPI_REG_INTERRUPT status)
{
    static unsigned long prevUs = 0xFFFFFFFF;

    if (status.VSYNC)
    {
        struct timeval curr;
        do_gettimeofday(&curr);

        if (prevUs < curr.tv_usec)
        {
            printk("[DPI][Debug] Receive 1 vsync in %lu us\n", 
                   curr.tv_usec - prevUs);
        }
        prevUs = curr.tv_usec;
    }
}
#else
#define _DPI_LogRefreshRate(x)  do {} while(0)
#endif

#if ENABLE_DPI_INTERRUPT
static __tcmfunc irqreturn_t _DPI_InterruptHandler(int irq, void *dev_id)
{   
    static int counter = 0;
    DPI_REG_INTERRUPT status = DPI_REG->INT_STATUS;
    if (status.FIFO_EMPTY) ++ counter;

    if (status.VSYNC && counter) {
        printk("[Error] DPI FIFO is empty, "
               "received %d times interrupt !!!\n", counter);
        counter = 0;
    }

    _DPI_LogRefreshRate(status);

    return IRQ_HANDLED;
}
#endif


DPI_STATUS DPI_Init(BOOL isDpiPoweredOn)
{
    DPI_REG_CNTL cntl;

    if (isDpiPoweredOn) {
        _BackupDPIRegisters();
        s_isDpiPowerOn = TRUE;
    } else {
        _ResetBackupedDPIRegisterValues();
        DPI_PowerOn();
    }

    // Always enable frame shift protection and recovery
    cntl = DPI_REG->CNTL;
    cntl.FS_PROT_EN = 1;
    cntl.FS_RC_EN = 1;

    // Enable adaptive FIFO high/low threshold control
    cntl.ADP_EN = 1;
     
    OUTREG32(&DPI_REG->CNTL, AS_UINT32(&cntl));

    // Config ultra high threshold water mark
    {
        DPI_REG_FIFO_TH th = DPI_REG->FIFO_TH;

        th.LOW = 64;
        th.HIGH = 128;

        OUTREG32(&DPI_REG->FIFO_TH, AS_UINT32(&th));
        DPI_REG->FIFO_INC = 8;
    }

#if ENABLE_DPI_INTERRUPT
    if (request_irq(MT6516_DPI_IRQ_LINE,
        _DPI_InterruptHandler, 0, MTKFB_DRIVER, NULL) < 0)
    {
        printk("[DPI][ERROR] fail to request DPI irq\n"); 
        return DPI_STATUS_ERROR;
    }

    {
        DPI_REG_INTERRUPT enInt = DPI_REG->INT_ENABLE;
        enInt.FIFO_EMPTY = 1;
        enInt.VSYNC = 1;
        OUTREG32(&DPI_REG->INT_ENABLE, AS_UINT32(&enInt));
    }
#endif

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_Deinit(void)
{
    DPI_DisableClk();
    DPI_PowerOff();

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_PowerOn()
{
    if (!s_isDpiPowerOn)
    {
        BOOL ret = hwEnableClock(MT6516_PDN_MM_DPI, "DPI");
        ASSERT(ret);
        _RestoreDPIRegisters();
        s_isDpiPowerOn = TRUE;
    }

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_PowerOff()
{
    if (s_isDpiPowerOn)
    {
        BOOL ret = TRUE;
        _BackupDPIRegisters();
        ret = hwDisableClock(MT6516_PDN_MM_DPI, "DPI");
        ASSERT(ret);
        s_isDpiPowerOn = FALSE;
    }

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_EnableClk()
{
    DPI_REG_CNTL cntl = DPI_REG->CNTL;
    cntl.DPI_EN = 1;
    OUTREG32(&DPI_REG->CNTL, AS_UINT32(&cntl));

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_DisableClk()
{
    DPI_REG_CNTL cntl = DPI_REG->CNTL;
    cntl.DPI_EN = 0;
    OUTREG32(&DPI_REG->CNTL, AS_UINT32(&cntl));

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_EnableSeqOutput(BOOL enable)
{
    DPI_REG_CNTL cntl = DPI_REG->CNTL;
    cntl.INTF_68_EN = enable ? 1 : 0;
    OUTREG32(&DPI_REG->CNTL, AS_UINT32(&cntl));

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_SetRGBOrder(DPI_RGB_ORDER input, DPI_RGB_ORDER output)
{
    DPI_REG_CNTL cntl = DPI_REG->CNTL;
    cntl.SRGB_ORDER = input;    // Source RGB Order
    cntl.IRGB_ORDER = output;   // Interface RGB Order
    OUTREG32(&DPI_REG->CNTL, AS_UINT32(&cntl));

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_ConfigPixelClk(DPI_POLARITY polarity, UINT32 divisor, UINT32 duty)
{
    DPI_REG_CLKCNTL ctrl;

    ASSERT(divisor >= 2);
    ASSERT(duty > 0 && duty < divisor);
    
    ctrl.POLARITY = (DPI_POLARITY_FALLING == polarity) ? 1 : 0;
    ctrl.DIVISOR = divisor - 1;
    ctrl.DUTY = duty;

    OUTREG32(&DPI_REG->CLK_CNTL, AS_UINT32(&ctrl));

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_ConfigDataEnable(DPI_POLARITY polarity)
{
    DPI_REG_TGEN_HCNTL hctrl = DPI_REG->TGEN_HCNTL;
    hctrl.DE_POL = (DPI_POLARITY_FALLING == polarity) ? 1 : 0;
    OUTREG32(&DPI_REG->TGEN_HCNTL, AS_UINT32(&hctrl));
    
    return DPI_STATUS_OK;
}


DPI_STATUS DPI_ConfigVsync(DPI_POLARITY polarity, UINT32 pulseWidth, UINT32 backPorch,
                           UINT32 frontPorch)
{
    DPI_REG_TGEN_VCNTL ctrl;

    ctrl.VSYNC_POL = (DPI_POLARITY_RISING == polarity) ? 1 : 0;
    ctrl.VPW = pulseWidth - 1;
    ctrl.VBP = backPorch - 1;
    ctrl.VFP = frontPorch - 1;

    OUTREG32(&DPI_REG->TGEN_VCNTL, AS_UINT32(&ctrl));
    
    return DPI_STATUS_OK;
}


DPI_STATUS DPI_ConfigHsync(DPI_POLARITY polarity, UINT32 pulseWidth, UINT32 backPorch,
                           UINT32 frontPorch)
{
    DPI_REG_TGEN_HCNTL ctrl = DPI_REG->TGEN_HCNTL;

    ctrl.HSYNC_POL = (DPI_POLARITY_RISING == polarity) ? 1 : 0;
    ctrl.HPW = pulseWidth - 1;
    ctrl.HBP = backPorch - 1;
    ctrl.HFP = frontPorch - 1;

    OUTREG32(&DPI_REG->TGEN_HCNTL, AS_UINT32(&ctrl));

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_FBEnable(DPI_FB_ID id, BOOL enable)
{
    DPI_REG_CNTL cntl = DPI_REG->CNTL;

    switch (id)
    {
        case DPI_FB_0:
            // do nothing
            break;

        case DPI_FB_1:
            cntl.FB1_EN = enable ? 1 : 0;
            break;

        case DPI_FB_2:
            cntl.FB2_EN = enable ? 1 : 0;
            break;

        default:
            ASSERT(0);
    }

    OUTREG32(&DPI_REG->CNTL, AS_UINT32(&cntl));

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_FBSyncFlipWithLCD(BOOL enable)
{
    DPI_REG_CNTL cntl = DPI_REG->CNTL;
    cntl.FB_CHK_EN = enable ? 1 : 0;
    OUTREG32(&DPI_REG->CNTL, AS_UINT32(&cntl));

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_FBSetFormat(DPI_FB_FORMAT format)
{
    DPI_REG_CNTL cntl = DPI_REG->CNTL;
    cntl.PXL_FMT = format;
    OUTREG32(&DPI_REG->CNTL, AS_UINT32(&cntl));

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_FBSetSize(UINT32 width, UINT32 height)
{
    DPI_REG_SIZE size;
    size.WIDTH = width - 1;
    size.HEIGHT = height - 1;
    
    OUTREG32(&DPI_REG->SIZE, AS_UINT32(&size));

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_FBSetAddress(DPI_FB_ID id, UINT32 address)
{
    ASSERT(id < DPI_FB_NUM);
    DPI_REG->FB[id].ADDR = address;

    return DPI_STATUS_OK;
}    


DPI_STATUS DPI_FBSetPitch(DPI_FB_ID id, UINT32 pitchInByte)
{
    ASSERT(id < DPI_FB_NUM);
    DPI_REG->FB[id].STEP = pitchInByte;

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_SetFifoThreshold(UINT32 low, UINT32 high)
{
    DPI_REG_FIFO_TH th = DPI_REG->FIFO_TH;

    if (high > DPI_FIFO_TH_MAX) {
        high = DPI_FIFO_TH_MAX;
    }
    if (low > high) {
        low = high;
    }

    th.LOW = low;
    th.HIGH = high;
    OUTREG32(&DPI_REG->FIFO_TH, AS_UINT32(&th));

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_DumpRegisters(void)
{
    UINT32 i;

    printk("---------- Start dump DPI registers ----------\n");
    
    for (i = 0; i < sizeof(DPI_REGS); i += 4)
    {
        printk("DPI+%04x : 0x%08x\n", i, INREG32(DPI_BASE + i));
    }

    return DPI_STATUS_OK;
}


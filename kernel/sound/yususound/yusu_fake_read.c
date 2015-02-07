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
*  permission of MediaTek Inc. (C) 2009
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


/*******************************************************************************
 *
 * Filename:
 * ---------
 * Yusu_fake_read.c
 *
 * Project:
 * --------
 *   yusu
 *
 * Description:
 * ------------
 *   basic ASM  setting
 *
 * Author:
 * -------
 *   ChiPeng Chang (mtk02308)
 *
 *
 *------------------------------------------------------------------------------
 * $Revision$
 * $Modtime:$
 * $Log:$
 *
 * 07 13 2010 chipeng.chang
 * [ALPS00121732][FactoryMode] Error in headset when enter it with headset plugged in 
 * modify kernel log.
 *
 * 07 13 2010 chipeng.chang
 * [ALPS00121732][FactoryMode] Error in headset when enter it with headset plugged in 
 * modify kernel log.
 *
 * 06 01 2010 chipeng.chang
 * [ALPS00002042][Patch rule]

If you fix the issue, please check-in your code and choose the correct type on the CR you use to check in BU Spec tab, thanks.

 
 * patch for I2S driver and add AudioyusuI2sTreamin for record
 *
 *******************************************************************************/

 #include <linux/kernel.h>
 #include <linux/module.h>
 #include <linux/init.h>
 #include <linux/device.h>
 #include <linux/slab.h>
 #include <linux/fs.h>
 #include <linux/mm.h>
 #include <linux/delay.h>
 #include <linux/interrupt.h>
 #include <linux/dma-mapping.h>
 #include <linux/vmalloc.h>
 #include <linux/wait.h>
 #include <linux/spinlock.h>
 #include <linux/semaphore.h>
 #include <asm/uaccess.h>
 #include <asm/io.h>
 #include <linux/jiffies.h>
 #include <mach/mt6516_typedefs.h>
 #include <linux/proc_fs.h>
 #include <linux/string.h>

 #include <linux/pmic6326_sw.h>
#include <mach/mt6516_typedefs.h>
#include "yusu_audio_stream.h"
#include <linux/spinlock.h>

#define SOUND_FAKE_READ_SIZE (128)

#ifdef CONFIG_DEBUG_MSG
#define PRINTK(format, args...) printk( KERN_EMERG format,##args )
#else
#define PRINTK(format, args...)
#endif


static short Tone_1k[] = {
    0x0   ,     0x0,          0xa02     ,0xa02,
    0x13a4   ,0x13a3,    0x1c83   ,0x1c83,
    0x2449   ,0x2449,    0x2aac   ,0x2aac,
    0x2f6a   ,0x2f69,    0x3254   ,0x3255,
    0x3352   ,0x3351,    0x3255   ,0x3255,
    0x2f6a   ,0x2f69,    0x2aac   ,0x2aac,
    0x2449   ,0x244a,    0x1c83   ,0x1c83,
    0x13a4   ,0x13a3,    0xa03     ,0xa02,
    0x0         ,0x0     ,    0xf5fd   ,0xf5fd,
    0xec5d   ,0xec5c,    0xe37e   ,0xe37e,
    0xdbb7   ,0xdbb7,    0xd555   ,0xd555,
    0xd097   ,0xd096,    0xcdab   ,0xcdac,
    0xccaf   ,0xccae,    0xcdab   ,0xcdab,
    0xd096   ,0xd097,    0xd554   ,0xd554,
    0xdbb7   ,0xdbb7,    0xe37e   ,0xe37d,
    0xec5d   ,0xec5d,    0xf5fd   ,0xf5fe
};

static char* Tone_ptr =(char*)Tone_1k;
static unsigned int  count = 0;

void copy_to_user_fake(char* Read_Data_Ptr ,int read_size)
{
    uint32_t copy_size =0;
    char*write_ptr = Read_Data_Ptr;
    PRINTK("copy_to_user_fake read_size = %d Tone_ptr count = %d\n",read_size,count);
    while(read_size > 0){
        if(count == 0){
            if (read_size >SOUND_FAKE_READ_SIZE){
                copy_size = SOUND_FAKE_READ_SIZE;
            }
            else{
                copy_size = read_size;
            }
            //printk("1 copy_to_user_  copy_size = %d Tone_ptr count = %d read_size =%d \n",copy_size,count,read_size);
            copy_to_user( (void __user *)write_ptr, Tone_ptr, copy_size);
            read_size -= copy_size;
            write_ptr+= copy_size;
            count += copy_size;
            count %= SOUND_FAKE_READ_SIZE;
        }
        else if(count > 0 && read_size >= (SOUND_FAKE_READ_SIZE - count)){
            copy_size = SOUND_FAKE_READ_SIZE - count;
            PRINTK("2 copy_to_user_  copy_size = %d Tone_ptr count = %d read_size =%d \n",copy_size,count,read_size);
            copy_to_user( (void __user *)write_ptr, (Tone_ptr+count) ,copy_size);
            read_size -= copy_size;
            write_ptr+= copy_size;
            count =0;
        }
        else if(count > 0 && read_size < (SOUND_FAKE_READ_SIZE - count)){
            copy_size = read_size;
            PRINTK("3 copy_to_user_  copy_size = %d Tone_ptr count = %d read_size =%d \n",copy_size,count,read_size);
            copy_to_user( (void __user *)write_ptr, (Tone_ptr+count) ,copy_size);
            read_size -= copy_size;
            write_ptr+= copy_size;
            count += copy_size;
            count %= SOUND_FAKE_READ_SIZE;
        }
    }
}





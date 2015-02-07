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

/***************************************************************************/

/*
 *  linux/arch/m68knommu/platform/68EZ328/config.c
 *
 *  Copyright (C) 1993 Hamish Macdonald
 *  Copyright (C) 1999 D. Jeff Dionne
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 */

/***************************************************************************/

#include <linux/types.h>
#include <linux/kernel.h>
#include <asm/system.h>
#include <asm/pgtable.h>
#include <asm/machdep.h>
#include <asm/MC68EZ328.h>
#ifdef CONFIG_UCSIMM
#include <asm/bootstd.h>
#endif

/***************************************************************************/

void m68328_timer_gettod(int *year, int *mon, int *day, int *hour, int *min, int *sec);

/***************************************************************************/

void m68ez328_reset(void)
{
  local_irq_disable();
  asm volatile (
    "moveal #0x10c00000, %a0;\n"
    "moveb #0, 0xFFFFF300;\n"
    "moveal 0(%a0), %sp;\n"
    "moveal 4(%a0), %a0;\n"
    "jmp (%a0);\n"
    );
}

/***************************************************************************/

unsigned char *cs8900a_hwaddr;
static int errno;

#ifdef CONFIG_UCSIMM
_bsc0(char *, getserialnum)
_bsc1(unsigned char *, gethwaddr, int, a)
_bsc1(char *, getbenv, char *, a)
#endif

void config_BSP(char *command, int len)
{
  unsigned char *p;

  printk(KERN_INFO "\n68EZ328 DragonBallEZ support (C) 1999 Rt-Control, Inc\n");

#ifdef CONFIG_UCSIMM
  printk(KERN_INFO "uCsimm serial string [%s]\n",getserialnum());
  p = cs8900a_hwaddr = gethwaddr(0);
  printk(KERN_INFO "uCsimm hwaddr %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
         p[0], p[1], p[2], p[3], p[4], p[5]);

  p = getbenv("APPEND");
  if (p) strcpy(p,command);
  else command[0] = 0;
#endif
 
  mach_gettod = m68328_timer_gettod;
  mach_reset = m68ez328_reset;
}

/***************************************************************************/

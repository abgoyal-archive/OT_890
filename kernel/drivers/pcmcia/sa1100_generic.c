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

/*======================================================================

    Device driver for the PCMCIA control functionality of StrongARM
    SA-1100 microprocessors.

    The contents of this file are subject to the Mozilla Public
    License Version 1.1 (the "License"); you may not use this file
    except in compliance with the License. You may obtain a copy of
    the License at http://www.mozilla.org/MPL/

    Software distributed under the License is distributed on an "AS
    IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
    implied. See the License for the specific language governing
    rights and limitations under the License.

    The initial developer of the original code is John G. Dorsey
    <john+@cs.cmu.edu>.  Portions created by John G. Dorsey are
    Copyright (C) 1999 John G. Dorsey.  All Rights Reserved.

    Alternatively, the contents of this file may be used under the
    terms of the GNU Public License version 2 (the "GPL"), in which
    case the provisions of the GPL are applicable instead of the
    above.  If you wish to allow the use of your version of this file
    only under the terms of the GPL and not to allow others to use
    your version of this file under the MPL, indicate your decision
    by deleting the provisions above and replace them with the notice
    and other provisions required by the GPL.  If you do not delete
    the provisions above, a recipient may use your version of this
    file under either the MPL or the GPL.
    
======================================================================*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>

#include <pcmcia/cs_types.h>
#include <pcmcia/cs.h>
#include <pcmcia/ss.h>

#include <asm/hardware/scoop.h>

#include "sa1100_generic.h"

int __init pcmcia_collie_init(struct device *dev);

static int (*sa11x0_pcmcia_hw_init[])(struct device *dev) = {
#ifdef CONFIG_SA1100_ASSABET
	pcmcia_assabet_init,
#endif
#ifdef CONFIG_SA1100_CERF
	pcmcia_cerf_init,
#endif
#ifdef CONFIG_SA1100_H3600
	pcmcia_h3600_init,
#endif
#ifdef CONFIG_SA1100_SHANNON
	pcmcia_shannon_init,
#endif
#ifdef CONFIG_SA1100_SIMPAD
	pcmcia_simpad_init,
#endif
#ifdef CONFIG_SA1100_COLLIE
       pcmcia_collie_init,
#endif
};

static int sa11x0_drv_pcmcia_probe(struct device *dev)
{
	int i, ret = -ENODEV;

	/*
	 * Initialise any "on-board" PCMCIA sockets.
	 */
	for (i = 0; i < ARRAY_SIZE(sa11x0_pcmcia_hw_init); i++) {
		ret = sa11x0_pcmcia_hw_init[i](dev);
		if (ret == 0)
			break;
	}

	return ret;
}

static struct device_driver sa11x0_pcmcia_driver = {
	.probe		= sa11x0_drv_pcmcia_probe,
	.remove		= soc_common_drv_pcmcia_remove,
	.name		= "sa11x0-pcmcia",
	.bus		= &platform_bus_type,
	.suspend 	= pcmcia_socket_dev_suspend,
	.resume 	= pcmcia_socket_dev_resume,
};

/* sa11x0_pcmcia_init()
 * ^^^^^^^^^^^^^^^^^^^^
 *
 * This routine performs low-level PCMCIA initialization and then
 * registers this socket driver with Card Services.
 *
 * Returns: 0 on success, -ve error code on failure
 */
static int __init sa11x0_pcmcia_init(void)
{
	return driver_register(&sa11x0_pcmcia_driver);
}

/* sa11x0_pcmcia_exit()
 * ^^^^^^^^^^^^^^^^^^^^
 * Invokes the low-level kernel service to free IRQs associated with this
 * socket controller and reset GPIO edge detection.
 */
static void __exit sa11x0_pcmcia_exit(void)
{
	driver_unregister(&sa11x0_pcmcia_driver);
}

MODULE_AUTHOR("John Dorsey <john+@cs.cmu.edu>");
MODULE_DESCRIPTION("Linux PCMCIA Card Services: SA-11x0 Socket Controller");
MODULE_LICENSE("Dual MPL/GPL");

fs_initcall(sa11x0_pcmcia_init);
module_exit(sa11x0_pcmcia_exit);

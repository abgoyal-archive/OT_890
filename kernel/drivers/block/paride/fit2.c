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
        fit2.c        (c) 1998  Grant R. Guenther <grant@torque.net>
                          Under the terms of the GNU General Public License.

	fit2.c is a low-level protocol driver for the older version
        of the Fidelity International Technology parallel port adapter.  
	This adapter is used in their TransDisk 2000 and older TransDisk
	3000 portable hard-drives.  As far as I can tell, this device
	supports 4-bit mode _only_.  

	Newer models of the FIT products use an enhanced protocol.
	The "fit3" protocol module should support current drives.

*/

#define FIT2_VERSION      "1.0"

#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/wait.h>
#include <asm/io.h>

#include "paride.h"

#define j44(a,b)                (((a>>4)&0x0f)|(b&0xf0))

/* cont = 0 - access the IDE register file 
   cont = 1 - access the IDE command set 

NB:  The FIT adapter does not appear to use the control registers.
So, we map ALT_STATUS to STATUS and NO-OP writes to the device
control register - this means that IDE reset will not work on these
devices.

*/

static void  fit2_write_regr( PIA *pi, int cont, int regr, int val)

{	if (cont == 1) return;
	w2(0xc); w0(regr); w2(4); w0(val); w2(5); w0(0); w2(4);
}

static int fit2_read_regr( PIA *pi, int cont, int regr )

{	int  a, b, r;

	if (cont) {
	  if (regr != 6) return 0xff;
	  r = 7;
	} else r = regr + 0x10;

	w2(0xc); w0(r); w2(4); w2(5); 
	         w0(0); a = r1();
	         w0(1); b = r1();
	w2(4);

	return j44(a,b);

}

static void fit2_read_block( PIA *pi, char * buf, int count )

{	int  k, a, b, c, d;

	w2(0xc); w0(0x10);

	for (k=0;k<count/4;k++) {

		w2(4); w2(5);
		w0(0); a = r1(); w0(1); b = r1();
		w0(3); c = r1(); w0(2); d = r1(); 
		buf[4*k+0] = j44(a,b);
		buf[4*k+1] = j44(d,c);

                w2(4); w2(5);
                       a = r1(); w0(3); b = r1();
                w0(1); c = r1(); w0(0); d = r1(); 
                buf[4*k+2] = j44(d,c);
                buf[4*k+3] = j44(a,b);

	}

	w2(4);

}

static void fit2_write_block( PIA *pi, char * buf, int count )

{	int k;


	w2(0xc); w0(0); 
	for (k=0;k<count/2;k++) {
		w2(4); w0(buf[2*k]); 
		w2(5); w0(buf[2*k+1]);
	}
	w2(4);
}

static void fit2_connect ( PIA *pi  )

{       pi->saved_r0 = r0();
        pi->saved_r2 = r2();
	w2(0xcc); 
}

static void fit2_disconnect ( PIA *pi )

{       w0(pi->saved_r0);
        w2(pi->saved_r2);
} 

static void fit2_log_adapter( PIA *pi, char * scratch, int verbose )

{       printk("%s: fit2 %s, FIT 2000 adapter at 0x%x, delay %d\n",
                pi->device,FIT2_VERSION,pi->port,pi->delay);

}

static struct pi_protocol fit2 = {
	.owner		= THIS_MODULE,
	.name		= "fit2",
	.max_mode	= 1,
	.epp_first	= 2,
	.default_delay	= 1,
	.max_units	= 1,
	.write_regr	= fit2_write_regr,
	.read_regr	= fit2_read_regr,
	.write_block	= fit2_write_block,
	.read_block	= fit2_read_block,
	.connect	= fit2_connect,
	.disconnect	= fit2_disconnect,
	.log_adapter	= fit2_log_adapter,
};

static int __init fit2_init(void)
{
	return paride_register(&fit2);
}

static void __exit fit2_exit(void)
{
	paride_unregister(&fit2);
}

MODULE_LICENSE("GPL");
module_init(fit2_init)
module_exit(fit2_exit)

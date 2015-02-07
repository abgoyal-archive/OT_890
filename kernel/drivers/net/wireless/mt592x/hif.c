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

/*
** $Id: //ALPS_SW/TRUNK/MAIN/alps/kernel/drivers/net/wireless/mt592x/sdio.c#3 $
*/

/*! \file   "sdio.c"
    \brief  This file includes the operation of sdio interface

    Detail description.
*/

/*******************************************************************************
* Copyright (c) 2007 MediaTek Inc.
*
* All rights reserved. Copying, compilation, modification, distribution
* or any other use whatsoever of this material is strictly prohibited
* except in accordance with a Software License Agreement with
* MediaTek Inc.
********************************************************************************
*/

/*******************************************************************************
* LEGAL DISCLAIMER
*
* BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND
* AGREES THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK
* SOFTWARE") RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE
* PROVIDED TO BUYER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY
* DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT
* LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE
* ANY WARRANTY WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY
* WHICH MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK
* SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY
* WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE
* FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION OR TO
* CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
* BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
* LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL
* BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT
* ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY
* BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
* THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
* WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT
* OF LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING
* THEREOF AND RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN
* FRANCISCO, CA, UNDER THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE
* (ICC).
********************************************************************************
*/

/*
** $Log: sdio.c $
 *
 * 11 07 2010 renbang.jiang
 * [ALPS00133024] [Power Consumption] [Wi-Fi] Turn off SD host power and clock when system suspend with Wi-Fi disconnected
 * .
 *
 * 06 13 2010 renbang.jiang
 * [ALPS00008206][Need Patch] [Volunteer Patch] ALPS.10X.W10.11 Volunteer patch for fixing build warning 
 * .
 *
 * 06 13 2010 renbang.jiang
 * [ALPS00008206][Need Patch] [Volunteer Patch] ALPS.10X.W10.11 Volunteer patch for fixing build warning 
 * .
 *
 * 06 10 2010 jim.zhang
 * [ALPS00008099]change mt592x to UNGPL version 
 * .
 *
 * 05 14 2010 renbang.jiang
 * [ALPS00006196][Need Patch] [Volunteer Patch] ALPS.10X.W10.11 Volunteer patch for 
 * .
**  \main\maintrunk.MT5921\4 2009-10-08 10:33:30 GMT mtk01090
**  Avoid accessing private data of net_device directly. Replace with netdev_priv(). Add more checking for input parameters and pointers.
**  \main\maintrunk.MT5921\3 2009-09-28 20:19:17 GMT mtk01090
**  Add private ioctl to carry OID structures. Restructure public/private ioctl interfaces to Linux kernel.
**  \main\maintrunk.MT5921\2 2009-08-18 22:57:02 GMT mtk01090
**  Add Linux SDIO (with mmc core) support.
**  Add Linux 2.6.21, 2.6.25, 2.6.26.
**  Fix compile warning in Linux.
**  \main\maintrunk.MT5921\8 2008-09-22 23:17:33 GMT mtk01461
**  Update driver for code review
**  \main\maintrunk.MT5921\7 2008-09-05 17:25:10 GMT mtk01461
**  Update Driver for Code Review
**  \main\maintrunk.MT5921\6 2008-04-22 22:57:34 GMT mtk01084
**  modify port access function prototype
**  \main\maintrunk.MT5921\5 2008-03-20 15:47:42 GMT mtk01088
**  fixed the EHPI-8 comiling error, and add function declaration
**  \main\maintrunk.MT5921\4 2008-03-14 18:03:47 GMT mtk01084
**  refine register and port access function
**  \main\maintrunk.MT5921\3 2008-01-07 15:07:35 GMT mtk01461
**  Adjust EHPI timing
** Revision 1.4  2007/07/05 07:25:33  MTK01461
** Add Linux initial code, modify doc, add 11BB, RF init code
**
** Revision 1.3  2007/06/27 02:18:51  MTK01461
** Update SCAN_FSM, Initial(Can Load Module), Proc(Can do Reg R/W), TX API
**
** Revision 1.2  2007/06/25 06:16:24  MTK01461
** Update illustrations, gl_init.c, gl_kal.c, gl_kal.h, gl_os.h and RX API
**
*/



/*******************************************************************************
*                         C O M P I L E R   F L A G S
********************************************************************************
*/

/*******************************************************************************
*                    E X T E R N A L   R E F E R E N C E S
********************************************************************************
*/

#include <linux/mmc/card.h>
#include <linux/mmc/host.h>
#include <linux/mmc/sdio.h>
//#include <linux/mmc/protocol.h>

#include <linux/mmc/sdio_func.h> 
#include <linux/mmc/sdio_ids.h> 

#include <asm/memory.h>
#include <linux/mm.h>

#include <linux/firmware.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/platform_device.h>

#include "gl_typedef.h"
#include "mt592x_reg.h"
#include "platform.h"

#define BW_COMODULE

/*******************************************************************************
*                              C O N S T A N T S
********************************************************************************
*/
static const struct sdio_device_id mtk_sdio_ids[] = {
	{ SDIO_DEVICE(0x037a, 0x5921) }, /* Not an SDIO standard class device */
	{ /* end: all zeroes */ },
};

//static const int MAX_IOREQ_NUM = 15;

MODULE_DEVICE_TABLE(sdio, mtk_sdio_ids);

/*******************************************************************************
*                             D A T A   T Y P E S
********************************************************************************
*/
typedef void(*isr)(unsigned long u4Data);
typedef	int (*inthandler)(void *);

typedef struct _mtk_sdio_drv_t {
    struct net_device *prNetDevice;
    isr pfnIsr;
    void * prDevInfo;
} mtk_sdio_drv_t, *p_mtk_sdio_drv_t;


enum _ENUM_FUNC_IDX{
    FUNC_IDX_DS = 0, //Daisy chain
    FUNC_IDX_PLUGIN,   // plug-in func
    FUNC_IDX_NUM
};

/*******************************************************************************
*                            P U B L I C   D A T A
********************************************************************************
*/
void * pvSdio = NULL;
/*******************************************************************************
*                           P R I V A T E   D A T A
********************************************************************************
*/
static probe_card pfPluginProbe = NULL;
static remove_card pfPluginRemove = NULL;
static inthandler pfPluginInthandler = NULL;
static struct sdio_driver mtk_sdio_driver = {
	.name		= "MTKSD", /* "MTK SDIO WLAN Driver" */
	.id_table		= mtk_sdio_ids,
	.probe		= NULL,
	.remove		= NULL,
};

static int gRefCount;

#define DRIVER_NAME	"mt5921"

static struct platform_device  *mt5921_dev = NULL;
struct platform_driver  mt5921_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	},	
};

/*******************************************************************************
*                                 M A C R O S
********************************************************************************
*/
#define DRV_NAME "[SDIO Client]"
/*******************************************************************************
*                   F U N C T I O N   D E C L A R A T I O N S
********************************************************************************
*/
extern int platform_init(void);
extern void platform_deinit(void);
extern int register_comodule_cb(void * cb);
extern int deregister_comodule_cb(void);
//extern void mmc_power_up(struct mmc_host *host);
//extern void mmc_power_off(struct mmc_host *host);
extern void mmc_power_up_ext(struct mmc_host *host);
extern void mmc_power_off_ext(struct mmc_host *host);

int comodule_status_handler(int funcIdx, int op);
BOOL EnableDaisyChain( void * pvData );
BOOL mt5921SetSysOff( void * pvData	);
BOOL
hifBusInit (
    void * pvData
    );

static BOOL
hifDevRegRead (
       void *          pHif,
       unsigned long         u4Register,
      unsigned long *       pu4Value
    )
{
    struct sdio_func *prSdioFunc = (struct sdio_func *)pHif;
    int ret = 0;

    //printk(KERN_INFO DRV_NAME"++hifDevRegRead++ buf:0x%p, reg:0x%x\n", pu4Value, u4Register);

    sdio_claim_host(prSdioFunc);
    *pu4Value = sdio_readl(prSdioFunc, u4Register, &ret);
    sdio_release_host(prSdioFunc);

    //printk(KERN_INFO DRV_NAME"--hifDevRegRead-- buf:0x%p, reg:0x%x, val:0x%x\n", pu4Value, u4Register, *pu4Value);

    return (ret) ? FALSE : TRUE;
} /* end of hifDevRegRead() */

static BOOL
hifDevRegWrite (
       void *          pHif,
      unsigned long        u4Register,
      unsigned long        u4Value
    )
{
    struct sdio_func *prSdioFunc = (struct sdio_func *)pHif;
    int ret = 0;
    
    //printk(KERN_INFO DRV_NAME"++hifDevRegWrite++ reg:0x%x, val:0x%x\n", u4Register, u4Value);

    sdio_claim_host(prSdioFunc);
    sdio_writel(prSdioFunc, u4Value, u4Register, &ret);
    sdio_release_host(prSdioFunc);
    //printk(KERN_INFO DRV_NAME"hifDevRegWrite MCR(%#x) = %#x, %d\n", u4Register, u4Value, ret);

    //printk(KERN_INFO DRV_NAME"--hifDevRegWrite-- ret=%d\n", ret);

    return (ret) ? FALSE : TRUE;
} /* end of hifDevRegWrite() */

static BOOL
hifDevPortRead (
       void *          pHif,
       unsigned short         u2Port,
       unsigned short         u2Len,
      unsigned char *        pucBuf,
       unsigned short         u2ValidOutBufSize
    )
{
    struct sdio_func *prSdioFunc = NULL;
    unsigned char * pucDst = NULL;
    int count = u2Len;
    int ret = 0;

    //printk(KERN_INFO DRV_NAME"++hifDevPortRead++ buf:0x%p, port:0x%x, length:%d\n", pucBuf, u2Port, u2Len);
    prSdioFunc = (struct sdio_func *)pHif;

    pucDst = pucBuf;

    sdio_claim_host(prSdioFunc);
#if 0
    ret = sdio_readsb(prSdioFunc, pucBuf, u2Port, u2Len);
#else
    /* Split buffer into multiple single block to workaround s3c-hsmmc driver */
    while (!ret && count >= prSdioFunc->cur_blksize) {
        ret = sdio_readsb(prSdioFunc, pucDst, u2Port, prSdioFunc->cur_blksize);
        pucDst += prSdioFunc->cur_blksize;
        count -= prSdioFunc->cur_blksize;
    }

    if (!ret && count > 0) {
        ret = sdio_readsb(prSdioFunc, pucDst, u2Port, count);
    }

    if (ret) {
        goto release_host;
    }
release_host:
#endif

    sdio_release_host(prSdioFunc);

    //printk(KERN_INFO DRV_NAME"-- hifDevPortRead-- ret=%d\n", ret);

    return (ret) ? FALSE : TRUE;
} /* end of hifDevPortRead() */


static BOOL
hifDevPortWrite (
      void *         pHif,
      unsigned short        u2Port,
      unsigned short        u2Len,
      unsigned char *       pucBuf,
      unsigned short        u2ValidInBufSize
    )
{
    struct sdio_func *prSdioFunc = NULL;
    unsigned char * pucSrc = NULL;
    int count = u2Len;
    int ret = 0;

    //printk(KERN_INFO DRV_NAME"++hifDevPortWrite++ buf:0x%p, port:0x%x, length:%d\n", pucBuf, u2Port, u2Len);
    prSdioFunc = (struct sdio_func *)pHif;

    pucSrc = pucBuf;

    sdio_claim_host(prSdioFunc);

    /* Split buffer into multiple single block to workaround s3c-hsmmc driver */
    while (!ret && count >= prSdioFunc->cur_blksize) {
        ret = sdio_writesb(prSdioFunc, u2Port, pucSrc, prSdioFunc->cur_blksize);
        pucSrc += prSdioFunc->cur_blksize;
        count -= prSdioFunc->cur_blksize;
    }

    if (!ret && count > 0) {
        ret = sdio_writesb(prSdioFunc, u2Port, pucSrc, count);
    }

    sdio_release_host(prSdioFunc);

    //printk(KERN_INFO DRV_NAME"-- hifDevPortWrite-- ret=%d\n", ret);

    return (ret) ? FALSE : TRUE;
} /* end of hifDevPortWrite() */
/*******************************************************************************
*                              F U N C T I O N S
********************************************************************************
*/

/*******************************************************************/
/* SDIO callbacks                                                  */
/*******************************************************************/
void sdio_irq_func(void * pHif)
{
    struct sdio_func *prSdioFunc = ( struct sdio_func *)pHif;
    int ret = 0;

    //printk(KERN_INFO DRV_NAME"++ sdio_irq_func++\n");
    if(pfPluginInthandler == NULL)
    {
    	/*sdio_claim_host(prSdioFunc);
    	sdio_release_irq(prSdioFunc);
    	sdio_release_host(prSdioFunc);*/
		return;
    }
	

    sdio_writel(prSdioFunc, 0x20, 0x3C, &ret);
    pfPluginInthandler((void *)prSdioFunc);
    //printk(KERN_INFO DRV_NAME"-- sdio_irq_func--\n");
	
}



static int mtk_sdio_probe (
    struct sdio_func *func,
    const struct sdio_device_id *id
    )
{
    int ret = 0;
    int i = 0;
    BOOL bRet;

    printk(KERN_INFO DRV_NAME "mtk_sdio_probe()\n");

    printk(KERN_INFO DRV_NAME "Basic struct size checking...\n");
    printk(KERN_INFO DRV_NAME "sizeof(struct device) = %d\n", sizeof(struct device));
    printk(KERN_INFO DRV_NAME "sizeof(struct mmc_host) = %d\n", sizeof(struct mmc_host));
    printk(KERN_INFO DRV_NAME "sizeof(struct mmc_card) = %d\n", sizeof(struct mmc_card));
    printk(KERN_INFO DRV_NAME "sizeof(struct mmc_driver) = %d\n", sizeof(struct mmc_driver));
    printk(KERN_INFO DRV_NAME "sizeof(struct mmc_data) = %d\n", sizeof(struct mmc_data));
    printk(KERN_INFO DRV_NAME "sizeof(struct mmc_command) = %d\n", sizeof(struct mmc_command));
    printk(KERN_INFO DRV_NAME "sizeof(struct mmc_request) = %d\n", sizeof(struct mmc_request));
    printk(KERN_INFO DRV_NAME "sizeof(struct sdio_func) = %d\n", sizeof(struct sdio_func));

    printk(KERN_INFO DRV_NAME "Card information checking...\n");
    printk(KERN_INFO DRV_NAME "func = 0x%p\n", func);
    printk(KERN_INFO DRV_NAME "Number of info = %d:\n", func->card->num_info);

    for (i = 0;i < func->card->num_info;i++) {
        printk(KERN_INFO DRV_NAME "info[%d]: %s\n", i, func->card->info[i]);
    }

    sdio_claim_host(func);
    ret = sdio_enable_func(func);
    sdio_release_host(func);
	printk(KERN_INFO "sdio_enable_func ret=%d\n", ret);

    if (ret) {
        printk(KERN_INFO DRV_NAME"sdio_enable_func failed!\n");
        goto out;
    }
    printk(KERN_INFO DRV_NAME"sdio_enable_func done!\n");

	
    bRet = hifBusInit((void *)func);

    /* Cannot get IO address from interface */
    if (FALSE == bRet) {
		printk(KERN_INFO DRV_NAME " bus init failed!\n");
	    ret = -EIO;
		goto out;
    }
    
    platform_init();
    
    pvSdio = (void *)func;
	
#ifdef BW_COMODULE
		register_comodule_cb((void *)comodule_status_handler);
	
#else
		if (pfPluginProbe((PVOID)func) != WLAN_STATUS_SUCCESS) {
			printk(KERN_WARNING DRV_NAME"pfPluginProbe fail!call pfWlanRemove()\n");
			pfWlanRemove();
		}
#endif

   mt5921_dev = platform_device_alloc(DRIVER_NAME, 1);
   if(NULL == mt5921_dev)
	printk(KERN_INFO"platform_device_alloc ERORO\n");    
 
   ret = platform_device_add(mt5921_dev);
out:
    printk(KERN_INFO DRV_NAME"mtk_sdio_probe() done(%d)\n", ret);

    return ret;
}


static void
mtk_sdio_remove (
    struct sdio_func *func
    )
{
    printk(KERN_INFO DRV_NAME"mtk_sdio_remove()\n");

    platform_device_unregister(mt5921_dev);

    if(func == NULL){
        printk(KERN_WARNING DRV_NAME"mtk_sdio_remove, error,  invalid param\n");
        return;
    }

#ifdef BW_COMODULE
		comodule_status_handler(0,0);
		comodule_status_handler(1,0);
	
		deregister_comodule_cb();
#else
		pfPluginRemove();
		printk(KERN_INFO DRV_NAME"pfPluginRemove done\n");
#endif
    platform_deinit();
    
    pvSdio = NULL;

    sdio_claim_host(func);
    sdio_disable_func(func);
    printk(KERN_INFO DRV_NAME"sdio_disable_func() done\n");
    sdio_release_host(func);

    printk(KERN_INFO DRV_NAME"mtk_sdio_remove() done\n");
}

static int
mtk_sdio_suspend (
    struct device * pDev,
    pm_message_t state
    )
{
    printk(KERN_INFO "mtk_sdio: mtk_sdio_suspend dev(0x%p)\n", pDev);
    
    return 0;
}

int mtk_sdio_resume (
    struct device * pDev
    )
{
    printk(KERN_INFO "mtk_sdio: mtk_sdio_resume dev(0x%p)\n", pDev);

    return 0;
}

int hifBusSetPower(int pwrState)
{
    printk("[RB][hifBusSetPower] turn Bus power to %d\n",pwrState);
    if( 1 == pwrState ){
		sdio_claim_host((struct sdio_func *)pvSdio);
		mmc_power_up_ext( ((struct sdio_func *)pvSdio)->card->host );
		sdio_release_host((struct sdio_func *)pvSdio);
		

    }else{
		sdio_claim_host((struct sdio_func *)pvSdio);		
		mmc_power_off_ext( ((struct sdio_func *)pvSdio)->card->host );
		sdio_release_host((struct sdio_func *)pvSdio);
	}
	return 0;
}

int
hifRegisterBus (
    struct MT5921_ops * mtops,
    struct SDIOBUS_ops * busops
    )
{
    int ret = 0;

    printk(KERN_INFO DRV_NAME"++hifRegisterBus++\n");
    printk(KERN_INFO DRV_NAME"mtk_sdio: Copyright MediaTek Inc.\n");

    pfPluginProbe = mtops->probe;
    pfPluginRemove = mtops->remove;
    pfPluginInthandler = mtops->inthandler;

    busops->DevPortRead = hifDevPortRead;
    busops->DevPortWrite = hifDevPortWrite;
    busops->DevRegRead = hifDevRegRead;
    busops->DevRegWrite = hifDevRegWrite;
    busops->DevIrq = sdio_irq_func;
	busops->DevHostPower = hifBusSetPower;
    mtk_sdio_driver.probe = mtk_sdio_probe;
    mtk_sdio_driver.remove = mtk_sdio_remove;

    mtk_sdio_driver.drv.suspend = mtk_sdio_suspend;
    mtk_sdio_driver.drv.resume = mtk_sdio_resume;

    ret = sdio_register_driver(&mtk_sdio_driver);
	printk(KERN_INFO DRV_NAME"sdio_reg_drv ret=%d\n", ret);

    gRefCount ++;
	
    printk(KERN_INFO DRV_NAME"--hifRegisterBus--\n");
	
    return ret;
} /* end of hifRegisterBus() */


void
hifUnregisterBus(
    remove_card pfRemove
    )
{
    printk(KERN_INFO DRV_NAME"++hifUnregisterBus++\n");

    pfPluginProbe = NULL;
    pfPluginRemove = NULL;
    pfPluginInthandler = NULL;
    
	gRefCount --;

    if (gRefCount<0)
    {
    	printk(KERN_INFO DRV_NAME"gRefCount<0\n");
    }

	sdio_unregister_driver(&mtk_sdio_driver);

    printk(KERN_INFO DRV_NAME"--hifUnregisterBus--\n");
    return;
}
/*----------------------------------------------------------------------------*/
/*!
* \brief This function will register platform driver to os
*
* \param[in] wlanSuspend    Function pointer to platform suspend function
* \param[in] wlanResume   Function pointer to platform resume 	function
*
* \return The result of registering platform driver
*/
/*----------------------------------------------------------------------------*/
int hifRegisterPlatformDriver (
	suspend_callback	wlanSuspend,
	resume_callback		wlanResume )
{
	int ret = 0;	

	if(NULL != wlanSuspend){
		mt5921_driver.suspend = wlanSuspend;
	}
	else{
		printk(KERN_INFO"glRegisterPlatformDriver wlanSuspend ERROR.\n");
		ret = -1;
	}

	if(NULL != wlanResume){
		mt5921_driver.resume = wlanResume;
	}
	else{
		printk(KERN_INFO"glRegisterPlatformDriver wlanResume ERROR.\n");
		ret = -1;
	}

	ret = platform_driver_register(&mt5921_driver);
	printk(KERN_INFO "platform_driver_register return %d\n", ret);
	return ret;
}

/*----------------------------------------------------------------------------*/
/*!
* \brief This function will unregister platform driver to os
*
* \param[in] wlanSuspend    Function pointer to platform suspend function
* \param[in] wlanResume   Function pointer to platform resume 	function
*
* \return The result of unregistering platform driver
*/
/*----------------------------------------------------------------------------*/
int hifUnregisterPlatformDriver ( void )
{
	int ret = 0;	

	platform_driver_unregister(&mt5921_driver);

	mt5921_driver.suspend = NULL;
	mt5921_driver.resume = NULL;

	return ret;
} /* end of hifUnregisterBus() */


BOOL
hifBusInit (
    void * pvData
    )
{
    struct sdio_func *func = NULL;
    int ret = 0;

    printk(KERN_INFO DRV_NAME"%s pvData = %p\n", __FUNCTION__, pvData);

    func = (struct sdio_func *)pvData;

    sdio_claim_host(func);
    ret = sdio_set_block_size(func, 512);
    sdio_release_host(func);

    if (ret) {
        printk(KERN_INFO DRV_NAME"sdio_set_block_size 512 failed!\n");
    }
    else {
        printk(KERN_INFO DRV_NAME"sdio_set_block_size 512 done!\n");
    }

    printk(KERN_INFO DRV_NAME"param: func->cur_blksize(%d)\n", func->cur_blksize);
    printk(KERN_INFO DRV_NAME"param: func->max_blksize(%d)\n", func->max_blksize);
    printk(KERN_INFO DRV_NAME"param: func->card->host->max_blk_size(%d)\n", func->card->host->max_blk_size);
    printk(KERN_INFO DRV_NAME"param: func->card->host->max_blk_count(%d)\n", func->card->host->max_blk_count);

    return TRUE;
} /* end of hifBusInit() */


void
hifBusRelease (
    void * pvData
    )
{
    return;
} /* end of hifBusRelease() */

int
hifBusSetIrq (
    void * pvData,
    void * pfnIsr
    )
{
    struct sdio_func *prSdioFunc = (struct sdio_func *)pvData;
	int ret = -1;

    if (!pvData || !pfnIsr) {
        return -1;
    }

    
    sdio_claim_host(prSdioFunc);
    ret = sdio_claim_irq(prSdioFunc, pfnIsr);
    sdio_release_host(prSdioFunc);
    printk(KERN_INFO "sdio_claim_irq ret=%d\n", ret);
    return 0;
} /* end of hifBusSetIrq() */


void
hifBusFreeIrq (
    void * pvData
    )
{
    struct sdio_func *prSdioFunc = (struct sdio_func *)pvData;
    printk(KERN_INFO DRV_NAME"++ hifBusFreeIrq++\n");
    
    sdio_claim_host(prSdioFunc);
    sdio_release_irq(prSdioFunc);
    sdio_release_host(prSdioFunc);
    printk(KERN_INFO DRV_NAME"-- hifBusFreeIrq -- \n");

    return;
} /* end of hifBusFreeIrq() */



int comodule_status_handler(int funcIdx, int op)
{
    static int funcStatus[FUNC_IDX_NUM]= { 0,0 };
    printk(KERN_INFO DRV_NAME"++ comodule_status_handler ++  fucIdx=%d op=%d \n", funcIdx, op);


    if( (funcIdx != FUNC_IDX_DS && funcIdx != FUNC_IDX_PLUGIN) || 
        (op != 0 && op != 1) ) {
        printk(KERN_ALERT DRV_NAME"comodule_status_handler: invalid param funcIdx %d, op %d!\n",
        funcIdx, op);
        return -1;
    }

    if( funcStatus[funcIdx] == op) {
        printk(KERN_WARNING DRV_NAME"comodule_status_handler: already in that status!\n");
        return 0;
    }
	
	if( (funcStatus[FUNC_IDX_DS] != 0 && funcStatus[FUNC_IDX_DS] != 1)||
		(funcStatus[FUNC_IDX_PLUGIN] != 0 && funcStatus[FUNC_IDX_PLUGIN] != 1) ) {
	    printk(KERN_ALERT DRV_NAME"comodule_status_handler: local funcStatus error: DS %d, Plug-in %d!\n",
			funcStatus[FUNC_IDX_DS], funcStatus[FUNC_IDX_PLUGIN]);
        return -1;
	}
	
    if( funcIdx == FUNC_IDX_DS) {
		//just enable or disable daisy chain
        if( op == 1 ) {
				if( funcStatus[FUNC_IDX_PLUGIN] == 1 ) {
						printk(KERN_ALERT "[MT5921] comodule_status_handler: cann't enable DS func since plug-in func running!\n");
						return -1;
				}

				if ( pvSdio == NULL ) {
						printk(KERN_ALERT "[MT5921] comodule_status_handler: invalid Sdio func handle!\n");
						return -1;
				}
				if( EnableDaisyChain(pvSdio) == FALSE ) {
						printk(KERN_ALERT "[MT5921] comodule_status_handler: Failed to enable daisy chain!\n");
						return -1;
				}

				if( mt5921SetSysOff(pvSdio) == FALSE ) {
						printk(KERN_ALERT "[MT5921] comodule_status_handler: Failed to lay mt5921 into system off!\n");
						return -1;
				}
				//mmc_power_off( ((struct sdio_func *)pvSdio)->card->host );
            		        mmc_power_off_ext( ((struct sdio_func *)pvSdio)->card->host );
			       //add by mtk80743

				funcStatus[FUNC_IDX_DS] = 1;
				return 0;
		} else {
				//mmc_power_up( ((struct sdio_func *)pvSdio)->card->host );
	    		        mmc_power_up_ext( ((struct sdio_func *)pvSdio)->card->host );
			        //add by mtk80743			
				funcStatus[FUNC_IDX_DS] = 0;
				return 0;
		    }
    }else {
  // need init or deinit plug in function
		if( op == 1 ) {
				if( funcStatus[FUNC_IDX_DS] == 1 ) {
						printk(KERN_WARNING "[MT5921] comodule_status_handler: disenable DS func first since DS func running!\n");
						//mmc_power_up( ((struct sdio_func *)pvSdio)->card->host );
						funcStatus[FUNC_IDX_DS] = 0;
				} 

				if( pfPluginProbe == NULL ) {
						printk(KERN_ALERT "[MT5921] comodule_status_handler: pfPluginProbe = NULL!\n");
						return -1;
				}
				if (pfPluginProbe((void *)pvSdio) != 0) {
						printk(KERN_ALERT "[MT5921] comodule_status_handler: failed to call pfPluginProbe!\n");
						pfPluginRemove();
						return -1;
				}

				funcStatus[FUNC_IDX_PLUGIN] = 1;	
				return 0;
		} else {
			if (pfPluginRemove) {
					pfPluginRemove();
					funcStatus[FUNC_IDX_PLUGIN] = 0;
			}
			return 0;
	}
    }
}


BOOL EnableDaisyChain( void * pvData )
{
    unsigned long u4Scr;

    printk(KERN_INFO DRV_NAME"++ EnableDaisyChain ++\n");

    /* Enable daisy chain before set MT5921 to system off */ 
    printk(KERN_INFO "[MT5921] EnableDaisyChain ++!\n");
	
    if (hifDevRegRead(pvData, MCR_SCR, &u4Scr) == FALSE) 
    {               
	    printk(KERN_ALERT "[MT5921] EnableDaisyChain: failed to read MCR_SCR!\n");
        return FALSE;
    } 
    else
    { 
        printk(KERN_ALERT "[MT5921] EnableDaisyChain: MCR_SCR = 0x%lx!\n",u4Scr);
        u4Scr &= ~(SCR_GPIO0_ENABLE_OUTPUT_MODE | 
                   SCR_GPIO0_CHAIN_SEL |
                   SCR_GPIO0_WDATA |
                   SCR_GPIO2_ENABLE_OUTPUT_MODE | 
                   SCR_GPIO2_CHAIN_SEL |
                   SCR_GPIO2_WDATA |
                   SCR_GPIO1_ENABLE_OUTPUT_MODE | 
                   SCR_GPIO1_CHAIN_SEL);/* gpi */
        u4Scr |= SCR_GPIO0_POLAR_HIGH | SCR_GPIO1_POLAR_HIGH|SCR_GPIO1_CHAIN_SEL; /* Input, daisy */

        if( hifDevRegWrite(pvData, MCR_SCR, u4Scr) == FALSE)
        {
		    printk(KERN_ALERT "[MT5921] EnableDaisyChain: failed to write MCR_SCR!\n");
            return FALSE;
        }
    }

    return TRUE; 
}


BOOL mt5921SetSysOff( void * pvData  )
{
    unsigned long u4Scr;
    printk(KERN_INFO DRV_NAME"++ mt5921SetSysOff ++\n");


    printk(KERN_INFO "[MT5921] mt5921SetSysOff ++!\n");

    /* Renbang :  lay the mt5921 to system off mode */
    if( hifDevRegWrite(pvData, MCR_HLPCR, HLPCR_DISABLE_GINT) == FALSE)
    {
		printk(KERN_ALERT "[MT5921] mt5921SetSysOff: failed to Disable INT!\n");		
        return FALSE;
    }
        
    if (hifDevRegRead(pvData, MCR_IOPCR, &u4Scr) == FALSE) 
    {               
		printk(KERN_ALERT "[MT5921] mt5921SetSysOff: failed to read MCR_IOPCR!\n");		
        return FALSE;
    } 
    else
    {
        u4Scr &= ~IOPCR_ALL_TRAP_PIN_OUTPUT_EN;
        if( hifDevRegWrite(pvData, MCR_IOPCR, u4Scr) == FALSE)
        {
			printk(KERN_ALERT "[MT5921] mt5921SetSysOff: failed to write MCR_IOPCR!\n"); 	
			return FALSE;
        }				  
    }

    if( hifDevRegWrite(pvData, MCR_HLPCR, HLPCR_RF_SX_PD | HLPCR_PLL_CLOCK_GATED) == FALSE)
    {
	    printk(KERN_ALERT "[MT5921] mt5921SetSysOff: failed to write MCR_HLPCR 1!\n"); 			
        return FALSE;
    }
    else 
    {
        if( hifDevRegWrite(pvData, MCR_HLPCR, HLPCR_PD_ALL | HLPCR_PLL_CLOCK_GATED) == FALSE)
        {
	        printk(KERN_ALERT "[MT5921] mt5921SetSysOff: failed to write MCR_HLPCR 2!\n"); 						
            return FALSE;
        }
    }
    
    return TRUE; 
}



void hifSetInfo(void * pHif, void *prGlueInfo)
{
    struct sdio_func *prSdioFunc = ( struct sdio_func *)pHif;
    
    printk(KERN_INFO DRV_NAME"++ hifSetInfo++\n");
    sdio_set_drvdata(prSdioFunc, prGlueInfo);
    printk(KERN_INFO DRV_NAME"-- hifSetInfo--\n");

}

static int __init initSdio(void)
{
    int ret = 0;
    printk(KERN_INFO DRV_NAME"++ initSdio++\n");
	
    gRefCount = 0;
#if 0	
    mtk_sdio_driver.probe = mtk_sdio_probe;
    mtk_sdio_driver.remove = mtk_sdio_remove;

    mtk_sdio_driver.drv.suspend = mtk_sdio_suspend;
    mtk_sdio_driver.drv.resume = mtk_sdio_resume;

    ret = sdio_register_driver(&mtk_sdio_driver);

    printk(KERN_INFO DRV_NAME"--initSdio(%d)--\n", ret);
#endif
    printk(KERN_INFO DRV_NAME"-- initSdio--\n");
	
    return ret;
}

static VOID __exit exitSdio(void)
{
    if (gRefCount > 0)
    {
    	return;
    }

//    sdio_unregister_driver(&mtk_sdio_driver);

    printk( KERN_INFO "exitSdio\n");

    return;
} /* end of exitWlan() */


module_init(initSdio);
module_exit(exitSdio);

MODULE_LICENSE("GPL");

EXPORT_SYMBOL(hifRegisterBus);
EXPORT_SYMBOL(hifUnregisterBus);
EXPORT_SYMBOL(hifRegisterPlatformDriver);
EXPORT_SYMBOL(hifUnregisterPlatformDriver);

EXPORT_SYMBOL(hifBusInit);
EXPORT_SYMBOL(hifBusRelease);
EXPORT_SYMBOL(hifBusSetIrq);
EXPORT_SYMBOL(hifBusFreeIrq);
//EXPORT_SYMBOL(hifDevRegRead);
//EXPORT_SYMBOL(hifDevRegWrite);
//EXPORT_SYMBOL(hifDevPortRead);
//EXPORT_SYMBOL(hifDevPortWrite);
EXPORT_SYMBOL(comodule_status_handler);
EXPORT_SYMBOL(sdio_irq_func);
EXPORT_SYMBOL(hifSetInfo);


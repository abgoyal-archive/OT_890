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
** $Id: //ALPS_SW/TRUNK/MAIN/alps/kernel/drivers/net/wireless/mt592x/platform.c#2 $
*/

/*! \file   "platform.c"
    \brief  This file including the protocol layer privacy function.

    This file provided the macros and functions library support for the 
    protocol layer security setting from wlan_oid.c and for parse.c and
    rsn.c and nic_privacy.c
    
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
** $Log: platform.c $
 *
 * 07 10 2010 renbang.jiang
 * [ALPS00121785][Need Patch] [Volunteer Patch] use NVRAM to save Wi-Fi custom data 
 * .
 *
 * 06 18 2010 renbang.jiang
 * [ALPS00008357][Need Patch] [Volunteer Patch] ALPS.10X.W10.11 Volunteer patch for support Rx/Tx indication led 
 * .
 *
 * 05 27 2010 renbang.jiang
 * [ALPS00007371][Need Patch] [Volunteer Patch] ALPS.10X.W10.11 Volunteer patch for WiFi Un-GPL license 
 * .
 *
 * 05 27 2010 renbang.jiang
 * [ALPS00007371][Need Patch] [Volunteer Patch] ALPS.10X.W10.11 Volunteer patch for WiFi Un-GPL license 
 * .
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
#include <linux/init.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "platform.h"


/*******************************************************************************
*                              C O N S T A N T S
********************************************************************************
*/

/*******************************************************************************
*                             D A T A   T Y P E S
********************************************************************************
*/

/*******************************************************************************
*                            P U B L I C   D A T A
********************************************************************************
*/

/*******************************************************************************
*                           P R I V A T E   D A T A
********************************************************************************
*/

/*******************************************************************************
*                                 M A C R O S
********************************************************************************
*/

#define WIFI_NVRAM_FILE_NAME   "/nvram/APCFG/APRDEB/WIFI"
#define WIFI_NVRAM_CUSTOM_NAME "/nvram/APCFG/APRDEB/WIFI_CUSTOM"

WIFI_CFG_DATA gPlatformCfg;
EXPORT_SYMBOL(gPlatformCfg);   
/*******************************************************************************
*                   F U N C T I O N   D E C L A R A T I O N S
********************************************************************************
*/
int platform_load_nvram_data( char * filename, char * buf, int len );

#if 0
bool
platformNvramRead16( void * prAdapter,unsigned char ucWordOffset, unsigned short * pu2Data );

bool
platformNvramWrite16( void * prAdapter,unsigned char ucWordOffset,unsigned short u2Data );
#endif


bool
cfgDataRead16( void * prAdapter,unsigned char ucWordOffset, unsigned short * pu2Data );
bool
cfgDataWrite16( void * prAdapter,unsigned char ucWordOffset,unsigned short u2Data );

bool
customDataRead8( void * prAdapter,unsigned char ucByteOffset, unsigned char * pucData );

bool
customDataWrite8( void * prAdapter,unsigned char ucByteOffset,unsigned char ucData );

int platform_init(void);
void platform_deinit(void);
/*******************************************************************************
*                              F U N C T I O N S
********************************************************************************
*/
static int nvram_read(char *filename, char *buf, ssize_t len, int offset)
{	
    struct file *fd;
    //ssize_t ret;
    int retLen = -1;
    
    mm_segment_t old_fs = get_fs();
    set_fs(KERNEL_DS);
    
    fd = filp_open(filename, O_WRONLY|O_CREAT, 0644);
    
    if(IS_ERR(fd)) {
        printk("[MT5921][nvram_read] : failed to open!!\n");
        return -1;
    }
    do{
        if ((fd->f_op == NULL) || (fd->f_op->read == NULL))
    		{
            printk("[MT5921][nvram_read] : file can not be read!!\n");
            break;
    		} 
    		
        if (fd->f_pos != offset) {
            if (fd->f_op->llseek) {
        		    if(fd->f_op->llseek(fd, offset, 0) != offset) {
						printk("[MT5921][nvram_read] : failed to seek!!\n");
					    break;
        		    }
        	  } else {
        		    fd->f_pos = offset;
        	  }
        }    		
        
    		retLen = fd->f_op->read(fd,
    									  buf,
    									  len,
    									  &fd->f_pos);			
    		
    }while(false);
    
    filp_close(fd, NULL);
    
    set_fs(old_fs);
    
    return retLen;
}

static int nvram_write(char *filename, char *buf, ssize_t len, int offset)
{	
    struct file *fd;
    //ssize_t ret;
    int retLen = -1;
        
    mm_segment_t old_fs = get_fs();
    set_fs(KERNEL_DS);
    
    fd = filp_open(filename, O_WRONLY|O_CREAT, 0644);
    
    if(IS_ERR(fd)) {
        printk("[MT5921][nvram_write] : failed to open!!\n");
        return -1;
    }
    do{
        if ((fd->f_op == NULL) || (fd->f_op->write == NULL))
    		{
            printk("[MT5921][nvram_write] : file can not be write!!\n");
            break;
    		} /* End of if */
    		
        if (fd->f_pos != offset) {
            if (fd->f_op->llseek) {
        	    if(fd->f_op->llseek(fd, offset, 0) != offset) {
				    printk("[MT5921][nvram_write] : failed to seek!!\n");
                    break;
                }
            } else {
                fd->f_pos = offset;
            }
        }       		
        
        retLen = fd->f_op->write(fd,
                                 buf,
                                 len,
                                 &fd->f_pos);			
    		
    }while(false);
    
    filp_close(fd, NULL);
    
    set_fs(old_fs);
    
    return retLen;
}
	

int platform_load_nvram_data( char * filename, char * buf, int len)
{
    //int ret;
    printk("[wifi] platform_load_nvram_data ++\n");

    return nvram_read( filename, buf, len, 0);
}

bool
cfgDataRead16( void * prAdapter,unsigned char ucWordOffset, unsigned short * pu2Data )
{
    if( pu2Data == NULL )
        return false;
        
    if( nvram_read( WIFI_NVRAM_FILE_NAME, (char *)pu2Data, sizeof(unsigned short), ucWordOffset*sizeof(unsigned short)) != sizeof(unsigned short) )
        return false;
    else 
    	  return true;	
}
EXPORT_SYMBOL(cfgDataRead16);

bool
cfgDataWrite16( void * prAdapter,unsigned char ucWordOffset,unsigned short u2Data )    
{
    if( nvram_write( WIFI_NVRAM_FILE_NAME, (char *)&u2Data, sizeof(unsigned short), ucWordOffset*sizeof(unsigned short)) != sizeof(unsigned short) )
        return false;
    else 
    	  return true;
}
EXPORT_SYMBOL(cfgDataWrite16);

bool
customDataRead8( void * prAdapter,unsigned char ucByteOffset, unsigned char * pucData )
{
    if( pucData == NULL )
        return false;
        
    if( nvram_read( WIFI_NVRAM_CUSTOM_NAME, (char *)pucData, sizeof(unsigned char), ucByteOffset*sizeof(unsigned char)) != sizeof(unsigned char) )
        return false;
    else 
    	  return true;	
}
EXPORT_SYMBOL(customDataRead8);

bool
customDataWrite8( void * prAdapter,unsigned char ucByteOffset,unsigned char ucData )    
{
    if( nvram_write( WIFI_NVRAM_CUSTOM_NAME, (char *)&ucData, sizeof(unsigned char), ucByteOffset*sizeof(unsigned char)) != sizeof(unsigned char) )
        return false;
    else 
    	  return true;
}
EXPORT_SYMBOL(customDataWrite8);

#if 0
bool
platformNvramRead16( void * prAdapter,unsigned char ucWordOffset, unsigned short * pu2Data )
{
    if( pu2Data == NULL )
        return false;
        
    if( nvram_read( WIFI_NVRAM_FILE_NAME, (char *)pu2Data, sizeof(unsigned short), ucWordOffset*sizeof(unsigned short)) != sizeof(unsigned short) )
        return false;
    else 
    	  return true;	
}

EXPORT_SYMBOL(platformNvramRead16);      
                      
bool
platformNvramWrite16( void * prAdapter,unsigned char ucWordOffset,unsigned short u2Data )    
{
    if( nvram_write( WIFI_NVRAM_FILE_NAME, (char *)&u2Data, sizeof(unsigned short), ucWordOffset*sizeof(unsigned short)) != sizeof(unsigned short) )
        return false;
    else 
    	  return true;
}
EXPORT_SYMBOL(platformNvramWrite16);      
#endif

int platform_init(void)
{
    memset( &gPlatformCfg, 0, sizeof(gPlatformCfg) );
#if !BUILD_USE_EEPROM	    
    gPlatformCfg.u4Cfglen = platform_load_nvram_data( WIFI_NVRAM_FILE_NAME, (char *)&gPlatformCfg.rWifiNvram, CFG_FILE_WIFI_REC_SIZE);
#endif    

    gPlatformCfg.u4Customlen = platform_load_nvram_data( WIFI_NVRAM_CUSTOM_NAME, (char *)&gPlatformCfg.rWifiCustom, CFG_FILE_WIFI_CUSTOM_REC_SIZE);
    return 0;
}

EXPORT_SYMBOL(platform_init);    

void platform_deinit(void)
{
#if !BUILD_USE_EEPROM	   
    memset( &gPlatformCfg, 0, sizeof(gPlatformCfg) );
#endif    
}

EXPORT_SYMBOL(platform_deinit);    

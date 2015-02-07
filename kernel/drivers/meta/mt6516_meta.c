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

/******************************************************************************
 * 
 * Copyright 2008-2009 MediaTek Co.,Ltd.
 * 
 * DESCRIPTION: - 
 *     This file is a module that includes essential functions about 
 *     for Mobile Engineering Test Achitecture (META)!
 *
 * modification history
 * ----------------------------------------
 * 20090709, Shu-Hsin Chang written
 * ----------------------------------------
 ******************************************************************************/
#include <mach/mt6516.h>
#include <mach/mt6516_gpt_sw.h>

#include <mach/irqs.h>
#include <asm/io.h>
#include <mach/dma.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/types.h>
#include <linux/module.h>
#include <asm/div64.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include "mt6516_meta.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


/******************************************************************************
 Debug Control
******************************************************************************/
//#define MONITOR_DEBUG
#define META_DRIVER

/******************************************************************************
 Meta Driver
******************************************************************************/
#ifdef META_DRIVER

#define META_TEST_ALL				0
#define META_TEST_MODEM				1
#define META_TEST_AP				2
#define META_TEST_CAMERA			3
#define META_FILE_RESTORE_BOOTDATA	4
#define META_FILE_RESTORE_ALL		5
#define META_FILE_BACKUP_ALL		6
#define META_MCLEAN					7
#define META_NVRAM					8
#define META_QUIT					9


/******************************************************************************
 Global Variables
******************************************************************************/
#define func_count 10

struct tasklet_struct meta_tlet[META_MODULE_COUNT];  
meta_unit_test meta_test[META_MODULE_COUNT][func_count];
static spinlock_t meta_test_lock = SPIN_LOCK_UNLOCKED;
volatile BOOL ready_bit = TRUE;

/******************************************************************************
 Meta API : MT6516_META_Register
 DESCRIPTION:
 	Provide register interface for testing module
******************************************************************************/
BOOL MT6516_META_Register(meta_module module,meta_callback func,const char* name)
{	
	int i=0;

	spin_lock(&meta_test_lock);		    

	for(i=0;i<func_count;i++)
	{	if(meta_test[module][i].callback==func)
		{	printk("[META] This callback function has already registered\n");
			spin_unlock(&meta_test_lock);	
			return TRUE;
		}
		if(meta_test[module][i].callback==NULL)
		{	meta_test[module][i].callback=func;
			strcpy(meta_test[module][i].name,name);
			printk("[META] Register callback function success\n");
			spin_unlock(&meta_test_lock);				
			return TRUE;
		}				
	}	
	
	printk("[META] Register callback function fail\n");
	return FALSE;
}


/******************************************************************************
 Meta API : MT6516_META_Unregister 
 DESCRIPTION:
 	Provide unregister interface for testing module
******************************************************************************/
BOOL MT6516_META_Unregister(meta_module module,meta_callback func,const char* name)
{	
	int i=0;

	spin_lock(&meta_test_lock);		    

	for(i=0;i<func_count;i++)
	{	
		if(meta_test[module][i].callback==func && strcmp(meta_test[module][i].name,name)==0)
		{	
			meta_test[module][i].callback=NULL;
			meta_test[module][i].enable=FALSE;
			strcpy(meta_test[module][i].name,"");	
			printk("[META] Unregister callback function success\n");

			//adjust meta_test array (fill the empty entry)
			while((i+1)<func_count)
			{	meta_test[module][i].callback=meta_test[module][i+1].callback;
				meta_test[module][i].enable=meta_test[module][i+1].enable;
				strcpy(meta_test[module][i].name,meta_test[module][i+1].name);
				i++;
			}
			
			spin_unlock(&meta_test_lock);	
			return TRUE;
		}
	}	
	
	printk("[META] Unregister callback function fail\n");
	return FALSE;
}


/******************************************************************************
 Meta Tasklet Handling Function
******************************************************************************/

void MT6516_META_Tasklet(unsigned long module)
{
	int i=0;
	BOOL val;

	ready_bit = FALSE;
	//Sequentially process all the registered function
	for(i=0;i<func_count;i++)
	{	
		if(meta_test[module][i].callback==NULL)
		{	ready_bit = TRUE;
			return ;
		}
		printk("----------------------\n");
		printk("[META] tasklet do '%s'\n",meta_test[module][i].name);		
		val=meta_test[module][i].callback();
		if(val)
		{	printk("[META] test pass\n");
		}
		else
		{	printk("[META] test fail\n");
		}		
		printk("----------------------\n");
	}
	ready_bit = TRUE;
	return ;
}

/******************************************************************************
 Meta IOTCL
******************************************************************************/
 static int MT6516_META_Ioctl(struct inode *inode, struct file *file, 
                             unsigned int cmd, unsigned long arg)
 {

    int i=0;

	printk("MT6516 Meta Driver IOCTL\n");

	switch (cmd) {
		case META_TEST_ALL: 
			printk("Test META_ALL ...\n");
			for(i=0;i<META_MODULE_COUNT;i++)
			{	ready_bit = FALSE;
				tasklet_schedule(&meta_tlet[i]);					
			}
			//polling ready bit to check these tasklets are all done
			while(ready_bit==FALSE){}		
		break;		
		case META_TEST_MODEM: 
			printk("Test META_MODEM ...\n");
			ready_bit = FALSE;
			tasklet_schedule(&meta_tlet[META_MODEM]);	    
			//polling ready bit to check these tasklets are all done
			while(ready_bit==FALSE){}	
		break;		
		case META_TEST_AP: 
			printk("Test META_AP ...\n");
			ready_bit = FALSE;
			tasklet_schedule(&meta_tlet[META_AP]);	    
			//polling ready bit to check these tasklets are all done
			while(ready_bit==FALSE){}			
		break;		
		case META_TEST_CAMERA: 
			printk("Test META_CAMERA ...\n");
			tasklet_schedule(&meta_tlet[META_CAMERA]);	    
			//polling ready bit to check these tasklets are all done
			while(ready_bit==FALSE){}			
		break;		
		case META_FILE_RESTORE_BOOTDATA: 
			printk("Test META_FILE_RESTORE_BOOTDATA ...\n");
		break;	
		case META_FILE_RESTORE_ALL: 
			printk("Test META_FILE_RESTORE_ALL ...\n");
		break;				
		case META_FILE_BACKUP_ALL: 
			printk("Test META_FILE_BACKUP_ALL ...\n");
		break;				
		case META_MCLEAN: 
			printk("Test META_MCLEAN ...\n");
		break;	
		case META_NVRAM: 
			printk("Test META_NVRAM ...\n");
		break;	
	}									
    
    return 0;
}

/******************************************************************************
 Meta Open
******************************************************************************/
 static int MT6516_META_Open(struct inode *inode, struct file *file)
 {
 	
    printk("MT6516 Meta Open\n");
    return 0;
 }

/******************************************************************************
 Meta Release
******************************************************************************/
 static int MT6516_META_Release(struct inode *inode, struct file *file)
 {
 	
    printk("MT6516 Meta Release\n");
    return 0;
 }

/******************************************************************************
 Meta File Operation
******************************************************************************/
static struct file_operations MT6516_META_fops = 
{
	.owner=		THIS_MODULE,
    .ioctl=		MT6516_META_Ioctl,
	.open=		MT6516_META_Open,    
	.release=	MT6516_META_Release,    
};

 #endif

/******************************************************************************
 Meta Initialization
******************************************************************************/
/* callback function example for using MT6516_META_Register */
#if 0
BOOL test_function_1(void)
{	printk("test 1\n");
	// ... test 
	// return result
	return TRUE;
}
BOOL test_function_2(void)
{	printk("test 2\n");
	// ... test 
	// return result
	return TRUE;
}
BOOL test_function_3(void)
{	printk("test 3\n");
	// ... test 
	// return result
	return TRUE;
}
#endif

static int __init mt_init_meta(void)
{
	
	/* Initialize Tasklet */
	int module=0;
	for(module=0;module<META_MODULE_COUNT;module++)
	{	tasklet_init(&meta_tlet[module],MT6516_META_Tasklet,module);		
	}
    	printk("MT6516 Meta is intialized!!\n");

	/* Meta Driver */
	#ifdef META_DRIVER
	    if (register_chrdev(MISC_DYNAMIC_MINOR, "META", &MT6516_META_fops)) {
	        printk(KERN_INFO "unable to get major %d for rtc\n",
	               MISC_DYNAMIC_MINOR);
	        return -1;
	    }	
	    printk("MT6516 Meta Driver is initialized!!\n");   	
	#endif	    

	/* sample code for using MT6516_META_Register and MT6516_META_Unregister */
	#if 0
	MT6516_META_Register(META_OTHER,test_function_1,"other::test_1");	
	MT6516_META_Register(META_OTHER,test_function_2,"other::test_2");	
	MT6516_META_Register(META_AP,test_function_3,"AP::test_3");		
	MT6516_META_Unregister(META_OTHER,test_function_2,"other::test_2");	
	#endif

    return 0;
}
arch_initcall(mt_init_meta);


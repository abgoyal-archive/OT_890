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
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   mt6516_mm_queue.c
 *
 * Project:
 * --------
 *   MT6516
 *
 * Description:
 * ------------
 *   MT6516 mm queue management 
 *
 * Author:
 * -------
 *   Jackal Chen   (mtk02532)
 *
 ****************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/mm_types.h>
#include <linux/mm.h>
#include <linux/jiffies.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <asm/page.h>
#include <linux/vmalloc.h>
#include <mach/mt6516_pll.h>
#include <mach/mt6516_typedefs.h> 
#include <mach/mt6516_reg_base.h> 
#include <linux/interrupt.h>
#include <mach/irqs.h>
#include <linux/wait.h>
#include <linux/spinlock.h>
#include <mach/mt6516_mm_queue.h> 

#define MM_QUEUE_DEVNAME "MT6516_MM_QUEUE"

//#define MT6516QUEUE_DEBUG
#ifdef MT6516QUEUE_DEBUG
#define MM_QUEUE_DEBUG printk
#else
#define MM_QUEUE_DEBUG(x,...)
#endif

#define MT6516QUEUE
#ifdef MT6516QUEUE
#define MM_QUEUE printk
#else
#define MM_QUEUE(x,...)
#endif

/*
#define DMA_ALLOC(ptr,dev,size,hdl,flag) \
do { \
    ptr = dma_alloc_coherent(dev, size, hdl, flag); \
    BUG_ON(!ptr); \
    printk("[MM_QUEUE_MEM_ALLOC] Alloc_vaddr=0x%08x, Alloc_paddr=0x%08x, Alloc_size=%d, LINE:%d\n", ptr, *hdl, size, __LINE__); \
} while (0)

#define DMA_FREE(dev, size, ptr, hdl)\
do { \
    printk("[MM_QUEUE_MEM_FREE] Free_addr=0x%08x, Alloc_size=%d, LINE:%d\n", ptr, size, __LINE__); \
    dma_free_coherent(dev, size, ptr, hdl); \
} while (0)
*/

#define DMA_ALLOC(ptr,dev,size,hdl,flag) \
do { \
    ptr = dma_alloc_coherent(dev, size, hdl, flag); \
    BUG_ON(!ptr); \
} while (0)

#define DMA_FREE(dev, size, ptr, hdl)\
do { \
    dma_free_coherent(dev, size, ptr, hdl); \
} while (0)


static struct class *mm_queue_class = NULL;
static dev_t mm_queue_devno;
static struct cdev *mm_queue_cdev;

static mm_queue_list * mm_queue_list_head = NULL;
static mm_queue_list * mm_queue_list_prev = NULL;
static mm_queue_list * mm_queue_list_current = NULL;
static mm_queue_list * mm_queue_list_temp = NULL;

static mm_queue_info * p_mm_queue_info_ptr_from_user = NULL;

static spinlock_t g_mm_queue_lock;

static bool error_flag = FALSE;

static INT32 g_mm_queue_pending_element_size;

void flush_mm_queue(void)
{
    g_mm_queue_pending_element_size = 0;
    
    mm_queue_list_temp = mm_queue_list_head;

    while(mm_queue_list_temp != NULL)
    {
        DMA_FREE(0, 1440, mm_queue_list_temp->info.q_table_vaddr, mm_queue_list_temp->info.q_table_paddr);
        mm_queue_list_head = mm_queue_list_head->next;
        kfree(mm_queue_list_temp);
        mm_queue_list_temp = mm_queue_list_head;
    }
    
    mm_queue_list_head = NULL;
    mm_queue_list_prev = NULL;
    mm_queue_list_current = NULL;
    mm_queue_list_temp = NULL;
}

void push_info_to_mm_queue(mm_queue_info* p_mm_queue_info_ptr_from_user)
{
    UINT8* temp_addr = NULL;
    mm_queue_info mm_queue_info_temp;
    UINT32 ret;

    if (g_mm_queue_pending_element_size >= 20)  //have some problem ..., need to check
    {
        MM_QUEUE("[MM_QUEUE][WARNING] g_mm_queue_pending_element_size >= 20\n");
        flush_mm_queue();
    }
    
    ret = copy_from_user(&mm_queue_info_temp, p_mm_queue_info_ptr_from_user, sizeof(mm_queue_info));

    if (mm_queue_list_head == NULL)
    {
        mm_queue_list_temp = NULL;
    }
    else
    {
        mm_queue_list_temp = mm_queue_list_head;
        while (mm_queue_list_temp->info.yuv_buf_paddr != mm_queue_info_temp.yuv_buf_paddr)
        {
            mm_queue_list_temp = mm_queue_list_temp->next;

            if (mm_queue_list_temp == NULL)
            {
                break;
            }
        }
    }

    if (mm_queue_list_temp != NULL)
    {
        // MP4_DEC_QS_ADDR: pa=> 0x4002BA30 va=> 0xF022BA30 size:1440
        temp_addr = (UINT8 *)0xF022BA30;
        memcpy(mm_queue_list_temp->info.q_table_vaddr,temp_addr, 1440);
    }
    else
    {
        mm_queue_list_temp = (mm_queue_list *)kmalloc(sizeof(mm_queue_list), GFP_KERNEL);
    
        if (mm_queue_list_temp == NULL)
        {
            MM_QUEUE("[MM_QUEUE][ERROR] alloc mm_queue_list_temp fail\n");
            error_flag = TRUE;
        }
        else
        {
            g_mm_queue_pending_element_size++;
            
            mm_queue_list_temp->next = NULL;
        
            memcpy(&mm_queue_list_temp->info, &mm_queue_info_temp, sizeof(mm_queue_info));
        
            // MP4_DEC_QS_ADDR: pa=> 0x4002BA30 va=> 0xF022BA30 size:1440
            DMA_ALLOC(mm_queue_list_temp->info.q_table_vaddr, 0, 1440, &mm_queue_list_temp->info.q_table_paddr, GFP_KERNEL);
            temp_addr = (UINT8 *)0xF022BA30;
            memcpy(mm_queue_list_temp->info.q_table_vaddr,temp_addr, 1440);
        
            if (mm_queue_list_head == NULL)
            {
                mm_queue_list_head = mm_queue_list_temp;
                mm_queue_list_current = mm_queue_list_temp;
            }
            else
            {
                mm_queue_list_current->next = mm_queue_list_temp;
                mm_queue_list_current = mm_queue_list_temp;
            }
        }  
    }

    NOT_REFERENCED(ret);
}

void show_top_info_from_mm_queue(mm_queue_info* p_mm_queue_info_ptr_from_user)
{
    UINT32 ret;
    UINT32 temp_pa;
    
    mm_queue_list_temp = mm_queue_list_head;

    if (mm_queue_list_temp == NULL)
    {
        // need to open!
        MM_QUEUE_DEBUG("[MM_QUEUE][ERROR] mm_queue_list_head = NULL from show_top_info_from_mm_queue\n");
        error_flag = TRUE;
        
    }
    else
    {
        ret = copy_from_user(&temp_pa, &p_mm_queue_info_ptr_from_user->yuv_buf_paddr, sizeof(UINT32));

        MM_QUEUE_DEBUG("[show_top_info] pa = 0x%08x\n", temp_pa);

        while (mm_queue_list_temp != NULL)
        {
            if (temp_pa == mm_queue_list_temp->info.yuv_buf_paddr)
            {
                break;
            }
            else
            {
                mm_queue_list_temp = mm_queue_list_temp->next;
            }
        }

        if (mm_queue_list_temp != NULL)
        {
            // MP4_DEC_QS_ADDR: pa=> 0x4002C000 va=> 0xF022C000 size:1440
            mm_queue_list_temp->info.int_sram_q_table_vaddr = (UINT8 *)0xF022C000;
            mm_queue_list_temp->info.int_sram_q_table_paddr = 0x4002C000;
            memcpy(mm_queue_list_temp->info.int_sram_q_table_vaddr, mm_queue_list_temp->info.q_table_vaddr, 1440);
            ret = copy_to_user(p_mm_queue_info_ptr_from_user, &mm_queue_list_temp->info, sizeof(mm_queue_info));
        }
        else
        {
            MM_QUEUE("[MM_QUEUE][ERROR] yuv_buf_paddr is not found from show_top_info_from_mm_queue\n");
            error_flag = TRUE;
        }
    }
    
    NOT_REFERENCED(ret);
}

void show_mm_queue(void)
{
    UINT32 index = 0;
    
    mm_queue_list_temp = mm_queue_list_head;

    while(mm_queue_list_temp != NULL)
    {
        MM_QUEUE_DEBUG("index = %d\n", index);
        MM_QUEUE_DEBUG("yuv_buf_vaddr = 0x%08x\n", mm_queue_list_temp->info.yuv_buf_vaddr);
        MM_QUEUE_DEBUG("yuv_buf_paddr = 0x%08x\n", mm_queue_list_temp->info.yuv_buf_paddr);
        MM_QUEUE_DEBUG("q_table_vaddr = 0x%08x\n", mm_queue_list_temp->info.q_table_vaddr);
        MM_QUEUE_DEBUG("q_table_paddr = 0x%08x\n", mm_queue_list_temp->info.q_table_paddr);
        mm_queue_list_temp = mm_queue_list_temp->next;
        index++;
    }
}

void make_pmem_to_noncached(unsigned int *p_PmemVAddr_from_user)
{
    struct vm_area_struct *vma;
    unsigned int PmemVAddr;
    int ret;

    ret = copy_from_user(&PmemVAddr, p_PmemVAddr_from_user, sizeof(unsigned int));
    MM_QUEUE("PmemVAddr = 0x%08x\n",PmemVAddr);
    vma = find_vma(current->mm, PmemVAddr);
    vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
    zap_page_range(vma, vma->vm_start, vma->vm_end - vma->vm_start, NULL);
    remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, vma->vm_end - vma->vm_start, vma->vm_page_prot);
}

static int mm_queue_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
    spin_lock(&g_mm_queue_lock);

    switch(cmd)
    {
        case FLUSH_MM_QUEUE:
            MM_QUEUE_DEBUG("[MM_QUEUE] FLUSH_MM_QUEUE case\n");
            flush_mm_queue();
            break;

        case PUSH_INFO_TO_MM_QUEUE:  //get yuv_buf_vaddr from user
            MM_QUEUE_DEBUG("[MM_QUEUE] PUSH_INFO_TO_MM_QUEUE case\n");
            p_mm_queue_info_ptr_from_user = (mm_queue_info *)arg;
            push_info_to_mm_queue(p_mm_queue_info_ptr_from_user);
#ifdef MT6516QUEUE_DEBUG
            show_mm_queue();
#endif
            break;
    
        case SHOW_TOP_INFO_FROM_MM_QUEUE:
            MM_QUEUE_DEBUG("[MM_QUEUE] SHOW_TOP_INFO_FROM_MM_QUEUE case\n");
            p_mm_queue_info_ptr_from_user = (mm_queue_info *)arg;
            show_top_info_from_mm_queue(p_mm_queue_info_ptr_from_user);
#ifdef MT6516QUEUE_DEBUG
            show_mm_queue();
#endif
            break;

        case SHOW_MM_QUEUE:
            MM_QUEUE_DEBUG("[MM_QUEUE] SHOW_MM_QUEUE case\n");
#ifdef MT6516QUEUE_DEBUG
            show_mm_queue();
#endif
            break;

        case MAKE_PMEM_TO_NONCACHED:
            MM_QUEUE_DEBUG("[MM_QUEUE] MAKE_PMEM_TO_NONCACHED case\n");
            make_pmem_to_noncached((unsigned int *)arg);
            break;

        default:
            MM_QUEUE("[MM_QUEUE][ERROR] default case\n");
            error_flag = TRUE;
            break;
    }

    if (error_flag == TRUE)
    {
        error_flag = FALSE;
        spin_unlock(&g_mm_queue_lock);
        return -EINVAL;     
    }
    else
    {
        spin_unlock(&g_mm_queue_lock);
        return 0;
    }
}

static int mm_queue_open(struct inode *inode, struct file *file)
{
    spin_lock(&g_mm_queue_lock);
    MM_QUEUE_DEBUG("[MM_QUEUE] mm_queue_open\n");
    spin_unlock(&g_mm_queue_lock);
    return 0;
}

static int mm_queue_release(struct inode *inode, struct file *file)
{
    spin_lock(&g_mm_queue_lock);
    MM_QUEUE_DEBUG("[MM_QUEUE] mm_queue_release\n");
    spin_unlock(&g_mm_queue_lock);
    return 0;
}

static struct file_operations mm_queue_fops = {
    .owner      = THIS_MODULE,
    .ioctl      = mm_queue_ioctl,
    .open       = mm_queue_open,
    .release    = mm_queue_release,
};

static int mm_queue_probe(struct platform_device *dev)
{
    struct class_device;
    
    int ret;
    struct class_device *class_dev = NULL;

    MM_QUEUE("[MM_QUEUE] mm_queue_probe\n");

    ret = alloc_chrdev_region(&mm_queue_devno, 0, 1, MM_QUEUE_DEVNAME);
    if(ret)
    {
        MM_QUEUE("[MM_QUEUE][Error] can't get major number for device\n");
    }

    mm_queue_cdev = cdev_alloc();
    mm_queue_cdev->owner = THIS_MODULE;
    mm_queue_cdev->ops = &mm_queue_fops;

    ret = cdev_add(mm_queue_cdev, mm_queue_devno, 1);

    if (ret < 0)
    {
        MM_QUEUE("[MM_QUEUE] cdev add error!!\n");
    }

    mm_queue_class = class_create(THIS_MODULE, MM_QUEUE_DEVNAME);
    class_dev = (struct class_device *)device_create(mm_queue_class,
                                              NULL,
                                              mm_queue_devno,
                                              NULL,
                                              MM_QUEUE_DEVNAME
                                              );

    NOT_REFERENCED(class_dev);
    
    MM_QUEUE("[MM_QUEUE] mm_queue_probe done\n");
 
    return 0;
}

static int mm_queue_remove(struct platform_device *dev)
{
    MM_QUEUE("[MM_QUEUE] mm_queue_remove\n");
    return 0;
}

static void mm_queue_shutdown(struct platform_device *dev)
{
    MM_QUEUE("[MM_QUEUE] mm_queue_shutdown\n");
}

static int mm_queue_suspend(struct platform_device *dev, pm_message_t state)
{    
    MM_QUEUE("[MM_QUEUE] mm_queue_suspend\n");
    return 0;
}

static int mm_queue_resume(struct platform_device *dev)
{   
    MM_QUEUE("[MM_QUEUE] mm_queue_resume\n");
    return 0;
}

static struct platform_driver mm_queue_driver = {
    .probe       = mm_queue_probe,
    .remove      = mm_queue_remove,
    .shutdown    = mm_queue_shutdown,
    .suspend     = mm_queue_suspend,
    .resume      = mm_queue_resume,
    .driver      = {
    .name        = MM_QUEUE_DEVNAME,
    },
};

static struct platform_device mm_queue_device = {
    .name     = MM_QUEUE_DEVNAME,
    .id       = 0,
};

static int __init mm_queue_driver_init(void)
{
    int ret;

    MM_QUEUE("[MM_QUEUE] driver_init\n");

    if (platform_device_register(&mm_queue_device)){
        MM_QUEUE("[MM_QUEUE][ERROR] fail to register device\n");
        ret = -ENODEV;
        return ret;
    }
    
    if (platform_driver_register(&mm_queue_driver)){
        MM_QUEUE("[MM_QUEUE][ERROR] fail to register driver\n");
        platform_device_unregister(&mm_queue_device);
        ret = -ENODEV;
        return ret;
    }

    MM_QUEUE("[MM_QUEUE] driver_init done\n");
    
    return 0;
}

static void __exit mm_queue_driver_exit(void)
{
    MM_QUEUE("[MM_QUEUE] driver_exit\n");

    device_destroy(mm_queue_class, mm_queue_devno);
    class_destroy(mm_queue_class);
   
    cdev_del(mm_queue_cdev);
    unregister_chrdev_region(mm_queue_devno, 1);
    platform_driver_unregister(&mm_queue_driver);
    platform_device_unregister(&mm_queue_device);
}

module_init(mm_queue_driver_init);
module_exit(mm_queue_driver_exit);
MODULE_AUTHOR("Jackal, Chen <jackal.chen@mediatek.com>");
MODULE_DESCRIPTION("MT6516 MM Queue Driver");
MODULE_LICENSE("GPL");


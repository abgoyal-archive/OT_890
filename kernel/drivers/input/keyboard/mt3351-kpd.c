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

/******************************************************************************
 * mt3351_kpd.c - MT3351 Android Linux Keypad Device Driver
 * 
 * Copyright 2008-2009 MediaTek Co.,Ltd.
 * 
 * DESCRIPTION:
 *     This file provid the other drivers Keypad relative functions
 *
 * modification history
 * ----------------------------------------
 * 01a, 22 Jan 2009, Koshi,Chiu written
 * 02a, 05 Feb 2009, Koshi,Chiu written
 * 03a, 13 Mar 2009, Koshi,Chiu written
 * ----------------------------------------
 ******************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/autoconf.h>

#include <linux/types.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>

#include <mach/mt3351_reg_base.h>
#include <mach/mt3351_gpio.h>
#include <asm/io.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <mach/irqs.h>

#include <linux/input.h>

#define  VERSION	    "v 0.2"
#define  KPD_DEVICE	    "mt3351-kpd"
#define  MENU_PIN_NO	    13
    
#define  COUNT_DOWN_TIME    1/2

/******************************************************************************
MT3351 KPD Base Addresses
******************************************************************************/
#define 	KP_STA      (KP_BASE+0x0000) /* Keypad status register       */
#define 	KP_MEM1     (KP_BASE+0x0004) /* Keypad scan output register1 */
#define 	KP_MEM2     (KP_BASE+0x0008) /* Keypad scan output register2 */
#define 	KP_MEM3     (KP_BASE+0x000C) /* Keypad scan output register3 */
#define 	KP_MEM4     (KP_BASE+0x0010) /* Keypad scan output register4 */
#define 	KP_MEM5     (KP_BASE+0x0014) /* Keypad scan output register5 */
#define 	KP_DEBOUNCE (KP_BASE+0x0014) /* Keypad debounce period       */

/******************************************************************************
Read / Write Register functions
******************************************************************************/
#define DRV_WriteReg32(addr, data)  __raw_writel(data, addr)
#define DRV_Reg32(addr)             __raw_readl(addr)

/******************************************************************************
Global Definations
******************************************************************************/
static volatile unsigned int pwk_pressed = 0;
static volatile unsigned int mnk_pressed = 0;
unsigned long press_stamp;
unsigned long menu_press_stamp;
unsigned int count_down;
struct timer_list timer;

static int *keymap;

extern void MT3351_IRQSensitivity(unsigned char code, unsigned char edge);
extern void MT3351_IRQMask(unsigned int line);
extern void MT3351_IRQUnmask(unsigned int line);
extern void MT3351_IRQClearInt(unsigned int line);

struct mt3351_kp_platform_data 
{
	int rows;
	int cols;
	int *keymap;
	unsigned int keymapsize;
	unsigned int rep:1;
	unsigned long delay;
	unsigned int dbounce:1;
};

struct mt3351_kp
{
    struct input_dev *input;
    int rows;
    int cols;
};

/* Marked for the press powerkey 3 secs shutdown, but reboot automatically */

/*
static void mt3351_pwk_timeout(unsigned long data)
{    
    if(pwk_pressed)
    {
        mod_timer(&timer, jiffies + 100/(1000/HZ));
        count_down--;
        if(!count_down)
        {
            gpio_hw_status.u8InputStatus |= ONOFF_MASK;
            gpio_hw_status.u8InputStatus |= DOCK_ONOFF_MASK;
            kernel_power_off();
        }
    }
    else 
    {
        del_timer(&timer);
    }
}
*/

/******************************************************************************
 * kpd_interrupt_handler
 * 
 * DESCRIPTION:
 *   KPD interrupt handler!
 * 
 * PARAMETERS: 
 *   int irq
 *   void *dev_id
 *   struct pt_regs *regs
 * 
 * RETURNS: 
 *   IRQ_HANDLED : Successfully handle the IRQ  
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 22 Jan 2009, Koshi, Chiu written
 * 02a, 05 Feb 2009, Koshi, Chiu written
 * 03a, 13 Mar 2009, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

static irqreturn_t kpd_interrupt_handler(int irq, void *dev_id)
{
    struct mt3351_kp *edev = dev_id;

    MT3351_IRQMask(MT3351_KPAD_IRQ_CODE);

    if(!pwk_pressed)
    {
	if(mnk_pressed)
	{
	    press_stamp = jiffies;
	    input_report_key(edev->input, 0x6b, 1);
	    pwk_pressed = 1;
	}
	else
	{
            press_stamp = jiffies;
	    pwk_pressed = 1;
            //printk("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	    input_event(edev->input, EV_KEY, 0x9e, 1);
	}
    }
    else
    {   
#if 1
        //if(time_after(jiffies, (press_stamp + 3 * HZ)) && mnk_pressed)
	if(mnk_pressed)
        {
	    input_report_key(edev->input, 0x6b, 0);
        }        
#endif	
        pwk_pressed = 0;
        input_event(edev->input, EV_KEY, 0x9e, 0);
    }

    MT3351_IRQClearInt(MT3351_IRQ_KPAD_CODE);
    MT3351_IRQUnmask(MT3351_IRQ_KPAD_CODE);
    
    return IRQ_HANDLED;
}

/******************************************************************************
 * menu_interrupt_handler
 * 
 * DESCRIPTION:
 *   Menu key interrupt handler!
 * 
 * PARAMETERS: 
 *   int irq
 *   void *dev_id
 *   struct pt_regs *regs
 * 
 * RETURNS: 
 *   IRQ_HANDLED : Successfully handle the IRQ  
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 13 Mar 2009, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

static irqreturn_t menu_interrupt_handler(int irq, void *dev_id)
{
    struct mt3351_kp *edev = dev_id;
    unsigned int get_eint;

    get_eint = mt_get_gpio_in(13);

    if(get_eint)
        __raw_writel(0x8004, 0xf0022190);
    else
        __raw_writel(0x8804, 0xf0022190);
    
    if(!mnk_pressed)
    {
	menu_press_stamp = jiffies;
	mnk_pressed = 1;
	//printk("##################################\n");
        input_event(edev->input, EV_KEY, 0xe5, 1);
    }
    else
    {   
	if(time_after(jiffies, (menu_press_stamp + COUNT_DOWN_TIME * HZ)))
        {
            mnk_pressed = 0;             
        }	
        mnk_pressed = 0;
        input_event(edev->input, EV_KEY, 0xe5, 0);
    }  
    return IRQ_HANDLED;
}

/******************************************************************************
 * kpd_set_debounce
 * 
 * DESCRIPTION:
 *   Set KPD debounce time !
 * 
 * PARAMETERS: 
 *   u16 u2Debounce
 * 
 * RETURNS: 
 *   0 : Successfully set the debounce time
 *  -1 : Failed to set the debounce time
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 22 Jan 2009, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

s32 kpd_set_debounce(u16 u2Debounce)
{
    if(u2Debounce > 0x4000)
    {
        printk("Keypad debounce time is too small !!\n");
        return -1;
    }
    else
    {
        DRV_WriteReg32(KP_DEBOUNCE, u2Debounce);
        return 0;
    }
}

/******************************************************************************
 * kpd_set_gpio
 * 
 * DESCRIPTION:
 *   Setup the KPD pinmux !
 * 
 * PARAMETERS: 
 *   RSUCCESS : Successfully set the KPD pinmux
 * 
 * RETURNS: 
 *   RSUCCESS : Success   
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 22 Jan 2009, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

s32 kpd_set_gpio(void)
{
    mt_set_gpio_dir(MENU_PIN_NO, GPIO_DIR_IN);
    mt_set_gpio_pulldown(MENU_PIN_NO, GPIO_PD_DISABLE);
    mt_set_gpio_pullup(MENU_PIN_NO, GPIO_PU_ENABLE);
    mt_set_gpio_dir(44, GPIO_DIR_OUT);
    mt_set_gpio_OCFG(GPIO13_OCTL_GPIO, GPIO13_OCTL);
    mt_set_gpio_OCFG(GPIO44_OCTL_GPIO, GPIO44_OCTL);
    mt_set_gpio_ICFG(KPCOL7_GPIO44_INPUT, KPCOL7_SRC);
    mt_set_gpio_ICFG(DSPEINT_GPIO6_INPUT, DSPEINT_SRC);
    mt_set_gpio_ICFG(PWRKEY_DEBOUNCE_INPUT, PWRKEY_SRC);
    mt_set_gpio_ICFG(EINT7_GPIO13_INPUT, EINT7_SRC);
    
#if 0	// For EVB Keypad (NAND IF)
    mt_set_gpio_pullup( 85, 1); // Column 0
    mt_set_gpio_pullup( 86, 1); // Column 1
    mt_set_gpio_pullup( 87, 1); // Column 2
    mt_set_gpio_pullup( 88, 1); // Column 3
    mt_set_gpio_pullup( 89, 1); // Column 4
    mt_set_gpio_pullup( 90, 1); // Column 5
    mt_set_gpio_pullup( 91, 1); // Column 6
    mt_set_gpio_pullup( 44, 1); // Column 7

    mt_set_gpio_PinMux(PIN_MUX_NLD_CTRLL_KEYPAD, NLD_CTLL);
    mt_set_gpio_PinMux(PIN_MUX_NLD_CTRLH_KEYPAD, NLD_CTLH);
    mt_set_gpio_PinMux(PIN_MUX_CAM0_CTRL_GPIO, CAM0_CTL);

    if(mt_get_gpio_PinMux(CAM1_CTL)==PIN_MUX_CAM1_CTRL_KEYPAD)
    {
        mt_set_gpio_PinMux(PIN_MUX_CAM1_CTRL_GPIO, CAM1_CTL);
    }

    mt_set_gpio_ICFG(KPCOL7_GPIO44_INPUT, KPCOL7_SRC);
#endif

    return RSUCCESS;
}

/******************************************************************************
* mt_kpd_eint_irq
* 
* DESCRIPTION:
*   Handle for the External Interrupt!
* 
* PARAMETERS: 
*   int irq
*   void *mt
*   struct pt_regs *regs
* 
* RETURNS: 
*   IRQ_HANDLED : Successfully handle the IRQ  
* 
* NOTES: 
*   None
* 
* modification history
* -----------------------------------------------
* 01a, 13 Mar 2009, Koshi, Chiu written
* -----------------------------------------------
******************************************************************************/

static irqreturn_t mt_kpd_eint_irq(int irq, void *mt, struct pt_regs *regs)
{
    irqreturn_t status;

    /* must exist to unmask uint irq line */
    printk("EXTERNAL INTERRUPT: should not go here, please set debounce time\n");
    
    status = IRQ_HANDLED;

    return status;
}

/******************************************************************************
 * kpd_probe
 * 
 * DESCRIPTION:
 *   register the kpd device file operations ! 
 * 
 * PARAMETERS: 
 *   struct device *dev : device structure
 * 
 * RETURNS: 
 *   RSUCCESS : Success   
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 22 Jan 2009, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

static int kpd_probe(struct platform_device *pdev)
{
    struct mt3351_kp *mt3351_kp;
    struct input_dev *input_dev;
    int i, ret;

    struct mt3351_kp_platform_data *pdata = pdev->dev.platform_data;
    
    if(!pdata->rows || !pdata->cols || !pdata->keymap)
    {
        printk("No rows, cols or keymap from pdev!\n");
        return -ENOMEM;
    }
    
    mt3351_kp = kzalloc(sizeof(struct mt3351_kp), GFP_KERNEL);
    input_dev = input_allocate_device();

    if (!input_dev || !mt3351_kp)
	return -ENOMEM;

    platform_set_drvdata(pdev, mt3351_kp); 
    
    mt3351_kp->input = input_dev;  
    
    /* set up pinmux */
    kpd_set_gpio();
    
    kpd_set_debounce(0x400);

    keymap = pdata->keymap;

    if(pdata->rep)
        __set_bit(EV_REP, input_dev->evbit);

    mt3351_kp->rows = pdata->rows;        
    mt3351_kp->cols = pdata->cols;

    __set_bit(EV_KEY, input_dev->evbit);
    
    for(i=0; keymap[i] !=0; i++)
        __set_bit(keymap[i] & KEY_MAX, input_dev->keybit);

    input_dev->name = KPD_DEVICE;

    input_dev->dev.parent = &pdev->dev;
    input_dev->id.bustype = BUS_HOST;
    input_dev->id.vendor  = 0x0001;
    input_dev->id.product = 0x0001;
    input_dev->id.version = 0x0100;

    ret = input_register_device(input_dev);

    if(ret < 0)
    {
        printk("Oops unable to register mt3351-keypad input device!\n");
    }
    
    /* register IRQ line and ISR */   
    MT3351_IRQSensitivity(MT3351_KPAD_IRQ_CODE, MT3351_EDGE_SENSITIVE);

    request_irq(MT3351_IRQ_KPAD_CODE, kpd_interrupt_handler, 0, "MT3351_KPD", mt3351_kp);    
    request_irq(MT3351_EIT_IRQ_CODE , mt_kpd_eint_irq, 0, "MT3351_MENU", mt3351_kp);    
    request_irq(71, menu_interrupt_handler, 0, "MT3351_MENU", mt3351_kp);
    
    return RSUCCESS;
}

/******************************************************************************
 * kpd_remove
 * 
 * DESCRIPTION:
 *   unregister the kpd device file operations ! 
 * 
 * PARAMETERS: 
 *   struct device *dev : device structure
 * 
 * RETURNS: 
 *   RSUCCESS : Success
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 22 Jan 2009, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

static int kpd_remove(struct device *dev)
{
    return RSUCCESS;
}

/******************************************************************************
 * kpd_shutdown
 * 
 * DESCRIPTION:
 *   unregister the kpd device file operations ! 
 * 
 * PARAMETERS: 
 *   struct device *dev : device structure
 * 
 * RETURNS: 
 *   RSUCCESS : Success
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 22 Jan 2009, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

static void kpd_shutdown(struct device *dev)
{
    printk("KPD Shut down\n");
}

/******************************************************************************
 * kpd_suspend
 * 
 * DESCRIPTION:
 *   Suspend the kpd device! 
 * 
 * PARAMETERS: 
 *   struct device *dev
 * 
 * RETURNS: 
 *   None
 * 
 * NOTES: 
 *   Needs to modify !
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 22 Jan 2009, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

static int kpd_suspend(struct device *dev)
{
    printk("KPD Suspend !\n");
    return RSUCCESS;
}

/******************************************************************************
 * kpd_resume
 * 
 * DESCRIPTION:
 *   Resume the kpd device! 
 * 
 * PARAMETERS: 
 *   struct device *dev
 * 
 * RETURNS: 
 *   None
 * 
 * NOTES: 
 *   Needs to modify !
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 22 Jan 2009, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

static int kpd_resume(struct device *dev)
{
    printk("KPD Resume !\n");
    return RSUCCESS;
}

/******************************************************************************
Device driver structure
******************************************************************************/

static struct platform_driver kpd_driver = 
{
    .probe		= kpd_probe,
    .driver     = {
        .name   = KPD_DEVICE,
    },	
	.remove	    = kpd_remove,
	.shutdown	= kpd_shutdown,
	.suspend	= kpd_suspend,
	.resume	    = kpd_resume,	
};

/******************************************************************************
Platform release function
******************************************************************************/

static void kpd_release_dev(struct device *dev)
{
	/* Nothing to release? */
}

/******************************************************************************
Platform device structure
******************************************************************************/

static int mt3351_keymap[] = 
{
    0xe5,
    0xe7,
    0x6c,
    0x6b,
    0x69,
    0xe8,
    0x6a,
    0x66,
    0x67,
    0x9e,
};

static struct resource mt3351_kp_resources[] = 
{
	[0] = 
	{
		.start	= MT3351_KPAD_IRQ_CODE,
		.end	= MT3351_KPAD_IRQ_CODE,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct mt3351_kp_platform_data mt3351_kp_data = 
{
	.rows		= 8,
	.cols		= 8,
	.keymap		= mt3351_keymap,
	.keymapsize	= ARRAY_SIZE(mt3351_keymap),
	.delay		= 4,
};

static struct platform_device kpd_dev = 
{
    .name = KPD_DEVICE,
    .id   = -1,
    .dev  = 
    {
        .platform_data = &mt3351_kp_data,
        .release  = kpd_release_dev,
    },
    .num_resources	= ARRAY_SIZE(mt3351_kp_resources),
    .resource	= mt3351_kp_resources,
};

/******************************************************************************
 * kpd_mod_init
 * 
 * DESCRIPTION:
 *   Register the kpd device driver ! 
 * 
 * PARAMETERS: 
 *   None
 * 
 * RETURNS: 
 *   None
 * 
 * NOTES: 
 *   RSUCCESS : Success
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 22 Jan 2009, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/

static s32 __devinit kpd_mod_init(void)
{
    s32 ret;
 
    printk("MediaTek MT3351 kpd driver register, version %s\n", VERSION);

    ret = platform_driver_register(&kpd_driver);

    if(ret) 
    {
		printk("Unable to register kpd driver (%d)\n", ret);
		return ret;
    }

    ret = platform_device_register(&kpd_dev);

    if(ret) 
    {
        printk("Failed to register kpd device\n");
        platform_driver_unregister(&kpd_driver);
        return ret;
    }   

    return RSUCCESS;
}

/******************************************************************************
 * kpd_mod_exit
 * 
 * DESCRIPTION: 
 *   Free the device driver ! 
 * 
 * PARAMETERS: 
 *   None
 * 
 * RETURNS: 
 *   None
 * 
 * NOTES: 
 *   None
 * 
 * modification history
 * -----------------------------------------------
 * 01a, 22 Jan 2009, Koshi, Chiu written
 * -----------------------------------------------
 ******************************************************************************/
 
static void __exit kpd_mod_exit(void)
{
    printk("MediaTek MT3351 keypad driver unregister, version %s\n", VERSION);
    platform_driver_unregister(&kpd_driver);    
    printk("Done\n");
}

module_init(kpd_mod_init);
module_exit(kpd_mod_exit);
MODULE_AUTHOR("Koshi, Chiu <koshi.chiu@mediatek.com>");
MODULE_DESCRIPTION("MT3351 Keypad Driver (KPD)");
MODULE_LICENSE("GPL");

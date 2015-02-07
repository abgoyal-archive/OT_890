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

extern struct platform_device pxa_device_mci;
extern struct platform_device pxa3xx_device_mci2;
extern struct platform_device pxa3xx_device_mci3;
extern struct platform_device pxa25x_device_udc;
extern struct platform_device pxa27x_device_udc;
extern struct platform_device pxa_device_fb;
extern struct platform_device pxa_device_ffuart;
extern struct platform_device pxa_device_btuart;
extern struct platform_device pxa_device_stuart;
extern struct platform_device pxa_device_hwuart;
extern struct platform_device pxa_device_i2c;
extern struct platform_device pxa_device_i2s;
extern struct platform_device pxa_device_ficp;
extern struct platform_device sa1100_device_rtc;
extern struct platform_device pxa_device_rtc;
extern struct platform_device pxa_device_ac97;

extern struct platform_device pxa27x_device_i2c_power;
extern struct platform_device pxa27x_device_ohci;
extern struct platform_device pxa27x_device_keypad;

extern struct platform_device pxa25x_device_ssp;
extern struct platform_device pxa25x_device_nssp;
extern struct platform_device pxa25x_device_assp;
extern struct platform_device pxa27x_device_ssp1;
extern struct platform_device pxa27x_device_ssp2;
extern struct platform_device pxa27x_device_ssp3;
extern struct platform_device pxa3xx_device_ssp4;

extern struct platform_device pxa25x_device_pwm0;
extern struct platform_device pxa25x_device_pwm1;
extern struct platform_device pxa27x_device_pwm0;
extern struct platform_device pxa27x_device_pwm1;

extern struct platform_device pxa3xx_device_nand;
extern struct platform_device pxa3xx_device_i2c_power;

void __init pxa_register_device(struct platform_device *dev, void *data);

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
 * linux/arch/arm/mach-sa1100/leds.c
 *
 * SA1100 LEDs dispatcher
 * 
 * Copyright (C) 2001 Nicolas Pitre
 */
#include <linux/compiler.h>
#include <linux/init.h>

#include <asm/leds.h>
#include <asm/mach-types.h>

#include "leds.h"

static int __init
sa1100_leds_init(void)
{
	if (machine_is_assabet())
		leds_event = assabet_leds_event;
 	if (machine_is_consus())
 	        leds_event = consus_leds_event;
	if (machine_is_badge4())
	  	leds_event = badge4_leds_event;
	if (machine_is_brutus())
		leds_event = brutus_leds_event;
	if (machine_is_cerf())
		leds_event = cerf_leds_event;
	if (machine_is_flexanet())
		leds_event = flexanet_leds_event;
	if (machine_is_graphicsclient())
		leds_event = graphicsclient_leds_event;
	if (machine_is_hackkit())
		leds_event = hackkit_leds_event;
	if (machine_is_lart())
		leds_event = lart_leds_event;
	if (machine_is_pfs168())
		leds_event = pfs168_leds_event;
	if (machine_is_graphicsmaster())
		leds_event = graphicsmaster_leds_event;
	if (machine_is_adsbitsy())
		leds_event = adsbitsy_leds_event;
	if (machine_is_pt_system3())
		leds_event = system3_leds_event;
	if (machine_is_simpad())
		leds_event = simpad_leds_event; /* what about machine registry? including led, apm... -zecke */

	leds_event(led_start);
	return 0;
}

core_initcall(sa1100_leds_init);

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

/* IO interface mux allocator for ETRAX100LX.
 * Copyright 2004, Axis Communications AB
 * $Id$
 */


#ifndef _IO_INTERFACE_MUX_H
#define _IO_INTERFACE_MUX_H


/* C.f. ETRAX100LX Designer's Reference 20.9 */

/* The order in enum must match the order of interfaces[] in
 * io_interface_mux.c */
enum cris_io_interface {
	/* Begin Non-multiplexed interfaces */
	if_eth = 0,
	if_serial_0,
	/* End Non-multiplexed interfaces */
	if_serial_1,
	if_serial_2,
	if_serial_3,
	if_sync_serial_1,
	if_sync_serial_3,
	if_shared_ram,
	if_shared_ram_w,
	if_par_0,
	if_par_1,
	if_par_w,
	if_scsi8_0,
	if_scsi8_1,
	if_scsi_w,
	if_ata,
	if_csp,
	if_i2c,
	if_usb_1,
	if_usb_2,
	/* GPIO pins */
	if_gpio_grp_a,
	if_gpio_grp_b,
	if_gpio_grp_c,
	if_gpio_grp_d,
	if_gpio_grp_e,
	if_gpio_grp_f,
	if_max_interfaces,
	if_unclaimed
};

int cris_request_io_interface(enum cris_io_interface ioif, const char *device_id);

void cris_free_io_interface(enum cris_io_interface ioif);

/* port can be 'a', 'b' or 'g' */
int cris_io_interface_allocate_pins(const enum cris_io_interface ioif,
				    const char port,
				    const unsigned start_bit,
				    const unsigned stop_bit);

/* port can be 'a', 'b' or 'g' */
int cris_io_interface_free_pins(const enum cris_io_interface ioif,
                                const char port,
                                const unsigned start_bit,
                                const unsigned stop_bit);

int cris_io_interface_register_watcher(void (*notify)(const unsigned int gpio_in_available,
						      const unsigned int gpio_out_available,
						      const unsigned char pa_available,
						      const unsigned char pb_available));

void cris_io_interface_delete_watcher(void (*notify)(const unsigned int gpio_in_available,
						     const unsigned int gpio_out_available,
                                                     const unsigned char pa_available,
						     const unsigned char pb_available));

#endif /* _IO_INTERFACE_MUX_H */

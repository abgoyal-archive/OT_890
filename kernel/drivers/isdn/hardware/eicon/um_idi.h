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

/* $Id$ */

#ifndef __DIVA_USER_MODE_IDI_CORE_H__
#define __DIVA_USER_MODE_IDI_CORE_H__


/*
  interface between UM IDI core and OS dependent part
  */
int diva_user_mode_idi_init(void);
void diva_user_mode_idi_finit(void);
void *divas_um_idi_create_entity(dword adapter_nr, void *file);
int divas_um_idi_delete_entity(int adapter_nr, void *entity);

typedef int (*divas_um_idi_copy_to_user_fn_t) (void *os_handle,
					       void *dst,
					       const void *src,
					       int length);
typedef int (*divas_um_idi_copy_from_user_fn_t) (void *os_handle,
						 void *dst,
						 const void *src,
						 int length);

int diva_um_idi_read(void *entity,
		     void *os_handle,
		     void *dst,
		     int max_length, divas_um_idi_copy_to_user_fn_t cp_fn);

int diva_um_idi_write(void *entity,
		      void *os_handle,
		      const void *src,
		      int length, divas_um_idi_copy_from_user_fn_t cp_fn);

int diva_user_mode_idi_ind_ready(void *entity, void *os_handle);
void *diva_um_id_get_os_context(void *entity);
int diva_os_get_context_size(void);
int divas_um_idi_entity_assigned(void *entity);
int divas_um_idi_entity_start_remove(void *entity);

void diva_um_idi_start_wdog(void *entity);
void diva_um_idi_stop_wdog(void *entity);

#endif

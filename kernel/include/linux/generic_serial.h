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
 *  generic_serial.h
 *
 *  Copyright (C) 1998 R.E.Wolff@BitWizard.nl
 *
 *  written for the SX serial driver.
 *     Contains the code that should be shared over all the serial drivers.
 *
 *  Version 0.1 -- December, 1998.
 */

#ifndef GENERIC_SERIAL_H
#define GENERIC_SERIAL_H

#ifdef __KERNEL__
#include <linux/mutex.h>
#include <linux/tty.h>

struct real_driver {
  void                    (*disable_tx_interrupts) (void *);
  void                    (*enable_tx_interrupts) (void *);
  void                    (*disable_rx_interrupts) (void *);
  void                    (*enable_rx_interrupts) (void *);
  void                    (*shutdown_port) (void*);
  int                     (*set_real_termios) (void*);
  int                     (*chars_in_buffer) (void*);
  void                    (*close) (void*);
  void                    (*hungup) (void*);
  void                    (*getserial) (void*, struct serial_struct *sp);
};



struct gs_port {
  int                     magic;
  struct tty_port	  port;
  unsigned char           *xmit_buf; 
  int                     xmit_head;
  int                     xmit_tail;
  int                     xmit_cnt;
  struct mutex            port_write_mutex;
  unsigned long           event;
  unsigned short          closing_wait;
  int                     close_delay;
  struct real_driver      *rd;
  int                     wakeup_chars;
  int                     baud_base;
  int                     baud;
  int                     custom_divisor;
  spinlock_t              driver_lock;
};

#endif /* __KERNEL__ */

/* Flags */
/* Warning: serial.h defines some ASYNC_ flags, they say they are "only"
   used in serial.c, but they are also used in all other serial drivers. 
   Make sure they don't clash with these here... */
#define GS_TX_INTEN      0x00800000
#define GS_RX_INTEN      0x00400000
#define GS_ACTIVE        0x00200000



#define GS_TYPE_NORMAL   1

#define GS_DEBUG_FLUSH   0x00000001
#define GS_DEBUG_BTR     0x00000002
#define GS_DEBUG_TERMIOS 0x00000004
#define GS_DEBUG_STUFF   0x00000008
#define GS_DEBUG_CLOSE   0x00000010
#define GS_DEBUG_FLOW    0x00000020
#define GS_DEBUG_WRITE   0x00000040

#ifdef __KERNEL__
int gs_put_char(struct tty_struct *tty, unsigned char ch);
int  gs_write(struct tty_struct *tty, 
             const unsigned char *buf, int count);
int  gs_write_room(struct tty_struct *tty);
int  gs_chars_in_buffer(struct tty_struct *tty);
void gs_flush_buffer(struct tty_struct *tty);
void gs_flush_chars(struct tty_struct *tty);
void gs_stop(struct tty_struct *tty);
void gs_start(struct tty_struct *tty);
void gs_hangup(struct tty_struct *tty);
int  gs_block_til_ready(void *port, struct file *filp);
void gs_close(struct tty_struct *tty, struct file *filp);
void gs_set_termios (struct tty_struct * tty, 
                     struct ktermios * old_termios);
int  gs_init_port(struct gs_port *port);
int  gs_setserial(struct gs_port *port, struct serial_struct __user *sp);
int  gs_getserial(struct gs_port *port, struct serial_struct __user *sp);
void gs_got_break(struct gs_port *port);
#endif /* __KERNEL__ */
#endif

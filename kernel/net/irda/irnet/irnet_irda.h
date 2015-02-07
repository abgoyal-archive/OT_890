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
 *	IrNET protocol module : Synchronous PPP over an IrDA socket.
 *
 *		Jean II - HPL `00 - <jt@hpl.hp.com>
 *
 * This file contains all definitions and declarations necessary for the
 * IRDA part of the IrNET module (dealing with IrTTP, IrIAS and co).
 * This file is a private header, so other modules don't want to know
 * what's in there...
 */

#ifndef IRNET_IRDA_H
#define IRNET_IRDA_H

/***************************** INCLUDES *****************************/
/* Please add other headers in irnet.h */

#include "irnet.h"		/* Module global include */

/************************ CONSTANTS & MACROS ************************/

/*
 * Name of the service (socket name) used by IrNET
 */
/* IAS object name (or part of it) */
#define IRNET_SERVICE_NAME	"IrNetv1"
/* IAS attribute */
#define IRNET_IAS_VALUE		"IrDA:TinyTP:LsapSel"
/* LMP notify name for client (only for /proc/net/irda/irlmp) */
#define IRNET_NOTIFY_NAME	"IrNET socket"
/* LMP notify name for server (only for /proc/net/irda/irlmp) */
#define IRNET_NOTIFY_NAME_SERV	"IrNET server"

/****************************** TYPES ******************************/

/*
 * This is the main structure where we store all the data pertaining to
 * the IrNET server (listen for connection requests) and the root
 * of the IrNET socket list
 */
typedef struct irnet_root
{
  irnet_socket		s;		/* To pretend we are a client... */

  /* Generic stuff */
  int			magic;		/* Paranoia */
  int			running;	/* Are we operational ? */

  /* Link list of all IrNET instances opened */
  hashbin_t *		list;
  spinlock_t		spinlock;	/* Serialize access to the list */
  /* Note : the way hashbin has been designed is absolutely not
   * reentrant, beware... So, we blindly protect all with spinlock */

  /* Handle for the hint bit advertised in IrLMP */
  void *		skey;

  /* Server socket part */
  struct ias_object *	ias_obj;	/* Our service name + lsap in IAS */

} irnet_root;


/**************************** PROTOTYPES ****************************/

/* ----------------------- CONTROL CHANNEL ----------------------- */
static void
	irnet_post_event(irnet_socket *,
			 irnet_event,
			 __u32,
			 __u32,
			 char *,
			 __u16);
/* ----------------------- IRDA SUBROUTINES ----------------------- */
static inline int
	irnet_open_tsap(irnet_socket *);
static inline __u8
	irnet_ias_to_tsap(irnet_socket *,
			  int,
			  struct ias_value *);
static inline int
	irnet_find_lsap_sel(irnet_socket *);
static inline int
	irnet_connect_tsap(irnet_socket *);
static inline int
	irnet_discover_next_daddr(irnet_socket *);
static inline int
	irnet_discover_daddr_and_lsap_sel(irnet_socket *);
static inline int
	irnet_dname_to_daddr(irnet_socket *);
/* ------------------------ SERVER SOCKET ------------------------ */
static inline int
	irnet_daddr_to_dname(irnet_socket *);
static inline irnet_socket *
	irnet_find_socket(irnet_socket *);
static inline int
	irnet_connect_socket(irnet_socket *,
			     irnet_socket *,
			     struct qos_info *,
			     __u32,
			     __u8);
static inline void
	irnet_disconnect_server(irnet_socket *,
				struct sk_buff *);
static inline int
	irnet_setup_server(void);
static inline void
	irnet_destroy_server(void);
/* ---------------------- IRDA-TTP CALLBACKS ---------------------- */
static int
	irnet_data_indication(void *,		/* instance */
			      void *,		/* sap */
			      struct sk_buff *);
static void
	irnet_disconnect_indication(void *,
				    void *,
				    LM_REASON,
				    struct sk_buff *);
static void
	irnet_connect_confirm(void *,
			      void *,
			      struct qos_info *,
			      __u32,
			      __u8,
			      struct sk_buff *);
static void
	irnet_flow_indication(void *,
			      void *,
			      LOCAL_FLOW);
static void
	irnet_status_indication(void *,
				LINK_STATUS,
				LOCK_STATUS);
static void
	irnet_connect_indication(void *,
				 void *,
				 struct qos_info *,
				 __u32,
				 __u8,
				 struct sk_buff *);
/* -------------------- IRDA-IAS/LMP CALLBACKS -------------------- */
static void
	irnet_getvalue_confirm(int,
			       __u16,
			       struct ias_value *,
			       void *);
static void
	irnet_discovervalue_confirm(int,
				    __u16,
				    struct ias_value *,
				    void *);
#ifdef DISCOVERY_EVENTS
static void
	irnet_discovery_indication(discinfo_t *,
				   DISCOVERY_MODE,
				   void *);
static void
	irnet_expiry_indication(discinfo_t *,
				DISCOVERY_MODE,
				void *);
#endif

/**************************** VARIABLES ****************************/

/*
 * The IrNET server. Listen to connection requests and co...
 */
static struct irnet_root	irnet_server;

/* Control channel stuff (note : extern) */
struct irnet_ctrl_channel	irnet_events;

/* The /proc/net/irda directory, defined elsewhere... */
#ifdef CONFIG_PROC_FS
extern struct proc_dir_entry *proc_irda;
#endif /* CONFIG_PROC_FS */

#endif /* IRNET_IRDA_H */

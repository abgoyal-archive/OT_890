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

#ifndef _LINUX_DN_H
#define _LINUX_DN_H

#include <linux/types.h>

/*

	DECnet Data Structures and Constants

*/

/* 
 * DNPROTO_NSP can't be the same as SOL_SOCKET, 
 * so increment each by one (compared to ULTRIX)
 */
#define DNPROTO_NSP     2                       /* NSP protocol number       */
#define DNPROTO_ROU     3                       /* Routing protocol number   */
#define DNPROTO_NML     4                       /* Net mgt protocol number   */
#define DNPROTO_EVL     5                       /* Evl protocol number (usr) */
#define DNPROTO_EVR     6                       /* Evl protocol number (evl) */
#define DNPROTO_NSPT    7                       /* NSP trace protocol number */


#define DN_ADDL		2
#define DN_MAXADDL	2 /* ULTRIX headers have 20 here, but pathworks has 2 */
#define DN_MAXOPTL	16
#define DN_MAXOBJL	16
#define DN_MAXACCL	40
#define DN_MAXALIASL	128
#define DN_MAXNODEL	256
#define DNBUFSIZE	65023

/* 
 * SET/GET Socket options  - must match the DSO_ numbers below
 */
#define SO_CONDATA      1
#define SO_CONACCESS    2
#define SO_PROXYUSR     3
#define SO_LINKINFO     7

#define DSO_CONDATA     1        /* Set/Get connect data                */
#define DSO_DISDATA     10       /* Set/Get disconnect data             */
#define DSO_CONACCESS   2        /* Set/Get connect access data         */
#define DSO_ACCEPTMODE  4        /* Set/Get accept mode                 */
#define DSO_CONACCEPT   5        /* Accept deferred connection          */
#define DSO_CONREJECT   6        /* Reject deferred connection          */
#define DSO_LINKINFO    7        /* Set/Get link information            */
#define DSO_STREAM      8        /* Set socket type to stream           */
#define DSO_SEQPACKET   9        /* Set socket type to sequenced packet */
#define DSO_MAXWINDOW   11       /* Maximum window size allowed         */
#define DSO_NODELAY	12       /* Turn off nagle                      */
#define DSO_CORK        13       /* Wait for more data!                 */
#define DSO_SERVICES	14       /* NSP Services field                  */
#define DSO_INFO	15       /* NSP Info field                      */
#define DSO_MAX         15       /* Maximum option number               */


/* LINK States */
#define LL_INACTIVE	0
#define LL_CONNECTING	1
#define LL_RUNNING	2
#define LL_DISCONNECTING 3

#define ACC_IMMED 0
#define ACC_DEFER 1

#define SDF_WILD        1                  /* Wild card object          */
#define SDF_PROXY       2                  /* Addr eligible for proxy   */
#define SDF_UICPROXY    4                  /* Use uic-based proxy       */

/* Structures */


struct dn_naddr 
{
	__le16		a_len;
	__u8 a_addr[DN_MAXADDL]; /* Two bytes little endian */
};

struct sockaddr_dn
{
	__u16		sdn_family;
	__u8		sdn_flags;
	__u8		sdn_objnum;
	__le16		sdn_objnamel;
	__u8		sdn_objname[DN_MAXOBJL];
	struct   dn_naddr	sdn_add;
};
#define sdn_nodeaddrl   sdn_add.a_len   /* Node address length  */
#define sdn_nodeaddr    sdn_add.a_addr  /* Node address         */



/*
 * DECnet set/get DSO_CONDATA, DSO_DISDATA (optional data) structure
 */
struct optdata_dn {
        __le16  opt_status;     /* Extended status return */
#define opt_sts opt_status
        __le16  opt_optl;       /* Length of user data    */
        __u8   opt_data[16];   /* User data              */
};

struct accessdata_dn
{
	__u8		acc_accl;
	__u8		acc_acc[DN_MAXACCL];
	__u8 		acc_passl;
	__u8		acc_pass[DN_MAXACCL];
	__u8 		acc_userl;
	__u8		acc_user[DN_MAXACCL];
};

/*
 * DECnet logical link information structure
 */
struct linkinfo_dn {
        __u16  idn_segsize;    /* Segment size for link */
        __u8   idn_linkstate;  /* Logical link state    */
};

/*
 * Ethernet address format (for DECnet)
 */
union etheraddress {
        __u8 dne_addr[6];             /* Full ethernet address */
  struct {
                __u8 dne_hiord[4];    /* DECnet HIORD prefix   */
                __u8 dne_nodeaddr[2]; /* DECnet node address   */
  } dne_remote;
};


/*
 * DECnet physical socket address format
 */
struct dn_addr {
        __le16 dna_family;      /* AF_DECnet               */
        union etheraddress dna_netaddr; /* DECnet ethernet address */
};

#define DECNET_IOCTL_BASE 0x89 /* PROTOPRIVATE range */

#define SIOCSNETADDR  _IOW(DECNET_IOCTL_BASE, 0xe0, struct dn_naddr)
#define SIOCGNETADDR  _IOR(DECNET_IOCTL_BASE, 0xe1, struct dn_naddr)
#define OSIOCSNETADDR _IOW(DECNET_IOCTL_BASE, 0xe0, int)
#define OSIOCGNETADDR _IOR(DECNET_IOCTL_BASE, 0xe1, int)

#endif /* _LINUX_DN_H */

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
 *  Copyright (c) 2000-2008 LSI Corporation.
 *
 *
 *           Name:  mpi_lan.h
 *          Title:  MPI LAN messages and structures
 *  Creation Date:  June 30, 2000
 *
 *    mpi_lan.h Version:  01.05.01
 *
 *  Version History
 *  ---------------
 *
 *  Date      Version   Description
 *  --------  --------  ------------------------------------------------------
 *  05-08-00  00.10.01  Original release for 0.10 spec dated 4/26/2000.
 *  05-24-00  00.10.02  Added LANStatus field to _MSG_LAN_SEND_REPLY.
 *                      Added LANStatus field to _MSG_LAN_RECEIVE_POST_REPLY.
 *                      Moved ListCount field in _MSG_LAN_RECEIVE_POST_REPLY.
 *  06-06-00  01.00.01  Update version number for 1.0 release.
 *  06-12-00  01.00.02  Added MPI_ to BUCKETSTATUS_ definitions.
 *  06-22-00  01.00.03  Major changes to match new LAN definition in 1.0 spec.
 *  06-30-00  01.00.04  Added Context Reply definitions per revised proposal.
 *                      Changed transaction context usage to bucket/buffer.
 *  07-05-00  01.00.05  Removed LAN_RECEIVE_POST_BUCKET_CONTEXT_MASK definition
 *                      to lan private header file
 *  11-02-00  01.01.01  Original release for post 1.0 work
 *  02-20-01  01.01.02  Started using MPI_POINTER.
 *  03-27-01  01.01.03  Added structure offset comments.
 *  08-08-01  01.02.01  Original release for v1.2 work.
 *  05-11-04  01.03.01  Original release for MPI v1.3.
 *  08-19-04  01.05.01  Original release for MPI v1.5.
 *  --------------------------------------------------------------------------
 */

#ifndef MPI_LAN_H
#define MPI_LAN_H


/******************************************************************************
*
*               L A N    M e s s a g e s
*
*******************************************************************************/

/* LANSend messages */

typedef struct _MSG_LAN_SEND_REQUEST
{
    U16                     Reserved;           /* 00h */
    U8                      ChainOffset;        /* 02h */
    U8                      Function;           /* 03h */
    U16                     Reserved2;          /* 04h */
    U8                      PortNumber;         /* 06h */
    U8                      MsgFlags;           /* 07h */
    U32                     MsgContext;         /* 08h */
    SGE_MPI_UNION           SG_List[1];         /* 0Ch */
} MSG_LAN_SEND_REQUEST, MPI_POINTER PTR_MSG_LAN_SEND_REQUEST,
  LANSendRequest_t, MPI_POINTER pLANSendRequest_t;


typedef struct _MSG_LAN_SEND_REPLY
{
    U16                     Reserved;           /* 00h */
    U8                      MsgLength;          /* 02h */
    U8                      Function;           /* 03h */
    U8                      Reserved2;          /* 04h */
    U8                      NumberOfContexts;   /* 05h */
    U8                      PortNumber;         /* 06h */
    U8                      MsgFlags;           /* 07h */
    U32                     MsgContext;         /* 08h */
    U16                     Reserved3;          /* 0Ch */
    U16                     IOCStatus;          /* 0Eh */
    U32                     IOCLogInfo;         /* 10h */
    U32                     BufferContext;      /* 14h */
} MSG_LAN_SEND_REPLY, MPI_POINTER PTR_MSG_LAN_SEND_REPLY,
  LANSendReply_t, MPI_POINTER pLANSendReply_t;


/* LANReceivePost */

typedef struct _MSG_LAN_RECEIVE_POST_REQUEST
{
    U16                     Reserved;           /* 00h */
    U8                      ChainOffset;        /* 02h */
    U8                      Function;           /* 03h */
    U16                     Reserved2;          /* 04h */
    U8                      PortNumber;         /* 06h */
    U8                      MsgFlags;           /* 07h */
    U32                     MsgContext;         /* 08h */
    U32                     BucketCount;        /* 0Ch */
    SGE_MPI_UNION           SG_List[1];         /* 10h */
} MSG_LAN_RECEIVE_POST_REQUEST, MPI_POINTER PTR_MSG_LAN_RECEIVE_POST_REQUEST,
  LANReceivePostRequest_t, MPI_POINTER pLANReceivePostRequest_t;


typedef struct _MSG_LAN_RECEIVE_POST_REPLY
{
    U16                     Reserved;           /* 00h */
    U8                      MsgLength;          /* 02h */
    U8                      Function;           /* 03h */
    U8                      Reserved2;          /* 04h */
    U8                      NumberOfContexts;   /* 05h */
    U8                      PortNumber;         /* 06h */
    U8                      MsgFlags;           /* 07h */
    U32                     MsgContext;         /* 08h */
    U16                     Reserved3;          /* 0Ch */
    U16                     IOCStatus;          /* 0Eh */
    U32                     IOCLogInfo;         /* 10h */
    U32                     BucketsRemaining;   /* 14h */
    U32                     PacketOffset;       /* 18h */
    U32                     PacketLength;       /* 1Ch */
    U32                     BucketContext[1];   /* 20h */
} MSG_LAN_RECEIVE_POST_REPLY, MPI_POINTER PTR_MSG_LAN_RECEIVE_POST_REPLY,
  LANReceivePostReply_t, MPI_POINTER pLANReceivePostReply_t;


/* LANReset */

typedef struct _MSG_LAN_RESET_REQUEST
{
    U16                     Reserved;           /* 00h */
    U8                      ChainOffset;        /* 02h */
    U8                      Function;           /* 03h */
    U16                     Reserved2;          /* 04h */
    U8                      PortNumber;         /* 05h */
    U8                      MsgFlags;           /* 07h */
    U32                     MsgContext;         /* 08h */
} MSG_LAN_RESET_REQUEST, MPI_POINTER PTR_MSG_LAN_RESET_REQUEST,
  LANResetRequest_t, MPI_POINTER pLANResetRequest_t;


typedef struct _MSG_LAN_RESET_REPLY
{
    U16                     Reserved;           /* 00h */
    U8                      MsgLength;          /* 02h */
    U8                      Function;           /* 03h */
    U16                     Reserved2;          /* 04h */
    U8                      PortNumber;         /* 06h */
    U8                      MsgFlags;           /* 07h */
    U32                     MsgContext;         /* 08h */
    U16                     Reserved3;          /* 0Ch */
    U16                     IOCStatus;          /* 0Eh */
    U32                     IOCLogInfo;         /* 10h */
} MSG_LAN_RESET_REPLY, MPI_POINTER PTR_MSG_LAN_RESET_REPLY,
  LANResetReply_t, MPI_POINTER pLANResetReply_t;


/****************************************************************************/
/* LAN Context Reply defines and macros                                     */
/****************************************************************************/

#define LAN_REPLY_PACKET_LENGTH_MASK            (0x0000FFFF)
#define LAN_REPLY_PACKET_LENGTH_SHIFT           (0)
#define LAN_REPLY_BUCKET_CONTEXT_MASK           (0x07FF0000)
#define LAN_REPLY_BUCKET_CONTEXT_SHIFT          (16)
#define LAN_REPLY_BUFFER_CONTEXT_MASK           (0x07FFFFFF)
#define LAN_REPLY_BUFFER_CONTEXT_SHIFT          (0)
#define LAN_REPLY_FORM_MASK                     (0x18000000)
#define LAN_REPLY_FORM_RECEIVE_SINGLE           (0x00)
#define LAN_REPLY_FORM_RECEIVE_MULTIPLE         (0x01)
#define LAN_REPLY_FORM_SEND_SINGLE              (0x02)
#define LAN_REPLY_FORM_MESSAGE_CONTEXT          (0x03)
#define LAN_REPLY_FORM_SHIFT                    (27)

#define GET_LAN_PACKET_LENGTH(x)    (((x) & LAN_REPLY_PACKET_LENGTH_MASK)   \
                                        >> LAN_REPLY_PACKET_LENGTH_SHIFT)

#define SET_LAN_PACKET_LENGTH(x, lth)                                       \
            ((x) = ((x) & ~LAN_REPLY_PACKET_LENGTH_MASK) |                  \
                            (((lth) << LAN_REPLY_PACKET_LENGTH_SHIFT) &     \
                                        LAN_REPLY_PACKET_LENGTH_MASK))

#define GET_LAN_BUCKET_CONTEXT(x)   (((x) & LAN_REPLY_BUCKET_CONTEXT_MASK)  \
                                        >> LAN_REPLY_BUCKET_CONTEXT_SHIFT)

#define SET_LAN_BUCKET_CONTEXT(x, ctx)                                      \
            ((x) = ((x) & ~LAN_REPLY_BUCKET_CONTEXT_MASK) |                 \
                            (((ctx) << LAN_REPLY_BUCKET_CONTEXT_SHIFT) &    \
                                        LAN_REPLY_BUCKET_CONTEXT_MASK))

#define GET_LAN_BUFFER_CONTEXT(x)   (((x) & LAN_REPLY_BUFFER_CONTEXT_MASK)  \
                                        >> LAN_REPLY_BUFFER_CONTEXT_SHIFT)

#define SET_LAN_BUFFER_CONTEXT(x, ctx)                                      \
            ((x) = ((x) & ~LAN_REPLY_BUFFER_CONTEXT_MASK) |                 \
                            (((ctx) << LAN_REPLY_BUFFER_CONTEXT_SHIFT) &    \
                                        LAN_REPLY_BUFFER_CONTEXT_MASK))

#define GET_LAN_FORM(x)             (((x) & LAN_REPLY_FORM_MASK)            \
                                        >> LAN_REPLY_FORM_SHIFT)

#define SET_LAN_FORM(x, frm)                                                \
            ((x) = ((x) & ~LAN_REPLY_FORM_MASK) |                           \
                            (((frm) << LAN_REPLY_FORM_SHIFT) &              \
                                        LAN_REPLY_FORM_MASK))


/****************************************************************************/
/* LAN Current Device State defines                                         */
/****************************************************************************/

#define MPI_LAN_DEVICE_STATE_RESET                     (0x00)
#define MPI_LAN_DEVICE_STATE_OPERATIONAL               (0x01)


/****************************************************************************/
/* LAN Loopback defines                                                     */
/****************************************************************************/

#define MPI_LAN_TX_MODES_ENABLE_LOOPBACK_SUPPRESSION   (0x01)

#endif


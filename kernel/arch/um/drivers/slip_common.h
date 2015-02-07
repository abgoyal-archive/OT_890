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

#ifndef __UM_SLIP_COMMON_H
#define __UM_SLIP_COMMON_H

#define BUF_SIZE 1500
 /* two bytes each for a (pathological) max packet of escaped chars +  *
  * terminating END char + initial END char                            */
#define ENC_BUF_SIZE (2 * BUF_SIZE + 2)

/* SLIP protocol characters. */
#define SLIP_END             0300	/* indicates end of frame	*/
#define SLIP_ESC             0333	/* indicates byte stuffing	*/
#define SLIP_ESC_END         0334	/* ESC ESC_END means END 'data'	*/
#define SLIP_ESC_ESC         0335	/* ESC ESC_ESC means ESC 'data'	*/

static inline int slip_unesc(unsigned char c, unsigned char *buf, int *pos,
                             int *esc)
{
	int ret;

	switch(c){
	case SLIP_END:
		*esc = 0;
		ret=*pos;
		*pos=0;
		return(ret);
	case SLIP_ESC:
		*esc = 1;
		return(0);
	case SLIP_ESC_ESC:
		if(*esc){
			*esc = 0;
			c = SLIP_ESC;
		}
		break;
	case SLIP_ESC_END:
		if(*esc){
			*esc = 0;
			c = SLIP_END;
		}
		break;
	}
	buf[(*pos)++] = c;
	return(0);
}

static inline int slip_esc(unsigned char *s, unsigned char *d, int len)
{
	unsigned char *ptr = d;
	unsigned char c;

	/*
	 * Send an initial END character to flush out any
	 * data that may have accumulated in the receiver
	 * due to line noise.
	 */

	*ptr++ = SLIP_END;

	/*
	 * For each byte in the packet, send the appropriate
	 * character sequence, according to the SLIP protocol.
	 */

	while (len-- > 0) {
		switch(c = *s++) {
		case SLIP_END:
			*ptr++ = SLIP_ESC;
			*ptr++ = SLIP_ESC_END;
			break;
		case SLIP_ESC:
			*ptr++ = SLIP_ESC;
			*ptr++ = SLIP_ESC_ESC;
			break;
		default:
			*ptr++ = c;
			break;
		}
	}
	*ptr++ = SLIP_END;
	return (ptr - d);
}

struct slip_proto {
	unsigned char ibuf[ENC_BUF_SIZE];
	unsigned char obuf[ENC_BUF_SIZE];
	int more; /* more data: do not read fd until ibuf has been drained */
	int pos;
	int esc;
};

static inline void slip_proto_init(struct slip_proto * slip)
{
	memset(slip->ibuf, 0, sizeof(slip->ibuf));
	memset(slip->obuf, 0, sizeof(slip->obuf));
	slip->more = 0;
	slip->pos = 0;
	slip->esc = 0;
}

extern int slip_proto_read(int fd, void *buf, int len,
			   struct slip_proto *slip);
extern int slip_proto_write(int fd, void *buf, int len,
			    struct slip_proto *slip);

#endif

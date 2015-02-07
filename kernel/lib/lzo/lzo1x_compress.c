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
 *  LZO1X Compressor from MiniLZO
 *
 *  Copyright (C) 1996-2005 Markus F.X.J. Oberhumer <markus@oberhumer.com>
 *
 *  The full LZO package can be found at:
 *  http://www.oberhumer.com/opensource/lzo/
 *
 *  Changed for kernel use by:
 *  Nitin Gupta <nitingupta910@gmail.com>
 *  Richard Purdie <rpurdie@openedhand.com>
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/lzo.h>
#include <asm/unaligned.h>
#include "lzodefs.h"

static noinline size_t
_lzo1x_1_do_compress(const unsigned char *in, size_t in_len,
		unsigned char *out, size_t *out_len, void *wrkmem)
{
	const unsigned char * const in_end = in + in_len;
	const unsigned char * const ip_end = in + in_len - M2_MAX_LEN - 5;
	const unsigned char ** const dict = wrkmem;
	const unsigned char *ip = in, *ii = ip;
	const unsigned char *end, *m, *m_pos;
	size_t m_off, m_len, dindex;
	unsigned char *op = out;

	ip += 4;

	for (;;) {
		dindex = ((size_t)(0x21 * DX3(ip, 5, 5, 6)) >> 5) & D_MASK;
		m_pos = dict[dindex];

		if (m_pos < in)
			goto literal;

		if (ip == m_pos || ((size_t)(ip - m_pos) > M4_MAX_OFFSET))
			goto literal;

		m_off = ip - m_pos;
		if (m_off <= M2_MAX_OFFSET || m_pos[3] == ip[3])
			goto try_match;

		dindex = (dindex & (D_MASK & 0x7ff)) ^ (D_HIGH | 0x1f);
		m_pos = dict[dindex];

		if (m_pos < in)
			goto literal;

		if (ip == m_pos || ((size_t)(ip - m_pos) > M4_MAX_OFFSET))
			goto literal;

		m_off = ip - m_pos;
		if (m_off <= M2_MAX_OFFSET || m_pos[3] == ip[3])
			goto try_match;

		goto literal;

try_match:
		if (get_unaligned((const unsigned short *)m_pos)
				== get_unaligned((const unsigned short *)ip)) {
			if (likely(m_pos[2] == ip[2]))
					goto match;
		}

literal:
		dict[dindex] = ip;
		++ip;
		if (unlikely(ip >= ip_end))
			break;
		continue;

match:
		dict[dindex] = ip;
		if (ip != ii) {
			size_t t = ip - ii;

			if (t <= 3) {
				op[-2] |= t;
			} else if (t <= 18) {
				*op++ = (t - 3);
			} else {
				size_t tt = t - 18;

				*op++ = 0;
				while (tt > 255) {
					tt -= 255;
					*op++ = 0;
				}
				*op++ = tt;
			}
			do {
				*op++ = *ii++;
			} while (--t > 0);
		}

		ip += 3;
		if (m_pos[3] != *ip++ || m_pos[4] != *ip++
				|| m_pos[5] != *ip++ || m_pos[6] != *ip++
				|| m_pos[7] != *ip++ || m_pos[8] != *ip++) {
			--ip;
			m_len = ip - ii;

			if (m_off <= M2_MAX_OFFSET) {
				m_off -= 1;
				*op++ = (((m_len - 1) << 5)
						| ((m_off & 7) << 2));
				*op++ = (m_off >> 3);
			} else if (m_off <= M3_MAX_OFFSET) {
				m_off -= 1;
				*op++ = (M3_MARKER | (m_len - 2));
				goto m3_m4_offset;
			} else {
				m_off -= 0x4000;

				*op++ = (M4_MARKER | ((m_off & 0x4000) >> 11)
						| (m_len - 2));
				goto m3_m4_offset;
			}
		} else {
			end = in_end;
			m = m_pos + M2_MAX_LEN + 1;

			while (ip < end && *m == *ip) {
				m++;
				ip++;
			}
			m_len = ip - ii;

			if (m_off <= M3_MAX_OFFSET) {
				m_off -= 1;
				if (m_len <= 33) {
					*op++ = (M3_MARKER | (m_len - 2));
				} else {
					m_len -= 33;
					*op++ = M3_MARKER | 0;
					goto m3_m4_len;
				}
			} else {
				m_off -= 0x4000;
				if (m_len <= M4_MAX_LEN) {
					*op++ = (M4_MARKER
						| ((m_off & 0x4000) >> 11)
						| (m_len - 2));
				} else {
					m_len -= M4_MAX_LEN;
					*op++ = (M4_MARKER
						| ((m_off & 0x4000) >> 11));
m3_m4_len:
					while (m_len > 255) {
						m_len -= 255;
						*op++ = 0;
					}

					*op++ = (m_len);
				}
			}
m3_m4_offset:
			*op++ = ((m_off & 63) << 2);
			*op++ = (m_off >> 6);
		}

		ii = ip;
		if (unlikely(ip >= ip_end))
			break;
	}

	*out_len = op - out;
	return in_end - ii;
}

int lzo1x_1_compress(const unsigned char *in, size_t in_len, unsigned char *out,
			size_t *out_len, void *wrkmem)
{
	const unsigned char *ii;
	unsigned char *op = out;
	size_t t;

	if (unlikely(in_len <= M2_MAX_LEN + 5)) {
		t = in_len;
	} else {
		t = _lzo1x_1_do_compress(in, in_len, op, out_len, wrkmem);
		op += *out_len;
	}

	if (t > 0) {
		ii = in + in_len - t;

		if (op == out && t <= 238) {
			*op++ = (17 + t);
		} else if (t <= 3) {
			op[-2] |= t;
		} else if (t <= 18) {
			*op++ = (t - 3);
		} else {
			size_t tt = t - 18;

			*op++ = 0;
			while (tt > 255) {
				tt -= 255;
				*op++ = 0;
			}

			*op++ = tt;
		}
		do {
			*op++ = *ii++;
		} while (--t > 0);
	}

	*op++ = M4_MARKER | 1;
	*op++ = 0;
	*op++ = 0;

	*out_len = op - out;
	return LZO_E_OK;
}
EXPORT_SYMBOL_GPL(lzo1x_1_compress);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LZO1X-1 Compressor");


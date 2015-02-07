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

/* 	pg.h (c) 1998  Grant R. Guenther <grant@torque.net>
 		       Under the terms of the GNU General Public License


	pg.h defines the user interface to the generic ATAPI packet
        command driver for parallel port ATAPI devices (pg). The
	driver is loosely modelled after the generic SCSI driver, sg,
	although the actual interface is different.

	The pg driver provides a simple character device interface for
        sending ATAPI commands to a device.  With the exception of the
	ATAPI reset operation, all operations are performed by a pair
        of read and write operations to the appropriate /dev/pgN device.
	A write operation delivers a command and any outbound data in
        a single buffer.  Normally, the write will succeed unless the
        device is offline or malfunctioning, or there is already another
	command pending.  If the write succeeds, it should be followed
        immediately by a read operation, to obtain any returned data and
        status information.  A read will fail if there is no operation
        in progress.

	As a special case, the device can be reset with a write operation,
        and in this case, no following read is expected, or permitted.

	There are no ioctl() operations.  Any single operation
	may transfer at most PG_MAX_DATA bytes.  Note that the driver must
        copy the data through an internal buffer.  In keeping with all
	current ATAPI devices, command packets are assumed to be exactly
	12 bytes in length.

	To permit future changes to this interface, the headers in the
	read and write buffers contain a single character "magic" flag.
        Currently this flag must be the character "P".

*/

#define PG_MAGIC	'P'
#define PG_RESET	'Z'
#define PG_COMMAND	'C'

#define PG_MAX_DATA	32768

struct pg_write_hdr {

	char	magic;		/* == PG_MAGIC */
	char	func;		/* PG_RESET or PG_COMMAND */
	int     dlen;		/* number of bytes expected to transfer */
	int     timeout;	/* number of seconds before timeout */
	char	packet[12];	/* packet command */

};

struct pg_read_hdr {

	char	magic;		/* == PG_MAGIC */
	char	scsi;		/* "scsi" status == sense key */
	int	dlen;		/* size of device transfer request */
	int     duration;	/* time in seconds command took */
	char    pad[12];	/* not used */

};

/* end of pg.h */

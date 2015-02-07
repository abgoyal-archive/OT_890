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
 * These structs are used by the system-use-sharing protocol, in which the
 * Rock Ridge extensions are embedded.  It is quite possible that other
 * extensions are present on the disk, and this is fine as long as they
 * all use SUSP
 */

struct SU_SP_s {
	unsigned char magic[2];
	unsigned char skip;
} __attribute__ ((packed));

struct SU_CE_s {
	char extent[8];
	char offset[8];
	char size[8];
};

struct SU_ER_s {
	unsigned char len_id;
	unsigned char len_des;
	unsigned char len_src;
	unsigned char ext_ver;
	char data[0];
} __attribute__ ((packed));

struct RR_RR_s {
	char flags[1];
} __attribute__ ((packed));

struct RR_PX_s {
	char mode[8];
	char n_links[8];
	char uid[8];
	char gid[8];
};

struct RR_PN_s {
	char dev_high[8];
	char dev_low[8];
};

struct SL_component {
	unsigned char flags;
	unsigned char len;
	char text[0];
} __attribute__ ((packed));

struct RR_SL_s {
	unsigned char flags;
	struct SL_component link;
} __attribute__ ((packed));

struct RR_NM_s {
	unsigned char flags;
	char name[0];
} __attribute__ ((packed));

struct RR_CL_s {
	char location[8];
};

struct RR_PL_s {
	char location[8];
};

struct stamp {
	char time[7];
} __attribute__ ((packed));

struct RR_TF_s {
	char flags;
	struct stamp times[0];	/* Variable number of these beasts */
} __attribute__ ((packed));

/* Linux-specific extension for transparent decompression */
struct RR_ZF_s {
	char algorithm[2];
	char parms[2];
	char real_size[8];
};

/*
 * These are the bits and their meanings for flags in the TF structure.
 */
#define TF_CREATE 1
#define TF_MODIFY 2
#define TF_ACCESS 4
#define TF_ATTRIBUTES 8
#define TF_BACKUP 16
#define TF_EXPIRATION 32
#define TF_EFFECTIVE 64
#define TF_LONG_FORM 128

struct rock_ridge {
	char signature[2];
	unsigned char len;
	unsigned char version;
	union {
		struct SU_SP_s SP;
		struct SU_CE_s CE;
		struct SU_ER_s ER;
		struct RR_RR_s RR;
		struct RR_PX_s PX;
		struct RR_PN_s PN;
		struct RR_SL_s SL;
		struct RR_NM_s NM;
		struct RR_CL_s CL;
		struct RR_PL_s PL;
		struct RR_TF_s TF;
		struct RR_ZF_s ZF;
	} u;
};

#define RR_PX 1			/* POSIX attributes */
#define RR_PN 2			/* POSIX devices */
#define RR_SL 4			/* Symbolic link */
#define RR_NM 8			/* Alternate Name */
#define RR_CL 16		/* Child link */
#define RR_PL 32		/* Parent link */
#define RR_RE 64		/* Relocation directory */
#define RR_TF 128		/* Timestamps */

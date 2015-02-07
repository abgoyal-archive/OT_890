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

#include <asm-generic/vmlinux.lds.h>

  .fini      : { *(.fini)    } =0x9090
  _etext = .;
  PROVIDE (etext = .);

  . = ALIGN(4096);
  _sdata = .;
  PROVIDE (sdata = .);

  RODATA

  .unprotected : { *(.unprotected) }
  . = ALIGN(4096);
  PROVIDE (_unprotected_end = .);

  . = ALIGN(4096);
  .note : { *(.note.*) }
  __ex_table : {
	__start___ex_table = .;
	*(__ex_table)
	__stop___ex_table = .;
  }

  BUG_TABLE

  .uml.setup.init : {
	__uml_setup_start = .;
	*(.uml.setup.init)
	__uml_setup_end = .;
  }
	
  .uml.help.init : {
	__uml_help_start = .;
	*(.uml.help.init)
	__uml_help_end = .;
  }
	
  .uml.postsetup.init : {
	__uml_postsetup_start = .;
	*(.uml.postsetup.init)
	__uml_postsetup_end = .;
  }
	
  .init.setup : {
	__setup_start = .;
	*(.init.setup)
	__setup_end = .;
  }

  . = ALIGN(32);
  .data.percpu : {
	__per_cpu_start = . ;
	*(.data.percpu)
	__per_cpu_end = . ;
  }
	
  .initcall.init : {
	__initcall_start = .;
	INITCALLS
	__initcall_end = .;
  }

  .con_initcall.init : {
	__con_initcall_start = .;
	*(.con_initcall.init)
	__con_initcall_end = .;
  }

  .uml.initcall.init : {
	__uml_initcall_start = .;
	*(.uml.initcall.init)
	__uml_initcall_end = .;
  }
  __init_end = .;

  SECURITY_INIT

  .exitcall : {
	__exitcall_begin = .;
	*(.exitcall.exit)
	__exitcall_end = .;
  }

  .uml.exitcall : {
	__uml_exitcall_begin = .;
	*(.uml.exitcall.exit)
	__uml_exitcall_end = .;
  }

  . = ALIGN(4);
  .altinstructions : {
	__alt_instructions = .;
	*(.altinstructions)
	__alt_instructions_end = .;
  }
  .altinstr_replacement : { *(.altinstr_replacement) }
  /* .exit.text is discard at runtime, not link time, to deal with references
     from .altinstructions and .eh_frame */
  .exit.text : { *(.exit.text) }
  .exit.data : { *(.exit.data) }

  .preinit_array : {
	__preinit_array_start = .;
	*(.preinit_array)
	__preinit_array_end = .;
  }
  .init_array : {
	__init_array_start = .;
	*(.init_array)
	__init_array_end = .;
  }
  .fini_array : {
	__fini_array_start = .;
	*(.fini_array)
	__fini_array_end = .;
  }

   . = ALIGN(4096);
  .init.ramfs : {
	__initramfs_start = .;
	*(.init.ramfs)
	__initramfs_end = .;
  }

  /* Sections to be discarded */
  /DISCARD/ : {
 	*(.exitcall.exit)
  }

